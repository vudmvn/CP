/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Tham lam (Greedy Algorithms) trong Competitive Programming (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <numeric>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. THAM LAM TRÊN KHOẢNG (INTERVAL SCHEDULING & PARTITIONING)
// ==============================================================================
namespace IntervalGreedy {

    struct Interval {
        long long l, r;
    };

    /**
     * @brief Chọn số lượng đoạn không giao nhau nhiều nhất
     * @details Chiến lược: Sắp xếp theo đầu mút phải r tăng dần O(N log N)
     */
    int maxNonOverlapping(vector<Interval> intervals) {
        if (intervals.empty()) return 0;
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            if (a.r != b.r) return a.r < b.r;
            return a.l < b.l;
        });

        int count = 0;
        long long last_end = -4e18;
        for (const auto& iv : intervals) {
            if (iv.l >= last_end) {
                count++;
                last_end = iv.r;
            }
        }
        return count;
    }

    /**
     * @brief Tìm số đoạn ít nhất để phủ kín đoạn mục tiêu [target_L, target_R]
     * @return Số đoạn nhỏ nhất, hoặc -1 nếu không thể phủ kín
     */
    int minCoverIntervals(vector<Interval> intervals, long long target_L, long long target_R) {
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            if (a.l != b.l) return a.l < b.l;
            return a.r > b.r;
        });

        int count = 0;
        long long cur_end = target_L;
        int n = intervals.size();
        int i = 0;

        while (cur_end < target_R) {
            long long max_reach = -4e18;
            bool found = false;
            while (i < n && intervals[i].l <= cur_end) {
                max_reach = max(max_reach, intervals[i].r);
                found = true;
                i++;
            }
            if (!found || max_reach <= cur_end) {
                return -1; // Không thể nối tiếp phủ kín
            }
            cur_end = max_reach;
            count++;
        }
        return count;
    }

    /**
     * @brief Tìm số phòng họp / máy tối thiểu để phục vụ tất cả các cuộc họp
     * @details Áp dụng kỹ thuật Min-Heap / Quét dòng sự kiện O(N log N)
     */
    int minRoomsRequired(vector<Interval> intervals) {
        if (intervals.empty()) return 0;
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            return a.l < b.l;
        });

        priority_queue<long long, vector<long long>, greater<long long>> min_heap;
        for (const auto& iv : intervals) {
            if (!min_heap.empty() && min_heap.top() <= iv.l) {
                min_heap.pop(); // Tái sử dụng phòng họp đã kết thúc
            }
            min_heap.push(iv.r);
        }
        return min_heap.size();
    }
}

// ==============================================================================
// 2. THAM LAM VỚI HÀM SO SÁNH TỰ ĐỊNH NGHĨA (CUSTOM COMPARATOR & SMITH'S RULE)
// ==============================================================================
namespace ComparatorGreedy {

    /**
     * @brief Ghép các số nguyên thành một số lớn nhất theo thứ tự từ điển
     * @details So sánh (a + b) > (b + a)
     */
    string largestNumber(vector<string> nums) {
        if (nums.empty()) return "0";
        sort(nums.begin(), nums.end(), [](const string& a, const string& b) {
            return (a + b) > (b + a);
        });
        if (nums[0] == "0") return "0";
        string res = "";
        for (const string& s : nums) res += s;
        return res;
    }

    struct Task {
        long long time_cost;
        long long weight;
    };

    /**
     * @brief Lập lịch công việc giảm thiểu tổng thời gian chờ có trọng số sum(w_i * C_i)
     * @details Quy tắc Smith: Sắp xếp giảm dần theo tỷ số weight / time_cost
     */
    long long minWeightedCompletionTime(vector<Task> tasks) {
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            // a.weight / a.time_cost > b.weight / b.time_cost <=> a.weight * b.time_cost > b.weight * a.time_cost
            return (__int128)a.weight * b.time_cost > (__int128)b.weight * a.time_cost;
        });

        long long current_time = 0;
        long long total_cost = 0;
        for (const auto& t : tasks) {
            current_time += t.time_cost;
            total_cost += t.weight * current_time;
        }
        return total_cost;
    }
}

// ==============================================================================
// 3. THAM LAM CÓ HỐI HẬN (BUY-SELL WITH REGRET / UNDO GREEDY)
// ==============================================================================
namespace RegretGreedy {

