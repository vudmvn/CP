/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Kỹ thuật Băm (Double Hashing, Mersenne-61, Anti-Hash Defense & XOR Hashing)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <unordered_map>
#include <cstdint>
#include <cassert>

using namespace std;

// Bộ sinh số ngẫu nhiên thời gian thực
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// ============================================================================
// 1. DOUBLE STRING HASHING VỚI RANDOMIZED BASE
// ============================================================================

class DoubleStringHash {
private:
    static inline const long long MOD1 = 1000000007LL;
    static inline const long long MOD2 = 1000000009LL;
    static inline long long BASE1 = 0;
    static inline long long BASE2 = 0;

    int n;
    vector<long long> H1, H2;
    vector<long long> P1, P2;

    static void initBases() {
        if (BASE1 == 0) {
            uniform_int_distribution<long long> dist(300, 10000);
            BASE1 = dist(rng);
            if (BASE1 % 2 == 0) ++BASE1;
            BASE2 = dist(rng);
            if (BASE2 % 2 == 0) ++BASE2;
        }
    }

public:
    DoubleStringHash(const string& s) {
        initBases();
        n = s.size();
        H1.assign(n + 1, 0);
        H2.assign(n + 1, 0);
        P1.assign(n + 1, 1);
        P2.assign(n + 1, 1);

        for (int i = 0; i < n; ++i) {
            H1[i + 1] = (H1[i] * BASE1 + (unsigned char)s[i]) % MOD1;
            H2[i + 1] = (H2[i] * BASE2 + (unsigned char)s[i]) % MOD2;
            P1[i + 1] = (P1[i] * BASE1) % MOD1;
            P2[i + 1] = (P2[i] * BASE2) % MOD2;
        }
    }

    /**
     * @brief Lấy mã băm kép của đoạn con s[L..R] (0-indexed) trong O(1)
     */
    pair<long long, long long> getHash(int L, int R) const {
        assert(0 <= L && L <= R && R < n);
        long long h1 = (H1[R + 1] - H1[L] * P1[R - L + 1]) % MOD1;
        if (h1 < 0) h1 += MOD1;

        long long h2 = (H2[R + 1] - H2[L] * P2[R - L + 1]) % MOD2;
        if (h2 < 0) h2 += MOD2;

        return {h1, h2};
    }
};

// ============================================================================
// 2. BĂM 61-BIT TRÊN SỐ NGUYÊN TỐ MERSENNE (2^61 - 1)
// ============================================================================

class Mersenne61Hash {
private:
    static const uint64_t MOD = (1ULL << 61) - 1;
    static inline uint64_t BASE = 0;

    int n;
    vector<uint64_t> H;
    vector<uint64_t> P;

    // Nhân an toàn modulo 2^61 - 1 bằng bitwise
    static uint64_t modMul(uint64_t a, uint64_t b) {
        __uint128_t prod = (__uint128_t)a * b;
        uint64_t res = (uint64_t)(prod >> 61) + (uint64_t)(prod & MOD);
        if (res >= MOD) res -= MOD;
        return res;
    }

    static void initBase() {
        if (BASE == 0) {
            uniform_int_distribution<uint64_t> dist(257, 1000000);
            BASE = dist(rng);
            if (BASE % 2 == 0) ++BASE;
        }
    }

public:
    Mersenne61Hash(const string& s) {
        initBase();
        n = s.size();
        H.assign(n + 1, 0);
        P.assign(n + 1, 1);

        for (int i = 0; i < n; ++i) {
            H[i + 1] = modMul(H[i], BASE) + (unsigned char)s[i];
            if (H[i + 1] >= MOD) H[i + 1] -= MOD;

            P[i + 1] = modMul(P[i], BASE);
        }
    }

    uint64_t getHash(int L, int R) const {
        assert(0 <= L && L <= R && R < n);
        uint64_t res = H[R + 1] + MOD - modMul(H[L], P[R - L + 1]);
        if (res >= MOD) res -= MOD;
        return res;
    }
};

// ============================================================================
// 3. THUẬT TOÁN SO KHỚP XÂU RABIN-KARP O(N + M)
// ============================================================================

vector<int> rabinKarp(const string& text, const string& pattern) {
    int n = text.size();
    int m = pattern.size();
    if (m == 0 || n < m) return {};

    DoubleStringHash hashText(text);
    DoubleStringHash hashPat(pattern);
    auto targetHash = hashPat.getHash(0, m - 1);

    vector<int> occurrences;
    for (int i = 0; i <= n - m; ++i) {
        if (hashText.getHash(i, i + m - 1) == targetHash) {
            occurrences.push_back(i);
        }
    }
    return occurrences;
}

// ============================================================================
// 4. BẢNG BĂM AN TOÀN CHỐNG ANTI-HASH (SPLITMIX64)
// ============================================================================

struct SafeHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

template<typename K, typename V>
using SafeUnorderedMap = unordered_map<K, V, SafeHash>;

// ============================================================================
// 5. BĂM TẬP HỢP BẰNG XOR HASHING (ZOBRIST HASHING)
// ============================================================================

class XORSetHash {
private:
    SafeUnorderedMap<long long, uint64_t> valMap;

    uint64_t getWeight(long long val) {
        auto it = valMap.find(val);
        if (it != valMap.end()) return it->second;
        uint64_t w = rng();
        valMap[val] = w;
        return w;
    }

public:
    uint64_t hashSet(const vector<long long>& elements) {
        uint64_t h = 0;
        for (long long x : elements) {
            h ^= getWeight(x);
        }
        return h;
    }
};

// ============================================================================
// 6. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    // 1. Test Double String Hash
    string s = "abacaba";
    DoubleStringHash dsh(s);
    // Đoạn "aba" tại [0..2] và [4..6] phải có hash bằng nhau
    assert(dsh.getHash(0, 2) == dsh.getHash(4, 6));
    // Đoạn "aba" tại [0..2] và "bac" tại [1..3] phải khác nhau
    assert(dsh.getHash(0, 2) != dsh.getHash(1, 3));

    // 2. Test Mersenne 61-bit Hash
    Mersenne61Hash m61(s);
    assert(m61.getHash(0, 2) == m61.getHash(4, 6));
    assert(m61.getHash(0, 2) != m61.getHash(1, 3));

    // 3. Test Rabin-Karp String Matching
    string text = "AABAACAADAABAABA";
    string pat = "AABA";
    auto occ = rabinKarp(text, pat);
    // Vị trí xuất hiện: 0, 9, 12
    vector<int> expected_occ = {0, 9, 12};
    assert(occ == expected_occ);

    // 4. Test XOR Set Hash
    XORSetHash xorHash;
    vector<long long> setA = {1, 5, 10, 20};
    vector<long long> setB = {20, 1, 10, 5}; // Cùng phần tử nhưng khác thứ tự
    vector<long long> setC = {1, 5, 10, 25}; // Khác phần tử
    assert(xorHash.hashSet(setA) == xorHash.hashSet(setB));
    assert(xorHash.hashSet(setA) != xorHash.hashSet(setC));

    // 5. Test SafeUnorderedMap
    SafeUnorderedMap<long long, string> safeMap;
    safeMap[12345678901234LL] = "anti-hash-safe";
    assert(safeMap[12345678901234LL] == "anti-hash-safe");

    cout << "==> ALL HASHING & ANTI-HASH TESTS PASSED SUCCESSFULLY!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
