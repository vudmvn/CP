/**
 * @file template.cpp
 * @brief Chuyên đề: Căn Bậc Hai Trên Mảng & Cấu Trúc Dữ Liệu (Square Root Decomposition - SRD)
 * @author Duc-Minh Vu (SLSCM Lab - FDA, Đại học Kinh tế Quốc dân - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * 
 * Mã nguồn chuẩn C++20 bao gồm 4 biến thể hoàn chỉnh:
 * 1. SqrtRangeSumLazy: Chia khối quản lý Range Add & Range Sum kèm thẻ trễ Lazy trong O(sqrt(N)).
 * 2. SqrtSortedBlocks: Khối sắp xếp hỗ trợ Range Add và đếm số phần tử <= X trong O(sqrt(N log N)).
 * 3. SqrtRebuilding: Kỹ thuật tái cấu trúc định kỳ (Batch Updates & Periodic Rebuild).
 * 4. HeavyLightTriangle: Đếm tam giác trong đồ thị đơn vô hướng theo ngưỡng bậc O(M sqrt(M)).
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// ============================================================================
// 1. CHIA KHỐI CƠ BẢN VỚI THẺ TRỄ LAZY: RANGE ADD & RANGE SUM
// ============================================================================
namespace SqrtLazy {
    class SqrtDecomposition {
    private:
        int n;
        int B; // Kích thước khối
        int num_blocks;
        vector<long long> a;
        vector<long long> block_sum;
        vector<long long> lazy;

        void push_lazy(int b) {
            if (lazy[b] == 0) return;
            int l = b * B;
            int r = min(n - 1, (b + 1) * B - 1);
            for (int i = l; i <= r; ++i) {
                a[i] += lazy[b];
            }
            lazy[b] = 0;
        }

        void recalculate(int b) {
            int l = b * B;
            int r = min(n - 1, (b + 1) * B - 1);
            block_sum[b] = 0;
            for (int i = l; i <= r; ++i) {
                block_sum[b] += a[i];
            }
        }

    public:
        SqrtDecomposition(const vector<long long>& initial) : a(initial), n(initial.size()) {
            B = max(1, (int)sqrt(n));
            num_blocks = (n + B - 1) / B;
            block_sum.assign(num_blocks, 0);
            lazy.assign(num_blocks, 0);

            for (int i = 0; i < n; ++i) {
                block_sum[i / B] += a[i];
            }
        }

        void range_add(int l, int r, long long val) {
            int bl = l / B;
            int br = r / B;

            if (bl == br) {
                push_lazy(bl);
                for (int i = l; i <= r; ++i) a[i] += val;
                recalculate(bl);
                return;
            }

            // Khối biên trái
            push_lazy(bl);
            for (int i = l; i < (bl + 1) * B; ++i) a[i] += val;
            recalculate(bl);

            // Các khối nguyên ở giữa
            for (int b = bl + 1; b < br; ++b) {
                lazy[b] += val;
                int cur_len = min(n, (b + 1) * B) - (b * B);
                block_sum[b] += val * cur_len;
            }

            // Khối biên phải
            push_lazy(br);
            for (int i = br * B; i <= r; ++i) a[i] += val;
            recalculate(br);
        }

        long long range_sum(int l, int r) {
            int bl = l / B;
            int br = r / B;
            long long total = 0;

            if (bl == br) {
                for (int i = l; i <= r; ++i) {
                    total += a[i] + lazy[bl];
                }
                return total;
            }

            // Khối biên trái
            for (int i = l; i < (bl + 1) * B; ++i) {
                total += a[i] + lazy[bl];
            }

            // Các khối nguyên ở giữa
            for (int b = bl + 1; b < br; ++b) {
                total += block_sum[b];
            }

            // Khối biên phải
            for (int i = br * B; i <= r; ++i) {
                total += a[i] + lazy[br];
            }

            return total;
        }
    };
}

// ============================================================================
// 2. KHỐI SẮP XẾP & CHẶT NHỊ PHÂN (SORTED BLOCKS RANGE COUNT <= X)
// ============================================================================
namespace SortedBlocks {
    class SqrtSortedBlocks {
    private:
        int n;
        int B;
        int num_blocks;
        vector<long long> a;
        vector<vector<long long>> sorted_blocks;
        vector<long long> lazy;

        void rebuild_block(int b) {
            int l = b * B;
            int r = min(n - 1, (b + 1) * B - 1);
            sorted_blocks[b].clear();
            for (int i = l; i <= r; ++i) {
                sorted_blocks[b].push_back(a[i]);
            }
            sort(sorted_blocks[b].begin(), sorted_blocks[b].end());
        }

    public:
        SqrtSortedBlocks(const vector<long long>& initial) : a(initial), n(initial.size()) {
            B = max(1, (int)sqrt(n * max(1.0, log2(n))));
            num_blocks = (n + B - 1) / B;
            sorted_blocks.resize(num_blocks);
            lazy.assign(num_blocks, 0);

            for (int b = 0; b < num_blocks; ++b) {
                rebuild_block(b);
            }
        }

        void range_add(int l, int r, long long val) {
            int bl = l / B, br = r / B;
            if (bl == br) {
                for (int i = l; i <= r; ++i) a[i] += val;
                rebuild_block(bl);
                return;
            }

            for (int i = l; i < (bl + 1) * B; ++i) a[i] += val;
            rebuild_block(bl);

            for (int b = bl + 1; b < br; ++b) {
                lazy[b] += val;
            }

            for (int i = br * B; i <= r; ++i) a[i] += val;
            rebuild_block(br);
        }

        // Đếm số phần tử trong [L, R] có giá trị <= X
        int count_less_equal(int l, int r, long long x) {
            int bl = l / B, br = r / B;
            int count = 0;

            if (bl == br) {
                for (int i = l; i <= r; ++i) {
                    if (a[i] + lazy[bl] <= x) count++;
                }
                return count;
            }

            // Khối biên trái
            for (int i = l; i < (bl + 1) * B; ++i) {
                if (a[i] + lazy[bl] <= x) count++;
            }

            // Các khối nguyên ở giữa: Chặt nhị phân trên sorted_block
            for (int b = bl + 1; b < br; ++b) {
                long long target = x - lazy[b];
                count += upper_bound(sorted_blocks[b].begin(), sorted_blocks[b].end(), target) - sorted_blocks[b].begin();
            }

            // Khối biên phải
            for (int i = br * B; i <= r; ++i) {
                if (a[i] + lazy[br] <= x) count++;
            }

            return count;
        }
    };
}

// ============================================================================
// 3. TÁI CẤU TRÚC ĐỊNH KỲ (SQRT REBUILDING / BATCH PROCESSING)
// ============================================================================
namespace SqrtRebuilding {
    /**
     * @brief Mô hình Rebuilding quản lý tập hợp động hỗ trợ:
     * - Thêm phần tử x
     * - Đếm số phần tử trong tập hợp thuộc đoạn [L, R]
     */
    class SqrtRebuilder {
    private:
        int B;
        vector<int> static_elements;  // Đã sắp xếp tĩnh
        vector<int> buffer;           // Các thao tác thêm mới gần đây

        void rebuild() {
            // Gộp static_elements và buffer lại thành một mảng đã sắp
            sort(buffer.begin(), buffer.end());
            vector<int> merged;
            merged.reserve(static_elements.size() + buffer.size());
            merge(static_elements.begin(), static_elements.end(),
                  buffer.begin(), buffer.end(),
                  back_inserter(merged));
            static_elements = move(merged);
            buffer.clear();
        }

    public:
        SqrtRebuilder(int expected_ops) {
            B = max(10, (int)sqrt(expected_ops));
        }

        void insert(int x) {
            buffer.push_back(x);
            if ((int)buffer.size() >= B) {
                rebuild();
            }
        }

        int count_range(int l, int r) {
            // 1. Tìm trên cấu trúc tĩnh đã sắp xếp
            int count_static = upper_bound(static_elements.begin(), static_elements.end(), r)
                             - lower_bound(static_elements.begin(), static_elements.end(), l);

            // 2. Quét trực tiếp buffer tạm thời (tối đa B phần tử)
            int count_buffer = 0;
            for (int x : buffer) {
                if (x >= l && x <= r) count_buffer++;
            }

            return count_static + count_buffer;
        }
    };
}

