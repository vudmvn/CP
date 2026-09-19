/**
 * @file template.cpp
 * @brief Thư viện Thuật toán Chuẩn: Quy Hoạch Động Chữ Số (Digit DP)
 * 
 * Chuyên đề: Competitive Programming Handbook
 * Đơn vị: SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * Soạn thảo: Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * Bản quyền (c) 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.
 * 
 * Các cấu trúc & thuật toán:
 *  1. AdjacentDistinctCounter: Đếm số trong [L, R] không có 2 chữ số liền kề giống nhau (CSES 2220).
 *  2. DigitSumDivisible: Đếm số trong [L, R] có tổng chữ số chia hết cho K.
 *  3. DistinctDigitsSum: Tính tổng các số trong [L, R] có tối đa K chữ số khác nhau modulo 1e9+7 (CF 1073E).
 *  4. ClassyNumbersCounter: Đếm số có tối đa 3 chữ số khác 0 (CF 1036C).
 *  5. KthNumberFinder: Tìm số thứ K thỏa mãn điều kiện chữ số qua Binary Search.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>

using namespace std;

// ============================================================================
// 1. ĐẾM SỐ KHÔNG CÓ 2 CHỮ SỐ LIỀN KỀ GIỐNG NHAU (ADJACENT DISTINCT)
// ============================================================================

namespace AdjacentDistinctCounter {
    long long memo[20][11]; // memo[pos][prev_digit + 1]

    long long dfs(const string& s, int pos, bool tight, bool leading_zero, int prev) {
        if (pos == (int)s.size()) return 1;

        if (!tight && !leading_zero && memo[pos][prev + 1] != -1) {
            return memo[pos][prev + 1];
        }

        int limit = tight ? (s[pos] - '0') : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_tight = tight && (d == limit);
            if (leading_zero && d == 0) {
                ans += dfs(s, pos + 1, next_tight, true, -1);
            } else {
                if (d == prev) continue; // Bỏ qua nếu trùng chữ số liền kề
                ans += dfs(s, pos + 1, next_tight, false, d);
            }
        }

        if (!tight && !leading_zero) {
            memo[pos][prev + 1] = ans;
        }
        return ans;
    }

    long long count_upto(long long n) {
        if (n < 0) return 0;
        string s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dfs(s, 0, true, true, -1);
    }

    long long query(long long L, long long R) {
        if (L > R) return 0;
        return count_upto(R) - (L > 0 ? count_upto(L - 1) : 0);
    }
}

// ============================================================================
// 2. ĐẾM SỐ CÓ TỔNG CHỮ SỐ CHIA HẾT CHO K
// ============================================================================

namespace DigitSumDivisible {
    long long memo[20][180]; // pos <= 19, sum <= 19 * 9 = 171

    long long dfs(const string& s, int pos, bool tight, int sum, int k) {
        if (pos == (int)s.size()) return (sum % k == 0 ? 1 : 0);

        if (!tight && memo[pos][sum] != -1) {
            return memo[pos][sum];
        }

        int limit = tight ? (s[pos] - '0') : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_tight = tight && (d == limit);
            ans += dfs(s, pos + 1, next_tight, sum + d, k);
        }

        if (!tight) {
            memo[pos][sum] = ans;
        }
        return ans;
    }

    long long count_upto(long long n, int k) {
        if (n < 0) return 0;
        string s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dfs(s, 0, true, 0, k);
    }

    long long query(long long L, long long R, int k) {
        if (L > R) return 0;
        return count_upto(R, k) - (L > 0 ? count_upto(L - 1, k) : 0);
    }
}

// ============================================================================
// 3. TÍNH TỔNG CÁC SỐ CÓ TỐI ĐA K CHỮ SỐ KHÁC NHAU (CODEFORCES 1073E)
// ============================================================================

namespace DistinctDigitsSum {
    const long long MOD = 1000000007;

    struct Result {
        long long count;
        long long sum;
    };

    Result memo[20][1 << 10];
    bool visited[20][1 << 10];
    long long power10[25];

    void init_powers() {
        power10[0] = 1;
        for (int i = 1; i <= 20; ++i) {
            power10[i] = (power10[i - 1] * 10) % MOD;
        }
    }

    Result dfs(const string& s, int pos, bool tight, bool leading_zero, int mask, int max_k) {
        if (pos == (int)s.size()) {
            if (leading_zero) return {0, 0};
            int cnt = __builtin_popcount(mask);
            if (cnt <= max_k) return {1, 0};
            return {0, 0};
        }

        if (!tight && !leading_zero && visited[pos][mask]) {
            return memo[pos][mask];
        }

        int limit = tight ? (s[pos] - '0') : 9;
        Result ans = {0, 0};

        int remaining_digits = (int)s.size() - 1 - pos;

        for (int d = 0; d <= limit; ++d) {
            bool next_tight = tight && (d == limit);
            Result child;

            if (leading_zero && d == 0) {
                child = dfs(s, pos + 1, next_tight, true, 0, max_k);
                ans.count = (ans.count + child.count) % MOD;
                ans.sum = (ans.sum + child.sum) % MOD;
            } else {
                int next_mask = mask | (1 << d);
                child = dfs(s, pos + 1, next_tight, false, next_mask, max_k);
                
                ans.count = (ans.count + child.count) % MOD;
                // Đóng góp của chữ số d: child.count * d * 10^(len - 1 - pos)
                long long contrib = (child.count * d) % MOD;
                contrib = (contrib * power10[remaining_digits]) % MOD;
                long long total_branch = (child.sum + contrib) % MOD;
                ans.sum = (ans.sum + total_branch) % MOD;
            }
        }

        if (!tight && !leading_zero) {
            visited[pos][mask] = true;
            memo[pos][mask] = ans;
        }
        return ans;
    }

    long long sum_upto(long long n, int max_k) {
        if (n <= 0) return 0;
        string s = to_string(n);
        memset(visited, 0, sizeof(visited));
        init_powers();
        return dfs(s, 0, true, true, 0, max_k).sum;
    }

    long long query(long long L, long long R, int max_k) {
        if (L > R) return 0;
        long long ans = sum_upto(R, max_k) - sum_upto(L - 1, max_k);
        return (ans % MOD + MOD) % MOD;
    }
}

// ============================================================================
// 4. CLASSY NUMBERS: CÓ TỐI ĐA 3 CHỮ SỐ KHÁC 0 (CODEFORCES 1036C)
// ============================================================================

namespace ClassyNumbersCounter {
    long long memo[20][4];

    long long dfs(const string& s, int pos, bool tight, int non_zero_cnt) {
        if (non_zero_cnt > 3) return 0;
        if (pos == (int)s.size()) return 1;

        if (!tight && memo[pos][non_zero_cnt] != -1) {
            return memo[pos][non_zero_cnt];
        }

        int limit = tight ? (s[pos] - '0') : 9;
        long long ans = 0;

        for (int d = 0; d <= limit; ++d) {
            bool next_tight = tight && (d == limit);
            ans += dfs(s, pos + 1, next_tight, non_zero_cnt + (d > 0 ? 1 : 0));
        }

        if (!tight) {
            memo[pos][non_zero_cnt] = ans;
        }
        return ans;
    }

    long long count_upto(long long n) {
        if (n < 0) return 0;
        string s = to_string(n);
        memset(memo, -1, sizeof(memo));
        return dfs(s, 0, true, 0);
    }

    long long query(long long L, long long R) {
        if (L > R) return 0;
        return count_upto(R) - (L > 0 ? count_upto(L - 1) : 0);
    }
}

// ============================================================================
// 5. TÌM SỐ THỨ K THỎA MÃN TÍNH CHẤT QUA BINARY SEARCH
// ============================================================================

namespace KthNumberFinder {
    /**
     * @brief Tìm số thứ K (1-indexed, > 0) không có hai chữ số liền kề giống nhau.
     */
    long long find_kth_adjacent_distinct(long long k) {
        long long low = 1, high = 2e18;
        long long ans = -1;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            // query(1, mid) loại trừ số 0
            if (AdjacentDistinctCounter::query(1, mid) >= k) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return ans;
    }
}

