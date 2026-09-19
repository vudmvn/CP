/**
 * @file template.cpp
 * @brief Kỹ thuật Hai con trỏ & Cửa sổ trượt Nâng cao (Advanced Two Pointers & Sliding Window)
 * @author Duc-Minh Vu
 * @affiliation SLSCM Lab - Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @date 2026
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * @note Soạn thảo và tối ưu bởi Agentic AI tool cho Olympic Tin học Sinh viên / ICPC.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include <cassert>
#include <functional>

using namespace std;

namespace AdvancedTwoPointers {

    /**
     * @brief Cấu trúc SWAG (Sliding Window Aggregation) sử dụng 2 Ngăn xếp.
     * Hỗ trợ mọi phép toán hai ngôi có tính kết hợp (Monoid / Semi-group) không cần phép nghịch đảo.
     * Độ phức tạp: Push O(1), Pop O(1) amortized, Query O(1).
     * 
     * @tparam T Kiểu dữ liệu phần tử.
     * @tparam Op Phép toán hai ngôi (mặc định là std::plus<T> hoặc lambda / functor).
     */
    template <typename T, typename Op>
    class SWAG {
    private:
        struct Node {
            T val;
            T agg;
        };

        vector<Node> stack_front; // Đầu pop (chứa hậu tố)
        vector<Node> stack_back;  // Đầu push (chứa tiền tố)
        Op op;

    public:
        SWAG(Op operation = Op()) : op(operation) {}

        bool empty() const {
            return stack_front.empty() && stack_back.empty();
        }

        size_t size() const {
            return stack_front.size() + stack_back.size();
        }

        void push(const T& val) {
            T new_agg = stack_back.empty() ? val : op(stack_back.back().agg, val);
            stack_back.push_back({val, new_agg});
        }

        void pop() {
            assert(!empty() && "Cannot pop from an empty SWAG queue!");
            if (stack_front.empty()) {
                // Trút toàn bộ từ stack_back sang stack_front
                T current_agg{};
                bool first = true;
                while (!stack_back.empty()) {
                    T v = stack_back.back().val;
                    stack_back.pop_back();
                    if (first) {
                        current_agg = v;
                        first = false;
                    } else {
                        current_agg = op(v, current_agg);
                    }
                    stack_front.push_back({v, current_agg});
                }
            }
            stack_front.pop_back();
        }

        T query() const {
            assert(!empty() && "Cannot query an empty SWAG queue!");
            if (stack_front.empty()) {
                return stack_back.back().agg;
            }
            if (stack_back.empty()) {
                return stack_front.back().agg;
            }
            return op(stack_front.back().agg, stack_back.back().agg);
        }

        void clear() {
            stack_front.clear();
            stack_back.clear();
        }
    };

    /**
     * @brief Duy trì bitwise OR và AND trên cửa sổ trượt bằng mảng đếm tần số bit.
     * Độ phức tạp: O(BITS) mỗi thao tác add / remove / query.
     */
    template <int BITS = 62>
    class BitwiseWindow {
    private:
        int bit_cnt[BITS];
        int len;

    public:
        BitwiseWindow() : len(0) {
            fill(begin(bit_cnt), end(bit_cnt), 0);
        }

        void add(long long x) {
            for (int b = 0; b < BITS; ++b) {
                if ((x >> b) & 1LL) {
                    bit_cnt[b]++;
                }
            }
            len++;
        }

        void remove(long long x) {
            assert(len > 0 && "Cannot remove from empty BitwiseWindow!");
            for (int b = 0; b < BITS; ++b) {
                if ((x >> b) & 1LL) {
                    bit_cnt[b]--;
                }
            }
            len--;
        }

        long long get_or() const {
            long long res = 0;
            for (int b = 0; b < BITS; ++b) {
                if (bit_cnt[b] > 0) {
                    res |= (1LL << b);
                }
            }
            return res;
        }

        long long get_and() const {
            if (len == 0) return 0;
            long long res = 0;
            for (int b = 0; b < BITS; ++b) {
                if (bit_cnt[b] == len) {
                    res |= (1LL << b);
                }
            }
            return res;
        }

        int size() const { return len; }
        void clear() {
            fill(begin(bit_cnt), end(bit_cnt), 0);
            len = 0;
        }
    };

    /**
     * @brief Duy trì Trung vị động (Running Median) và Tổng khoảng cách L1 trên cửa sổ trượt.
     * Sử dụng 2 Multisets cân bằng.
     * Độ phức tạp: O(log W) cho add/remove, O(1) cho get_median và get_l1_cost.
     */
    class SlidingWindowMedian {
    private:
        multiset<long long> left_set;  // Nửa dưới (chứa ceil(N / 2) phần tử)
        multiset<long long> right_set; // Nửa trên (chứa floor(N / 2) phần tử)
        long long sum_left;
        long long sum_right;

        void balance() {
            // Đảm bảo mọi phần tử trong left_set <= mọi phần tử trong right_set
            while (!left_set.empty() && !right_set.empty() && *left_set.rbegin() > *right_set.begin()) {
                auto it_left = prev(left_set.end());
                auto it_right = right_set.begin();
                long long val_left = *it_left;
                long long val_right = *it_right;

                sum_left += val_right - val_left;
                sum_right += val_left - val_right;

                left_set.erase(it_left);
                right_set.erase(it_right);
                left_set.insert(val_right);
                right_set.insert(val_left);
            }

            // Đảm bảo size(left_set) = size(right_set) hoặc size(right_set) + 1
            while (left_set.size() > right_set.size() + 1) {
                auto it = prev(left_set.end());
                long long val = *it;
                sum_left -= val;
                sum_right += val;
                left_set.erase(it);
                right_set.insert(val);
            }

            while (left_set.size() < right_set.size()) {
                auto it = right_set.begin();
                long long val = *it;
                sum_right -= val;
                sum_left += val;
                right_set.erase(it);
                left_set.insert(val);
            }
        }

    public:
        SlidingWindowMedian() : sum_left(0), sum_right(0) {}

        void add(long long x) {
            if (left_set.empty() || x <= *left_set.rbegin()) {
                left_set.insert(x);
                sum_left += x;
            } else {
                right_set.insert(x);
                sum_right += x;
            }
            balance();
        }

        void remove(long long x) {
            auto it_left = left_set.find(x);
            if (it_left != left_set.end()) {
                sum_left -= x;
                left_set.erase(it_left);
            } else {
                auto it_right = right_set.find(x);
                assert(it_right != right_set.end() && "Value not found in SlidingWindowMedian!");
                sum_right -= x;
                right_set.erase(it_right);
            }
            balance();
        }

        long long get_median() const {
            assert(!left_set.empty() && "Empty window has no median!");
            return *left_set.rbegin();
        }

        /**
         * @brief Tính tổng chi phí tối thiểu sum |A[i] - c| để đưa mọi phần tử về cùng một giá trị.
         * Đạt cực tiểu khi c = get_median().
         */
        long long get_l1_cost() const {
            if (left_set.empty()) return 0;
            long long med = get_median();
            long long cost_left = med * (long long)left_set.size() - sum_left;
            long long cost_right = sum_right - med * (long long)right_set.size();
            return cost_left + cost_right;
        }

        size_t size() const {
            return left_set.size() + right_set.size();
        }
    };

    /**
     * @brief Đếm số đoạn con liên tiếp có bitwise OR >= K trong O(N * 60).
     */
    long long countSubarraysOrAtLeastK(const vector<long long>& a, long long K) {
        int n = a.size();
        BitwiseWindow<62> window;
        long long count = 0;
        int L = 0;

        for (int R = 0; R < n; ++R) {
            window.add(a[R]);
            while (L <= R && window.get_or() >= K) {
                // Mọi đoạn con bắt đầu từ L đến R, R+1, ..., n-1 đều có OR >= K
                count += (n - R);
                window.remove(a[L]);
                L++;
            }
        }
        return count;
    }

    /**
     * @brief Đếm số đoạn con liên tiếp có GCD == 1 sử dụng Two Pointers + SWAG.
     * Độ phức tạp: O(N * log(MAX_A)).
     */
    long long countSubarraysWithGCD1(const vector<long long>& a) {
        int n = a.size();
        auto gcd_op = [](long long x, long long y) { return std::gcd(x, y); };
        SWAG<long long, decltype(gcd_op)> swag(gcd_op);

        long long count = 0;
        int L = 0;

        for (int R = 0; R < n; ++R) {
            swag.push(a[R]);
            while (L <= R && swag.query() == 1) {
                count += (n - R);
                swag.pop();
                L++;
            }
        }
        return count;
    }
}

