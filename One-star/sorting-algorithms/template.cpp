/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Các Thuật toán Sắp xếp & Đếm Nghịch thế (Inversion Counting)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <random>
#include <chrono>
#include <cassert>

using namespace std;

// ============================================================================
// 1. CÁC THUẬT TOÁN SẮP XẾP CHUẨN MỰC
// ============================================================================

namespace Sorting {

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    /**
     * @brief Sắp xếp nhanh QuickSort với Pivot ngẫu nhiên (Randomized Pivot)
     * Tránh hoàn toàn trường hợp xấu nhất O(N^2)
     */
    template<typename T>
    void quickSort(vector<T>& a, int left, int right) {
        if (left >= right) return;

        // Chọn Pivot ngẫu nhiên
        int pivotIdx = left + rng() % (right - left + 1);
        swap(a[pivotIdx], a[right]);
        T pivot = a[right];

        int i = left;
        for (int j = left; j < right; ++j) {
            if (a[j] < pivot) {
                swap(a[i], a[j]);
                i++;
            }
        }
        swap(a[i], a[right]);

        quickSort(a, left, i - 1);
        quickSort(a, i + 1, right);
    }

    template<typename T>
    void quickSort(vector<T>& a) {
        if (!a.empty()) quickSort(a, 0, (int)a.size() - 1);
    }

    /**
     * @brief Sắp xếp trộn MergeSort (Thuật toán Ổn định - Stable Sort)
     * Độ phức tạp: O(N log N) trong mọi trường hợp
     */
    template<typename T>
    void mergeSort(vector<T>& a, vector<T>& temp, int left, int right) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(a, temp, left, mid);
        mergeSort(a, temp, mid + 1, right);

        int i = left, j = mid + 1, k = left;
        while (i <= mid && j <= right) {
            if (a[i] <= a[j]) temp[k++] = a[i++];
            else temp[k++] = a[j++];
        }
        while (i <= mid) temp[k++] = a[i++];
        while (j <= right) temp[k++] = a[j++];
        for (i = left; i <= right; ++i) a[i] = temp[i];
    }

    template<typename T>
    void mergeSort(vector<T>& a) {
        if (a.size() <= 1) return;
        vector<T> temp(a.size());
        mergeSort(a, temp, 0, (int)a.size() - 1);
    }

    /**
     * @brief Sắp xếp đếm phân phối Counting Sort trong O(N + K)
     * Thích hợp khi miền giá trị [minVal, maxVal] nhỏ (<= 10^7)
     */
    void countingSort(vector<int>& a, int minVal, int maxVal) {
        int range = maxVal - minVal + 1;
        vector<int> count(range, 0);
        for (int x : a) count[x - minVal]++;
        int idx = 0;
        for (int i = 0; i < range; ++i) {
            while (count[i] > 0) {
                a[idx++] = i + minVal;
                count[i]--;
            }
        }
    }

    /**
     * @brief Sắp xếp cơ số Radix Sort cho số nguyên 32-bit không dấu
     * Độ phức tạp: O(4 * (N + 256)) = O(N) cực nhanh
     */
    void radixSort32(vector<uint32_t>& a) {
        int n = a.size();
        if (n <= 1) return;
        vector<uint32_t> temp(n);

        // Duyệt qua 4 byte (mỗi byte 8 bit, cơ số 256)
        for (int shift = 0; shift < 32; shift += 8) {
            int count[256] = {0};
            for (int i = 0; i < n; ++i) count[(a[i] >> shift) & 0xFF]++;
            for (int i = 1; i < 256; ++i) count[i] += count[i - 1];
            for (int i = n - 1; i >= 0; --i) {
                temp[--count[(a[i] >> shift) & 0xFF]] = a[i];
            }
            a = temp;
        }
    }

} // namespace Sorting

// ============================================================================
// 2. CHUYÊN ĐỀ ĐẾM NGHỊCH THẾ (INVERSION COUNTING)
// ============================================================================

namespace Inversions {

    /**
     * @brief Đếm số nghịch thế bằng thuật toán Merge Sort trong O(N log N)
     * Trả về kiểu long long tránh tràn số 32-bit khi số nghịch thế đạt N(N-1)/2
     */
    template<typename T>
    long long countByMergeSortInternal(vector<T>& a, vector<T>& temp, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        long long inv = 0;
        inv += countByMergeSortInternal(a, temp, left, mid);
        inv += countByMergeSortInternal(a, temp, mid + 1, right);

        int i = left, j = mid + 1, k = left;
        while (i <= mid && j <= right) {
            if (a[i] <= a[j]) {
                temp[k++] = a[i++];
            } else {
                temp[k++] = a[j++];
                // Toàn bộ các phần tử từ a[i..mid] đều lớn hơn a[j]
                inv += (mid - i + 1);
            }
        }
        while (i <= mid) temp[k++] = a[i++];
        while (j <= right) temp[k++] = a[j++];
        for (i = left; i <= right; ++i) a[i] = temp[i];

        return inv;
    }

