/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Số học Modulo & Nghịch đảo Modulo trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

using namespace std;

// ==========================================
// 1. CÁC HÀM SỐ HỌC CƠ BẢN
// ==========================================

/**
 * @brief Lũy thừa nhị phân: (base^exp) % mod
 * @complexity O(log exp)
 */
long long power(long long base, long long exp, long long mod) {
    long long res = 1 % mod;
    base %= mod;
    if (base < 0) base += mod;
    while (exp > 0) {
        if (exp & 1) res = (long long)((__int128)res * base % mod);
        base = (long long)((__int128)base * base % mod);
        exp >>= 1;
    }
    return res;
}

/**
 * @brief Thuật toán Euclid mở rộng: Tìm x, y thỏa mãn a*x + b*y = gcd(a, b)
 * @return d = gcd(a, b)
 * @complexity O(log(min(a, b)))
 */
long long ext_gcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long x1, y1;
    long long d = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

/**
 * @brief Tìm nghịch đảo modulo m của a: a * x = 1 (mod m)
 * @note Hoạt động với mọi m (kể cả hợp số) miễn là gcd(a, m) == 1.
 * @return Giá trị x thuộc [0, m - 1], hoặc -1 nếu không tồn tại nghịch đảo.
 */
long long inv_mod(long long a, long long m) {
    long long x, y;
    long long g = ext_gcd(a, m, x, y);
    if (g != 1) return -1; // Không tồn tại nghịch đảo
    return (x % m + m) % m;
}

// ==========================================
// 2. STRUCT MINT (MODULAR ARITHMETIC WRAPPER)
// ==========================================

template <int MOD>
struct Mint {
    int v;
    static const int get_mod() { return MOD; }

    Mint() : v(0) {}
    Mint(long long _v) {
        v = int((-MOD < _v && _v < MOD) ? _v : _v % MOD);
        if (v < 0) v += MOD;
    }

    friend bool operator==(const Mint &a, const Mint &b) { return a.v == b.v; }
    friend bool operator!=(const Mint &a, const Mint &b) { return a.v != b.v; }
    friend bool operator<(const Mint &a, const Mint &b) { return a.v < b.v; }

    Mint &operator+=(const Mint &o) {
        if ((v += o.v) >= MOD) v -= MOD;
        return *this;
    }
    Mint &operator-=(const Mint &o) {
        if ((v -= o.v) < 0) v += MOD;
        return *this;
    }
    Mint &operator*=(const Mint &o) {
        v = int((long long)v * o.v % MOD);
        return *this;
    }
    Mint &operator/=(const Mint &o) { return (*this) *= o.inv(); }

    Mint pow(long long p) const {
        assert(p >= 0);
        Mint a = *this, res = 1;
        while (p > 0) {
            if (p & 1) res *= a;
            a *= a;
            p >>= 1;
        }
        return res;
    }

    Mint inv() const {
        int x = inv_mod(v, MOD);
        assert(x != -1); // Đảm bảo nghịch đảo tồn tại
        return Mint(x);
    }

    Mint operator+() const { return *this; }
    Mint operator-() const { return Mint(0) - *this; }

    friend Mint operator+(Mint a, const Mint &b) { return a += b; }
    friend Mint operator-(Mint a, const Mint &b) { return a -= b; }
    friend Mint operator*(Mint a, const Mint &b) { return a *= b; }
    friend Mint operator/(Mint a, const Mint &b) { return a /= b; }

    friend istream &operator>>(istream &is, Mint &a) {
        long long t;
        is >> t;
        a = Mint(t);
        return is;
    }
    friend ostream &operator<<(ostream &os, const Mint &a) {
        return os << a.v;
    }
};

const int MOD = 1e9 + 7;
using mint = Mint<MOD>;

// ==========================================
// 3. TIỀN XỬ LÝ TỔ HỢP nCr VÀ NGHỊCH ĐẢO k! MODULO P
// ==========================================

struct Combinatorics {
    int n;
    vector<mint> fact, invFact;

    Combinatorics(int _n = 0) { init(_n); }

