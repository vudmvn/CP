/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Kỹ thuật Hai con trỏ & Cửa sổ trượt trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>

using namespace std;

// ==========================================
// 1. HAI CON TRỎ NGƯỢC CHIỀU: 2SUM TRÊN MẢNG ĐÃ SẮP XẾP O(N)
// ==========================================

/**
 * @brief Tìm cặp chỉ số (1-indexed) của 2 phần tử có tổng bằng target trên mảng ban đầu
 * @return pair<int, int> {chỉ số 1, chỉ số 2} hoặc {-1, -1} nếu không tồn tại
 * @complexity O(N log N) do sắp xếp ban đầu, duyệt 2 con trỏ tốn O(N)
 */
pair<int, int> two_sum_sorted(const vector<long long>& a, long long target) {
    int n = a.size();
    vector<pair<long long, int>> elements(n);
    for (int i = 0; i < n; ++i) {
        elements[i] = {a[i], i + 1}; // 1-indexed
    }
    sort(elements.begin(), elements.end());

    int L = 0, R = n - 1;
    while (L < R) {
        long long sum = elements[L].first + elements[R].first;
        if (sum == target) {
            return {elements[L].second, elements[R].second};
        }
        if (sum < target) L++;
        else R--;
    }
    return {-1, -1};
}

// ==========================================
// 2. HAI CON TRỎ NGƯỢC CHIỀU: 3SUM TÌM TẤT CẢ BỘ BA KHÔNG TRÙNG LẶP O(N^2)
// ==========================================

/**
 * @brief Tìm tất cả các bộ ba giá trị {a[i], a[j], a[k]} phân biệt có tổng bằng target
 * @complexity O(N^2)
 */
vector<vector<long long>> three_sum(vector<long long> a, long long target) {
    int n = a.size();
    sort(a.begin(), a.end());
    vector<vector<long long>> results;

    for (int i = 0; i < n - 2; ++i) {
        // Bỏ qua các phần tử trùng lặp ở vị trí thứ nhất
        if (i > 0 && a[i] == a[i - 1]) continue;

        int L = i + 1, R = n - 1;
        long long need = target - a[i];

        while (L < R) {
            long long sum = a[L] + a[R];
            if (sum == need) {
                results.push_back({a[i], a[L], a[R]});
                // Bỏ qua các phần tử trùng lặp ở L và R
                while (L < R && a[L] == a[L + 1]) L++;
                while (L < R && a[R] == a[R - 1]) R--;
                L++;
                R--;
            } else if (sum < need) {
                L++;
            } else {
                R--;
            }
        }
    }
    return results;
}

// ==========================================
// 3. CỬA SỔ TRƯỢT: ĐẾM SỐ ĐOẠN CON CÓ TỔNG <= K (SỐ KHÔNG ÂM) O(N)
// ==========================================

/**
 * @brief Đếm số đoạn con liên tiếp có tổng <= k (với a[i] >= 0)
 * @complexity O(N) thời gian, O(1) bộ nhớ
 */
long long count_subarrays_sum_at_most_k(const vector<long long>& a, long long k) {
    int n = a.size();
    long long ans = 0;
    long long cur_sum = 0;
    int L = 0;

    for (int R = 0; R < n; ++R) {
        cur_sum += a[R];
        // Thu hẹp cửa sổ khi tổng vượt quá k
        while (L <= R && cur_sum > k) {
            cur_sum -= a[L];
            L++;
        }
        // Tất cả các đoạn con kết thúc tại R và bắt đầu từ l in [L, R] đều hợp lệ
        ans += (R - L + 1);
    }
    return ans;
}

// ==========================================
// 4. CỬA SỔ TRƯỢT: ĐẾM SỐ ĐOẠN CON CÓ <= K PHẦN TỬ PHÂN BIỆT O(N)
// ==========================================

/**
 * @brief Đếm số đoạn con liên tiếp có không quá k phần tử phân biệt
 * @complexity O(N log N) nếu dùng map (hoặc O(N) nếu dùng mảng tĩnh / hash table)
 */
