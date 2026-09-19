/**
 * @file template.cpp
 * @brief Các Thuật toán Đường đi Ngắn nhất (Shortest Path Algorithms)
 * @author Duc-Minh Vu
 * @affiliation SLSCM Lab - Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @date 2026
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * @note Soạn thảo và tối ưu bởi Agentic AI tool cho Olympic Tin học Sinh viên / ICPC.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cassert>

using namespace std;

namespace ShortestPath {

    const long long INF = 1e18;

    /**
     * @brief Cấu trúc Dijkstra giải bài toán SSSP với trọng số không âm.
     * Sử dụng Min-Heap và kỹ thuật Lazy Deletion.
     * Độ phức tạp: O((V + E) log V).
     */
    class DijkstraSolver {
    public:
        int n;
        vector<vector<pair<int, long long>>> adj;

        DijkstraSolver(int vertices) : n(vertices), adj(vertices + 1) {}

        void add_edge(int u, int v, long long w, bool directed = false) {
            adj[u].push_back({v, w});
            if (!directed) {
                adj[v].push_back({u, w});
            }
        }

        pair<vector<long long>, vector<int>> solve(int s) {
            vector<long long> dist(n + 1, INF);
            vector<int> parent(n + 1, -1);
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();

                if (d > dist[u]) continue;

                for (const auto& [v, w] : adj[u]) {
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                        pq.push({dist[v], v});
                    }
                }
            }
            return {dist, parent};
        }

        vector<int> restore_path(int s, int t, const vector<int>& parent) {
            vector<int> path;
            for (int cur = t; cur != -1; cur = parent[cur]) {
                path.push_back(cur);
                if (cur == s) break;
            }
            reverse(path.begin(), path.end());
            if (path.empty() || path[0] != s) return {};
            return path;
        }

        /**
         * @brief Tìm K đường đi ngắn nhất từ s đến t.
         * @return Vector chứa tối đa K khoảng cách tăng dần.
         */
        vector<long long> k_shortest_paths(int s, int t, int K) {
            vector<vector<long long>> dist(n + 1);
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();

                if ((int)dist[u].size() >= K) continue;
                dist[u].push_back(d);

                if (u == t && (int)dist[t].size() == K) break;

                for (const auto& [v, w] : adj[u]) {
                    if ((int)dist[v].size() < K) {
                        pq.push({d + w, v});
                    }
                }
            }
            return dist[t];
        }
    };

    /**
     * @brief Thuật toán Bellman-Ford xử lý đồ thị có trọng số âm và phát hiện chu trình âm.
     * Độ phức tạp: O(V * E).
     */
    class BellmanFordSolver {
    public:
        int n;
        vector<tuple<int, int, long long>> edges;

        BellmanFordSolver(int vertices) : n(vertices) {}

        void add_edge(int u, int v, long long w) {
            edges.push_back({u, v, w});
        }

        /**
         * @return tuple<has_negative_cycle, dist, parent>
         */
        tuple<bool, vector<long long>, vector<int>> solve(int s) {
            vector<long long> dist(n + 1, INF);
            vector<int> parent(n + 1, -1);
            dist[s] = 0;

            for (int i = 1; i <= n - 1; ++i) {
                bool updated = false;
                for (const auto& [u, v, w] : edges) {
                    if (dist[u] < INF && dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                        updated = true;
                    }
                }
                if (!updated) break;
            }

            // Kiểm tra chu trình âm đến được từ s
            for (const auto& [u, v, w] : edges) {
                if (dist[u] < INF && dist[u] + w < dist[v]) {
                    return {true, dist, parent};
                }
            }
            return {false, dist, parent};
        }

        /**
         * @brief Tìm và in ra các đỉnh thuộc một chu trình âm bất kỳ trên toàn bộ đồ thị.
         * @return Vector chứa các đỉnh theo thứ tự chu trình khép kín (u1 -> u2 -> ... -> u1).
         */
        vector<int> find_any_negative_cycle() {
            vector<long long> dist(n + 1, 0); // Đặt dist = 0 để tìm chu trình âm ở bất kỳ thành phần nào
            vector<int> parent(n + 1, -1);
            int last_relaxed = -1;

            for (int i = 1; i <= n; ++i) {
                last_relaxed = -1;
                for (const auto& [u, v, w] : edges) {
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        parent[v] = u;
                        last_relaxed = v;
                    }
                }
            }

            if (last_relaxed == -1) return {}; // Không có chu trình âm

            // Nhảy lùi n bước để chắc chắn nằm trong chu trình
            int cur = last_relaxed;
            for (int i = 0; i < n; ++i) {
                cur = parent[cur];
            }

            vector<int> cycle;
            for (int v = cur;; v = parent[v]) {
                cycle.push_back(v);
                if (v == cur && cycle.size() > 1) break;
            }
            reverse(cycle.begin(), cycle.end());
            return cycle;
        }
    };

    /**
     * @brief Thuật toán Floyd-Warshall tìm đường đi ngắn nhất giữa mọi cặp đỉnh (APSP).
     * Độ phức tạp: O(V^3).
     */
    class FloydWarshallSolver {
    public:
        int n;
        vector<vector<long long>> dist;

        FloydWarshallSolver(int vertices) 
            : n(vertices), dist(vertices + 1, vector<long long>(vertices + 1, INF)) {
            for (int i = 1; i <= n; ++i) dist[i][i] = 0;
        }

        void add_edge(int u, int v, long long w, bool directed = false) {
            dist[u][v] = min(dist[u][v], w);
            if (!directed) {
                dist[v][u] = min(dist[v][u], w);
            }
        }

        void solve() {
            for (int k = 1; k <= n; ++k) {
                for (int i = 1; i <= n; ++i) {
                    for (int j = 1; j <= n; ++j) {
                        if (dist[i][k] < INF && dist[k][j] < INF) {
                            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                        }
                    }
                }
            }
        }

        bool has_negative_cycle() const {
            for (int i = 1; i <= n; ++i) {
                if (dist[i][i] < 0) return true;
            }
            return false;
        }

        long long get_dist(int u, int v) const {
            return dist[u][v];
        }
    };

    /**
     * @brief Đếm số lượng đường đi ngắn nhất từ s đến t bằng Shortest Path DAG.
     */
    long long count_shortest_paths(int s, int t, int n, 
                                   const vector<vector<pair<int, long long>>>& adj, 
                                   long long MOD = 1e9 + 7) {
        DijkstraSolver dijkstra(n);
        dijkstra.adj = adj;
        auto [dist, parent] = dijkstra.solve(s);

        if (dist[t] == INF) return 0;

        // Xây dựng Shortest Path DAG và tính bán bậc vào
        vector<vector<int>> dag(n + 1);
        vector<int> in_degree(n + 1, 0);

        for (int u = 1; u <= n; ++u) {
            if (dist[u] == INF) continue;
            for (const auto& [v, w] : adj[u]) {
                if (dist[u] + w == dist[v]) {
                    dag[u].push_back(v);
                    in_degree[v]++;
                }
            }
        }

        // Quy hoạch động theo thứ tự Topo
        queue<int> q;
        vector<long long> ways(n + 1, 0);
        ways[s] = 1;

        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] == 0) q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : dag[u]) {
                ways[v] = (ways[v] + ways[u]) % MOD;
                if (--in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return ways[t];
    }
}

