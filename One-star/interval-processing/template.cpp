/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Xử lý Đoạn & Khoảng trên Đường thẳng (1D Interval Processing) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <tuple>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. CÁC THUẬT TOÁN XỬ LÝ ĐOẠN TĨNH (STATIC INTERVAL ALGORITHMS)
// ==============================================================================
namespace IntervalAlgorithms {

    struct Interval {
        long long l, r;
        int id = -1;
    };

    /**
     * @brief Gộp các đoạn giao nhau thành các đoạn rời rạc trong O(N log N)
     */
    vector<Interval> mergeIntervals(vector<Interval> intervals) {
        if (intervals.empty()) return {};
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            if (a.l != b.l) return a.l < b.l;
            return a.r < b.r;
        });

        vector<Interval> merged;
        long long cur_l = intervals[0].l;
        long long cur_r = intervals[0].r;

        for (size_t i = 1; i < intervals.size(); ++i) {
            if (intervals[i].l <= cur_r) {
                cur_r = max(cur_r, intervals[i].r);
            } else {
                merged.push_back({cur_l, cur_r, -1});
                cur_l = intervals[i].l;
                cur_r = intervals[i].r;
            }
        }
        merged.push_back({cur_l, cur_r, -1});
        return merged;
    }

    /**
     * @brief Tính tổng độ dài hợp các đoạn trong O(N log N)
     */
    long long unionLength(const vector<Interval>& intervals) {
        auto merged = mergeIntervals(intervals);
        long long total_len = 0;
        for (const auto& iv : merged) {
            total_len += (iv.r - iv.l);
        }
        return total_len;
    }

    /**
     * @brief Tìm số tầng phủ cực đại bằng kỹ thuật Dòng quét (1D Sweep-line)
     * @param is_closed true nếu là đoạn đóng [L, R], false nếu là nửa mở [L, R)
     */
    int maxOverlap(const vector<Interval>& intervals, bool is_closed = true) {
        // Sự kiện: {tọa độ x, loại sự kiện}
        // Với đoạn đóng: START (+1) ưu tiên hơn END (-1) khi trùng tọa độ
        // Với đoạn nửa mở: END (-1) ưu tiên hơn START (+1) khi trùng tọa độ
        vector<pair<long long, int>> events;
        events.reserve(intervals.size() * 2);

        for (const auto& iv : intervals) {
            events.push_back({iv.l, +1});
            events.push_back({iv.r, -1});
        }

        if (is_closed) {
            sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
                if (a.first != b.first) return a.first < b.first;
                return a.second > b.second; // +1 trước -1
            });
        } else {
            sort(events.begin(), events.end(), [](const auto& a, const auto& b) {
                if (a.first != b.first) return a.first < b.first;
                return a.second < b.second; // -1 trước +1
            });
        }

        int cur_layers = 0;
        int max_layers = 0;
        for (const auto& ev : events) {
            cur_layers += ev.second;
            max_layers = max(max_layers, cur_layers);
        }
        return max_layers;
    }

    /**
     * @brief Phủ đoạn [target_L, target_R] bằng số đoạn ít nhất trong O(N log N)
     * @return Số đoạn tối thiểu, hoặc -1 nếu không thể phủ kín
     */
    int minIntervalCover(vector<Interval> intervals, long long target_L, long long target_R) {
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
            if (!found || max_reach <= cur_end) return -1;
            cur_end = max_reach;
            count++;
        }
        return count;
    }

    /**
     * @brief Quy hoạch động trên đoạn có trọng số (Weighted Interval Scheduling)
     * @param jobs Danh sách {L, R, Weight}
     * @return Tổng trọng số lớn nhất của tập đoạn không giao nhau
     */
    long long weightedIntervalScheduling(vector<tuple<long long, long long, long long>> jobs) {
        if (jobs.empty()) return 0;
        // Sắp xếp theo R tăng dần
        sort(jobs.begin(), jobs.end(), [](const auto& a, const auto& b) {
            return get<1>(a) < get<1>(b);
        });

        int n = jobs.size();
        vector<long long> r_coords(n);
        for (int i = 0; i < n; ++i) r_coords[i] = get<1>(jobs[i]);

        vector<long long> dp(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            auto [l, r, weight] = jobs[i - 1];
            // Không chọn công việc i
            dp[i] = dp[i - 1];

            // Chọn công việc i: Tìm công việc p kết thúc <= l
            auto it = upper_bound(r_coords.begin(), r_coords.end(), l);
            int p = distance(r_coords.begin(), it); // 0-indexed count of jobs ending <= l
            dp[i] = max(dp[i], dp[p] + weight);
        }
        return dp[n];
    }

    /**
     * @brief Kiểm tra quan hệ đoạn chứa nhau (CSES 2168 Nested Ranges Check)
     * @param contains[i] = 1 nếu đoạn i chứa ít nhất một đoạn khác
     * @param is_contained[i] = 1 nếu đoạn i bị ít nhất một đoạn khác chứa nó
     */
    void checkNestedIntervals(vector<Interval> intervals, vector<bool>& contains, vector<bool>& is_contained) {
        int n = intervals.size();
        contains.assign(n, false);
        is_contained.assign(n, false);

        for (int i = 0; i < n; ++i) intervals[i].id = i;

        // Sắp xếp L tăng dần, nếu L bằng nhau thì R GIẢM DẦN
        sort(intervals.begin(), intervals.end(), [](const Interval& a, const Interval& b) {
            if (a.l != b.l) return a.l < b.l;
            return a.r > b.r;
        });

        // 1. Kiểm tra đoạn i có bị đoạn nào trước đó chứa không: R_prev >= R_cur
        long long max_r = -4e18;
        for (int i = 0; i < n; ++i) {
            if (intervals[i].r <= max_r) {
                is_contained[intervals[i].id] = true;
            }
            max_r = max(max_r, intervals[i].r);
        }

        // 2. Kiểm tra đoạn i có chứa đoạn nào phía sau không: R_next <= R_cur
        long long min_r = 4e18;
        for (int i = n - 1; i >= 0; --i) {
            if (intervals[i].r >= min_r) {
                contains[intervals[i].id] = true;
            }
            min_r = min(min_r, intervals[i].r);
        }
    }
}

