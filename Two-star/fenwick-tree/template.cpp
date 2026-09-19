/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Cây Chỉ số Nhị phân (Fenwick Tree / Binary Indexed Tree - BIT) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. FENWICK TREE CƠ BẢN: POINT UPDATE, RANGE QUERY (PURQ)
// ==============================================================================
namespace FenwickPURQ {

    template<typename T = long long>
    class FenwickTree {
    private:
        int n;
        vector<T> bit;

    public:
        FenwickTree(int n = 0) : n(n), bit(n + 1, 0) {}

        /// Cập nhật giá trị: A[p] += val trong O(log N)
        void update(int p, T val) {
            assert(p >= 1 && p <= n);
            for (; p <= n; p += p & -p) {
                bit[p] += val;
            }
        }

        /// Truy vấn tổng tiền tố A[1..p] trong O(log N)
        T query(int p) const {
            T sum = 0;
            for (; p > 0; p -= p & -p) {
                sum += bit[p];
            }
            return sum;
        }

        /// Truy vấn tổng đoạn A[l..r] trong O(log N)
        T query_range(int l, int r) const {
            if (l > r) return 0;
            return query(r) - query(l - 1);
        }
    };
}

// ==============================================================================
// 2. FENWICK TREE MẢNG HIỆU: RANGE UPDATE, POINT QUERY (RUPQ)
// ==============================================================================
namespace FenwickRUPQ {

    template<typename T = long long>
    class RangeUpdatePointQuery {
    private:
        int n;
        FenwickPURQ::FenwickTree<T> ft;

    public:
        RangeUpdatePointQuery(int n = 0) : n(n), ft(n) {}

        /// Cộng thêm val vào tất cả phần tử trong đoạn [l, r] trong O(log N)
        void update_range(int l, int r, T val) {
            if (l > r) return;
            ft.update(l, val);
            if (r + 1 <= n) {
                ft.update(r + 1, -val);
            }
        }

        /// Lấy giá trị của phần tử tại vị trí p trong O(log N)
        T query_point(int p) const {
            return ft.query(p);
        }
    };
}

// ==============================================================================
// 3. FENWICK TREE ĐOẠN - ĐOẠN: RANGE UPDATE, RANGE QUERY (RURQ)
// ==============================================================================
namespace FenwickRURQ {

    template<typename T = long long>
    class RangeUpdateRangeQuery {
    private:
        int n;
        FenwickPURQ::FenwickTree<T> b1; // Quản lý D[i]
        FenwickPURQ::FenwickTree<T> b2; // Quản lý i * D[i]

        T query_prefix(int p) const {
            return (p + 1) * b1.query(p) - b2.query(p);
        }

    public:
        RangeUpdateRangeQuery(int n = 0) : n(n), b1(n), b2(n) {}

        /// Cộng thêm val vào tất cả phần tử trong đoạn [l, r] trong O(log N)
        void update_range(int l, int r, T val) {
            if (l > r) return;
            // Cập nhật b1
            b1.update(l, val);
            if (r + 1 <= n) b1.update(r + 1, -val);

            // Cập nhật b2
            b2.update(l, l * val);
            if (r + 1 <= n) b2.update(r + 1, -((r + 1) * val));
        }

        /// Truy vấn tổng đoạn A[l..r] trong O(log N)
        T query_range(int l, int r) const {
            if (l > r) return 0;
            return query_prefix(r) - query_prefix(l - 1);
        }
    };
}

// ==============================================================================
// 4. NHẢY NHỊ PHÂN TRÊN FENWICK TREE (BINARY LIFTING - K-TH ELEMENT O(LOG N))
// ==============================================================================
namespace FenwickOrderStatistic {

    class OrderStatisticTree {
    private:
        int n;
        vector<int> bit; // Lưu tần số xuất hiện của các giá trị [1..n]

    public:
        OrderStatisticTree(int n = 0) : n(n), bit(n + 1, 0) {}

        void insert(int val, int count = 1) {
            for (int p = val; p <= n; p += p & -p) {
                bit[p] += count;
            }
        }

        void erase(int val, int count = 1) {
            insert(val, -count);
        }

        /// Đếm số phần tử nhỏ hơn hoặc bằng val trong O(log N)
        int order_of_key(int val) const {
            int sum = 0;
            for (int p = min(val, n); p > 0; p -= p & -p) {
                sum += bit[p];
            }
            return sum;
        }

        /// Tìm giá trị nhỏ nhất có tổng tần số tiền tố >= target trong O(log N) bằng Binary Lifting
        int find_by_order(int k) const {
            int sum = 0;
            int pos = 0;

            // Nhảy các lũy thừa của 2 từ lớn về bé
            for (int i = 1 << (31 - __builtin_clz(n)); i > 0; i >>= 1) {
                if (pos + i <= n && sum + bit[pos + i] < k) {
                    sum += bit[pos + i];
                    pos += i;
                }
            }
            return pos + 1; // Vị trí phần tử thứ k (1-indexed)
        }
    };
}

