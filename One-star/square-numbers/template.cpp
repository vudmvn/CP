/**
 * @file template.cpp
 * @brief Bộ thư viện chuẩn: Số chính phương, Square-Free Core, Hàm Möbius & Đếm Square-Free
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
#include <map>
#include <cassert>

using namespace std;

namespace SquareMath {

    /**
     * @brief Tính căn bậc hai nguyên lớn nhất floor(sqrt(n)) an toàn 100% với số 64-bit.
     * Độ phức tạp: O(1)
     */
    long long integer_sqrt(long long n) {
        if (n <= 0) return 0;
        long long r = (long long)round(sqrtl((long double)n));
        // Điều chỉnh biên an toàn
        while ((__int128)(r + 1) * (r + 1) <= n) r++;
        while ((__int128)r * r > n) r--;
        return r;
    }

    /**
     * @brief Kiểm tra n có phải là số chính phương hay không.
     * Tối ưu hóa bằng bộ lọc bitmask modulo 64 loại bỏ ~82% số không chính phương trong 1 thao tác bit.
     * Sau đó kiểm tra bằng integer_sqrt an toàn sai số.
     */
    bool is_perfect_square(long long n) {
        if (n < 0) return false;
        // Bảng lọc thặng dư chính phương modulo 64
        // Các số chính phương mod 64 chỉ có thể là 0, 1, 4, 9, 16, 17, 25, 36, 41, 49, 57
        constexpr unsigned long long mask64 = 
            (1ULL << 0)  | (1ULL << 1)  | (1ULL << 4)  | (1ULL << 9)  |
            (1ULL << 16) | (1ULL << 17) | (1ULL << 25) | (1ULL << 36) |
            (1ULL << 41) | (1ULL << 49) | (1ULL << 57);

        if (((mask64 >> (n & 63)) & 1ULL) == 0) {
            return false; // Chắc chắn không phải số chính phương
        }

        long long r = integer_sqrt(n);
        return r * r == n;
    }

    /**
     * @brief Tìm Square-Free Core (hạt nhân không chính phương) của số n: n = core * k^2.
     * core là tích các thừa số nguyên tố có số mũ lẻ.
     * Độ phức tạp: O(sqrt(n))
     */
    long long get_square_free_core(long long n) {
        if (n <= 0) return n;
        long long core = 1;
        for (long long d = 2; d * d <= n; d++) {
            if (n % d == 0) {
                int cnt = 0;
                while (n % d == 0) {
                    cnt++;
                    n /= d;
                }
                if (cnt % 2 != 0) {
                    core *= d;
                }
            }
        }
        if (n > 1) {
            core *= n; // Thừa số nguyên tố cuối cùng có số mũ 1
        }
        return core;
    }

    /**
     * @brief Cấu trúc Sàng tuyến tính Euler tiền xử lý:
     * - Hàm Möbius mu[i] (-1, 0, 1)
     * - Square-Free Core core[i]
     * - Đánh dấu số square-free
     * Độ phức tạp tiền xử lý: O(MAX)
     */
    struct LinearSieve {
        int max_val;
        vector<int> primes;
        vector<int> min_prime;
        vector<int> mu;
        vector<int> core;
        vector<bool> is_sqfree;

        LinearSieve(int n) : max_val(n), min_prime(n + 1, 0), mu(n + 1, 0), 
                             core(n + 1, 1), is_sqfree(n + 1, false) {
            mu[1] = 1;
            core[1] = 1;
            is_sqfree[1] = true;

            for (int i = 2; i <= n; ++i) {
                if (min_prime[i] == 0) {
                    min_prime[i] = i;
                    primes.push_back(i);
                    mu[i] = -1;
                    core[i] = i;
                    is_sqfree[i] = true;
                }
                for (int p : primes) {
                    if (p > min_prime[i] || i * p > n) break;
                    min_prime[i * p] = p;
                    if (i % p == 0) {
                        mu[i * p] = 0; // i * p chia hết cho p^2 => không square-free
                        is_sqfree[i * p] = false;
                        // core của (i * p): nếu i đã có thừa số p lẻ thì nhân p vào sẽ thành chẵn (bỏ p),
                        // ngược lại nếu i có thừa số p chẵn thì nhân p vào sẽ thành lẻ.
                        // Đơn giản: core[i * p] = core[i / (p^(cnt))] ...
                        // Hoặc tính: nếu core[i] chia hết cho p thì core[i * p] = core[i] / p, ngược lại core[i] * p
                        core[i * p] = (core[i] % p == 0) ? (core[i] / p) : (core[i] * p);
                        break;
                    } else {
                        mu[i * p] = -mu[i];
                        is_sqfree[i * p] = is_sqfree[i];
                        core[i * p] = core[i] * p;
                    }
                }
            }
        }
    };

    /**
     * @brief Đếm số lượng số square-free trong đoạn [1, N] bằng công thức bao hàm loại trừ:
     * count = sum_{k=1}^{floor(sqrt(N))} mu(k) * floor(N / k^2)
     * Yêu cầu: mảng mu đã được tiền xử lý tối thiểu tới floor(sqrt(N)).
     * Độ phức tạp: O(sqrt(N))
     */
    long long count_square_free(long long N, const vector<int>& mu) {
        if (N <= 0) return 0;
        long long limit = integer_sqrt(N);
        long long count = 0;
        for (long long k = 1; k <= limit; ++k) {
            if (mu[k] != 0) {
                count += (long long)mu[k] * (N / (k * k));
            }
        }
        return count;
    }

    /**
     * @brief Tìm số Square-Free thứ K bằng Chặt nhị phân kết quả.
     * Cận trên high = 2 * K (vì mật độ số square-free là 6 / pi^2 ~ 60.79%).
     * Độ phức tạp: O(sqrt(K) * log(K))
     */
    long long kth_square_free(long long K, const vector<int>& mu) {
        long long low = 1, high = 2 * K + 50;
        long long ans = high;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (count_square_free(mid, mu) >= K) {
                ans = mid;
                high = mid - 1; // Thử tìm giá trị nhỏ hơn
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }

    /**
     * @brief Đếm số cặp (i < j) có tích a[i] * a[j] là số chính phương.
     * Sử dụng tính chất: a[i] * a[j] là chính phương <=> core(a[i]) == core(a[j]).
     * Độ phức tạp: O(N * sqrt(max A)) hoặc O(N log N)
     */
    long long count_pairs_product_perfect_square(const vector<long long>& arr) {
        map<long long, long long> freq;
        long long ans = 0;
        for (long long x : arr) {
            long long c = get_square_free_core(x);
            ans += freq[c];
            freq[c]++;
        }
        return ans;
    }

} // namespace SquareMath

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    cout << "=== 1. DEMO KIỂM TRA SỐ CHÍNH PHƯƠNG ===" << "\n";
    assert(SquareMath::is_perfect_square(0) == true);
    assert(SquareMath::is_perfect_square(1) == true);
    assert(SquareMath::is_perfect_square(4) == true);
    assert(SquareMath::is_perfect_square(144) == true);
    assert(SquareMath::is_perfect_square(1000000000000000000LL) == true); // 10^18 = (10^9)^2
    assert(SquareMath::is_perfect_square(999999999999999999LL) == false);
    assert(SquareMath::is_perfect_square(2) == false);
    assert(SquareMath::is_perfect_square(-4) == false);
    cout << "is_perfect_square: PASS!\n";

    cout << "\n=== 2. DEMO SQUARE-FREE CORE ===" << "\n";
    // 72 = 2 * 6^2 => core = 2
    assert(SquareMath::get_square_free_core(72) == 2);
    // 75 = 3 * 5^2 => core = 3
    assert(SquareMath::get_square_free_core(75) == 3);
    // 12 = 3 * 2^2 => core = 3
    assert(SquareMath::get_square_free_core(12) == 3);
    cout << "Square-free core: PASS!\n";

    cout << "\n=== 3. DEMO ĐẾM CẶP CÓ TÍCH LÀ SỐ CHÍNH PHƯƠNG ===" << "\n";
    // A = [12, 18, 27, 8, 50]
    // core: 12->3, 18->2, 27->3, 8->2, 50->2
    // Cặp (12, 27): 1 cặp
    // Cặp (18, 8), (18, 50), (8, 50): 3 cặp => Tổng = 4
    vector<long long> a = {12, 18, 27, 8, 50};
    long long pairs = SquareMath::count_pairs_product_perfect_square(a);
    cout << "So cap co tich la so chinh phuong: " << pairs << " (Ky vong: 4)\n";
    assert(pairs == 4);

    cout << "\n=== 4. DEMO SÀNG MÖBIUS & ĐẾM SQUARE-FREE ===" << "\n";
    int N_LIMIT = 1000000;
    SquareMath::LinearSieve sieve(N_LIMIT);

    // Kiểm tra các giá trị hàm mu nhỏ:
    // mu(1)=1, mu(2)=-1, mu(3)=-1, mu(4)=0, mu(5)=-1, mu(6)=1, mu(7)=-1, mu(8)=0, mu(9)=0, mu(10)=1
    assert(sieve.mu[1] == 1);
    assert(sieve.mu[4] == 0);
    assert(sieve.mu[6] == 1);
    assert(sieve.mu[30] == -1); // 2 * 3 * 5 => 3 ước => (-1)^3 = -1

    // Đếm số square-free <= 30:
    // Các số không square-free <= 30 là: 4, 8, 9, 12, 16, 18, 20, 24, 25, 27, 28 (11 số)
    // Tổng số square-free <= 30 là: 30 - 11 = 19 số.
    long long sf_30 = SquareMath::count_square_free(30, sieve.mu);
    cout << "So luong square-free <= 30: " << sf_30 << " (Ky vong: 19)\n";
    assert(sf_30 == 19);

    cout << "\n=== 5. DEMO TÌM SỐ SQUARE-FREE THỨ K ===" << "\n";
    // 10 số square-free đầu tiên: 1, 2, 3, 5, 6, 7, 10, 11, 13, 14
    // Số thứ 10 là 14
    long long kth_10 = SquareMath::kth_square_free(10, sieve.mu);
    cout << "So square-free thu 10: " << kth_10 << " (Ky vong: 14)\n";
    assert(kth_10 == 14);

    // Số thứ 19 là 30
    long long kth_19 = SquareMath::kth_square_free(19, sieve.mu);
    cout << "So square-free thu 19: " << kth_19 << " (Ky vong: 30)\n";
    assert(kth_19 == 30);

    cout << "\nTat ca cac assertions deu PASS 100% thanh cong!\n";
    return 0;
}
