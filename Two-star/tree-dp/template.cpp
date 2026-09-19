/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Quy hoạch Động trên Cây (Tree Dynamic Programming) (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. ĐƯỜNG KÍNH CÂY QUA DP 1 LƯỢT DUYỆT (TREE DIAMETER VIA 1-PASS DP)
// ==============================================================================
namespace TreeDiameterDP {
    struct Edge {
        int to;
        long long weight;
    };

    /**
     * @brief Tìm đường kính cây có trọng số bằng DP 1 lượt DFS (Hỗ trợ trọng số âm/dương)
     * @param n Số đỉnh
     * @param adj Danh sách kề
     * @return pair<long long, pair<int, int>> {độ dài đường kính, {đỉnh cao nhất, ...}}
     */
    pair<long long, int> get_diameter(int n, const vector<vector<Edge>>& adj) {
        long long max_diameter = 0;
        vector<long long> down(n + 1, 0); // Khoảng cách xa nhất từ u xuống một lá trong cây con u

        auto dfs = [&](auto self, int u, int p) -> void {
            down[u] = 0;
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                long long w = edge.weight;
                if (v == p) continue;

                self(self, v, u);
                // Đường đi dài nhất đi qua u nối 2 nhánh con
                max_diameter = max(max_diameter, down[u] + down[v] + w);
                // Cập nhật đường đi dài nhất từ u xuống lá
                down[u] = max(down[u], down[v] + w);
            }
        };

        dfs(dfs, 1, 0);
        return {max_diameter, 1};
    }
}

// ==============================================================================
// 2. TẬP ĐỘC LẬP TRỌNG SỐ CỰC ĐẠI TRÊN CÂY (MAXIMUM WEIGHT INDEPENDENT SET)
// ==============================================================================
namespace MaximumIndependentSet {
    /**
     * @brief Tìm tập độc lập có tổng trọng số lớn nhất trên cây
     * @param n Số lượng đỉnh
     * @param weight Trọng số của từng đỉnh (1-indexed)
     * @param adj Danh sách kề
     * @return pair<long long, vector<int>> {tổng trọng số lớn nhất, danh sách các đỉnh được chọn}
     */
    pair<long long, vector<int>> solve(int n, const vector<long long>& weight, const vector<vector<int>>& adj) {
        vector<vector<long long>> dp(n + 1, vector<long long>(2, 0));

        // dp[u][0]: max trong cây con u khi KHÔNG chọn u
        // dp[u][1]: max trong cây con u khi CÓ chọn u
        auto dfs1 = [&](auto self, int u, int p) -> void {
            dp[u][0] = 0;
            dp[u][1] = weight[u];

            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u);
                dp[u][0] += max(dp[v][0], dp[v][1]);
                dp[u][1] += dp[v][0];
            }
        };

        dfs1(dfs1, 1, 0);

        // Truy vết tìm tập đỉnh được chọn
        vector<int> chosen;
        auto trace = [&](auto self, int u, int p, bool parent_chosen) -> void {
            bool pick_u = false;
            if (!parent_chosen && dp[u][1] > dp[u][0]) {
                pick_u = true;
                chosen.push_back(u);
            }

            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u, pick_u);
            }
        };

        long long max_ans = max(dp[1][0], dp[1][1]);
        trace(trace, 1, 0, false);

        return {max_ans, chosen};
    }
}

// ==============================================================================
// 3. TREE KNAPSACK: DP BALO TRÊN CÂY TỐI ƯU O(N * K)
// ==============================================================================
namespace TreeKnapsack {
    const long long INF = 1e18;

