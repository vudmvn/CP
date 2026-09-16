/**
 * @file template.cpp
 * @brief Bộ thư viện chuẩn: GCD, LCM, Euclid mở rộng & Phương trình Diophantine tuyến tính
 * @standard C++20
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <map>
#include <cassert>

using namespace std;

namespace MathGCD {

    /**
     * @brief Tính ước chung lớn nhất (GCD) của 2 số nguyên 64-bit.
     * Luôn trả về giá trị không âm. std::abs xử lý các số âm.
     * Độ phức tạp: O(log(min(|a|, |b|)))
     */
    inline long long gcd(long long a, long long b) {
        return std::gcd(std::abs(a), std::abs(b));
    }

    /**
     * @brief Tính bội chung nhỏ nhất (LCM) của 2 số nguyên 64-bit an toàn tràn số.
     * Thực hiện phép chia trước, nhân sau: (a / gcd(a, b)) * b.
     * Độ phức tạp: O(log(min(|a|, |b|)))
     */
    inline long long lcm(long long a, long long b) {
        if (a == 0 || b == 0) return 0;
        a = std::abs(a);
        b = std::abs(b);
        return (a / std::gcd(a, b)) * b;
    }

    /**
     * @brief Thuật toán Euclid mở rộng (Extended Euclidean Algorithm).
     * Tìm nghiệm nguyên (x, y) thỏa phương trình: a * x + b * y = gcd(a, b).
     * @return g = gcd(a, b) (luôn > 0 nếu a, b không đồng thời bằng 0).
     */
    long long extgcd(long long a, long long b, long long &x, long long &y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        long long x1, y1;
        long long g = extgcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - (a / b) * y1;
        return g;
    }

    /**
     * @brief Giải phương trình Diophantine tuyến tính: a * x + b * y = c.
     * Sử dụng kiểu __int128_t để chống tràn số khi nhân nghiệm riêng.
     * @param a Hệ số a
     * @param b Hệ số b
     * @param c Hằng số c
     * @param x0 Nghiệm riêng x0 (nếu có)
     * @param y0 Nghiệm riêng y0 (nếu có)
     * @param g Ước chung lớn nhất gcd(|a|, |b|)
     * @return true nếu phương trình có nghiệm nguyên, false nếu vô nghiệm.
     */
    bool solve_diophantine(long long a, long long b, long long c, 
                           long long &x0, long long &y0, long long &g) {
        if (a == 0 && b == 0) {
            if (c == 0) {
                x0 = y0 = g = 0;
                return true;
            }
            return false;
        }

        long long x_g, y_g;
        g = extgcd(std::abs(a), std::abs(b), x_g, y_g);

        if (c % g != 0) return false;

        // Dùng __int128_t để tránh tràn số khi nhân (c / g) * x_g
        __int128 factor = c / g;
        __int128 x_ans = (__int128)x_g * factor;
        __int128 y_ans = (__int128)y_g * factor;

        if (a < 0) x_ans = -x_ans;
        if (b < 0) y_ans = -y_ans;

        x0 = (long long)x_ans;
        y0 = (long long)y_ans;
        return true;
    }

    /**
     * @brief Tìm nghiệm nguyên (x, y) của ax + by = c sao cho x > 0 và x là nhỏ nhất.
     * Dựa trên họ nghiệm: x = x0 + k * (b / g).
     */
    bool find_min_positive_x(long long a, long long b, long long c, 
                            long long &min_x, long long &corresp_y) {
        long long x0, y0, g;
        if (!solve_diophantine(a, b, c, x0, y0, g)) return false;

        long long step_x = std::abs(b / g);
        if (step_x == 0) {
            // b = 0, x cố định là c / a
            if (x0 > 0) {
                min_x = x0;
                corresp_y = y0;
                return true;
            }
            return false;
        }

        // Dịch x0 về số dương nhỏ nhất: x = x0 + k * step_x > 0
        long long k = -x0 / step_x;
        x0 += k * step_x;
        if (x0 <= 0) {
            x0 += step_x;
            k++;
        }

        // Tính y tương ứng từ phương trình ax + by = c => by = c - ax
        min_x = x0;
        corresp_y = (c - a * min_x) / b;
        return true;
    }

    /**
     * @brief Tính GCD của cả mảng trong O(N + log(max A)).
     */
    long long gcd_array(const vector<long long>& arr) {
        long long g = 0;
        for (long long x : arr) {
            g = std::gcd(g, std::abs(x));
            if (g == 1) break; // Tối ưu: Nếu đã bằng 1 thì không thể giảm thêm
        }
        return g;
    }

    /**
     * @brief Đếm số lượng đoạn con (subarray) có GCD bằng mỗi giá trị g.
     * Sử dụng tính chất dãy GCD tiền tố kết thúc tại mỗi vị trí chỉ có tối đa log2(max A) giá trị phân biệt.
     * Độ phức tạp: O(N * log(max A))
     * @return map chứa cặp {giá trị gcd, số lượng đoạn con đạt giá trị đó}
     */
    map<long long, long long> count_all_subarray_gcds(const vector<long long>& arr) {
        map<long long, long long> total_gcd_counts;
        // previous_gcds lưu các cặp: {giá trị gcd, số đoạn con kết thúc tại i-1 có gcd đó}
        vector<pair<long long, long long>> prev;

        for (long long x : arr) {
            vector<pair<long long, long long>> curr;
            curr.push_back({x, 1});

            for (const auto& [g_val, count] : prev) {
                long long new_g = std::gcd(g_val, x);
                bool merged = false;
                for (auto& item : curr) {
                    if (item.first == new_g) {
                        item.second += count;
                        merged = true;
                        break;
                    }
                }
                if (!merged) {
                    curr.push_back({new_g, count});
                }
            }

            for (const auto& [g_val, count] : curr) {
                total_gcd_counts[g_val] += count;
            }
            prev = move(curr);
        }

        return total_gcd_counts;
    }

} // namespace MathGCD