// ==============================================================================
// 2. CẤU TRÚC DỮ LIỆU DUY TRÌ TẬP ĐOẠN RỜI RẠC ĐỘNG (DYNAMIC INTERVAL SET)
// ==============================================================================
class DynamicIntervalSet {
private:
    // Lưu các đoạn [L, R] rời rạc, sắp xếp theo L
    set<pair<long long, long long>> intervals;
    long long total_length = 0;

public:
    /**
     * @brief Chèn đoạn [l, r] vào tập hợp và tự động gộp các đoạn giao nhau
     * @details Độ phức tạp khấu hao: O(log N)
     */
    void insert(long long l, long long r) {
        if (l > r) return;
        auto it = intervals.lower_bound({l, -4e18});

        // Lùi lại 1 phần tử nếu đoạn trước có thể giao hoặc tiếp xúc với [l, r]
        if (it != intervals.begin()) {
            auto prev_it = prev(it);
            if (prev_it->second >= l) { // Tiếp xúc hoặc chồng lấn (đối với số thực)
                it = prev_it;
            }
        }

        // Gộp tất cả các đoạn chồng lấn
        while (it != intervals.end() && it->first <= r) {
            l = min(l, it->first);
            r = max(r, it->second);
            total_length -= (it->second - it->first);
            it = intervals.erase(it);
        }

        intervals.insert({l, r});
        total_length += (r - l);
    }

    /**
     * @brief Trả về tổng độ dài hợp các đoạn trong O(1)
     */
    long long getUnionLength() const {
        return total_length;
    }

    /**
     * @brief Kiểm tra xem điểm x có thuộc vào đoạn nào không trong O(log N)
     */
    bool containsPoint(long long x) const {
        auto it = intervals.upper_bound({x, 4e18});
        if (it == intervals.begin()) return false;
        --it;
        return (it->first <= x && x <= it->second);
    }

    int count() const {
        return intervals.size();
    }
};

// ==============================================================================
// 3. KIỂM THỬ ĐƠN VỊ & CHẠY MẪU (UNIT TESTS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: Merge Intervals & Union Length
    vector<IntervalAlgorithms::Interval> ivs = {{2, 6}, {1, 3}, {8, 10}, {15, 18}};
    auto merged = IntervalAlgorithms::mergeIntervals(ivs);
    assert(merged.size() == 3);
    assert(merged[0].l == 1 && merged[0].r == 6);
    long long u_len = IntervalAlgorithms::unionLength(ivs);
    assert(u_len == 10); // [1, 6] (5) + [8, 10] (2) + [15, 18] (3) = 10
    cout << "[TEST 1 PASSED] Merge Intervals size: " << merged.size() << ", Union Length: " << u_len << "\n";

    // Test 2: Max Overlap (Sweep-line)
    vector<IntervalAlgorithms::Interval> overlap_test = {{1, 5}, {3, 8}, {6, 9}};
    int max_ov = IntervalAlgorithms::maxOverlap(overlap_test);
    assert(max_ov == 2);
    cout << "[TEST 2 PASSED] Max Overlap: " << max_ov << "\n";

    // Test 3: Minimum Interval Cover
    vector<IntervalAlgorithms::Interval> cover_test = {{1, 4}, {3, 7}, {6, 10}, {8, 12}};
    int min_cov = IntervalAlgorithms::minIntervalCover(cover_test, 1, 10);
    assert(min_cov == 3); // {1, 4} -> {3, 7} -> {6, 10}
    cout << "[TEST 3 PASSED] Min Interval Cover: " << min_cov << "\n";

    // Test 4: Weighted Interval Scheduling
    vector<tuple<long long, long long, long long>> jobs = {
        {1, 3, 50}, {2, 5, 20}, {4, 6, 70}, {6, 7, 30}
    };
    long long max_profit = IntervalAlgorithms::weightedIntervalScheduling(jobs);
    assert(max_profit == 150); // {1, 3, 50} + {4, 6, 70} + {6, 7, 30} = 150
    cout << "[TEST 4 PASSED] Weighted Interval Scheduling: " << max_profit << "\n";

    // Test 5: Dynamic Interval Set
    DynamicIntervalSet dis;
    dis.insert(1, 4);
    assert(dis.getUnionLength() == 3);
    dis.insert(3, 7);
    assert(dis.getUnionLength() == 6); // [1, 7] -> length 6
    assert(dis.count() == 1);
    dis.insert(10, 15);
    assert(dis.getUnionLength() == 11);
    assert(dis.containsPoint(5) == true);
    assert(dis.containsPoint(8) == false);
    cout << "[TEST 5 PASSED] Dynamic Interval Set verified successfully!\n";

    cout << "\n==> All Interval Processing tests passed successfully!\n";
    return 0;
}
