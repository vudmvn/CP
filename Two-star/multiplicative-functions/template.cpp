/**
 * @file template.cpp
 * @brief Multiplicative Functions, Dirichlet Convolution, Linear Sieve & Du Sieve
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations for:
 * 1. Linear Sieve O(N) for classic functions (mu, phi, divisor count d, divisor sum sigma).
 * 2. General Linear Sieve Framework for any multiplicative function via prime powers.
 * 3. Square Root Block Division (Hyperbola method) for 1D and 2D floor sums.
 * 4. Du Sieve (Sieve of Du) for Mertens function M(N) and Totient sum Phi(N) in O(N^(2/3)).
 * 5. Fast Solvers for Coprime Pairs and Sum of GCDs.
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <functional>
#include <unordered_map>
#include <cassert>
#include <algorithm>

namespace cp::multiplicative {

// =========================================================================
// 1. Classic Multiplicative Sieve (mu, phi, d, sigma) in O(N)
// =========================================================================
class ClassicSieve {
public:
    int n;
    std::vector<int> primes;
    std::vector<int> spf;       // Smallest Prime Factor
    std::vector<int> mu;        // Mobius function
    std::vector<int> phi;       // Euler's Totient function
    std::vector<int> d;         // Number of divisors d(n)
    std::vector<long long> sig; // Sum of divisors sigma(n)
    std::vector<int> num;       // Smallest prime factor count for d(n)
    std::vector<long long> sp_sum; // Sum of prime powers 1 + p + ... + p^k for sigma

    std::vector<int> pref_mu;
    std::vector<long long> pref_phi;

    explicit ClassicSieve(int max_n) : n(max_n), spf(max_n + 1), mu(max_n + 1),
        phi(max_n + 1), d(max_n + 1), sig(max_n + 1), num(max_n + 1), sp_sum(max_n + 1),
        pref_mu(max_n + 1), pref_phi(max_n + 1) {
        
        mu[1] = 1;
        phi[1] = 1;
        d[1] = 1;
        sig[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
                mu[i] = -1;
                phi[i] = i - 1;
                d[i] = 2;
                sig[i] = 1 + i;
                num[i] = 1;
                sp_sum[i] = 1 + i;
            }
            for (int p : primes) {
                if (p > spf[i] || 1LL * i * p > n) break;
                int nxt = i * p;
                spf[nxt] = p;
                if (spf[i] == p) {
                    mu[nxt] = 0;
                    phi[nxt] = phi[i] * p;
                    num[nxt] = num[i] + 1;
                    d[nxt] = d[i] / (num[i] + 1) * (num[nxt] + 1);
                    sp_sum[nxt] = sp_sum[i] * p + 1;
                    sig[nxt] = sig[i] / sp_sum[i] * sp_sum[nxt];
                    break;
                } else {
                    mu[nxt] = -mu[i];
                    phi[nxt] = phi[i] * (p - 1);
                    d[nxt] = d[i] * 2;
                    sig[nxt] = sig[i] * (1 + p);
                    num[nxt] = 1;
                    sp_sum[nxt] = 1 + p;
                }
            }
        }

        // Build prefix sums
        pref_mu[0] = 0;
        pref_phi[0] = 0;
        for (int i = 1; i <= n; ++i) {
            pref_mu[i] = pref_mu[i - 1] + mu[i];
            pref_phi[i] = pref_phi[i - 1] + phi[i];
        }
    }
};

// =========================================================================
// 2. General Linear Sieve Framework for ANY Multiplicative Function
// =========================================================================
template <typename T>
class GeneralMultiplicativeSieve {
public:
    int n;
    std::vector<int> primes;
    std::vector<int> spf;
    std::vector<int> low; // Highest power of spf[x] dividing x: p^k || x
    std::vector<T> f;     // Function values f(x)
    std::vector<T> pref;  // Prefix sums

    /**
     * @param max_n Maximum value to sieve
     * @param f_pk Function computing f(p^k) given prime p, exponent k, and value p^k
     */
    GeneralMultiplicativeSieve(int max_n, const std::function<T(int p, int k, int pk)>& f_pk)
        : n(max_n), spf(max_n + 1, 0), low(max_n + 1, 0), f(max_n + 1), pref(max_n + 1) {
        
        f[1] = 1;
        low[1] = 1;

        for (int i = 2; i <= n; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
                low[i] = i;
                f[i] = f_pk(i, 1, i);
            }
            for (int p : primes) {
                if (p > spf[i] || 1LL * i * p > n) break;
                int nxt = i * p;
                spf[nxt] = p;
                if (spf[i] == p) {
                    low[nxt] = low[i] * p;
                    if (nxt == low[nxt]) {
                        // nxt is a pure prime power p^(k+1)
                        // Count exponent
                        int temp = nxt, k = 0;
                        while (temp % p == 0) {
                            temp /= p;
                            ++k;
                        }
                        f[nxt] = f_pk(p, k, nxt);
                    } else {
                        // Split into coprime parts: low[nxt] and nxt / low[nxt]
                        f[nxt] = f[low[nxt]] * f[nxt / low[nxt]];
                    }
                    break;
                } else {
                    low[nxt] = p;
                    f[nxt] = f[p] * f[i];
                }
            }
        }

        pref[0] = 0;
        for (int i = 1; i <= n; ++i) {
            pref[i] = pref[i - 1] + f[i];
        }
    }
};