long long count_subarrays_at_most_k_distinct(const vector<int>& a, int k) {
    int n = a.size();
    long long ans = 0;
    map<int, int> freq;
    int distinct_count = 0;
    int L = 0;

    for (int R = 0; R < n; ++R) {
        if (freq[a[R]] == 0) distinct_count++;
        freq[a[R]]++;

        while (distinct_count > k) {
            freq[a[L]]--;
            if (freq[a[L]] == 0) distinct_count--;
            L++;
        }
        ans += (R - L + 1);
    }
    return ans;
}

// ==========================================
// 5. THÙNG CHỨA NHIỀU NƯỚC NHẤT (CONTAINER WITH MOST WATER) O(N)
// ==========================================

/**
 * @brief Tìm diện tích lớn nhất tạo bởi 2 cột thẳng đứng
 * @complexity O(N) thời gian, O(1) bộ nhớ
 */
long long container_with_most_water(const vector<long long>& height) {
    int L = 0, R = (int)height.size() - 1;
    long long max_area = 0;

    while (L < R) {
        long long h = min(height[L], height[R]);
        max_area = max(max_area, h * (R - L));
        // Luôn di chuyển thanh thấp hơn để tìm cơ hội có chiều cao lớn hơn
        if (height[L] < height[R]) L++;
        else R--;
    }
    return max_area;
}

// ==========================================
// 6. HAI CON TRỎ TRÊN 2 DÃY: TRỘN 2 MẢNG TĂNG DẦN O(N + M)
// ==========================================

vector<long long> merge_sorted_arrays(const vector<long long>& a, const vector<long long>& b) {
    int n = a.size(), m = b.size();
    vector<long long> res;
    res.reserve(n + m);

    int i = 0, j = 0;
    while (i < n && j < m) {
        if (a[i] <= b[j]) {
            res.push_back(a[i]);
            i++;
        } else {
            res.push_back(b[j]);
            j++;
        }
    }
    while (i < n) res.push_back(a[i++]);
    while (j < m) res.push_back(b[j++]);

    return res;
}

// ==========================================
// 7. MAIN DEMO & TEST CASES
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Demo 2Sum
    cout << "=== 1. DEMO 2SUM SORTED ===\n";
    vector<long long> arr1 = {2, 7, 5, 1};
    long long target1 = 8;
    auto p = two_sum_sorted(arr1, target1);
    cout << "Cap chi so co tong bang " << target1 << ": " << p.first << ", " << p.second << "\n\n";

    // 2. Demo 3Sum
    cout << "=== 2. DEMO 3SUM ===\n";
    vector<long long> arr2 = {-1, 0, 1, 2, -1, -4};
    auto triplets = three_sum(arr2, 0);
    cout << "Cac bo ba co tong bang 0:\n";
    for (auto &t : triplets) {
        cout << "[ " << t[0] << " " << t[1] << " " << t[2] << " ]\n";
    }
    cout << "\n";

    // 3. Demo Cửa sổ trượt đếm số đoạn con có tổng <= K
    cout << "=== 3. DEMO SUBARRAYS SUM <= K ===\n";
    vector<long long> arr3 = {1, 3, 2, 5, 1};
    long long k3 = 5;
    cout << "So doan con co tong <= " << k3 << ": " << count_subarrays_sum_at_most_k(arr3, k3) << "\n\n";

    // 4. Demo Container With Most Water
    cout << "=== 4. DEMO CONTAINER WITH MOST WATER ===\n";
    vector<long long> heights = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "Dien tich nuoc lon nhat: " << container_with_most_water(heights) << " (Ky vong: 49)\n\n";

    // 5. Demo Merge 2 sorted arrays
    cout << "=== 5. DEMO MERGE 2 SORTED ARRAYS ===\n";
    vector<long long> a = {1, 4, 7, 9};
    vector<long long> b = {2, 3, 6, 8, 10};
    auto merged = merge_sorted_arrays(a, b);
    cout << "Mang tron: ";
    for (auto x : merged) cout << x << " ";
    cout << "\n";

    return 0;
}
