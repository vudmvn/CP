/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Quy hoạch Động Cơ bản (Basic Dynamic Programming) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. CÁC MÔ HÌNH QUY HOẠCH ĐỘNG CƠ BẢN (BASIC DP MODELS)
// ==============================================================================
namespace BasicDP {

    const long long INF_LL = 2e18;
    const int INF_INT = 1e9;
    const int MOD = 1e9 + 7;

    /**
     * @brief Cái túi 0/1 (0/1 Knapsack) tối ưu bộ nhớ 1D O(W)
     * @details Duyệt ngược W -> w_i
     */
    long long knapsack01(int W, const vector<int>& weights, const vector<long long>& values) {
        vector<long long> dp(W + 1, 0);
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            long long v = values[i];
            for (int j = W; j >= w; --j) {
                dp[j] = max(dp[j], dp[j - w] + v);
            }
        }
        return dp[W];
    }

    /**
     * @brief Cái túi không giới hạn (Unbounded Knapsack) tối ưu bộ nhớ 1D O(W)
     * @details Duyệt xuôi w_i -> W
     */
    long long knapsackUnbounded(int W, const vector<int>& weights, const vector<long long>& values) {
        vector<long long> dp(W + 1, 0);
        int n = weights.size();
        for (int i = 0; i < n; ++i) {
            int w = weights[i];
            long long v = values[i];
            for (int j = w; j <= W; ++j) {
                dp[j] = max(dp[j], dp[j - w] + v);
            }
        }
        return dp[W];
    }

    /**
     * @brief Dãy con tăng dài nhất (LIS) trong O(N log N) kèm truy vết dãy nghiệm
     * @param a Dãy số ban đầu
     * @param lis_sequence Dãy con tăng dài nhất được khôi phục
     * @return Độ dài lớn nhất của LIS
     */
    int lisNlogN(const vector<long long>& a, vector<long long>& lis_sequence) {
        int n = a.size();
        if (n == 0) return 0;

        vector<long long> tails;     // tails[len] = giá trị nhỏ nhất kết thúc LIS độ dài len + 1
        vector<int> tail_indices;    // Chỉ số trong mảng a của tails[len]
        vector<int> parent(n, -1);   // Mảng truy vết cha

        for (int i = 0; i < n; ++i) {
            auto it = lower_bound(tails.begin(), tails.end(), a[i]);
            int idx = distance(tails.begin(), it);

            if (it == tails.end()) {
                tails.push_back(a[i]);
                tail_indices.push_back(i);
            } else {
                *it = a[i];
                tail_indices[idx] = i;
            }

            if (idx > 0) {
                parent[i] = tail_indices[idx - 1];
            }
        }

        // Truy vết nghiệm
        int cur = tail_indices.back();
        lis_sequence.clear();
        while (cur != -1) {
            lis_sequence.push_back(a[cur]);
            cur = parent[cur];
        }
        reverse(lis_sequence.begin(), lis_sequence.end());

        return tails.size();
    }

    /**
     * @brief Dãy con chung dài nhất (LCS) trong O(N * M) kèm truy vết xâu chung
     */
    int lcs(const string& s, const string& t, string& lcs_str) {
        int n = s.size();
        int m = t.size();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                if (s[i - 1] == t[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Truy vết
        lcs_str.clear();
        int i = n, j = m;
        while (i > 0 && j > 0) {
            if (s[i - 1] == t[j - 1]) {
                lcs_str.push_back(s[i - 1]);
                i--; j--;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(lcs_str.begin(), lcs_str.end());

        return dp[n][m];
    }

    /**
     * @brief Đường đi có tổng chi phí nhỏ nhất trên Lưới 2D tối ưu bộ nhớ 1D O(M)
     */
    long long gridMinPathSum(const vector<vector<long long>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        int n = grid.size();
        int m = grid[0].size();
        vector<long long> dp(m, INF_LL);

        dp[0] = grid[0][0];
        for (int j = 1; j < m; ++j) {
            dp[j] = dp[j - 1] + grid[0][j];
        }

        for (int i = 1; i < n; ++i) {
            dp[0] += grid[i][0];
            for (int j = 1; j < m; ++j) {
                dp[j] = grid[i][j] + min(dp[j], dp[j - 1]);
            }
        }
        return dp[m - 1];
    }

    /**
     * @brief Đổi tiền - Đếm số TỔ HỢP (Combinations - Không phân biệt thứ tự lấy xu)
     * @note Vòng for COIN ở NGOÀI
     */
    int coinChangeCombinations(int target, const vector<int>& coins, int mod = MOD) {
        vector<int> dp(target + 1, 0);
        dp[0] = 1;
        for (int c : coins) {
            for (int w = c; w <= target; ++w) {
                dp[w] = (dp[w] + dp[w - c]) % mod;
            }
        }
        return dp[target];
    }

    /**
     * @brief Đổi tiền - Đếm số HOÁN VỊ (Permutations - Phân biệt thứ tự lấy xu)
     * @note Vòng for TIỀN ở NGOÀI
     */
    int coinChangePermutations(int target, const vector<int>& coins, int mod = MOD) {
        vector<int> dp(target + 1, 0);
        dp[0] = 1;
        for (int w = 1; w <= target; ++w) {
            for (int c : coins) {
                if (w >= c) {
                    dp[w] = (dp[w] + dp[w - c]) % mod;
                }
            }
        }
        return dp[target];
    }

    /**
     * @brief Đổi tiền - Số lượng đồng xu ít nhất để đổi số tiền target
     * @return Số xu nhỏ nhất, hoặc -1 nếu không thể đổi
     */
    int coinChangeMinCoins(int target, const vector<int>& coins) {
        vector<int> dp(target + 1, INF_INT);
        dp[0] = 0;
        for (int c : coins) {
            for (int w = c; w <= target; ++w) {
                if (dp[w - c] != INF_INT) {
                    dp[w] = min(dp[w], dp[w - c] + 1);
                }
            }
        }
        return (dp[target] == INF_INT ? -1 : dp[target]);
    }
}

// ==============================================================================
// 2. KIỂM THỬ ĐƠN VỊ & CHẠY MẪU (UNIT TESTS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: 0/1 Knapsack
    vector<int> weights = {2, 3, 4};
    vector<long long> values = {3, 4, 5};
    long long max_val_01 = BasicDP::knapsack01(5, weights, values);
    assert(max_val_01 == 7);
    cout << "[TEST 1 PASSED] 0/1 Knapsack max value: " << max_val_01 << "\n";

    // Test 2: Unbounded Knapsack
    long long max_val_unb = BasicDP::knapsackUnbounded(5, weights, values);
    // Chọn 2 đồ vật 1 (w=2, v=3 -> w=4, v=6) hoặc (w=2, v=3 + w=3, v=4 -> w=5, v=7)
    assert(max_val_unb == 7);
    cout << "[TEST 2 PASSED] Unbounded Knapsack max value: " << max_val_unb << "\n";

    // Test 3: LIS O(N log N) + Path
    vector<long long> arr = {10, 9, 2, 5, 3, 7, 101, 18};
    vector<long long> lis_path;
    int lis_len = BasicDP::lisNlogN(arr, lis_path);
    assert(lis_len == 4);
    cout << "[TEST 3 PASSED] LIS length: " << lis_len << ", Sequence: ";
    for (long long x : lis_path) cout << x << " ";
    cout << "\n";

    // Test 4: LCS + Path
    string s = "abcde", t = "ace";
    string lcs_str;
    int lcs_len = BasicDP::lcs(s, t, lcs_str);
    assert(lcs_len == 3);
    assert(lcs_str == "ace");
    cout << "[TEST 4 PASSED] LCS length: " << lcs_len << ", String: " << lcs_str << "\n";

    // Test 5: Coin Change (Combinations vs Permutations)
    vector<int> coins = {2, 3, 5};
    int target = 9;
    int comb = BasicDP::coinChangeCombinations(target, coins);
    int perm = BasicDP::coinChangePermutations(target, coins);
    int min_coins = BasicDP::coinChangeMinCoins(target, coins);
    // Tổ hợp cho 9 từ {2, 3, 5}: {2, 2, 2, 3}, {2, 2, 5}, {3, 3, 3} -> 3 combinations
    assert(comb == 3);
    assert(perm == 8);
    assert(min_coins == 3);
    cout << "[TEST 5 PASSED] Coin Change target 9: Combinations = " << comb
         << ", Permutations = " << perm
         << ", Min Coins = " << min_coins << "\n";

    // Test 6: Grid Min Path Sum
    vector<vector<long long>> grid = {
        {1, 3, 1},
        {1, 5, 1},
        {4, 2, 1}
    };
    long long min_grid = BasicDP::gridMinPathSum(grid);
    // 1 -> 3 -> 1 -> 1 -> 1 = 7
    assert(min_grid == 7);
    cout << "[TEST 6 PASSED] Grid Min Path Sum: " << min_grid << "\n";

    cout << "\n==> All Basic DP tests passed successfully!\n";
    return 0;
}