// ============================================================================
// 4. PHÂN NGƯỠNG HEAVY-LIGHT: ĐẾM TAM GIÁC TRONG ĐỒ THỊ O(M sqrt(M))
// ============================================================================
namespace HeavyLightGraph {
    /**
     * @brief Đếm số lượng tam giác trong đồ thị đơn vô hướng
     * @param n Số đỉnh (1-indexed)
     * @param edges Danh sách cạnh {u, v}
     * @return long long Số lượng bộ 3 đỉnh (u, v, w) nối với nhau đôi một
     */
    long long count_triangles(int n, const vector<pair<int, int>>& edges) {
        vector<int> deg(n + 1, 0);
        for (const auto& [u, v] : edges) {
            deg[u]++;
            deg[v]++;
        }

        // Định hướng cạnh tạo DAG: u -> v nếu deg[u] < deg[v] hoặc (deg[u] == deg[v] và u < v)
        vector<vector<int>> out_adj(n + 1);
        for (const auto& [u, v] : edges) {
            if (deg[u] < deg[v] || (deg[u] == deg[v] && u < v)) {
                out_adj[u].push_back(v);
            } else {
                out_adj[v].push_back(u);
            }
        }

        vector<int> tag(n + 1, 0);
        long long triangle_count = 0;

        for (int u = 1; u <= n; ++u) {
            for (int v : out_adj[u]) {
                tag[v] = u;
            }
            for (int v : out_adj[u]) {
                for (int w : out_adj[v]) {
                    if (tag[w] == u) {
                        triangle_count++;
                    }
                }
            }
        }

        return triangle_count;
    }
}

