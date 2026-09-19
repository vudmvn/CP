/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Xử lý Giá trị Tuyệt đối & Khoảng cách Manhattan (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. TỐI ƯU HÓA TRUNG VỊ 1 CHIỀU (1D MEDIAN & WEIGHTED MEDIAN)
// ==============================================================================
namespace MedianOptimizer {

    /**
     * @brief Tìm giá trị trung vị của một vector số nguyên trong O(N)
     * @note Sử dụng std::nth_element thay vì sắp xếp O(N log N)
     */
    long long getMedian(vector<long long> a) {
        if (a.empty()) return 0;
        int n = a.size();
        int mid = n / 2;
        nth_element(a.begin(), a.begin() + mid, a.end());
        return a[mid];
    }

    /**
     * @brief Tính tổng khoảng cách nhỏ nhất sum(|x - a_i|) trong O(N)
     */
    long long minTotalDistance(vector<long long> a) {
        if (a.empty()) return 0;
        long long med = getMedian(a);
        long long total_cost = 0;
        for (long long x : a) {
            total_cost += llabs(x - med);
        }
        return total_cost;
    }

    /**
     * @brief Tìm trung vị có trọng số (Weighted Median) trong O(N log N)
     * @param items Vector chứa các cặp {tọa độ a_i, trọng số w_i > 0}
     * @return Tọa độ a_k tối ưu
     */
    long long getWeightedMedian(vector<pair<long long, long long>> items) {
        if (items.empty()) return 0;
        sort(items.begin(), items.end(), [](const auto& p1, const auto& p2) {
            return p1.first < p2.first;
        });

        long long total_weight = 0;
        for (const auto& [coord, weight] : items) {
            total_weight += weight;
        }

        long long target = (total_weight + 1) / 2;
        long long current_weight = 0;
        for (const auto& [coord, weight] : items) {
            current_weight += weight;
            if (current_weight >= target) {
                return coord;
            }
        }
        return items.back().first;
    }
}

// ==============================================================================
// 2. TỐI ƯU HÓA KHOẢNG CÁCH MANHATTAN (2D & K-DIMENSIONAL)
// ==============================================================================
namespace ManhattanOptimizer {

    /**
     * @brief Tìm cặp điểm có khoảng cách Manhattan lớn nhất trong O(N)
     * @details Dựa trên phép xoay trục Chebyshev: |x1 - x2| + |y1 - y2| = max(|u1 - u2|, |v1 - v2|)
     *          với u = x + y, v = x - y.
     */
    long long maxManhattan2D(const vector<pair<long long, long long>>& pts) {
        if (pts.size() < 2) return 0;
        long long max_u = -4e18, min_u = 4e18;
        long long max_v = -4e18, min_v = 4e18;

        for (const auto& [x, y] : pts) {
            long long u = x + y;
            long long v = x - y;
            max_u = max(max_u, u);
            min_u = min(min_u, u);
            max_v = max(max_v, v);
            min_v = min(min_v, v);
        }

        return max(max_u - min_u, max_v - min_v);
    }

    /**
     * @brief Tìm điểm họp mặt tối ưu (Best Meeting Point) trong 2D Manhattan
     * @return Cặp tọa độ {x*, y*} tối ưu
     */
    pair<long long, long long> bestMeetingPoint2D(const vector<pair<long long, long long>>& pts) {
        if (pts.empty()) return {0, 0};
        vector<long long> xs, ys;
        xs.reserve(pts.size());
        ys.reserve(pts.size());
        for (const auto& [x, y] : pts) {
            xs.push_back(x);
            ys.push_back(y);
        }
        return {MedianOptimizer::getMedian(xs), MedianOptimizer::getMedian(ys)};
    }

    /**
     * @brief Tìm khoảng cách Manhattan lớn nhất giữa 2 điểm trong không gian d chiều
     * @details Độ phức tạp: O(N * 2^d) với kỹ thuật bitmask duyệt tổ hợp dấu.
     */
    long long maxManhattanKD(const vector<vector<long long>>& pts, int d) {
        if (pts.size() < 2) return 0;
        int total_masks = 1 << d;
        long long ans = 0;

        for (int mask = 0; mask < total_masks; ++mask) {
            long long max_val = -4e18;
            long long min_val = 4e18;

            for (const auto& pt : pts) {
                long long cur = 0;
                for (int i = 0; i < d; ++i) {
                    if ((mask >> i) & 1) cur += pt[i];
                    else cur -= pt[i];
                }
                max_val = max(max_val, cur);
                min_val = min(min_val, cur);
            }
            ans = max(ans, max_val - min_val);
        }
        return ans;
    }
}

// ==============================================================================
// 3. CẤU TRÚC DỮ LIỆU DUY TRÌ TRUNG VỊ ĐỘNG (DYNAMIC MEDIAN 2-HEAPS)
// ==============================================================================
class DynamicMedianManager {
private:
    priority_queue<long long> left_max_heap;                                // Chứa nửa phần tử nhỏ hơn
    priority_queue<long long, vector<long long>, greater<long long>> right_min_heap; // Chứa nửa phần tử lớn hơn
    unordered_map<long long, int> dead_count;                               // Kỹ thuật xóa lười (Lazy Deletion)
    long long sum_left = 0;
    long long sum_right = 0;
    int size_left = 0;
    int size_right = 0;

    void clean_left() {
        while (!left_max_heap.empty() && dead_count[left_max_heap.top()] > 0) {
            dead_count[left_max_heap.top()]--;
            left_max_heap.pop();
        }
    }

    void clean_right() {
        while (!right_min_heap.empty() && dead_count[right_min_heap.top()] > 0) {
            dead_count[right_min_heap.top()]--;
            right_min_heap.pop();
        }
    }

    void balance() {
        // Luôn giữ size_left == size_right hoặc size_left == size_right + 1
        while (size_left > size_right + 1) {
            clean_left();
            long long val = left_max_heap.top();
            left_max_heap.pop();
            sum_left -= val;
            size_left--;

            right_min_heap.push(val);
            sum_right += val;
            size_right++;
        }
        while (size_left < size_right) {
            clean_right();
            long long val = right_min_heap.top();
            right_min_heap.pop();
            sum_right -= val;
            size_right--;

            left_max_heap.push(val);
            sum_left += val;
            size_left++;
        }
    }

public:
    void insert(long long x) {
        clean_left();
        if (left_max_heap.empty() || x <= left_max_heap.top()) {
            left_max_heap.push(x);
            sum_left += x;
            size_left++;
        } else {
            right_min_heap.push(x);
            sum_right += x;
            size_right++;
        }
        balance();
    }

    void erase(long long x) {
        clean_left();
        clean_right();
        if (!left_max_heap.empty() && x <= left_max_heap.top()) {
            size_left--;
            sum_left -= x;
        } else {
            size_right--;
            sum_right -= x;
        }
        dead_count[x]++;
        balance();
    }

    long long getMedian() {
        clean_left();
        assert(!left_max_heap.empty());
        return left_max_heap.top();
    }

    /**
     * @brief Trả về tổng khoảng cách nhỏ nhất sum(|x* - a_i|) trong O(1)
     */
    long long getMinCost() {
        if (size_left + size_right == 0) return 0;
        long long med = getMedian();
        long long cost_left = med * size_left - sum_left;
        long long cost_right = sum_right - med * size_right;
        return cost_left + cost_right;
    }

    int size() const {
        return size_left + size_right;
    }
};

// ==============================================================================
// 4. KIỂM THỬ MẪU (UNIT TEST / MAIN)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: 1D Median Minimization
    vector<long long> arr = {7, 1, 9, 3, 5};
    long long med = MedianOptimizer::getMedian(arr);
    long long cost = MedianOptimizer::minTotalDistance(arr);
    assert(med == 5);
    assert(cost == 12);
    cout << "[TEST 1 PASSED] 1D Median: " << med << ", Cost: " << cost << "\n";

    // Test 2: 2D Manhattan Maximum Distance
    vector<pair<long long, long long>> points = {
        {1, 2}, {3, 8}, {5, 1}, {8, 9}
    };
    long long max_dist = ManhattanOptimizer::maxManhattan2D(points);
    // (1, 2) to (8, 9): |1-8| + |2-9| = 7 + 7 = 14
    // (5, 1) to (3, 8): |5-3| + |1-8| = 2 + 7 = 9
    assert(max_dist == 14);
    cout << "[TEST 2 PASSED] 2D Max Manhattan: " << max_dist << "\n";

    // Test 3: Dynamic Median 2-Heaps
    DynamicMedianManager dm;
    for (long long x : {2, 4, 3, 5, 8}) {
        dm.insert(x);
    }
    // Dãy: {2, 3, 4, 5, 8} -> Trung vị: 4
    assert(dm.getMedian() == 4);
    // Cost: |4-2| + |4-3| + |4-4| + |4-5| + |4-8| = 2 + 1 + 0 + 1 + 4 = 8
    assert(dm.getMinCost() == 8);

    dm.erase(4);
    // Dãy còn lại: {2, 3, 5, 8} -> Trung vị: 3 (hoặc 5 tùy quy ước)
    assert(dm.getMedian() == 3);
    cout << "[TEST 3 PASSED] Dynamic Median Cost: " << dm.getMinCost() << "\n";

    cout << "\n==> All test cases passed successfully!\n";
    return 0;
}