// ==============================================================================
// 5. FENWICK TREE 2 CHIỀU: 2D POINT UPDATE, 2D RANGE QUERY
// ==============================================================================
namespace Fenwick2D {

    template<typename T = long long>
    class FenwickTree2D {
    private:
        int n, m;
        vector<vector<T>> bit;

    public:
        FenwickTree2D(int n = 0, int m = 0) 
            : n(n), m(m), bit(n + 1, vector<T>(m + 1, 0)) {}

        /// Cập nhật A[x][y] += val trong O(log N * log M)
        void update(int x, int y, T val) {
            for (int i = x; i <= n; i += i & -i) {
                for (int j = y; j <= m; j += j & -j) {
                    bit[i][j] += val;
                }
            }
        }

        /// Truy vấn tổng hình chữ nhật con [1..x][1..y] trong O(log N * log M)
        T query_prefix(int x, int y) const {
            T sum = 0;
            for (int i = x; i > 0; i -= i & -i) {
                for (int j = y; j > 0; j -= j & -j) {
                    sum += bit[i][j];
                }
            }
            return sum;
        }

        /// Truy vấn tổng hình chữ nhật con [x1..x2][y1..y2] bằng nguyên lý bù trừ
        T query_rect(int x1, int y1, int x2, int y2) const {
            if (x1 > x2 || y1 > y2) return 0;
            return query_prefix(x2, y2) 
                 - query_prefix(x1 - 1, y2) 
                 - query_prefix(x2, y1 - 1) 
                 + query_prefix(x1 - 1, y1 - 1);
        }
    };
}

// ==============================================================================
// VÍ DỤ MINH HỌA KIỂM THỬ (TEST HARNESS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 1. Kiểm thử Fenwick PURQ
    FenwickPURQ::FenwickTree<long long> ft(8);
    ft.update(3, 5);
    ft.update(6, 7);
    cout << "=== 1. Fenwick PURQ Test ===" << "\n";
    cout << "Prefix sum(7): " << ft.query(7) << "\n";             // 12 (5 + 7)
    cout << "Range sum(3, 6): " << ft.query_range(3, 6) << "\n";   // 12
    cout << "Range sum(4, 5): " << ft.query_range(4, 5) << "\n";   // 0

    // 2. Kiểm thử Fenwick RUPQ (Range Update, Point Query)
    FenwickRUPQ::RangeUpdatePointQuery<long long> rupq(8);
    rupq.update_range(2, 5, 10);
    rupq.update_range(4, 7, 5);
    cout << "\n=== 2. Fenwick RUPQ Test ===" << "\n";
    cout << "Value at index 3: " << rupq.query_point(3) << "\n";   // 10
    cout << "Value at index 4: " << rupq.query_point(4) << "\n";   // 15 (10 + 5)
    cout << "Value at index 6: " << rupq.query_point(6) << "\n";   // 5
    cout << "Value at index 8: " << rupq.query_point(8) << "\n";   // 0

    // 3. Kiểm thử Fenwick RURQ (Range Update, Range Query)
    FenwickRURQ::RangeUpdateRangeQuery<long long> rurq(8);
    rurq.update_range(1, 4, 3); // A = [3, 3, 3, 3, 0, 0, 0, 0]
    rurq.update_range(3, 6, 2); // A = [3, 3, 5, 5, 2, 2, 0, 0]
    cout << "\n=== 3. Fenwick RURQ Test ===" << "\n";
    cout << "Sum(1..8): " << rurq.query_range(1, 8) << "\n";       // 3+3+5+5+2+2 = 20
    cout << "Sum(3..5): " << rurq.query_range(3, 5) << "\n";       // 5+5+2 = 12

    // 4. Kiểm thử Binary Lifting on Fenwick Tree (Order Statistic)
    FenwickOrderStatistic::OrderStatisticTree ost(100);
    ost.insert(10); ost.insert(25); ost.insert(25); ost.insert(40); ost.insert(50);
    cout << "\n=== 4. Binary Lifting on BIT (Order Statistic) ===" << "\n";
    cout << "1st element: " << ost.find_by_order(1) << "\n";       // 10
    cout << "2nd element: " << ost.find_by_order(2) << "\n";       // 25
    cout << "3rd element: " << ost.find_by_order(3) << "\n";       // 25
    cout << "4th element: " << ost.find_by_order(4) << "\n";       // 40
    cout << "Elements <= 25: " << ost.order_of_key(25) << "\n";    // 3

    // 5. Kiểm thử 2D Fenwick Tree
    Fenwick2D::FenwickTree2D<long long> bit2d(4, 4);
    bit2d.update(2, 2, 5);
    bit2d.update(3, 3, 10);
    cout << "\n=== 5. 2D Fenwick Tree Test ===" << "\n";
    cout << "Rect sum [1..3][1..3]: " << bit2d.query_rect(1, 1, 3, 3) << "\n"; // 15
    cout << "Rect sum [2..3][2..3]: " << bit2d.query_rect(2, 2, 3, 3) << "\n"; // 15
    cout << "Rect sum [3..4][3..4]: " << bit2d.query_rect(3, 3, 4, 4) << "\n"; // 10

    return 0;
}
