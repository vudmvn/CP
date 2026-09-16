/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Sàng số nguyên tố & Thừa số nguyên tố trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

// ==========================================
// 1. KIỂM TRA NGUYÊN TỐ ĐƠN LẺ O(sqrt(N)) VỚI BƯỚC NHẢY 6k +- 1
// ==========================================

bool is_prime_trial(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// ==========================================
// 2. SÀNG ERATOSTHENES CỔ ĐIỂN DÙNG BITSET (TIẾT KIỆM BỘ NHỚ)
// ==========================================

template <int MAXN>
struct SieveClassical {
    bitset<MAXN + 1> is_prime;
    vector<int> primes;

    SieveClassical() { init(); }

    void init() {
        is_prime.set();
        is_prime[0] = is_prime[1] = 0;
        for (int i = 2; 1LL * i * i <= MAXN; ++i) {
            if (is_prime[i]) {
                for (int j = i * i; j <= MAXN; j += i)
                    is_prime[j] = 0;
            }
        }
        for (int i = 2; i <= MAXN; ++i) {
            if (is_prime[i]) primes.push_back(i);
        }
    }

    bool check(int x) const { return is_prime[x]; }
};

// ==========================================
// 3. SÀNG TUYẾN TÍNH EULER O(N) & MẢNG SPF (SMALLEST PRIME FACTOR)
// ==========================================

template <int MAXN>
struct LinearSieve {
    int spf[MAXN + 1];
    vector<int> primes;

    LinearSieve() { init(); }

    void init() {
        fill(spf, spf + MAXN + 1, 0);
        for (int i = 2; i <= MAXN; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || 1LL * i * p > MAXN) break;
                spf[i * p] = p; // p là ước nguyên tố nhỏ nhất của i * p
            }
        }
    }

    // Phân tích thừa số nguyên tố của x trong O(log x): trả về danh sách {thừa số nguyên tố, số mũ}
    vector<pair<int, int>> factorize(int x) const {
        assert(x >= 1 && x <= MAXN);
        vector<pair<int, int>> factors;
        while (x > 1) {
            int p = spf[x];
            int count = 0;
            while (x % p == 0) {
                count++;
                x /= p;
            }
            factors.push_back({p, count});
        }
        return factors;
    }

    // Đếm số lượng ước số của x trong O(log x)
    int count_divisors(int x) const {
        int count = 1;
        auto factors = factorize(x);
        for (auto &f : factors) count *= (f.second + 1);
        return count;
    }

    // Tính tổng tất cả các ước số của x trong O(log x)
    long long sum_divisors(int x) const {
        long long sum = 1;
        auto factors = factorize(x);
        for (auto &f : factors) {
            long long cur = 1, p_pow = 1;
            for (int i = 0; i < f.second; ++i) {
                p_pow *= f.first;
                cur += p_pow;
            }
            sum *= cur;
        }
        return sum;
    }

    // Tính hàm phi Euler phi(x) trong O(log x)
    int phi(int x) const {
        int res = x;
        auto factors = factorize(x);
        for (auto &f : factors) res -= res / f.first;
        return res;
    }
};

// ==========================================
// 4. SÀNG TRÊN ĐOẠN [L, R] (SEGMENTED SIEVE)
// ==========================================

/**
 * @brief Sàng tìm các số nguyên tố trong đoạn [L, R]
 * @param L Giới hạn dưới (L >= 1)
 * @param R Giới hạn trên (R <= 10^12, R - L <= 10^7)
 * @return Danh sách các số nguyên tố trong [L, R]
 */
vector<long long> segmented_sieve(long long L, long long R) {
    long long lim = sqrt(R);
    vector<bool> is_prime_small(lim + 1, true);
    vector<long long> primes;
    for (long long i = 2; i <= lim; ++i) {
        if (is_prime_small[i]) {
            primes.push_back(i);
            for (long long j = i * i; j <= lim; j += i)
                is_prime_small[j] = false;
        }
    }

    int len = R - L + 1;
    vector<bool> is_prime(len, true);
    for (long long p : primes) {
        long long start = max(p * p, ((L + p - 1) / p) * p);
        for (long long j = start; j <= R; j += p) {
            is_prime[j - L] = false;
        }
    }

    if (L == 1 && len > 0) is_prime[0] = false;

    vector<long long> result;
    for (int i = 0; i < len; ++i) {
        if (is_prime[i]) result.push_back(L + i);
    }
    return result;
}