// =========================================================================
// 3. Square Root Block Division Utilities (Floor Division Trick)
// =========================================================================

/**
 * @brief Iterate over intervals [l, r] where floor(n / i) is constant.
 * @complexity O(sqrt(n))
 */
template <typename Callback>
void block_division_1d(long long n, Callback&& func) {
    for (long long l = 1, r; l <= n; l = r + 1) {
        long long v = n / l;
        r = n / v;
        func(l, r, v);
    }
}

/**
 * @brief Iterate over intervals [l, r] where both floor(n / i) and floor(m / i) are constant.
 * @complexity O(sqrt(n) + sqrt(m))
 */
template <typename Callback>
void block_division_2d(long long n, long long m, Callback&& func) {
    long long lim = std::min(n, m);
    for (long long l = 1, r; l <= lim; l = r + 1) {
        long long vn = n / l;
        long long vm = m / l;
        r = std::min(n / vn, m / vm);
        func(l, r, vn, vm);
    }
}

// =========================================================================
// 4. Sieve of Du (Du Sieve) — O(N^(2/3)) Prefix Sum for Large N (up to 10^11)
// =========================================================================

struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = 1999999973ULL;
        return splitmix64(x + FIXED_RANDOM);
    }
};

class DuSieve {
private:
    int threshold;
    std::vector<int> primes;
    std::vector<int> spf;
    std::vector<int> mu;
    std::vector<int> phi;
    std::vector<int> pref_mu;
    std::vector<long long> pref_phi;

    std::unordered_map<long long, int, CustomHash> memo_mu;
    std::unordered_map<long long, long long, CustomHash> memo_phi;

public:
    /**
     * @param max_sieve Sieve threshold B. Typically choose B ~ N^(2/3).
     * For N = 10^9, B = 2_000_000 is optimal.
     */
    explicit DuSieve(int max_sieve = 2000000)
        : threshold(max_sieve), spf(max_sieve + 1), mu(max_sieve + 1),
          phi(max_sieve + 1), pref_mu(max_sieve + 1), pref_phi(max_sieve + 1) {
        
        mu[1] = 1;
        phi[1] = 1;

        for (int i = 2; i <= threshold; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
                mu[i] = -1;
                phi[i] = i - 1;
            }
            for (int p : primes) {
                if (p > spf[i] || 1LL * i * p > threshold) break;
                int nxt = i * p;
                spf[nxt] = p;
                if (spf[i] == p) {
                    mu[nxt] = 0;
                    phi[nxt] = phi[i] * p;
                    break;
                } else {
                    mu[nxt] = -mu[i];
                    phi[nxt] = phi[i] * (p - 1);
                }
            }
        }

        pref_mu[0] = 0;
        pref_phi[0] = 0;
        for (int i = 1; i <= threshold; ++i) {
            pref_mu[i] = pref_mu[i - 1] + mu[i];
            pref_phi[i] = pref_phi[i - 1] + phi[i];
        }
    }

    /**
     * @brief Compute Mertens function M(n) = sum_{i=1}^n mu(i)
     */
    int query_mu(long long n) {
        if (n <= threshold) return pref_mu[n];
        if (auto it = memo_mu.find(n); it != memo_mu.end()) return it->second;

        // Identity: mu * 1 = epsilon => S_eps(n) = 1
        // M(n) = 1 - sum_{d=2}^n M(floor(n / d))
        int ans = 1;
        for (long long l = 2, r; l <= n; l = r + 1) {
            long long v = n / l;
            r = n / v;
            ans -= static_cast<int>((r - l + 1) * query_mu(v));
        }
        return memo_mu[n] = ans;
    }

    /**
     * @brief Compute Totient sum Phi(n) = sum_{i=1}^n phi(i)
     */
    long long query_phi(long long n) {
        if (n <= threshold) return pref_phi[n];
        if (auto it = memo_phi.find(n); it != memo_phi.end()) return it->second;

        // Identity: phi * 1 = Id => S_Id(n) = n * (n + 1) / 2
        // Phi(n) = n * (n + 1) / 2 - sum_{d=2}^n Phi(floor(n / d))
        long long ans;
        if (n % 2 == 0) {
            ans = (n / 2) * (n + 1);
        } else {
            ans = n * ((n + 1) / 2);
        }

        for (long long l = 2, r; l <= n; l = r + 1) {
            long long v = n / l;
            r = n / v;
            ans -= (r - l + 1) * query_phi(v);
        }
        return memo_phi[n] = ans;
    }
};

// =========================================================================
// 5. Classic CP Solvers
// =========================================================================

