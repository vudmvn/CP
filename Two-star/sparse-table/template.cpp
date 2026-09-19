/**
 * @file template.cpp
 * @brief High-Performance Sparse Table, 2D RMQ, Index RMQ, and Disjoint Sparse Table
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations for:
 * 1. SparseTable<T, Op>: Cache-friendly O(N log N) build, O(1) query for idempotent operations.
 * 2. SparseTableIndex<T, Compare>: Returns the index of the extremum (for Cartesian Tree).
 * 3. SparseTable2D<T, Op>: 2D RMQ for O(1) static rectangle subgrid queries.
 * 4. DisjointSparseTable<T, Op>: O(1) query for ANY associative operation (Monoids).
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <functional>
#include <bit>
#include <cassert>
#include <algorithm>

namespace cp::data_structures {

// =========================================================================
// 1. 1D Sparse Table for Idempotent Operations (Min, Max, GCD, Bitwise)
// =========================================================================
template <typename T, typename Op = std::less<T>>
class SparseTable {
private:
    int n;
    int max_k;
    std::vector<std::vector<T>> st; // st[k][i] for L1/L2 cache locality
    Op op;

    T combine(const T& a, const T& b) const {
        if constexpr (std::is_same_v<Op, std::less<T>>) {
            return std::min(a, b);
        } else if constexpr (std::is_same_v<Op, std::greater<T>>) {
            return std::max(a, b);
        } else {
            return op(a, b);
        }
    }

public:
    SparseTable() : n(0), max_k(0) {}

    explicit SparseTable(const std::vector<T>& arr, Op op_func = Op())
        : n(static_cast<int>(arr.size())), op(op_func) {
        if (n == 0) return;
        max_k = std::bit_width(static_cast<unsigned>(n));
        st.assign(max_k, std::vector<T>(n));

        st[0] = arr;
        for (int k = 1; k < max_k; ++k) {
            int len = 1 << (k - 1);
            for (int i = 0; i + (1 << k) <= n; ++i) {
                st[k][i] = combine(st[k - 1][i], st[k - 1][i + len]);
            }
        }
    }

    /**
     * @brief O(1) Query on [l, r] for idempotent operations (x op x = x)
     */
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int len = r - l + 1;
        int k = std::bit_width(static_cast<unsigned>(len)) - 1;
        return combine(st[k][l], st[k][r - (1 << k) + 1]);
    }

    /**
     * @brief O(log N) Query on [l, r] for non-idempotent associative operations
     */
    T query_associative(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int len = r - l + 1;
        int cur = l;
        T ans{};
        bool first = true;
        for (int k = 0; (1 << k) <= len; ++k) {
            if ((len >> k) & 1) {
                if (first) {
                    ans = st[k][cur];
                    first = false;
                } else {
                    ans = combine(ans, st[k][cur]);
                }
                cur += (1 << k);
            }
        }
        return ans;
    }
};

// =========================================================================
// 2. Sparse Table Storing Indices (Index RMQ - for Cartesian Tree / DC)
// =========================================================================
template <typename T, typename Compare = std::less<T>>
class SparseTableIndex {
private:
    int n;
    int max_k;
    const std::vector<T>* data;
    std::vector<std::vector<int>> st;
    Compare comp;

    int better_idx(int i, int j) const {
        const auto& a = (*data)[i];
        const auto& b = (*data)[j];
        if (comp(a, b)) return i;
        if (comp(b, a)) return j;
        return std::min(i, j); // Prefer smaller index on tie
    }

public:
    SparseTableIndex() : n(0), max_k(0), data(nullptr) {}

    explicit SparseTableIndex(const std::vector<T>& arr, Compare compare_func = Compare())
        : n(static_cast<int>(arr.size())), data(&arr), comp(compare_func) {
        if (n == 0) return;
        max_k = std::bit_width(static_cast<unsigned>(n));
        st.assign(max_k, std::vector<int>(n));

        for (int i = 0; i < n; ++i) st[0][i] = i;

        for (int k = 1; k < max_k; ++k) {
            int len = 1 << (k - 1);
            for (int i = 0; i + (1 << k) <= n; ++i) {
                st[k][i] = better_idx(st[k - 1][i], st[k - 1][i + len]);
            }
        }
    }