    /**
     * @brief Chọn tối đa K đỉnh sao cho nếu chọn v thì phải chọn cha của v
     * @param n Số đỉnh
     * @param max_k Ngân sách chọn đỉnh tối đa
     * @param val Giá trị khi chọn đỉnh u (1-indexed)
     * @param adj Danh sách kề
     * @return long long Giá trị lớn nhất đạt được khi chọn k đỉnh (0 <= k <= max_k)
     */
    long long solve(int n, int max_k, const vector<long long>& val, const vector<vector<int>>& adj) {
        vector<int> sz(n + 1, 0);
        // dp[u][i]: giá trị max khi chọn i đỉnh trong subtree u (đã bao gồm đỉnh u)
        vector<vector<long long>> dp(n + 1);

        auto dfs = [&](auto self, int u, int p) -> void {
            sz[u] = 1;
            dp[u].assign(2, -INF);
            dp[u][0] = 0;       // Chọn 0 đỉnh trong subtree u (không chọn u)
            dp[u][1] = val[u];  // Chọn 1 đỉnh là chính u

            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u);

                int limit = min(max_k, sz[u] + sz[v]);
                vector<long long> next_dp(limit + 1, -INF);

                // Ghép balo u và balo v với cận chặt subtree sizes
                for (int i = 1; i <= min(sz[u], max_k); ++i) {
                    if (dp[u][i] < -INF / 2) continue;
                    // Trường hợp không chọn gì từ subtree v
                    next_dp[i] = max(next_dp[i], dp[u][i]);

                    for (int j = 1; j <= min(sz[v], max_k - i); ++j) {
                        if (dp[v][j] < -INF / 2) continue;
                        next_dp[i + j] = max(next_dp[i + j], dp[u][i] + dp[v][j]);
                    }
                }

                sz[u] += sz[v];
                dp[u] = move(next_dp);
            }
        };

        dfs(dfs, 1, 0);

        long long res = 0;
        for (int i = 0; i <= min(sz[1], max_k); ++i) {
            res = max(res, dp[1][i]);
        }
        return res;
    }
}

// ==============================================================================
// 4. REROOTING DP: TỔNG KHOẢNG CÁCH TỚI TẤT CẢ CÁC ĐỈNH (CSES TREE DISTANCES II)
// ==============================================================================
namespace RerootingAllDistances {
    /**
     * @brief Tính tổng khoảng cách từ mỗi đỉnh u tới mọi đỉnh còn lại trên cây O(N)
     * @param n Số đỉnh
     * @param adj Danh sách kề
     * @return vector<long long> mảng ans[1..n]
     */
    vector<long long> solve(int n, const vector<vector<int>>& adj) {
        vector<long long> sz(n + 1, 0);
        vector<long long> in_dist(n + 1, 0);
        vector<long long> ans(n + 1, 0);

        // DFS 1: Bottom-up tính sz và khoảng cách trong cây con
        auto dfs1 = [&](auto self, int u, int p) -> void {
            sz[u] = 1;
            in_dist[u] = 0;
            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u);
                sz[u] += sz[v];
                in_dist[u] += in_dist[v] + sz[v];
            }
        };

        dfs1(dfs1, 1, 0);
        ans[1] = in_dist[1];

        // DFS 2: Top-down đổi gốc từ u sang v
        auto dfs2 = [&](auto self, int u, int p) -> void {
            for (int v : adj[u]) {
                if (v == p) continue;
                // Khi đổi gốc sang v:
                // Cây con v gần hơn 1 đơn vị -> giảm sz[v]
                // Phần còn lại xa hơn 1 đơn vị -> tăng (n - sz[v])
                ans[v] = ans[u] + (n - sz[v]) - sz[v];
                self(self, v, u);
            }
        };

        dfs2(dfs2, 1, 0);
        return ans;
    }
}

// ==============================================================================
// 5. REROOTING DP: KHOẢNG CÁCH XA NHẤT TỪ MỖI ĐỈNH (CSES TREE DISTANCES I)
// ==============================================================================
namespace RerootingMaxDistance {
    /**
     * @brief Tính khoảng cách xa nhất từ mỗi đỉnh u tới đỉnh xa nhất trên cây O(N)
     * @param n Số đỉnh
     * @param adj Danh sách kề
     * @return vector<int> ans[1..n]
     */
    vector<int> solve(int n, const vector<vector<int>>& adj) {
        // mx1[u]: độ dài lớn nhất đi xuống subtree của u
        // mx2[u]: độ dài lớn thứ nhì đi xuống subtree của u
        // c1[u]: con v mang lại độ dài lớn nhất mx1
        vector<int> mx1(n + 1, 0), mx2(n + 1, 0), c1(n + 1, 0);
        vector<int> up(n + 1, 0); // độ dài lớn nhất đi ngược lên phía cha của u

        // DFS 1: Bottom-up tìm 2 chiều cao lớn nhất trong subtree
        auto dfs1 = [&](auto self, int u, int p) -> void {
            for (int v : adj[u]) {
                if (v == p) continue;
                self(self, v, u);
                int depth_v = mx1[v] + 1;
                if (depth_v > mx1[u]) {
                    mx2[u] = mx1[u];
                    mx1[u] = depth_v;
                    c1[u] = v;
                } else if (depth_v > mx2[u]) {
                    mx2[u] = depth_v;
                }
            }
        };

        dfs1(dfs1, 1, 0);

        // DFS 2: Top-down đẩy giá trị up từ u sang các con
        auto dfs2 = [&](auto self, int u, int p) -> void {
            for (int v : adj[u]) {
                if (v == p) continue;
                // Nếu v là nhánh mang lại mx1[u], nhánh thay thế cho v sẽ là mx2[u]
                int best_other = (c1[u] == v) ? mx2[u] : mx1[u];
                up[v] = max(up[u] + 1, best_other + 1);
                self(self, v, u);
            }
        };

        dfs2(dfs2, 1, 0);

        vector<int> ans(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            ans[i] = max(mx1[i], up[i]);
        }
        return ans;
    }
}

