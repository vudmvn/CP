/**
 * @file template.cpp
 * @brief Bộ công cụ Toán học Nền tảng: Chia trần/sàn, Cấp số, Khoảng cách Manhattan, Shoelace & Đếm
 * @standard C++20
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

namespace BasicMath {

    /**
     * @brief Phép chia trần an toàn cho số không âm: ceil(a / b) = (a + b - 1) / b.
     */
    inline long long ceil_div(long long a, long long b) {
        assert(b > 0);
        return (a + b - 1) / b;
    }

    /**
     * @brief Phép chia sàn chuẩn xác cho mọi số nguyên (âm & dương).
     * Ví dụ: floor_div(-7, 3) = -3.
     */
    inline long long floor_div(long long a, long long b) {
        assert(b != 0);
        long long res = a / b;
        long long rem = a % b;
        if (rem != 0 && ((a < 0) ^ (b < 0))) res--;
        return res;
    }

    /**
     * @brief Phép chia trần chuẩn xác cho mọi số nguyên (âm & dương).
     * Ví dụ: ceil_div_general(-7, 3) = -2.
     */
    inline long long ceil_div_general(long long a, long long b) {
        assert(b != 0);
        long long res = a / b;
        long long rem = a % b;
        if (rem != 0 && ((a > 0) == (b > 0))) res++;
        return res;
    }

    /**
     * @brief Tổng 1 + 2 + ... + n bằng công thức Gauss. Dùng __int128 để chống tràn số.
     */
    inline long long sum_first_n(long long n) {
        if (n <= 0) return 0;
        return (long long)((__int128)n * (n + 1) / 2);
    }

    /**
     * @brief Tổng bình phương: 1^2 + 2^2 + ... + n^2 = n*(n+1)*(2n+1)/6.
     */
    inline long long sum_squares_first_n(long long n) {
        if (n <= 0) return 0;
        return (long long)((__int128)n * (n + 1) * (2 * n + 1) / 6);
    }

    /**
     * @brief Tổng cấp số cộng n số hạng: S_n = n * (u1 + un) / 2.
     */
    inline long long ap_sum(long long n, long long u1, long long un) {
        return (long long)((__int128)n * (u1 + un) / 2);
    }

    /**
     * @brief Khoảng cách Manhattan giữa 2 điểm: |x1 - x2| + |y1 - y2|.
     */
    inline long long manhattan_dist(pair<long long, long long> p1, pair<long long, long long> p2) {
        return std::abs(p1.first - p2.first) + std::abs(p1.second - p2.second);
    }

    /**
     * @brief Tìm khoảng cách Manhattan lớn nhất giữa N điểm trong O(N) bằng biến đổi Chebyshev.
     */
    long long max_manhattan_distance(const vector<pair<long long, long long>>& points) {
        assert(!points.empty());
        long long min_u = (long long)4e18, max_u = -(long long)4e18;
        long long min_v = (long long)4e18, max_v = -(long long)4e18;

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
     * @brief Tính 2 lần diện tích đa giác nguyên bằng công thức Dây giày (Shoelace).
     * Trả về số nguyên 2*S để triệt tiêu hoàn toàn sai số số thực.
     */
    long long double_shoelace_area(const vector<pair<long long, long long>>& p) {
        int n = p.size();
        __int128 total = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            total += (__int128)p[i].first * p[j].second - (__int128)p[j].first * p[i].second;
        }
        if (total < 0) total = -total;
        return (long long)total;
    }

    /**
     * @brief Số chữ số của số nguyên dương n trong hệ cơ số base.
     */
    inline int count_digits(long long n, int base = 10) {
        if (n == 0) return 1;
        n = std::abs(n);
        int digits = 0;
        while (n > 0) {
            digits++;
            n /= base;
        }
        return digits;
    }

} // namespace BasicMath

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    cout << "=== 1. DEMO CHIA TRẦN / SÀN NGUYÊN ===" << "\n";
    assert(BasicMath::ceil_div(7, 3) == 3);
    assert(BasicMath::ceil_div(6, 3) == 2);
    assert(BasicMath::floor_div(-7, 3) == -3);
    assert(BasicMath::ceil_div_general(-7, 3) == -2);
    cout << "Chia tran/san nguyen: PASS!\n";

    cout << "\n=== 2. DEMO CẤP SỐ CỘNG & TỔNG FAULHABER ===" << "\n";
    // 1 + 2 + ... + 10 = 55
    assert(BasicMath::sum_first_n(10) == 55);
    // 1^2 + 2^2 + 3^2 + 4^2 = 1 + 4 + 9 + 16 = 30
    assert(BasicMath::sum_squares_first_n(4) == 30);
    // Cấp số cộng: 2 + 5 + 8 + 11 (n=4, u1=2, un=11) => 4 * 13 / 2 = 26
    assert(BasicMath::ap_sum(4, 2, 11) == 26);
    cout << "Tong cap so cong: PASS!\n";

    cout << "\n=== 3. DEMO KHOẢNG CÁCH MANHATTAN LỚN NHẤT O(N) ===" << "\n";
    vector<pair<long long, long long>> pts = {
        {1, 2}, {2, 3}, {10, 20}, {5, 6}
    };
    long long max_d = BasicMath::max_manhattan_distance(pts);
    // Khoảng cách giữa (1, 2) và (10, 20) là |1 - 10| + |2 - 20| = 9 + 18 = 27
    cout << "Khoang cach Manhattan lon nhat: " << max_d << " (Ky vong: 27)\n";
    assert(max_d == 27);

    cout << "\n=== 4. DEMO CÔNG THỨC DÂY GIÀY (SHOELACE FORMULA) ===" << "\n";
    // Tam giác vuông tại gốc tọa độ: (0, 0), (4, 0), (0, 3) => Diện tích S = 6 => 2*S = 12
    vector<pair<long long, long long>> triangle = {
        {0, 0}, {4, 0}, {0, 3}
    };
    long long double_area = BasicMath::double_shoelace_area(triangle);
    cout << "2 * Dien tich tam giac: " << double_area << " (Ky vong: 12)\n";
    assert(double_area == 12);

    cout << "\nTat ca cac assertions deu PASS 100% thanh cong!\n";
    return 0;
}