/**
 * @brief Count number of pairs (i, j) with 1 <= i <= n, 1 <= j <= m such that gcd(i, j) = 1.
 * Uses Mobius inversion: sum_{d=1}^min(n, m) mu(d) * floor(n / d) * floor(m / d)
 */
long long count_coprime_pairs(long long n, long long m, const ClassicSieve& sieve) {
    long long ans = 0;
    block_division_2d(n, m, [&](long long l, long long r, long long vn, long long vm) {
        long long sum_mu = sieve.pref_mu[r] - sieve.pref_mu[l - 1];
        ans += sum_mu * vn * vm;
    });
    return ans;
}

/**
 * @brief Calculate sum_{i=1}^n sum_{j=1}^m gcd(i, j).
 * Identity: gcd(i, j) = sum_{d | gcd(i, j)} phi(d)
 * Sum = sum_{d=1}^min(n, m) phi(d) * floor(n / d) * floor(m / d)
 */
long long sum_gcd(long long n, long long m, const ClassicSieve& sieve) {
    long long ans = 0;
    block_division_2d(n, m, [&](long long l, long long r, long long vn, long long vm) {
        long long sum_phi = sieve.pref_phi[r] - sieve.pref_phi[l - 1];
        ans += sum_phi * vn * vm;
    });
    return ans;
}

} // namespace cp::multiplicative

// =========================================================================
// Self-Testing & Verification
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using namespace cp::multiplicative;

    const int MAX_VAL = 100000;
    ClassicSieve classic(MAX_VAL);

    // 1. Verify Classic Sieve values
    assert(classic.mu[1] == 1);
    assert(classic.mu[2] == -1);
    assert(classic.mu[4] == 0);
    assert(classic.mu[6] == 1);

    assert(classic.phi[1] == 1);
    assert(classic.phi[6] == 2);
    assert(classic.phi[12] == 4);

    assert(classic.d[1] == 1);
    assert(classic.d[6] == 4);
    assert(classic.d[12] == 6);

    assert(classic.sig[1] == 1);
    assert(classic.sig[6] == 12);
    assert(classic.sig[12] == 28);

    // 2. Verify General Multiplicative Sieve (reconstructing sigma_2)
    // sigma_2(p^k) = (p^(2(k+1)) - 1) / (p^2 - 1) = 1 + p^2 + p^4 + ... + p^(2k)
    GeneralMultiplicativeSieve<long long> sigma2(MAX_VAL, [](int p, int k, int pk) -> long long {
        (void)pk;
        long long res = 1, cur = 1, p2 = 1LL * p * p;
        for (int i = 1; i <= k; ++i) {
            cur *= p2;
            res += cur;
        }
        return res;
    });

    // For n = 6: divisors are 1, 2, 3, 6 => sum of squares = 1 + 4 + 9 + 36 = 50
    assert(sigma2.f[6] == 50);
    // For n = 4: divisors 1, 2, 4 => 1 + 4 + 16 = 21
    assert(sigma2.f[4] == 21);

    // 3. Verify Block Division 1D for sum of divisors sum_{i=1}^n d(i)
    long long n = 1000;
    long long sum_d_block = 0;
    block_division_1d(n, [&](long long l, long long r, long long v) {
        sum_d_block += (r - l + 1) * v;
    });
    long long sum_d_naive = 0;
    for (int i = 1; i <= n; ++i) sum_d_naive += classic.d[i];
    assert(sum_d_block == sum_d_naive);

    // 4. Verify Du Sieve for Mertens and Totient sums
    DuSieve du(50000);
    assert(du.query_mu(1000) == classic.pref_mu[1000]);
    assert(du.query_phi(1000) == classic.pref_phi[1000]);

    // Test Du Sieve on larger value N = 100_000
    assert(du.query_mu(MAX_VAL) == classic.pref_mu[MAX_VAL]);
    assert(du.query_phi(MAX_VAL) == classic.pref_phi[MAX_VAL]);

    // 5. Verify Coprime count & Sum GCD
    long long n_coprime = count_coprime_pairs(100, 100, classic);
    long long naive_coprime = 0;
    for (int i = 1; i <= 100; ++i) {
        for (int j = 1; j <= 100; ++j) {
            if (std::gcd(i, j) == 1) ++naive_coprime;
        }
    }
    assert(n_coprime == naive_coprime);

    long long n_sum_gcd = sum_gcd(100, 100, classic);
    long long naive_sum_gcd = 0;
    for (int i = 1; i <= 100; ++i) {
        for (int j = 1; j <= 100; ++j) {
            naive_sum_gcd += std::gcd(i, j);
        }
    }
    assert(n_sum_gcd == naive_sum_gcd);

    std::cout << "[SUCCESS] All Multiplicative Functions test cases passed perfectly!\n";
    std::cout << "Mertens(10^5) = " << du.query_mu(100000) << "\n";
    std::cout << "PhiSum(10^5)  = " << du.query_phi(100000) << "\n";
    std::cout << "CoprimePairs(100, 100) = " << n_coprime << "\n";
    std::cout << "SumGCD(100, 100)       = " << n_sum_gcd << "\n";

    return 0;
}
