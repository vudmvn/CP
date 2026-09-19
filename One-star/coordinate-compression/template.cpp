/**
 * @file template.cpp
 * @brief Kỹ thuật Rời rạc hóa & Nén Tọa độ (Coordinate Compression & Discretization)
 * @author Duc-Minh Vu
 * @affiliation SLSCM Lab - Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @date 2026
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * @note Soạn thảo và tối ưu bởi Agentic AI tool cho Olympic Tin học Sinh viên / ICPC.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

namespace CoordCompression {

    /**
     * @brief Bộ nén điểm 1D (Point Compressor).
     * Ánh xạ tập giá trị lớn bất kỳ về tập các số nguyên 0-based [0, K-1] hoặc 1-based [1, K].
     * @tparam T Kiểu dữ liệu phần tử (mặc định long long).
     */
    template <typename T = long long>
    class PointCompressor {
    private:
        vector<T> vals;
        bool is_built;

    public:
        PointCompressor() : is_built(false) {}

        void add(const T& x) {
            vals.push_back(x);
            is_built = false;
        }

        void add(const vector<T>& vec) {
            vals.insert(vals.end(), vec.begin(), vec.end());
            is_built = false;
        }

        void build() {
            sort(vals.begin(), vals.end());
            vals.erase(unique(vals.begin(), vals.end()), vals.end());
            is_built = true;
        }

        /**
         * @brief Lấy chỉ số nén của giá trị x.
         * @param x Giá trị cần nén.
         * @param one_based True nếu muốn chỉ số [1, K] cho Fenwick/BIT, False nếu muốn [0, K-1].
         * @return Chỉ số thứ hạng tương ứng.
         */
        int get_rank(const T& x, bool one_based = false) const {
            assert(is_built && "Compressor must be built before querying!");
            auto it = lower_bound(vals.begin(), vals.end(), x);
            assert(it != vals.end() && *it == x && "Value not found in compressor!");
            int rank = it - vals.begin();
            return one_based ? rank + 1 : rank;
        }

        /**
         * @brief Khôi phục giá trị gốc từ chỉ số nén.
         */
        T get_orig(int rank, bool one_based = false) const {
            assert(is_built && "Compressor must be built before querying!");
            int idx = one_based ? rank - 1 : rank;
            assert(idx >= 0 && idx < (int)vals.size() && "Rank index out of bounds!");
            return vals[idx];
        }

        int size() const {
            assert(is_built && "Compressor must be built before checking size!");
            return vals.size();
        }

        const vector<T>& get_unique_vals() const {
            return vals;
        }
    };

    /**
     * @brief Bộ nén đoạn 1D có bảo toàn khoảng hở (Interval Compressor with Gap Preservation).
     * Tự động chèn các điểm trung gian (L-1, L, R, R+1) để tránh việc dính liền các đoạn rời rạc.
     */
    template <typename T = long long>
    class IntervalCompressor {
    private:
        vector<T> vals;
        bool is_built;

    public:
        IntervalCompressor() : is_built(false) {}

        /**
         * @brief Thêm đoạn [L, R] vào bộ nén.
         * @param preserve_gaps Nếu true, tự động chèn thêm L-1 và R+1 để giữ cấu trúc không gian rời rạc.
         */
        void add_interval(T L, T R, bool preserve_gaps = true) {
            vals.push_back(L);
            vals.push_back(R);
            if (preserve_gaps) {
                vals.push_back(L - 1);
                vals.push_back(R + 1);
            }
            is_built = false;
        }

        void build() {
            sort(vals.begin(), vals.end());
            vals.erase(unique(vals.begin(), vals.end()), vals.end());
            is_built = true;
        }

        int get_rank(T x) const {
            assert(is_built && "Must build before query!");
            auto it = lower_bound(vals.begin(), vals.end(), x);
            assert(it != vals.end() && *it == x && "Value not found in interval compressor!");
            return it - vals.begin();
        }

        T get_orig(int rank) const {
            return vals[rank];
        }

        /**
         * @brief Lấy độ dài thực tế của khoảng nén thứ i: [vals[i], vals[i+1]).
         */
        T get_segment_length(int rank) const {
            assert(rank + 1 < (int)vals.size() && "Cannot get segment length for last point!");
            return vals[rank + 1] - vals[rank];
        }

        int size() const { return vals.size(); }
    };

    /**
     * @brief Bộ nén tọa độ lưới 2D (2D Grid Coordinate Compressor).
     * Nén độc lập trục X và trục Y để đưa ma trận 10^9 x 10^9 về lưới kích thước O(N) x O(N).
     */
    template <typename T = long long>
    class GridCompressor2D {
    private:
        PointCompressor<T> comp_x;
        PointCompressor<T> comp_y;

    public:
        void add_point(T x, T y) {
            comp_x.add(x);
            comp_y.add(y);
        }

        void add_rect(T x1, T y1, T x2, T y2, bool add_adjacent = false) {
            comp_x.add(x1); comp_x.add(x2);
            comp_y.add(y1); comp_y.add(y2);
            if (add_adjacent) {
                comp_x.add(x1 - 1); comp_x.add(x2 + 1);
                comp_y.add(y1 - 1); comp_y.add(y2 + 1);
            }
        }

        void build() {
            comp_x.build();
            comp_y.build();
        }

        pair<int, int> get_grid_dims() const {
            return {comp_x.size(), comp_y.size()};
        }

        pair<int, int> map_point(T x, T y) const {
            return {comp_x.get_rank(x), comp_y.get_rank(y)};
        }

        /**
         * @return {rx1, ry1, rx2, ry2} tọa độ nén của hình chữ nhật.
         */
        vector<int> map_rect(T x1, T y1, T x2, T y2) const {
            return {comp_x.get_rank(x1), comp_y.get_rank(y1),
                    comp_x.get_rank(x2), comp_y.get_rank(y2)};
        }

        const PointCompressor<T>& get_x_compressor() const { return comp_x; }
        const PointCompressor<T>& get_y_compressor() const { return comp_y; }
    };

    /**
     * @brief Ứng dụng: Đếm số nghịch thế (Inversion Count) bằng Coordinate Compression + Fenwick Tree.
     * Độ phức tạp: O(N log N).
     */
    long long count_inversions(const vector<long long>& a) {
        int n = a.size();
        if (n <= 1) return 0;

        PointCompressor<long long> comp;
        comp.add(a);
        comp.build();

        int max_val = comp.size();
        vector<int> bit(max_val + 2, 0);

        auto update = [&](int idx, int delta) {
            for (; idx <= max_val; idx += idx & -idx) bit[idx] += delta;
        };

        auto query = [&](int idx) -> int {
            int sum = 0;
            for (; idx > 0; idx -= idx & -idx) sum += bit[idx];
            return sum;
        };

        long long inv_count = 0;
        for (int i = 0; i < n; ++i) {
            int rank = comp.get_rank(a[i], true); // 1-based cho Fenwick Tree
            // Số lượng phần tử đã duyệt lớn hơn ngặt a[i] = i - query(rank)
            inv_count += (i - query(rank));
            update(rank, 1);
        }
        return inv_count;
    }
}

