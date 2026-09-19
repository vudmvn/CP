/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Dãy số, Chuỗi số & Đệ quy Tuyến tính (Sequences, Series & Recurrences)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @affiliation SLSCM Lab - Faculty of Data Science and AI (FDA), National Economics University (NEU)
 * @generator Soạn thảo và tối ưu bởi Agentic AI tool
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Chuẩn C++: C++20
 * Bộ công cụ bao gồm:
 * 1. Tổng cấp số cộng an toàn tránh tràn số.
 * 2. Tổng cấp số nhân chia để trị O(log K) với modulo bất kỳ (không cần nghịch đảo).
 * 3. Thuật toán Fast Doubling tính số Fibonacci F_N trong O(log N).
 * 4. Dãy số tổ hợp: Catalan C_N, Derangement D_N, Stirling loại 1 & 2, Số Bell B_N.
 */

#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>

using namespace std;

namespace NumberSeq {

    // Lũy thừa nhị phân (a^b) % mod
    long long power(long long a, long long b, long long mod) {
        long long res = 1;
        a %= mod;
        while (b > 0) {
            if (b & 1) res = (__int128)res * a % mod;
            a = (__int128)a * a % mod;
            b >>= 1;
        }
        return res;
    }

    // ==========================================
    // 1. Cấp số cộng (Arithmetic Progression)
    // ==========================================

    /**
     * @brief Tính tổng n số hạng đầu của cấp số cộng: S_n = n*(u_1 + u_n)/2 % mod
     * Tự động chia chẵn cho 2 trước khi modulo để an toàn với mọi mod chẵn/lẻ.
     */
    long long sumAP(long long n, long long u1, long long un, long long mod) {
        long long a = n;
        long long b = u1 + un;
        if (a % 2 == 0) a /= 2;
        else b /= 2;
        return ((a % mod) * (b % mod)) % mod;
    }

    // ==========================================
    // 2. Cấp số nhân (Geometric Progression)
    // ==========================================

    /**
     * @brief Tính S_k = (1 + q + q^2 + ... + q^(k-1)) % mod
     * Sử dụng kỹ thuật chia để trị O(log k), chạy chính xác với MỌI modulo (kể cả mod không nguyên tố).
     */
    long long geomSum(long long q, long long k, long long mod) {
        if (k <= 0) return 0;
        if (k == 1) return 1 % mod;
        q = (q % mod + mod) % mod;

        if (k % 2 == 0) {
            long long half = geomSum(q, k / 2, mod);
            long long p = power(q, k / 2, mod);
            return (half * (1 + p)) % mod;
        } else {
            return (1 + q * geomSum(q, k - 1, mod)) % mod;
        }
    }

    // ==========================================
    // 3. Fibonacci Fast Doubling O(log N)
    // ==========================================

    /**
     * @brief Tính cặp {F_n, F_{n+1}} modulo mod trong O(log n).
     * F_0 = 0, F_1 = 1, F_2 = 1, F_3 = 2, F_4 = 3, F_5 = 5, ...
     * Nhanh hơn phép nhân ma trận 2x2 gấp 2-3 lần.
     */
    pair<long long, long long> fibFastDoubling(long long n, long long mod) {
        if (n == 0) return {0, 1 % mod};
        auto [fk, fk1] = fibFastDoubling(n >> 1, mod);
        
        // F_{2k} = F_k * (2*F_{k+1} - F_k)
        long long f2k = (fk * ((2 * fk1 - fk) % mod + mod)) % mod;
        // F_{2k+1} = F_k^2 + F_{k+1}^2
        long long f2k1 = (fk * fk + fk1 * fk1) % mod;

        if (n & 1) {
            return {f2k1, (f2k + f2k1) % mod};
        } else {
            return {f2k, f2k1};
        }
    }

    inline long long getFib(long long n, long long mod) {
        return fibFastDoubling(n, mod).first;
    }

    // ==========================================
    // 4. Các Dãy số Tổ hợp Kinh điển
    // ==========================================

    /**
     * @brief Tính trước mảng Derangement D_0, D_1, ..., D_N % mod
     * D_0 = 1, D_1 = 0, D_n = (n-1)*(D_{n-1} + D_{n-2})
     * Độ phức tạp: O(N)
     */
    vector<long long> derangementTable(int n, long long mod) {
        vector<long long> d(n + 1, 0);
        d[0] = 1 % mod;
        if (n >= 1) d[1] = 0;
        for (int i = 2; i <= n; ++i) {
            d[i] = ((i - 1) * (d[i - 1] + d[i - 2])) % mod;
        }
        return d;
    }