// ============================================================================
// HÀM KIỂM THỬ ĐƠN VỊ TOÀN DIỆN (UNIT TESTS)
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== BẮT ĐẦU KIỂM THỬ SQUARE ROOT DECOMPOSITION (SRD) ===" << "\n";

    // 1. Test SqrtRangeSumLazy (Range Add & Range Sum)
    {
        vector<long long> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        SqrtLazy::SqrtDecomposition srd(a);

        // Sum [0, 9] = 55
        assert(srd.range_sum(0, 9) == 55);
        // Sum [2, 6] = 3+4+5+6+7 = 25
        assert(srd.range_sum(2, 6) == 25);

        // Add 10 to [1, 5] -> a = {1, 12, 13, 14, 15, 16, 7, 8, 9, 10}
        srd.range_add(1, 5, 10);
        // Sum [2, 6] = 13+14+15+16+7 = 65
        assert(srd.range_sum(2, 6) == 65);
        // Sum [0, 9] = 55 + 50 = 105
        assert(srd.range_sum(0, 9) == 105);

        cout << "[OK] Test 1: Sqrt Range Sum with Lazy Passed!" << "\n";
    }

    // 2. Test SqrtSortedBlocks (Range Add & Count <= X)
    {
        vector<long long> a = {9, 2, 6, 3, 8, 1, 5, 4, 7};
        SortedBlocks::SqrtSortedBlocks sorted_srd(a);

        // Đếm số phần tử <= 4 trong [0, 8]: {2, 3, 1, 4} -> 4
        assert(sorted_srd.count_less_equal(0, 8, 4) == 4);
        // Đếm số phần tử <= 6 trong [2, 6]: a[2..6] = {6, 3, 8, 1, 5} -> {6, 3, 1, 5} -> 4
        assert(sorted_srd.count_less_equal(2, 6, 6) == 4);

        // Cộng 5 vào [2, 5]: a = {9, 2, 11, 8, 13, 6, 5, 4, 7}
        sorted_srd.range_add(2, 5, 5);
        // Đếm số phần tử <= 6 trong [2, 6]: {11, 8, 13, 6, 5} -> {6, 5} -> 2
        assert(sorted_srd.count_less_equal(2, 6, 6) == 2);

        cout << "[OK] Test 2: Sqrt Sorted Blocks (Count <= X) Passed!" << "\n";
    }

    // 3. Test SqrtRebuilding (Batch Updates & Periodic Rebuild)
    {
        SqrtRebuilding::SqrtRebuilder rebuilder(100);
        rebuilder.insert(10);
        rebuilder.insert(20);
        rebuilder.insert(5);
        rebuilder.insert(15);
        rebuilder.insert(30);

        // Đếm trong khoảng [10, 25]: {10, 15, 20} -> 3
        assert(rebuilder.count_range(10, 25) == 3);
        // Đếm trong khoảng [1, 5]: {5} -> 1
        assert(rebuilder.count_range(1, 5) == 1);

        for (int i = 0; i < 20; ++i) {
            rebuilder.insert(i * 2); // Kích hoạt rebuild định kỳ
        }
        // Kiểm tra sau nhiều lần rebuild
        assert(rebuilder.count_range(0, 10) >= 6);

        cout << "[OK] Test 3: Sqrt Rebuilding Passed!" << "\n";
    }

    // 4. Test HeavyLightGraph (Triangle Counting O(M sqrt(M)))
    {
        int n = 5;
        // Đồ thị chứa 2 tam giác: (1, 2, 3) và (2, 3, 4)
        // Cạnh: (1, 2), (2, 3), (3, 1), (2, 4), (3, 4), (4, 5)
        vector<pair<int, int>> edges = {
            {1, 2}, {2, 3}, {3, 1},
            {2, 4}, {3, 4},
            {4, 5}
        };

        long long triangles = HeavyLightGraph::count_triangles(n, edges);
        assert(triangles == 2);

        // Thêm cạnh (1, 4) -> tạo thêm tam giác (1, 2, 4) và (1, 3, 4) -> tổng 4 tam giác (K4 trên {1, 2, 3, 4})
        edges.push_back({1, 4});
        assert(HeavyLightGraph::count_triangles(n, edges) == 4);

        cout << "[OK] Test 4: Heavy-Light Triangle Counting Passed!" << "\n";
    }

    cout << "=== TẤT CẢ 4 BỘ TEST SQUARE ROOT DECOMPOSITION ĐÃ VƯỢT QUA THÀNH CÔNG ===" << "\n";
    return 0;
}
