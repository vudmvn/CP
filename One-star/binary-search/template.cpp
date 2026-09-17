/**
 * @file template.cpp
 * @brief Chuẩn hóa Thuật toán Tìm kiếm Nhị phân & Tìm kiếm Tam phân (C++20)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @affiliation SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @generated_by Agentic AI tool (Antigravity AI Agent)
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Chuẩn mực CP:
 * - C++20 Standard
 * - Fast I/O, an toàn tràn số 64-bit (long long)
 * - Mẫu lặp chống treo vô tận (Infinite Loop-free)
 * - Sẵn sàng sao chép dự thi ICPC / Olympic Tin học
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iomanip>

using namespace std;

namespace BinarySearch {

    /**
     * @brief Tìm kiếm nhị phân phần tử target trên mảng đã sắp xếp
     * @return Chỉ số 0-indexed nếu tìm thấy, -1 nếu không tồn tại
     */
    template<typename T>
    int find(const vector<T>& arr, T target) {
        int low = 0, high = (int)arr.size() - 1;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] == target) return mid;
            if (arr[mid] < target) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    /**
     * @brief Tự cài đặt lower_bound: Phần tử đầu tiên >= target
     * @return Chỉ số đầu tiên thỏa mãn, hoặc arr.size() nếu mọi phần tử < target
     */
    template<typename T>
    int lowerBound(const vector<T>& arr, T target) {
        int low = 0, high = (int)arr.size() - 1;
        int ans = (int)arr.size();
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] >= target) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }

    /**
     * @brief Tự cài đặt upper_bound: Phần tử đầu tiên > target
     * @return Chỉ số đầu tiên thỏa mãn, hoặc arr.size() nếu mọi phần tử <= target
     */
    template<typename T>
    int upperBound(const vector<T>& arr, T target) {
        int low = 0, high = (int)arr.size() - 1;
        int ans = (int)arr.size();
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (arr[mid] > target) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }

    /**
     * @brief Mẫu First True: Tìm giá trị nguyên nhỏ nhất x trong [low, high] sao cho pred(x) == true
     * Dãy trạng thái: [0, 0, ..., 0, 1, 1, ..., 1]
     */
    long long firstTrue(long long low, long long high, const function<bool(long long)>& pred) {
        long long ans = high + 1;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (pred(mid)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }

    /**
     * @brief Mẫu Last True: Tìm giá trị nguyên lớn nhất x trong [low, high] sao cho pred(x) == true
     * Dãy trạng thái: [1, 1, ..., 1, 0, 0, ..., 0]
     */
    long long lastTrue(long long low, long long high, const function<bool(long long)>& pred) {
        long long ans = low - 1;
        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (pred(mid)) {
                ans = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return ans;
    }

    /**
     * @brief Chặt nhị phân trên miền số thực với số vòng lặp cố định (Fixed Iterations)
     * Triệt tiêu hoàn toàn lỗi vòng lặp vô hạn do sai số float/double.
     */
    double realBinarySearch(double low, double high, const function<bool(double)>& pred, int iters = 100) {
        for (int i = 0; i < iters; ++i) {
            double mid = low + (high - low) / 2.0;
            if (pred(mid)) {
                high = mid;
            } else {
                low = mid;
            }
        }
        return low;
    }

} // namespace BinarySearch

namespace TernarySearch {

    /**
     * @brief Tìm kiếm tam phân trên hàm liên tục số thực (Continuous Unimodal Function)
     * @param findMax True nếu tìm điểm cực đại, False nếu tìm điểm cực tiểu
     */
    double realTernarySearch(double low, double high, const function<double(double)>& f, bool findMax = true, int iters = 100) {
        for (int i = 0; i < iters; ++i) {
            double m1 = low + (high - low) / 3.0;
            double m2 = high - (high - low) / 3.0;
            double f1 = f(m1);
            double f2 = f(m2);
            if (findMax) {
                if (f1 < f2) low = m1;
                else high = m2;
            } else {
                if (f1 > f2) low = m1;
                else high = m2;
            }
        }
        return (low + high) / 2.0;
    }

    /**
     * @brief Tìm kiếm tam phân trên hàm rời rạc số nguyên (Discrete Domain)
     * Kỹ thuật co khoảng an toàn: Khi (high - low <= 4), quét cạn toàn bộ để bảo đảm tính đúng đắn.
     */
    long long discreteTernarySearch(long long low, long long high, const function<long long(long long)>& f, bool findMax = true) {
        while (high - low > 4) {
            long long m1 = low + (high - low) / 3;
            long long m2 = high - (high - low) / 3;
            long long f1 = f(m1);
            long long f2 = f(m2);
            if (findMax) {
                if (f1 < f2) low = m1;
                else high = m2;
            } else {
                if (f1 > f2) low = m1;
                else high = m2;
            }
        }
        long long best_x = low;
        long long best_val = f(low);
        for (long long x = low + 1; x <= high; ++x) {
            long long cur = f(x);
            if (findMax ? (cur > best_val) : (cur < best_val)) {
                best_val = cur;
                best_x = x;
            }
        }
        return best_x;
    }

    /**
     * @brief Tìm cực trị hàm đơn đỉnh rời rạc bằng Binary Search trên hàm hiệu f(k) < f(k+1)
     * Ưu điểm: Nhanh hơn Ternary Search (~2.88 ln N so với ~4.93 ln N), code ngắn gọn, không lo bẫy chia nguyên.
     * Yêu cầu bắt buộc: Hàm f phải đơn đỉnh NGẶT (Strictly Unimodal), không có đoạn bằng phẳng.
     */
    long long binarySearchUnimodalDiscrete(long long low, long long high, const function<long long(long long)>& f, bool findMax = true) {
        long long l = low, r = high - 1, best = low;
        while (l <= r) {
            long long mid = l + (r - l) / 2;
            bool condition = findMax ? (f(mid) < f(mid + 1)) : (f(mid) > f(mid + 1));
            if (condition) {
                best = mid + 1; // Đi theo chiều dốc tiến tới cực trị
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        return best;
    }

} // namespace TernarySearch

int main() {
    // Fast I/O tiêu chuẩn CP
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Kiểm thử tìm kiếm trên mảng rời rạc
    vector<int> a = {1, 3, 5, 7, 7, 7, 9, 11, 15};
    cout << "Mảng mẫu: 1, 3, 5, 7, 7, 7, 9, 11, 15\n";
    cout << "Vị trí số 9: " << BinarySearch::find(a, 9) << "\n";
    cout << "lower_bound(7): " << BinarySearch::lowerBound(a, 7) << "\n";
    cout << "upper_bound(7): " << BinarySearch::upperBound(a, 7) << "\n";
    cout << "Số lần xuất hiện của 7: " 
         << BinarySearch::upperBound(a, 7) - BinarySearch::lowerBound(a, 7) << "\n\n";

    // 2. Kiểm thử Chặt nhị phân trên không gian nghiệm: Căn bậc hai nguyên của N
    long long N = 1000000000000LL; // 10^12
    auto pred = [&](long long x) {
        return x * x >= N;
    };
    long long sqrtN = BinarySearch::firstTrue(1, 2000000, pred);
    cout << "Căn bậc hai nguyên đầu tiên của " << N << " là: " << sqrtN << "\n\n";

    // 3. Kiểm thử Tìm kiếm Tam phân liên tục: Parabol f(x) = -(x - 42)^2 + 100
    auto parabola = [](double x) {
        return -(x - 42.0) * (x - 42.0) + 100.0;
    };
    double peak_x = TernarySearch::realTernarySearch(0.0, 100.0, parabola, true);
    cout << fixed << setprecision(6);
    cout << "Cực đại Parabola liên tục tại x = " << peak_x << " với f(x) = " << parabola(peak_x) << "\n";

    // 4. Kiểm thử Binary Search tìm cực đại rời rạc: g(k) = -(k - 77)^2 + 500
    auto discrete_f = [](long long k) -> long long {
        return -(k - 77) * (k - 77) + 500;
    };
    long long peak_k = TernarySearch::binarySearchUnimodalDiscrete(0, 1000, discrete_f, true);
    cout << "Cực đại hàm rời rạc bằng BS đạo hàm tại k = " << peak_k 
         << " với f(k) = " << discrete_f(peak_k) << "\n";

    return 0;
}