void run_unit_tests() {
    using namespace CoordCompression;

    // 1. Kiểm thử PointCompressor 1D
    {
        vector<long long> a = {1000000000LL, -500LL, 42LL, 42LL, -500LL, 1000000000LL};
        PointCompressor<long long> comp;
        comp.add(a);
        comp.build();

        assert(comp.size() == 3);
        assert(comp.get_rank(-500LL) == 0);
        assert(comp.get_rank(42LL) == 1);
        assert(comp.get_rank(1000000000LL) == 2);

        // 1-based
        assert(comp.get_rank(-500LL, true) == 1);
        assert(comp.get_rank(1000000000LL, true) == 3);

        // Decompression
        assert(comp.get_orig(0) == -500LL);
        assert(comp.get_orig(1) == 42LL);
        assert(comp.get_orig(2) == 1000000000LL);
    }

    // 2. Kiểm thử IntervalCompressor với bảo toàn khoảng hở
    {
        IntervalCompressor<long long> int_comp;
        // Thêm 2 đoạn [1, 2] và [4, 5]
        int_comp.add_interval(1, 2, true);
        int_comp.add_interval(4, 5, true);
        int_comp.build();

        // Kiểm tra thứ tự tương đối: 1 < 2 < 3 < 4 < 5
        int r1 = int_comp.get_rank(1);
        int r2 = int_comp.get_rank(2);
        int r4 = int_comp.get_rank(4);
        int r5 = int_comp.get_rank(5);

        // Phải có ít nhất 1 điểm trung gian giữa r2 và r4!
        assert(r4 > r2 + 1 && "Gap between disjoint intervals must be preserved!");
    }

    // 3. Kiểm thử GridCompressor2D
    {
        GridCompressor2D<long long> grid_comp;
        grid_comp.add_rect(10, 20, 30, 40);
        grid_comp.add_rect(25, 35, 50, 60);
        grid_comp.build();

        auto [W, H] = grid_comp.get_grid_dims();
        assert(W >= 4 && H >= 4);

        auto rect1 = grid_comp.map_rect(10, 20, 30, 40);
        assert(rect1[0] < rect1[2] && rect1[1] < rect1[3]);
    }

    // 4. Kiểm thử đếm nghịch thế Inversion Count
    {
        vector<long long> arr1 = {2, 4, 1, 3, 5};
        // Các cặp nghịch thế: (2, 1), (4, 1), (4, 3) -> 3 cặp
        assert(count_inversions(arr1) == 3);

        vector<long long> arr2 = {1000000000000LL, 500000000000LL, 100000000LL};
        // Dãy giảm dần độ dài 3 -> 3 * 2 / 2 = 3 cặp
        assert(count_inversions(arr2) == 3);

        vector<long long> arr3 = {1, 2, 3, 4};
        assert(count_inversions(arr3) == 0);
    }

    cout << "✅ [CoordCompression] Tất cả các Unit Tests đều vượt qua thành công!\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_unit_tests();
    return 0;
}