    /**
     * @brief Tiền xử lý k! và (k!)^(-1) trong O(N)
     * Sử dụng phương pháp lan truyền lùi từ N!
     */
    void init(int _n) {
        n = _n;
        fact.resize(n + 1);
        invFact.resize(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;

        // B1: Tính nghịch đảo của N! bằng Fermat nhỏ
        invFact[n] = fact[n].inv();

        // B2: Lan truyền lùi: ((i)!)^(-1) = ((i+1)!)^(-1) * (i+1)
        for (int i = n - 1; i >= 0; --i) {
            invFact[i] = invFact[i + 1] * (i + 1);
        }
        assert(invFact[0] == 1); // Sanity check: (0!)^(-1) phải bằng 1
    }

    mint nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * invFact[r] * invFact[n - r];
    }

    mint nPr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return fact[n] * invFact[n - r];
    }

    /**
     * @brief Nghịch đảo modulo của số k trong O(1): k^(-1) = invFact[k] * fact[k-1]
     */
    mint inv(int k) {
        assert(k >= 1 && k <= n);
        return invFact[k] * fact[k - 1];
    }
};

// ==========================================
// 4. HỆ THẶNG DƯ TRUNG HOA TỔNG QUÁT (ExCRT)
// ==========================================

/**
 * @brief Giải hệ phương trình đồng dư: x = a[i] (mod m[i])
 * @param a Mảng số dư
 * @param m Mảng modulo (không yêu cầu nguyên tố cùng nhau)
 * @return Cặp {x, M} sao cho nghiệm là x (mod M). Trả về {-1, -1} nếu vô nghiệm.
 */
pair<long long, long long> crt(const vector<long long> &a, const vector<long long> &m) {
    long long x0 = 0, m0 = 1;
    for (size_t i = 0; i < a.size(); ++i) {
        long long a1 = (a[i] % m[i] + m[i]) % m[i];
        long long m1 = m[i];
        long long p, q;
        long long g = ext_gcd(m0, m1, p, q);
        if ((a1 - x0) % g != 0) return {-1, -1}; // Vô nghiệm
        long long mod = m1 / g;
        long long step = (long long)((__int128)((a1 - x0) / g) % mod * (p % mod) % mod);
        if (step < 0) step += mod;
        x0 += step * m0;
        m0 = (m0 / g) * m1;
        x0 = (x0 % m0 + m0) % m0;
    }
    return {x0, m0};
}

// ==========================================
// 5. MAIN DEMO
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Thử nghiệm Mint
    mint a = 1000000000;
    mint b = 2;
    cout << "Mint demo:\n";
    cout << "a + b = " << a + b << "\n";
    cout << "a * b = " << a * b << "\n";
    cout << "a / b = " << a / b << "\n";
    cout << "a^3   = " << a.pow(3) << "\n\n";

    // 2. Thử nghiệm Combinatorics & Nghịch đảo giai thừa
    Combinatorics C(1000);
    cout << "Combinatorics demo:\n";
    cout << "5!       = " << C.fact[5] << "\n";
    cout << "(5!)^(-1) = " << C.invFact[5] << "\n";
    cout << "5! * (5!)^(-1) mod MOD = " << C.fact[5] * C.invFact[5] << " (phai bang 1)\n";
    cout << "5^(-1)   = " << C.inv(5) << "\n";
    cout << "5 * 5^(-1) mod MOD = " << mint(5) * C.inv(5) << " (phai bang 1)\n";
    cout << "C(10, 3) = " << C.nCr(10, 3) << "\n";
    cout << "C(100, 50) mod (1e9+7) = " << C.nCr(100, 50) << "\n\n";

    // 3. Thử nghiệm CRT
    // Hệ phương trình:
    // x = 2 (mod 3)
    // x = 3 (mod 5)
    // x = 2 (mod 7)
    // Nghiệm chuẩn: x = 23 (mod 105)
    vector<long long> rem = {2, 3, 2};
    vector<long long> mod = {3, 5, 7};
    auto ans = crt(rem, mod);
    cout << "CRT demo:\n";
    cout << "x = " << ans.first << " (mod " << ans.second << ")\n";

    return 0;
}
