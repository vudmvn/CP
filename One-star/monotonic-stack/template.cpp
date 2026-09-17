/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Ngăn xếp (Stack) & Ngăn xếp Đơn điệu (Monotone Stack)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

// ============================================================================
// 1. CÁC THUẬT TOÁN BIÊN KINH ĐIỂN CỦA MONOTONE STACK
// ============================================================================

namespace MonotonicStack {

    /**
     * @brief Tìm chỉ số phần tử nhỏ hơn gần nhất ở bên trái (Previous Smaller Element - PSE)
     * @return Vector chỉ số j < i lớn nhất sao cho a[j] < a[i], hoặc -1 nếu không tồn tại
     */
    template<typename T>
    vector<int> findPSE(const vector<T>& a) {
        int n = a.size();
        vector<int> pse(n);
        stack<int> st; // Increasing Stack lưu index

        for (int i = 0; i < n; ++i) {
            while (!st.empty() && a[st.top()] >= a[i]) {
                st.pop();
            }
            pse[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }
        return pse;
    }

    /**
     * @brief Tìm chỉ số phần tử nhỏ hơn gần nhất ở bên phải (Next Smaller Element - NSE)
     * @return Vector chỉ số j > i nhỏ nhất sao cho a[j] < a[i], hoặc n nếu không tồn tại
     */
    template<typename T>
    vector<int> findNSE(const vector<T>& a) {
        int n = a.size();
        vector<int> nse(n);
        stack<int> st; // Increasing Stack lưu index

        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && a[st.top()] >= a[i]) {
                st.pop();
            }
            nse[i] = st.empty() ? n : st.top();
            st.push(i);
        }
        return nse;
    }

    /**
     * @brief Tìm chỉ số phần tử lớn hơn gần nhất ở bên trái (Previous Greater Element - PGE)
     * @return Vector chỉ số j < i lớn nhất sao cho a[j] > a[i], hoặc -1 nếu không tồn tại
     */
    template<typename T>
    vector<int> findPGE(const vector<T>& a) {
        int n = a.size();
        vector<int> pge(n);
        stack<int> st; // Decreasing Stack lưu index

        for (int i = 0; i < n; ++i) {
            while (!st.empty() && a[st.top()] <= a[i]) {
                st.pop();
            }
            pge[i] = st.empty() ? -1 : st.top();
            st.push(i);
        }
        return pge;
    }

    /**
     * @brief Tìm chỉ số phần tử lớn hơn gần nhất ở bên phải (Next Greater Element - NGE)
     * @return Vector chỉ số j > i nhỏ nhất sao cho a[j] > a[i], hoặc n nếu không tồn tại
     */
    template<typename T>
    vector<int> findNGE(const vector<T>& a) {
        int n = a.size();
        vector<int> nge(n);
        stack<int> st; // Decreasing Stack lưu index

        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && a[st.top()] <= a[i]) {
                st.pop();
            }
            nge[i] = st.empty() ? n : st.top();
            st.push(i);
        }
        return nge;
    }

    // ============================================================================
    // 2. ỨNG DỤNG HÌNH HỌC & DIỆN TÍCH (HISTOGRAM & 2D MATRIX)
    // ============================================================================

    /**
     * @brief Tìm diện tích hình chữ nhật lớn nhất trong biểu đồ cột (Histogram) trong O(N)
     * @param h Vector chiều cao các cột
     */
    long long largestRectangleHistogram(const vector<long long>& h) {
        int n = h.size();
        stack<int> st;
        long long max_area = 0;

        for (int i = 0; i <= n; ++i) {
            // Lính canh chiều cao 0 ở cuối mảng để pop toàn bộ stack
            long long cur_h = (i == n) ? 0 : h[i];

            while (!st.empty() && h[st.top()] > cur_h) {
                int mid = st.top();
                st.pop();
                long long height = h[mid];
                long long width = st.empty() ? i : (i - st.top() - 1);
                max_area = max(max_area, height * width);
            }
            st.push(i);
        }
        return max_area;
    }

    /**
     * @brief Tìm diện tích hình chữ nhật lớn nhất chỉ chứa số 1 trong ma trận nhị phân 2D
     * Độ phức tạp: O(N * M)
     */
    long long maximalRectangleBinaryMatrix(const vector<vector<int>>& mat) {
        if (mat.empty() || mat[0].empty()) return 0;
        int n = mat.size(), m = mat[0].size();
        vector<long long> height(m, 0);
        long long max_area = 0;

        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                height[c] = (mat[r][c] == 1) ? (height[c] + 1) : 0;
            }
            max_area = max(max_area, largestRectangleHistogram(height));
        }
        return max_area;
    }

    // ============================================================================
    // 3. TÍNH TỔNG ĐÓNG GÓP MẢNG CON (SUBARRAY CONTRIBUTIONS)
    // ============================================================================

    /**
     * @brief Tính tổng giá trị nhỏ nhất của tất cả các mảng con liên tiếp trong O(N)
     * Áp dụng kỹ thuật phân tách dấu > và >= để triệt tiêu đếm trùng lặp
     */
    long long sumSubarrayMins(const vector<int>& a, int mod = 1e9 + 7) {
        int n = a.size();
        vector<int> left(n), right(n);
        stack<int> st;

        // Trái: tìm phần tử nhỏ hơn ngặt (strictly smaller: >)
        for (int i = 0; i < n; ++i) {
            while (!st.empty() && a[st.top()] > a[i]) st.pop();
            left[i] = st.empty() ? (i + 1) : (i - st.top());
            st.push(i);
        }

        while (!st.empty()) st.pop();

        // Phải: tìm phần tử nhỏ hơn hoặc bằng (smaller or equal: >=)
        for (int i = n - 1; i >= 0; --i) {
            while (!st.empty() && a[st.top()] >= a[i]) st.pop();
            right[i] = st.empty() ? (n - i) : (st.top() - i);
            st.push(i);
        }

        long long total_sum = 0;
        for (int i = 0; i < n; ++i) {
            long long ways = (1LL * left[i] * right[i]) % mod;
            total_sum = (total_sum + ways * a[i]) % mod;
        }
        return total_sum;
    }

} // namespace MonotonicStack