int main() {
    // Tối ưu hóa I/O chuẩn thi đấu
    cin.tie(nullptr)->sync_with_stdio(false);

    cout << "=== 1. DEMO GCD & SAFE LCM ===" << "\n";
    long long a = 12, b = 18;
    cout << "gcd(" << a << ", " << b << ") = " << MathGCD::gcd(a, b) << "\n";
    cout << "lcm(" << a << ", " << b << ") = " << MathGCD::lcm(a, b) << "\n";
    assert(MathGCD::gcd(a, b) == 6);
    assert(MathGCD::lcm(a, b) == 36);

    cout << "\n=== 2. DEMO EXTENDED EUCLIDEAN ALGORITHM ===" << "\n";
    long long x, y;
    long long g = MathGCD::extgcd(30, 11, x, y);
    cout << "30*(" << x << ") + 11*(" << y << ") = " << g << "\n";
    assert(30 * x + 11 * y == g);

    cout << "\n=== 3. DEMO LINEAR DIOPHANTINE SOLVER ===" << "\n";
    // Giải 6x + 9y = 15 => gcd(6, 9) = 3 | 15 => Có nghiệm
    long long x0, y0;
    bool has_sol = MathGCD::solve_diophantine(6, 9, 15, x0, y0, g);
    cout << "Giai 6x + 9y = 15: " << (has_sol ? "Co nghiem" : "Vo nghiem") << "\n";
    cout << "Nghiem rieng: x0 = " << x0 << ", y0 = " << y0 << "\n";
    assert(has_sol && (6 * x0 + 9 * y0 == 15));

    // Tìm nghiệm có x > 0 nhỏ nhất
    long long min_x, corresp_y;
    MathGCD::find_min_positive_x(6, 9, 15, min_x, corresp_y);
    cout << "Nghiem co x > 0 nho nhat: x = " << min_x << ", y = " << corresp_y << "\n";
    assert(min_x > 0 && (6 * min_x + 9 * corresp_y == 15));

    cout << "\n=== 4. DEMO SUBARRAY GCDs COUNTING ===" << "\n";
    vector<long long> arr = {2, 6, 3, 4};
    auto gcd_counts = MathGCD::count_all_subarray_gcds(arr);
    cout << "Thong ke so luong doan con theo gia tri GCD:" << "\n";
    for (auto [gcd_val, cnt] : gcd_counts) {
        cout << "GCD = " << gcd_val << ": " << cnt << " doan con\n";
    }
    // Tổng số đoạn con của mảng 4 phần tử là 4 * 5 / 2 = 10
    long long total_subarrays = 0;
    for (auto [k, v] : gcd_counts) total_subarrays += v;
    assert(total_subarrays == 10);

    cout << "\nTat ca cac assertions deu PASS thanh cong!\n";
    return 0;
}
