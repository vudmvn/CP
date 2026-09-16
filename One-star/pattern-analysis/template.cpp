/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Nhận dạng Quy luật & Công thức đóng trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

// ==========================================
// 1. MÔ HÌNH 1: DÃY CHẴN LẺ (EVEN ODDS) O(1)
// ==========================================

/**
 * @brief Tìm phần tử thứ k của dãy 1..n gồm toàn bộ số chẵn tăng dần rồi đến số lẻ tăng dần
 * @complexity O(1) Time, O(1) Space
 */
long long solve_even_odds(long long n, long long k) {
    assert(1 <= k && k <= n);
    long long num_even = n / 2;
    if (k <= num_even) {
        return 2 * k;
    } else {
        long long k_odd = k - num_even;
        return 2 * k_odd - 1;
    }
}

// ==========================================
// 2. MÔ HÌNH 2: BẢNG SỐ XOẮN ỐC (NUMBER SPIRAL) O(1)
// ==========================================

/**
 * @brief Tìm giá trị tại ô (y, x) trên bảng số xoắn ốc vô hạn (CSES 1071)
 * @param y Chỉ số hàng (1-indexed)
 * @param x Chỉ số cột (1-indexed)
 * @complexity O(1) Time, O(1) Space
 */
long long solve_number_spiral(long long y, long long x) {
    long long L = max(y, x);
    if (L % 2 == 0) {
        if (y == L) return L * L - (x - 1);
        else return (L - 1) * (L - 1) + y;
    } else {
        if (x == L) return L * L - (y - 1);
        else return (L - 1) * (L - 1) + x;
    }
}

// ==========================================
// 3. MÔ HÌNH 3: CHỮ SỐ THỨ K CỦA DÃY SỐ (DIGIT QUERIES) O(log10 K)
// ==========================================

/**
 * @brief Tìm chữ số thứ k trong dãy các số tự nhiên viết liền 12345678910111213... (CSES 2431)
 * @param k Vị trí chữ số cần tìm (1-indexed, k <= 10^18)
 * @return Chữ số (từ 0 đến 9)
 * @complexity O(log10 k) Time
 */
int solve_digit_queries(long long k) {
    long long len = 1;
    long long count = 9;
    long long start = 1;

    // B1: Trừ dần để tìm độ dài của số chứa chữ số thứ k
    while (k > len * count) {
        k -= len * count;
        len++;
        count *= 10;
        start *= 10;
        if (len >= 19) break; // Tránh tràn số khi k quá lớn
    }

    // B2: Xác định số nguyên cụ thể
    long long number = start + (k - 1) / len;

    // B3: Lấy chữ số thứ ((k - 1) % len) tính từ trái sang
    string s = to_string(number);
    return s[(k - 1) % len] - '0';
}

// ==========================================
// 4. MÔ HÌNH 4: CHUỖI ĐỐI XỨNG THUE-MORSE O(1)
// ==========================================

/**
 * @brief Tìm ký tự thứ k trong chuỗi Thue-Morse (0-indexed): 0, 01, 0110, 01101001...
 * @complexity O(1) Time
 */
int solve_thue_morse(long long k) {
    return __builtin_popcountll(k) % 2;
}

// ==========================================
// 5. MÔ HÌNH 5: ĐƯỜNG ZICZAC TRÊN LƯỚI (CANTOR DIAGONAL) O(1)
// ==========================================

/**
 * @brief Tìm tọa độ (hàng, cột) của phần tử thứ k trên đường đi ziczac chéo
 * @return pair<long long, long long> {hàng, cột} (1-indexed)
 * @complexity O(1) Time
 */
pair<long long, long long> solve_cantor_diagonal(long long k) {
    // Đường chéo thứ d chứa d phần tử. Tổng số phần tử đến đường chéo d là d*(d+1)/2
    // d ~ ceil((-1 + sqrt(1 + 8k)) / 2)
    long long d = (sqrt(8.0 * k + 1) - 1) / 2;
    while (d * (d + 1) / 2 < k) d++;

    long long prev_sum = (d - 1) * d / 2;
    long long step = k - prev_sum; // Bước thứ mấy trên đường chéo d

    if (d % 2 != 0) {
        // Đường chéo lẻ đi lên: hàng giảm, cột tăng
        return {d - step + 1, step};
    } else {
        // Đường chéo chẵn đi xuống: hàng tăng, cột giảm
        return {step, d - step + 1};
    }
}

// ==========================================
// 6. MAIN DEMO & TEST CASES
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Demo Even Odds
    cout << "=== 1. DEMO EVEN ODDS ===\n";
    long long n = 10;
    cout << "Day voi N = 10:\n";
    for (int k = 1; k <= n; ++k) {
        cout << solve_even_odds(n, k) << " ";
    }
    cout << "\n(Ky vong: 2 4 6 8 10 1 3 5 7 9)\n\n";

    // 2. Demo Number Spiral
    cout << "=== 2. DEMO NUMBER SPIRAL ===\n";
    cout << "O (2, 3) = " << solve_number_spiral(2, 3) << " (Ky vong: 8)\n";
    cout << "O (1, 1) = " << solve_number_spiral(1, 1) << " (Ky vong: 1)\n";
    cout << "O (4, 2) = " << solve_number_spiral(4, 2) << " (Ky vong: 15)\n\n";

    // 3. Demo Digit Queries
    cout << "=== 3. DEMO DIGIT QUERIES ===\n";
    cout << "Chu so thu 7  = " << solve_digit_queries(7) << " (Ky vong: 7)\n";
    cout << "Chu so thu 19 = " << solve_digit_queries(19) << " (Ky vong: 4 vi so 14 chua chu so 1 va 4)\n";
    cout << "Chu so thu 12 = " << solve_digit_queries(12) << " (Ky vong: 1 vi ...10[1]1...)\n\n";

    // 4. Demo Thue-Morse
    cout << "=== 4. DEMO THUE-MORSE ===\n";
    cout << "16 ky tu dau cua chuoi Thue-Morse: ";
    for (long long k = 0; k < 16; ++k) {
        cout << solve_thue_morse(k) << " ";
    }
    cout << "\n(Ky vong: 0 1 1 0 1 0 0 1 1 0 0 1 0 1 1 0)\n\n";

    // 5. Demo Cantor Diagonal
    cout << "=== 5. DEMO CANTOR DIAGONAL ===\n";
    auto [r, c] = solve_cantor_diagonal(5);
    cout << "Phan tu thu 5 nam tai hang: " << r << ", cot: " << c << "\n";

    return 0;
}