// ============================================================================
// 4. CHƯƠNG TRÌNH KIỂM THỬ (TEST DRIVER)
// ============================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== KIỂM THỬ MONOTONE STACK C++20 ===\n\n";

    // 1. Kiểm thử PSE và NSE
    vector<int> a = {2, 1, 5, 6, 2, 3};
    auto pse = MonotonicStack::findPSE(a);
    auto nse = MonotonicStack::findNSE(a);

    cout << "Mảng mẫu a: [2, 1, 5, 6, 2, 3]\n";
    cout << "PSE (chỉ số phần tử nhỏ hơn gần nhất bên trái): ";
    for (int x : pse) cout << x << " ";
    cout << "\n(Kỳ vọng: -1 -1 1 2 1 4)\n";

    cout << "NSE (chỉ số phần tử nhỏ hơn gần nhất bên phải): ";
    for (int x : nse) cout << x << " ";
    cout << "\n(Kỳ vọng: 1 6 4 4 6 6)\n\n";

    // 2. Kiểm thử Diện tích lớn nhất trong Histogram
    vector<long long> hist = {2, 1, 5, 6, 2, 3};
    long long max_rect = MonotonicStack::largestRectangleHistogram(hist);
    cout << "2. Diện tích hình chữ nhật lớn nhất trong Histogram [2, 1, 5, 6, 2, 3]: " 
         << max_rect << " (kỳ vọng: 10)\n\n";
    assert(max_rect == 10);

    // 3. Kiểm thử Maximal Rectangle trong Ma trận nhị phân 2D
    vector<vector<int>> mat = {
        {1, 0, 1, 0, 0},
        {1, 0, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0}
    };
    long long max_mat_rect = MonotonicStack::maximalRectangleBinaryMatrix(mat);
    cout << "3. Diện tích hình chữ nhật số 1 lớn nhất trong ma trận: " 
         << max_mat_rect << " (kỳ vọng: 6)\n\n";
    assert(max_mat_rect == 6);

    // 4. Kiểm thử Tổng cực tiểu mảng con (Sum of Subarray Minimums)
    vector<int> sub_arr = {3, 1, 2, 4};
    // Các mảng con: [3]->3, [1]->1, [2]->2, [4]->4, [3,1]->1, [1,2]->1, [2,4]->2, [3,1,2]->1, [1,2,4]->1, [3,1,2,4]->1
    // Tổng = 3 + 1 + 2 + 4 + 1 + 1 + 2 + 1 + 1 + 1 = 17
    long long sum_mins = MonotonicStack::sumSubarrayMins(sub_arr);
    cout << "4. Tổng cực tiểu mọi mảng con của [3, 1, 2, 4]: " 
         << sum_mins << " (kỳ vọng: 17)\n";
    assert(sum_mins == 17);

    return 0;
}