    /**
     * @brief Bài toán Potions (Codeforces 1526C): Uống được nhiều lọ thuốc nhất sao cho máu >= 0
     * @details Khi máu < 0, "hối hận" bỏ lọ thuốc âm nặng nhất từng uống
     */
    int solvePotions(const vector<long long>& a) {
        long long cur_health = 0;
        priority_queue<long long, vector<long long>, greater<long long>> min_heap; // Lưu các giá trị âm

        for (long long x : a) {
            cur_health += x;
            if (x < 0) {
                min_heap.push(x);
            }
            if (cur_health < 0) {
                // Hối hận: Bỏ lọ thuốc âm lớn nhất
                cur_health -= min_heap.top();
                min_heap.pop();
            }
        }
        // Tổng số lọ = (số lọ dương ban đầu) + (số lọ âm được giữ lại trong heap)
        int positive_count = 0;
        for (long long x : a) if (x >= 0) positive_count++;
        return positive_count + min_heap.size();
    }

    /**
     * @brief Mua bán cổ phiếu tối đa hóa lợi nhuận với số lần giao dịch không giới hạn
     * @details Buy Low Sell High với kỹ thuật hoàn tác Priority Queue O(N log N)
     */
    long long maxProfitStock(const vector<long long>& prices) {
        long long total_profit = 0;
        priority_queue<long long, vector<long long>, greater<long long>> min_heap;

        for (long long p : prices) {
            if (!min_heap.empty() && min_heap.top() < p) {
                total_profit += p - min_heap.top();
                min_heap.pop();
                min_heap.push(p); // Đẩy lại giá hiện tại cho phép "nối dài" chuỗi mua bán nếu giá sau cao hơn
            }
            min_heap.push(p);
        }
        return total_profit;
    }
}

// ==============================================================================
// 4. THAM LAM HAI CON TRỎ (TWO POINTERS GREEDY)
// ==============================================================================
namespace TwoPointersGreedy {

    /**
     * @brief Tìm số thuyền ít nhất để chở N người, mỗi thuyền chở tối đa 2 người và tổng cân nặng <= limit
     */
    int minBoats(vector<long long> weights, long long limit) {
        sort(weights.begin(), weights.end());
        int i = 0;
        int j = (int)weights.size() - 1;
        int boats = 0;

        while (i <= j) {
            if (weights[i] + weights[j] <= limit) {
                i++; // Ghép người nhẹ nhất với người nặng nhất
            }
            j--;     // Người nặng nhất luôn phải lên thuyền
            boats++;
        }
        return boats;
    }
}

// ==============================================================================
// 5. KIỂM THỬ ĐƠN VỊ & CHẠY MẪU (UNIT TESTS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: Interval Scheduling (Max non-overlapping)
    vector<IntervalGreedy::Interval> ivs = {{1, 3}, {2, 5}, {4, 7}, {6, 9}, {8, 10}};
    int max_iv = IntervalGreedy::maxNonOverlapping(ivs);
    // Chọn {1, 3}, {4, 7}, {8, 10} -> 3 đoạn
    assert(max_iv == 3);
    cout << "[TEST 1 PASSED] Max Non-overlapping Intervals: " << max_iv << "\n";

    // Test 2: Largest Number Concatenation
    vector<string> nums = {"3", "30", "34", "5", "9"};
    string l_num = ComparatorGreedy::largestNumber(nums);
    assert(l_num == "9534330");
    cout << "[TEST 2 PASSED] Largest Number: " << l_num << "\n";

    // Test 3: Potions with Regret Greedy
    vector<long long> potions = {4, -4, 1, -3, 1, -3};
    int max_potions = RegretGreedy::solvePotions(potions);
    assert(max_potions == 5);
    cout << "[TEST 3 PASSED] Max Potions Drank: " << max_potions << "\n";

    // Test 4: Two Pointers Boats
    vector<long long> people = {3, 2, 2, 1};
    int boats = TwoPointersGreedy::minBoats(people, 3);
    // Ghép: (1, 2), (2), (3) -> 3 thuyền
    assert(boats == 3);
    cout << "[TEST 4 PASSED] Min Boats: " << boats << "\n";

    cout << "\n==> All Greedy Algorithm tests passed successfully!\n";
    return 0;
}