    /**
     * @brief Returns index of extremum in [l, r] in O(1)
     */
    int query_idx(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        int len = r - l + 1;
        int k = std::bit_width(static_cast<unsigned>(len)) - 1;
        return better_idx(st[k][l], st[k][r - (1 << k) + 1]);
    }

    T query_val(int l, int r) const {
        return (*data)[query_idx(l, r)];
    }
};

// =========================================================================
// 3. 2D Sparse Table for 2D Grid RMQ in O(1)
// =========================================================================
template <typename T, typename Op = std::less<T>>
class SparseTable2D {
private:
    int n, m;
    int kx, ky;
    // st[kx][ky][i][j]
    std::vector<std::vector<std::vector<std::vector<T>>>> st;
    Op op;

    T combine(const T& a, const T& b) const {
        if constexpr (std::is_same_v<Op, std::less<T>>) {
            return std::min(a, b);
        } else if constexpr (std::is_same_v<Op, std::greater<T>>) {
            return std::max(a, b);
        } else {
            return op(a, b);
        }
    }

public:
    SparseTable2D() : n(0), m(0), kx(0), ky(0) {}

    explicit SparseTable2D(const std::vector<std::vector<T>>& grid, Op op_func = Op())
        : op(op_func) {
        n = static_cast<int>(grid.size());
        if (n == 0) return;
        m = static_cast<int>(grid[0].size());
        if (m == 0) return;

        kx = std::bit_width(static_cast<unsigned>(n));
        ky = std::bit_width(static_cast<unsigned>(m));

        st.assign(kx, std::vector<std::vector<std::vector<T>>>(
            ky, std::vector<std::vector<T>>(n, std::vector<T>(m))));

        // Base case: kx = 0, ky = 0
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                st[0][0][i][j] = grid[i][j];
            }
        }

        // Build across columns (ky)
        for (int q = 1; q < ky; ++q) {
            int len = 1 << (q - 1);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j + (1 << q) <= m; ++j) {
                    st[0][q][i][j] = combine(st[0][q - 1][i][j], st[0][q - 1][i][j + len]);
                }
            }
        }

        // Build across rows (kx)
        for (int p = 1; p < kx; ++p) {
            int len = 1 << (p - 1);
            for (int q = 0; q < ky; ++q) {
                for (int i = 0; i + (1 << p) <= n; ++i) {
                    for (int j = 0; j + (1 << q) <= m; ++j) {
                        st[p][q][i][j] = combine(st[p - 1][q][i][j], st[p - 1][q][i + len][j]);
                    }
                }
            }
        }
    }

    /**
     * @brief O(1) RMQ on rectangle [r1, c1] to [r2, c2]
     */
    T query(int r1, int c1, int r2, int c2) const {
        assert(0 <= r1 && r1 <= r2 && r2 < n);
        assert(0 <= c1 && c1 <= c2 && c2 < m);

        int pr = std::bit_width(static_cast<unsigned>(r2 - r1 + 1)) - 1;
        int qc = std::bit_width(static_cast<unsigned>(c2 - c1 + 1)) - 1;

        T top_left     = st[pr][qc][r1][c1];
        T top_right    = st[pr][qc][r1][c2 - (1 << qc) + 1];
        T bottom_left  = st[pr][qc][r2 - (1 << pr) + 1][c1];
        T bottom_right = st[pr][qc][r2 - (1 << pr) + 1][c2 - (1 << qc) + 1];

        return combine(combine(top_left, top_right), combine(bottom_left, bottom_right));
    }
};

// =========================================================================
// 4. Disjoint Sparse Table (DST) — O(1) for ANY Associative Operation
// =========================================================================
template <typename T, typename Op>
class DisjointSparseTable {
private:
    int n;
    int padded_n;
    int levels;
    std::vector<std::vector<T>> table; // table[level][idx]
    Op op;

public:
    DisjointSparseTable() : n(0), padded_n(0), levels(0) {}