    template<typename T>
    long long countByMergeSort(vector<T> a) {
        if (a.size() <= 1) return 0;
        vector<T> temp(a.size());
        return countByMergeSortInternal(a, temp, 0, (int)a.size() - 1);
    }

    /**
     * @brief Đếm số nghịch thế bằng Fenwick Tree (BIT) kết hợp Nén Tọa độ
     * Độ phức tạp: O(N log N), bộ nhớ O(N)
     */
    template<typename T>
    long long countByBIT(const vector<T>& a) {
        int n = a.size();
        if (n <= 1) return 0;

        // 1. Nén tọa độ (Coordinate Compression)
        vector<T> vals = a;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());

        auto getRank = [&](T x) -> int {
            return lower_bound(vals.begin(), vals.end(), x) - vals.begin() + 1; // 1-indexed
        };

        // 2. Fenwick Tree để đếm tần suất
        int sz = vals.size();
        vector<int> bit(sz + 1, 0);

        auto update = [&](int idx, int delta) {
            for (; idx <= sz; idx += idx & (-idx)) bit[idx] += delta;
        };

        auto query = [&](int idx) -> int {
            int sum = 0;
            for (; idx > 0; idx -= idx & (-idx)) sum += bit[idx];
            return sum;
        };

        // 3. Duyệt từ phải sang trái
        long long inv = 0;
        for (int i = n - 1; i >= 0; --i) {
            int r = getRank(a[i]);
            inv += query(r - 1); // Đếm số phần tử nhỏ hơn a[i] đã xuất hiện bên phải
            update(r, 1);
        }
        return inv;
    }

    /**
     * @brief Xác định tính chẵn lẻ của hoán vị: Parity = inv % 2
     * Trả về true nếu là hoán vị chẵn (Even Permutation), false nếu hoán vị lẻ
     */
    template<typename T>
    bool isEvenPermutation(const vector<T>& p) {
        return (countByBIT(p) % 2 == 0);
    }

} // namespace Inversions

// ============================================================================
// 3. CHƯƠNG TRÌNH KIỂM THỬ (TEST DRIVER)
// ============================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== KIỂM THỬ THUẬT TOÁN SẮP XẾP & ĐẾM NGHỊCH THẾ ===\n\n";

    // 1. Kiểm thử QuickSort và MergeSort
    vector<int> arr1 = {38, 27, 43, 3, 9, 82, 10};
    vector<int> arr2 = arr1;

    Sorting::quickSort(arr1);
    Sorting::mergeSort(arr2);

    cout << "Mảng đã sắp xếp bằng QuickSort: ";
    for (int x : arr1) cout << x << " ";
    cout << "\n";

    cout << "Mảng đã sắp xếp bằng MergeSort: ";
    for (int x : arr2) cout << x << " ";
    cout << "\n\n";
    assert(arr1 == arr2);

    // 2. Kiểm thử RadixSort 32-bit
    vector<uint32_t> unsigned_arr = {170, 45, 75, 90, 802, 24, 2, 66};
    Sorting::radixSort32(unsigned_arr);
    cout << "Mảng đã sắp xếp bằng RadixSort: ";
    for (uint32_t x : unsigned_arr) cout << x << " ";
    cout << "\n\n";

    // 3. Kiểm thử Đếm nghịch thế
    // Mảng mẫu: [2, 4, 1, 3, 5] -> các cặp (2, 1), (4, 1), (4, 3) -> 3 cặp
    vector<int> inv_arr = {2, 4, 1, 3, 5};
    long long inv_merge = Inversions::countByMergeSort(inv_arr);
    long long inv_bit   = Inversions::countByBIT(inv_arr);

    cout << "3. Đếm số nghịch thế của [2, 4, 1, 3, 5]:\n";
    cout << "- Tính bằng Merge Sort: " << inv_merge << " (kỳ vọng: 3)\n";
    cout << "- Tính bằng Fenwick BIT: " << inv_bit << " (kỳ vọng: 3)\n";
    assert(inv_merge == 3 && inv_bit == 3);

    // Kiểm thử mảng giảm dần hoàn toàn: [5, 4, 3, 2, 1] -> 5 * 4 / 2 = 10 cặp
    vector<int> rev_arr = {5, 4, 3, 2, 1};
    cout << "Số nghịch thế của mảng giảm dần [5, 4, 3, 2, 1]: " 
         << Inversions::countByBIT(rev_arr) << " (kỳ vọng: 10)\n";
    assert(Inversions::countByBIT(rev_arr) == 10);

    // Tính chẵn lẻ của hoán vị
    cout << "Hoán vị [2, 4, 1, 3, 5] có phải hoán vị chẵn? " 
         << (Inversions::isEvenPermutation(inv_arr) ? "CHẴN" : "LẺ") 
         << " (kỳ vọng: LẺ vì 3 là số lẻ)\n";

    return 0;
}