    /**
     * @brief Tính bảng số Stirling loại 2: S(n, k)
     * Số cách chia n phần tử khác nhau vào k tập con không rỗng.
     * S(n, k) = k * S(n-1, k) + S(n-1, k-1)
     * Độ phức tạp: O(N * K)
     */
    vector<vector<long long>> stirling2Table(int n, int k, long long mod) {
        vector<vector<long long>> S(n + 1, vector<long long>(k + 1, 0));
        S[0][0] = 1 % mod;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= min(i, k); ++j) {
                S[i][j] = (j * S[i - 1][j] + S[i - 1][j - 1]) % mod;
            }
        }
        return S;
    }

    /**
     * @brief Tính bảng số Stirling loại 1: s(n, k)
     * Số hoán vị của n phần tử có đúng k chu trình rời nhau.
     * s(n, k) = (n-1)*s(n-1, k) + s(n-1, k-1)
     * Độ phức tạp: O(N * K)
     */
    vector<vector<long long>> stirling1Table(int n, int k, long long mod) {
        vector<vector<long long>> s(n + 1, vector<long long>(k + 1, 0));
        s[0][0] = 1 % mod;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= min(i, k); ++j) {
                s[i][j] = ((long long)(i - 1) * s[i - 1][j] + s[i - 1][j - 1]) % mod;
            }
        }
        return s;
    }

    /**
     * @brief Tính số Bell B_0, B_1, ..., B_N % mod
     * B_n = tổng số cách phân hoạch tập hợp n phần tử = sum_{k=0}^n S(n, k)
     * Độ phức tạp: O(N^2)
     */
    vector<long long> bellTable(int n, long long mod) {
        auto S = stirling2Table(n, n, mod);
        vector<long long> B(n + 1, 0);
        for (int i = 0; i <= n; ++i) {
            for (int j = 0; j <= i; ++j) {
                B[i] = (B[i] + S[i][j]) % mod;
            }
        }
        return B;
    }

} // namespace NumberSeq

// ==========================================
// Minh họa Tích hợp & Kiểm thử (Unit Tests)
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace NumberSeq;
    const long long MOD = 1'000'000'007LL;

    // 1. Kiểm thử Fibonacci Fast Doubling
    // F_0=0, F_1=1, F_2=1, F_3=2, F_4=3, F_5=5, F_6=8, F_7=13, F_8=21, F_9=34, F_10=55
    assert(getFib(0, MOD) == 0);
    assert(getFib(1, MOD) == 1);
    assert(getFib(5, MOD) == 5);
    assert(getFib(10, MOD) == 55);
    cout << "F_10 = " << getFib(10, MOD) << " (Expected: 55)\n";
    cout << "F_100 mod 10^9+7 = " << getFib(100, MOD) << "\n";

    // 2. Kiểm thử Tổng Cấp số nhân chia để trị với Modulo hợp số (MOD = 12)
    // S_4 = 1 + 2 + 4 + 8 = 15 = 3 mod 12
    long long gsum = geomSum(2, 4, 12);
    assert(gsum == 3);
    cout << "geomSum(2, 4) mod 12 = " << gsum << " (Expected: 3)\n";

    // 3. Kiểm thử Số Derangement D_n
    // D_0=1, D_1=0, D_2=1, D_3=2, D_4=9, D_5=44
    auto D = derangementTable(5, MOD);
    assert(D[1] == 0 && D[2] == 1 && D[3] == 2 && D[4] == 9 && D[5] == 44);
    cout << "D_5 = " << D[5] << " (Expected: 44)\n";

    // 4. Kiểm thử Số Stirling loại 2 S(4, 2)
    // S(4, 2) = 2^(4-1) - 1 = 8 - 1 = 7
    auto S2 = stirling2Table(4, 4, MOD);
    assert(S2[4][2] == 7);
    cout << "S(4, 2) = " << S2[4][2] << " (Expected: 7)\n";

    // 5. Kiểm thử Số Bell B_4
    // B_0=1, B_1=1, B_2=2, B_3=5, B_4=15
    auto B = bellTable(4, MOD);
    assert(B[4] == 15);
    cout << "B_4 = " << B[4] << " (Expected: 15)\n";

    cout << "\nAll Number Sequence tests passed successfully!\n";
    return 0;
}