void run_unit_tests() {
    using namespace ShortestPath;

    // 1. Kiểm thử Dijkstra chuẩn
    {
        DijkstraSolver solver(5);
        solver.add_edge(1, 2, 4);
        solver.add_edge(1, 3, 2);
        solver.add_edge(3, 2, 1);
        solver.add_edge(2, 4, 5);
        solver.add_edge(3, 4, 8);
        solver.add_edge(4, 5, 3);

        auto [dist, parent] = solver.solve(1);
        assert(dist[1] == 0);
        assert(dist[3] == 2);
        assert(dist[2] == 3); // 1 -> 3 -> 2 (2 + 1 = 3)
        assert(dist[4] == 8); // 1 -> 3 -> 2 -> 4 (3 + 5 = 8)
        assert(dist[5] == 11);

        vector<int> path = solver.restore_path(1, 5, parent);
        assert(path.size() == 5);
        assert(path[0] == 1 && path[1] == 3 && path[2] == 2 && path[3] == 4 && path[4] == 5);
    }

    // 2. Kiểm thử K-Shortest Paths
    {
        DijkstraSolver solver(4);
        solver.add_edge(1, 2, 1, true);
        solver.add_edge(2, 4, 2, true);
        solver.add_edge(1, 3, 2, true);
        solver.add_edge(3, 4, 2, true);
        solver.add_edge(1, 4, 10, true);

        // Đường đi 1: 1 -> 2 -> 4 (chi phí 3)
        // Đường đi 2: 1 -> 3 -> 4 (chi phí 4)
        // Đường đi 3: 1 -> 4 (chi phí 10)
        vector<long long> k_paths = solver.k_shortest_paths(1, 4, 3);
        assert(k_paths.size() == 3);
        assert(k_paths[0] == 3);
        assert(k_paths[1] == 4);
        assert(k_paths[2] == 10);
    }

    // 3. Kiểm thử Bellman-Ford & Chu trình âm
    {
        BellmanFordSolver solver(4);
        solver.add_edge(1, 2, 1);
        solver.add_edge(2, 3, 2);
        solver.add_edge(3, 4, 3);
        solver.add_edge(4, 2, -6); // 2 -> 3 -> 4 -> 2 có tổng: 2 + 3 - 6 = -1 < 0

        vector<int> cycle = solver.find_any_negative_cycle();
        assert(!cycle.empty());
        assert(cycle.front() == cycle.back()); // Chu trình khép kín
    }

    // 4. Kiểm thử Floyd-Warshall
    {
        FloydWarshallSolver solver(3);
        solver.add_edge(1, 2, 8, true);
        solver.add_edge(1, 3, 5, true);
        solver.add_edge(3, 2, 2, true);
        solver.solve();

        assert(solver.get_dist(1, 2) == 7); // 1 -> 3 -> 2 (5 + 2 = 7 < 8)
        assert(solver.get_dist(1, 3) == 5);
        assert(solver.get_dist(2, 1) == INF);
        assert(!solver.has_negative_cycle());
    }

    // 5. Kiểm thử Đếm số đường đi ngắn nhất
    {
        int n = 4;
        vector<vector<pair<int, long long>>> adj(n + 1);
        adj[1].push_back({2, 1});
        adj[1].push_back({3, 1});
        adj[2].push_back({4, 2});
        adj[3].push_back({4, 2});

        // 1 -> 2 -> 4 (chi phí 3)
        // 1 -> 3 -> 4 (chi phí 3)
        // Có đúng 2 đường đi ngắn nhất có cùng độ dài 3
        long long ways = count_shortest_paths(1, 4, n, adj);
        assert(ways == 2);
    }

    cout << "✅ [ShortestPath] Tất cả các Unit Tests đều vượt qua thành công!\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_unit_tests();
    return 0;
}
