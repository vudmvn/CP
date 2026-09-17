/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Kỹ thuật Thiết kế Thuật toán — Phân rã, Quy đổi & Biến đổi Tương đương
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stack>
#include <cstdint>
#include <cassert>

using namespace std;

// ============================================================================
// 1. BIẾN ĐỔI TỌA ĐỘ MANHATTAN SANG CHEBYSHEV (XOAY 45 ĐỘ) O(N)
// ============================================================================

namespace GeometryReduction {

    /**
     * @brief Tìm khoảng cách Manhattan lớn nhất giữa N điểm trong O(N)
     * Quy đổi: (x, y) -> (x + y, x - y)
     * max(|x1 - x2| + |y1 - y2|) = max(max(x') - min(x'), max(y') - min(y'))
     */
    long long maxManhattanDistance(const vector<pair<long long, long long>>& points) {
        assert(!points.empty());
        long long min_u = 4e18, max_u = -4e18;
        long long min_v = 4e18, max_v = -4e18;

        for (const auto& [x, y] : points) {
            long long u = x + y;
            long long v = x - y;
            min_u = min(min_u, u);
            max_u = max(max_u, u);
            min_v = min(min_v, v);
            max_v = max(max_v, v);
        }

        return max(max_u - min_u, max_v - min_v);
    }

    /**
     * @brief Tìm tọa độ điểm hội tụ tối ưu (tối thiểu hóa tổng khoảng cách Manhattan)
     * Phân rã bài toán 2D thành hai bài toán 1D độc lập (Trung vị)
     */
    pair<long long, long long> optimalMeetingPoint(vector<long long> x, vector<long long> y) {
        int n = x.size();
        assert(n > 0);
        nth_element(x.begin(), x.begin() + n / 2, x.end());
        nth_element(y.begin(), y.begin() + n / 2, y.end());
        return {x[n / 2], y[n / 2]};
    }
}

// ============================================================================
// 2. PHÂN RÃ ĐỘC LẬP TỪNG BIT (BITWISE INDEPENDENCE)
// ============================================================================

namespace BitwiseDecomposition {

    /**
     * @brief Tính tổng XOR của mọi cặp phần tử: sum_{i < j} (a[i] ^ a[j]) trong O(60 * N)
     */
    long long sumOfXORAllPairs(const vector<long long>& a) {
        int n = a.size();
        long long totalSum = 0;

        for (int bit = 0; bit < 62; ++bit) {
            long long count1 = 0;
            for (long long x : a) {
                if ((x >> bit) & 1) {
                    ++count1;
                }
            }
            long long count0 = n - count1;
            long long pairs = count1 * count0;
            totalSum += pairs * (1ULL << bit);
        }
        return totalSum;
    }
}

// ============================================================================
// 3. KỸ THUẬT TÍNH ĐÓNG GÓP (CONTRIBUTION TO THE SUM)
// ============================================================================

namespace ContributionTechnique {

    /**
     * @brief Tính tổng giá trị nhỏ nhất của mọi mảng con trong O(N)
     * Đóng góp của A[i] = A[i] * (L_i * R_i)
     */
    long long sumSubarrayMinimums(const vector<long long>& a, long long MOD = 1000000007LL) {
        int n = a.size();
        vector<int> left(n), right(n);
        stack<int> st;

        // Tìm L_i (phần tử nhỏ hơn nghiêm ngặt bên trái)
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && a[st.top()] > a[i]) {
                st.pop();
            }
            left[i] = st.empty() ? (i + 1) : (i - st.top());
            st.push(i);
        }

        while (!st.empty()) st.pop();

        // Tìm R_i (phần tử nhỏ hơn hoặc bằng bên phải để tránh đếm trùng)
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && a[st.top()] >= a[i]) {
                st.pop();
            }
            right[i] = st.empty() ? (n - i) : (st.top() - i);
            st.push(i);
        }

        long long total = 0;
        for (int i = 0; i < n; ++i) {
            long long count = ((long long)left[i] * right[i]) % MOD;
            long long contrib = (count * (a[i] % MOD)) % MOD;
            total = (total + contrib) % MOD;
        }
        return (total + MOD) % MOD;
    }
}