// ==========================================
// 5. TEST NGUYÊN TỐ 64-BIT MILLER-RABIN (DETERMINISTIC)
// ==========================================

namespace MillerRabin {
    using u64 = unsigned long long;
    using u128 = __uint128_t;

    u64 power_mod(u64 base, u64 exp, u64 mod) {
        u64 res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) res = (u64)((u128)res * base % mod);
            base = (u64)((u128)base * base % mod);
            exp >>= 1;
        }
        return res;
    }

    bool check_composite(u64 n, u64 a, u64 d, int s) {
        u64 x = power_mod(a, d, n);
        if (x == 1 || x == n - 1) return false;
        for (int r = 1; r < s; ++r) {
            x = (u64)((u128)x * x % n);
            if (x == n - 1) return false;
        }
        return true;
    }

    /**
     * @brief Kiểm tra số nguyên tố chính xác 100% cho mọi số n < 2^64
     */
    bool is_prime(u64 n) {
        if (n < 2) return false;
        int r = 0;
        u64 d = n - 1;
        while ((d & 1) == 0) {
            d >>= 1;
            r++;
        }
        // Tập 7 cơ sở tiền định đảm bảo tính đúng tuyệt đối cho mọi n < 2^64
        for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            if (n <= a) break;
            if (check_composite(n, a, d, r)) return false;
        }
        return true;
    }
}

// ==========================================
// 6. MAIN DEMO
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Thử nghiệm Linear Sieve & SPF
    cout << "=== 1. LINEAR SIEVE DEMO ===\n";
    static LinearSieve<1000000> sieve;
    cout << "So luong so nguyen to <= 10^6: " << sieve.primes.size() << "\n";
    cout << "10 so nguyen to dau tien: ";
    for (int i = 0; i < 10; ++i) cout << sieve.primes[i] << " ";
    cout << "\n\n";

    // 2. Thử nghiệm phân tích thừa số siêu tốc trong O(log N)
    int test_val = 360;
    cout << "=== 2. PHAN TICH THUA SO NGUYEN TO (X = " << test_val << ") ===\n";
    auto factors = sieve.factorize(test_val);
    cout << test_val << " = ";
    for (size_t i = 0; i < factors.size(); ++i) {
        cout << factors[i].first << "^" << factors[i].second;
        if (i + 1 < factors.size()) cout << " * ";
    }
    cout << "\n";
    cout << "So luong uoc so d(" << test_val << ") = " << sieve.count_divisors(test_val) << "\n";
    cout << "Tong cac uoc so sigma(" << test_val << ") = " << sieve.sum_divisors(test_val) << "\n";
    cout << "Phi ham Euler phi(" << test_val << ") = " << sieve.phi(test_val) << "\n\n";

    // 3. Thử nghiệm Sàng đoạn [100, 150]
    cout << "=== 3. SANG DOAN [100, 150] ===\n";
    auto seg_primes = segmented_sieve(100, 150);
    cout << "Cac so nguyen to trong [100, 150]: ";
    for (auto p : seg_primes) cout << p << " ";
    cout << "\n\n";

    // 4. Thử nghiệm Miller-Rabin cho số lớn 64-bit
    cout << "=== 4. MILLER-RABIN 64-BIT DEMO ===\n";
    unsigned long long big_prime = 1000000000000000003ULL; // 10^18 + 3
    unsigned long long big_composite = 1000000000000000005ULL; // chia hết cho 5
    cout << big_prime << " la nguyen to? -> " << (MillerRabin::is_prime(big_prime) ? "DUNG" : "SAI") << "\n";
    cout << big_composite << " la nguyen to? -> " << (MillerRabin::is_prime(big_composite) ? "DUNG" : "SAI") << "\n";

    return 0;
}