void run_unit_tests() {
    using namespace AdvancedTwoPointers;

    // 1. Kiểm thử SWAG với GCD
    {
        auto gcd_op = [](int a, int b) { return std::gcd(a, b); };
        SWAG<int, decltype(gcd_op)> swag_gcd(gcd_op);

        swag_gcd.push(12);
        swag_gcd.push(18);
        assert(swag_gcd.query() == 6);

        swag_gcd.push(24);
        assert(swag_gcd.query() == 6);

        swag_gcd.pop(); // Loại bỏ 12 -> còn 18, 24
        assert(swag_gcd.query() == 6);

        swag_gcd.push(9); // 18, 24, 9 -> gcd = 3
        assert(swag_gcd.query() == 3);

        swag_gcd.pop(); // Loại bỏ 18 -> còn 24, 9 -> gcd = 3
        assert(swag_gcd.query() == 3);

        swag_gcd.pop(); // Loại bỏ 24 -> còn 9 -> gcd = 9
        assert(swag_gcd.query() == 9);
    }

    // 2. Kiểm thử SWAG với Min/Max
    {
        auto min_op = [](int a, int b) { return std::min(a, b); };
        SWAG<int, decltype(min_op)> swag_min(min_op);

        swag_min.push(5);
        swag_min.push(2);
        swag_min.push(8);
        assert(swag_min.query() == 2);

        swag_min.pop(); // loại 5
        assert(swag_min.query() == 2);

        swag_min.pop(); // loại 2
        assert(swag_min.query() == 8);
    }

    // 3. Kiểm thử BitwiseWindow
    {
        BitwiseWindow<30> bit_win;
        bit_win.add(1);  // 001
        bit_win.add(2);  // 010
        assert(bit_win.get_or() == 3);
        assert(bit_win.get_and() == 0);

        bit_win.add(3);  // 011
        assert(bit_win.get_or() == 3);
        assert(bit_win.get_and() == 0);

        bit_win.remove(1); // còn 2, 3 -> and = 2, or = 3
        assert(bit_win.get_or() == 3);
        assert(bit_win.get_and() == 2);
    }

    // 4. Kiểm thử SlidingWindowMedian & L1 Cost
    {
        SlidingWindowMedian med;
        vector<long long> vals = {2, 4, 3, 5, 1};
        for (long long x : vals) med.add(x);

        // Tập hợp đã sắp xếp: {1, 2, 3, 4, 5} -> trung vị là 3
        assert(med.get_median() == 3);
        // Cost: |1-3| + |2-3| + |3-3| + |4-3| + |5-3| = 2 + 1 + 0 + 1 + 2 = 6
        assert(med.get_l1_cost() == 6);

        med.remove(1); // Còn {2, 3, 4, 5} -> size chẵn = 4, ceil(4/2) = 2 -> med = 3
        assert(med.get_median() == 3);
        // Cost: |2-3| + |3-3| + |4-3| + |5-3| = 1 + 0 + 1 + 2 = 4
        assert(med.get_l1_cost() == 4);
    }

    // 5. Kiểm thử countSubarraysWithGCD1
    {
        vector<long long> a = {2, 3, 6};
        // Các đoạn con:
        // [2] -> 2
        // [3] -> 3
        // [6] -> 6
        // [2, 3] -> 1
        // [3, 6] -> 3
        // [2, 3, 6] -> 1
        // Tổng số đoạn con có GCD = 1 là 2 ([2, 3] và [2, 3, 6])
        assert(countSubarraysWithGCD1(a) == 2);
    }

    // 6. Kiểm thử countSubarraysOrAtLeastK
    {
        vector<long long> a = {1, 2, 4};
        // [1] = 1, [2] = 2, [4] = 4
        // [1, 2] = 3, [2, 4] = 6, [1, 2, 4] = 7
        // Với K = 4: [4] (>=4), [2, 4] (6 >= 4), [1, 2, 4] (7 >= 4) -> tổng 3 đoạn
        assert(countSubarraysOrAtLeastK(a, 4) == 3);
    }

    cout << "✅ [AdvancedTwoPointers] Tất cả các Unit Tests đều vượt qua thành công!\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_unit_tests();
    return 0;
}