// ============================================================================
// 4. BIẾN ĐỔI NHỊ PHÂN HÓA (MẢNG {-1, +1}) ĐỂ KIỂM TRA TRUNG VỊ
// ============================================================================

namespace MedianBinaryTransformation {

    /**
     * @brief Kiểm tra xem có tồn tại mảng con độ dài >= k có trung vị >= target hay không
     * Quy đổi: A[i] >= target -> +1, A[i] < target -> -1
     * Bài toán quy về: Có mảng con độ dài >= k có tổng >= 0 hay không trong O(N)
     */
    bool checkSubarrayMedianAtLeast(const vector<long long>& a, int k, long long target) {
        int n = a.size();
        vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            b[i] = (a[i] >= target) ? 1 : -1;
        }

        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            pref[i + 1] = pref[i] + b[i];
        }

        // Cần tìm cặp (i, j) sao cho j - i >= k và pref[j] - pref[i] >= 0
        // Tương đương: pref[j] >= min_{0 <= i <= j - k} pref[i]
        int min_pref = 1e9;
        for (int j = k; j <= n; ++j) {
            min_pref = min(min_pref, pref[j - k]);
            if (pref[j] - min_pref >= 0) {
                return true;
            }
        }
        return false;
    }
}

// ============================================================================
// 5. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    // 1. Test Max Manhattan Distance (Xoay 45 độ)
    vector<pair<long long, long long>> pts = {
        {1, 1}, {2, 5}, {6, 3}, {8, 9}
    };
    // Khoảng cách giữa (1,1) và (8,9) là |1-8| + |1-9| = 7 + 8 = 15
    long long max_d = GeometryReduction::maxManhattanDistance(pts);
    assert(max_d == 15);

    // 2. Test Optimal Meeting Point
    vector<long long> xs = {1, 2, 8};
    vector<long long> ys = {1, 5, 9};
    auto meeting = GeometryReduction::optimalMeetingPoint(xs, ys);
    assert(meeting.first == 2 && meeting.second == 5);

    // 3. Test Bitwise XOR Sum
    vector<long long> a = {1, 2, 3}; // (1^2) + (1^3) + (2^3) = 3 + 2 + 1 = 6
    long long xor_sum = BitwiseDecomposition::sumOfXORAllPairs(a);
    assert(xor_sum == 6);

    // 4. Test Contribution Technique (Sum of Subarray Mins)
    vector<long long> arr = {3, 1, 2, 4};
    // Mảng con: [3]->3, [1]->1, [2]->2, [4]->4, [3,1]->1, [1,2]->1, [2,4]->2, [3,1,2]->1, [1,2,4]->1, [3,1,2,4]->1
    // Tổng = 3 + 1 + 2 + 4 + 1 + 1 + 2 + 1 + 1 + 1 = 17
    long long sum_mins = ContributionTechnique::sumSubarrayMinimums(arr);
    assert(sum_mins == 17);

    // 5. Test Median Binary Transformation (Mảng {-1, +1})
    vector<long long> med_arr = {1, 3, 5, 2, 4};
    // Đoạn [3, 5, 2] có độ dài 3 >= 3, sắp xếp là {2, 3, 5} -> trung vị là 3 >= 3
    assert(MedianBinaryTransformation::checkSubarrayMedianAtLeast(med_arr, 3, 3) == true);
    // Không có đoạn nào độ dài >= 3 có trung vị >= 5
    assert(MedianBinaryTransformation::checkSubarrayMedianAtLeast(med_arr, 3, 5) == false);

    cout << "==> ALL PROBLEM REDUCTION & DECOMPOSITION SELF-TESTS PASSED!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