// ==============================================================================
// 6. REROOTING TỔNG QUÁT BẰNG MẢNG TIỀN TỐ & HẬU TỐ (PREFIX/SUFFIX ACCUMULATION)
// ==============================================================================
namespace RerootingPrefixSuffix {
    /**
     * Cấu trúc mẫu hỗ trợ gộp thông tin cây con khi phép toán không có phép nghịch đảo
     * (Ví dụ: gcd, max, phép nhân modulo 0, cấu trúc tập hợp...)
     */
    template<typename T, typename CombineOp>
    struct GenericRerooting {
        int n;
        vector<vector<int>> adj;
        vector<T> in_val;
        vector<T> ans;
        T identity;
        CombineOp combine;

        GenericRerooting(int n, T id, CombineOp op) 
            : n(n), adj(n + 1), in_val(n + 1), ans(n + 1), identity(id), combine(op) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void dfs1(int u, int p) {
            in_val[u] = identity;
            for (int v : adj[u]) {
                if (v == p) continue;
                dfs1(v, u);
                in_val[u] = combine(in_val[u], in_val[v]);
            }
        }

        void dfs2(int u, int p, T out_val) {
            ans[u] = combine(in_val[u], out_val);

            // Thu thập các nút con
            vector<int> children;
            for (int v : adj[u]) {
                if (v != p) children.push_back(v);
            }

            int k = children.size();
            if (k == 0) return;

            // Xây dựng mảng tích lũy tiền tố và hậu tố
            vector<T> pref(k), suff(k);
            for (int i = 0; i < k; ++i) {
                pref[i] = in_val[children[i]];
                if (i > 0) pref[i] = combine(pref[i - 1], pref[i]);
            }
            for (int i = k - 1; i >= 0; --i) {
                suff[i] = in_val[children[i]];
                if (i + 1 < k) suff[i] = combine(suff[i], suff[i + 1]);
            }

            // Đẩy sang từng con
            for (int i = 0; i < k; ++i) {
                int v = children[i];
                T from_siblings = identity;
                if (i > 0) from_siblings = combine(from_siblings, pref[i - 1]);
                if (i + 1 < k) from_siblings = combine(from_siblings, suff[i + 1]);

                T to_v = combine(out_val, from_siblings);
                dfs2(v, u, to_v);
            }
        }

        void run(int root = 1) {
            dfs1(root, 0);
            dfs2(root, 0, identity);
        }
    };
}

// ==============================================================================
// VÍ DỤ MINH HỌA KIỂM THỬ (TEST HARNESS)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Cây mẫu: 5 đỉnh
    // 1 - 2, 1 - 3, 2 - 4, 2 - 5
    int n = 5;
    vector<vector<int>> adj(n + 1);
    adj[1] = {2, 3}; adj[2] = {1, 4, 5}; adj[3] = {1};
    adj[4] = {2};    adj[5] = {2};

    // 1. Kiểm thử Tree Distances II (Tổng khoảng cách)
    auto dist_sum = RerootingAllDistances::solve(n, adj);
    cout << "=== Rerooting Tree Distances II ===" << "\n";
    for (int i = 1; i <= n; ++i) {
        cout << "Node " << i << ": sum dist = " << dist_sum[i] << "\n";
    }

    // 2. Kiểm thử Tree Distances I (Khoảng cách lớn nhất)
    auto max_dist = RerootingMaxDistance::solve(n, adj);
    cout << "\n=== Rerooting Tree Distances I ===" << "\n";
    for (int i = 1; i <= n; ++i) {
        cout << "Node " << i << ": max dist = " << max_dist[i] << "\n";
    }

    // 3. Kiểm thử Maximum Independent Set
    vector<long long> weights = {0, 10, 5, 20, 15, 12};
    auto [mis_val, chosen_nodes] = MaximumIndependentSet::solve(n, weights, adj);
    cout << "\n=== Maximum Weight Independent Set ===" << "\n";
    cout << "Max Weight: " << mis_val << "\nChosen nodes: ";
    for (int u : chosen_nodes) cout << u << " ";
    cout << "\n";

    return 0;
}