// ============================================================================
// HỆ THỐNG KIỂM THỬ TỰ ĐỘNG (UNIT TESTS)
// ============================================================================

void run_all_tests() {
    cerr << "=== BAT DAU KIEM THU DIGIT DP ===" << endl;

    // 1. Kiểm thử AdjacentDistinctCounter (CSES 2220)
    // [0, 10]: tất cả 11 số đều thỏa mãn (0, 1..9, 10)
    assert(AdjacentDistinctCounter::query(0, 10) == 11);
    // [0, 11]: chỉ trừ số 11 -> 11 số (0..10)
    assert(AdjacentDistinctCounter::query(0, 11) == 11);
    // [0, 22]: trừ 11 và 22 -> 23 - 2 = 21 số
    assert(AdjacentDistinctCounter::query(0, 22) == 21);
    // [123, 321]: 171 số thỏa mãn
    assert(AdjacentDistinctCounter::query(123, 321) == 171);
    cerr << "[PASS] Test 1: AdjacentDistinctCounter (CSES 2220) hoat dong chinh xac" << endl;

    // 2. Kiểm thử DigitSumDivisible
    // [1, 100] có tổng chữ số chẵn (k=2): chính xác 49 số (trong [0, 99] có 50 số, trừ số 0; 100 có tổng lẻ)
    assert(DigitSumDivisible::query(1, 100, 2) == 49);
    assert(DigitSumDivisible::query(1, 101, 2) == 50);
    // [1, 9] có tổng chia hết cho 3: {3, 6, 9} -> 3 số
    assert(DigitSumDivisible::query(1, 9, 3) == 3);
    cerr << "[PASS] Test 2: DigitSumDivisible hoat dong chinh xac" << endl;

    // 3. Kiểm thử DistinctDigitsSum (Codeforces 1073E)
    // [1, 20] có <= 1 chữ số khác nhau: {1..9, 11} -> sum = 45 + 11 = 56
    assert(DistinctDigitsSum::query(1, 20, 1) == 56);
    // [1, 100] có <= 1 chữ số khác nhau: sum = 540
    assert(DistinctDigitsSum::query(1, 100, 1) == 540);
    cerr << "[PASS] Test 3: DistinctDigitsSum (CF 1073E) hoat dong chinh xac" << endl;

    // 4. Kiểm thử ClassyNumbersCounter (Codeforces 1036C)
    // Mọi số từ 1 đến 1000 đều có <= 3 chữ số khác 0
    assert(ClassyNumbersCounter::query(1, 1000) == 1000);
    // 1111 có 4 chữ số khác 0 -> không phải classy number
    assert(ClassyNumbersCounter::query(1, 1111) == 1110);
    cerr << "[PASS] Test 4: ClassyNumbersCounter (CF 1036C) hoat dong chinh xac" << endl;

    // 5. Kiểm thử KthNumberFinder
    // Số thứ 5 không có 2 chữ số liền kề trùng nhau: 5
    assert(KthNumberFinder::find_kth_adjacent_distinct(5) == 5);
    // Số thứ 10: 10
    assert(KthNumberFinder::find_kth_adjacent_distinct(10) == 10);
    // Số thứ 11: 12 (bỏ qua số 11)
    assert(KthNumberFinder::find_kth_adjacent_distinct(11) == 12);
    cerr << "[PASS] Test 5: KthNumberFinder hoat dong chinh xac" << endl;

    cerr << "=== TAT CA KIEM THU DIGIT DP DA HOAN TAT XUAT SAC! ===" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_all_tests();

    return 0;
}