    explicit DisjointSparseTable(const std::vector<T>& arr, Op op_func)
        : n(static_cast<int>(arr.size())), op(op_func) {
        if (n == 0) return;
        levels = std::bit_width(static_cast<unsigned>(std::max(1, n - 1)));
        padded_n = 1 << levels;

        table.assign(levels, std::vector<T>(padded_n));

        for (int k = 0; k < levels; ++k) {
            int block_size = 1 << (k + 1);
            int half = 1 << k;
            for (int block_start = 0; block_start < padded_n; block_start += block_size) {
                int mid = block_start + half - 1;

                // Suffix from mid downwards
                if (mid < n) {
                    table[k][mid] = arr[mid];
                    for (int i = mid - 1; i >= block_start; --i) {
                        table[k][i] = op(arr[i], table[k][i + 1]);
                    }
                }

                // Prefix from mid + 1 upwards
                int p_start = mid + 1;
                if (p_start < n) {
                    table[k][p_start] = arr[p_start];
                    int p_end = std::min(n - 1, block_start + block_size - 1);
                    for (int i = p_start + 1; i <= p_end; ++i) {
                        table[k][i] = op(table[k][i - 1], arr[i]);
                    }
                }
            }
        }
    }

    /**
     * @brief O(1) query for any associative operation
     */
    T query(int l, int r) const {
        assert(0 <= l && l <= r && r < n);
        if (l == r) return table[0][l];
        int k = std::bit_width(static_cast<unsigned>(l ^ r)) - 1;
        return op(table[k][l], table[k][r]);
    }
};

} // namespace cp::data_structures

// =========================================================================
// Self-Testing & Verification
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using namespace cp::data_structures;

    // 1. Test 1D Sparse Table (Min, Max, GCD)
    std::vector<int> a = {4, 7, 2, 8, 1, 9, 3, 5, 6, 2};
    int n = static_cast<int>(a.size());

    SparseTable<int, std::less<int>> st_min(a);
    SparseTable<int, std::greater<int>> st_max(a);
    SparseTable<int, decltype([](int x, int y) { return std::gcd(x, y); })> st_gcd(a);

    for (int l = 0; l < n; ++l) {
        int brute_min = a[l];
        int brute_max = a[l];
        int brute_gcd = a[l];
        for (int r = l; r < n; ++r) {
            brute_min = std::min(brute_min, a[r]);
            brute_max = std::max(brute_max, a[r]);
            brute_gcd = std::gcd(brute_gcd, a[r]);

            assert(st_min.query(l, r) == brute_min);
            assert(st_max.query(l, r) == brute_max);
            assert(st_gcd.query(l, r) == brute_gcd);
        }
    }

    // 2. Test Sparse Table Index
    SparseTableIndex<int, std::less<int>> st_idx(a);
    for (int l = 0; l < n; ++l) {
        for (int r = l; r < n; ++r) {
            int idx = st_idx.query_idx(l, r);
            assert(l <= idx && idx <= r);
            assert(a[idx] == st_min.query(l, r));
        }
    }

    // 3. Test 2D Sparse Table
    std::vector<std::vector<int>> grid = {
        {12,  6, 15, 20},
        { 8,  3, 11, 14},
        { 9,  4,  2, 18},
        {17, 10,  5,  1}
    };
    int rows = 4, cols = 4;
    SparseTable2D<int> st2d(grid);

    for (int r1 = 0; r1 < rows; ++r1) {
        for (int c1 = 0; c1 < cols; ++c1) {
            for (int r2 = r1; r2 < rows; ++r2) {
                for (int c2 = c1; c2 < cols; ++c2) {
                    int brute = grid[r1][c1];
                    for (int i = r1; i <= r2; ++i) {
                        for (int j = c1; j <= c2; ++j) {
                            brute = std::min(brute, grid[i][j]);
                        }
                    }
                    assert(st2d.query(r1, c1, r2, c2) == brute);
                }
            }
        }
    }

    // 4. Test Disjoint Sparse Table for Range Sum O(1)
    std::vector<long long> arr_sum = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int m_size = static_cast<int>(arr_sum.size());
    DisjointSparseTable<long long, std::plus<long long>> dst(arr_sum, std::plus<long long>());

    for (int l = 0; l < m_size; ++l) {
        long long brute_sum = 0;
        for (int r = l; r < m_size; ++r) {
            brute_sum += arr_sum[r];
            assert(dst.query(l, r) == brute_sum);
        }
    }

    std::cout << "[SUCCESS] All Sparse Table test cases passed perfectly!\n";
    std::cout << "RMQ Min on [2, 7]: " << st_min.query(2, 7) << "\n";
    std::cout << "2D RMQ on [1, 1] to [3, 2]: " << st2d.query(1, 1, 3, 2) << "\n";
    std::cout << "DST Sum on [2, 8]: " << dst.query(2, 8) << "\n";

    return 0;
}
