/**
 * @file template.cpp
 * @brief Thuật toán Duyệt Đồ thị Cơ bản: BFS & DFS (Graph Traversal & Search Algorithms)
 * @author Duc-Minh Vu
 * @affiliation SLSCM Lab - Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @date 2026
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * @note Soạn thảo và tối ưu bởi Agentic AI tool cho Olympic Tin học Sinh viên / ICPC.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <cassert>

using namespace std;

namespace GraphTraversal {

    const int INF = 1e9 + 7;

    /**
     * @brief Cấu trúc Đồ thị chuẩn trong CP.
     * Hỗ trợ BFS, DFS, 0-1 BFS, kiểm tra đồ thị hai phía, phát hiện chu trình và sắp xếp tô-pô.
     */
    class Graph {
    public:
        int n;
        bool is_directed;
        vector<vector<int>> adj;
        vector<vector<pair<int, int>>> adj_weight; // Cho 0-1 BFS (đỉnh kề, trọng số 0/1)

        Graph(int vertices, bool directed = false) 
            : n(vertices), is_directed(directed), adj(vertices + 1), adj_weight(vertices + 1) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            if (!is_directed) {
                adj[v].push_back(u);
            }
        }

        void add_edge_01(int u, int v, int w) {
            adj_weight[u].push_back({v, w});
            if (!is_directed) {
                adj_weight[v].push_back({u, w});
            }
        }

        /**
         * @brief BFS tìm đường đi ngắn nhất không trọng số từ đỉnh nguồn S.
         * Độ phức tạp: O(V + E).
         * @return pair<dist, parent>
         */
        pair<vector<int>, vector<int>> bfs(int s) {
            vector<int> dist(n + 1, INF);
            vector<int> parent(n + 1, -1);
            queue<int> q;

            dist[s] = 0;
            q.push(s);

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : adj[u]) {
                    if (dist[v] == INF) {
                        dist[v] = dist[u] + 1;
                        parent[v] = u;
                        q.push(v);
                    }
                }
            }
            return {dist, parent};
        }

        /**
         * @brief Truy vết đường đi ngắn nhất từ s đến t sau khi chạy BFS.
         */
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
         * @brief Multi-Source BFS tìm khoảng cách tới nguồn gần nhất từ tập nguồn S.
         * Độ phức tạp: O(V + E).
         */
        vector<int> multi_source_bfs(const vector<int>& sources) {
            vector<int> dist(n + 1, INF);
            queue<int> q;

            for (int s : sources) {
                dist[s] = 0;
                q.push(s);
            }

            while (!q.empty()) {
                int u = q.front();
                q.pop();

                for (int v : adj[u]) {
                    if (dist[v] == INF) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
            return dist;
        }

        /**
         * @brief 0-1 BFS tìm đường đi ngắn nhất với trọng số cạnh chỉ gồm 0 và 1.
         * Sử dụng std::deque, đạt độ phức tạp O(V + E) nhanh hơn Dijkstra O((V+E) log V).
         */
        vector<int> bfs_01(int s) {
            vector<int> dist(n + 1, INF);
            deque<int> dq;

            dist[s] = 0;
            dq.push_front(s);

            while (!dq.empty()) {
                int u = dq.front();
                dq.pop_front();

                for (auto& edge : adj_weight[u]) {
                    int v = edge.first;
                    int w = edge.second; // 0 hoặc 1

                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        if (w == 0) {
                            dq.push_front(v);
                        } else {
                            dq.push_back(v);
                        }
                    }
                }
            }
            return dist;
        }

        /**
         * @brief Kiểm tra đồ thị hai phía (Bipartite Graph Check) bằng tô 2 màu (0 và 1).
         * @return pair<bool, vector<int>> {is_bipartite, color}
         */
        pair<bool, vector<int>> check_bipartite() {
            vector<int> color(n + 1, -1);
            for (int i = 1; i <= n; ++i) {
                if (color[i] != -1) continue;

                queue<int> q;
                color[i] = 0;
                q.push(i);

                while (!q.empty()) {
                    int u = q.front();
                    q.pop();

                    for (int v : adj[u]) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            q.push(v);
                        } else if (color[v] == color[u]) {
                            return {false, {}};
                        }
                    }
                }
            }
            return {true, color};
        }

        /**
         * @brief Phát hiện chu trình trên đồ thị có hướng bằng DFS 3 màu.
         * 0: Trắng (chưa thăm), 1: Xám (đang duyệt trong stack), 2: Đen (đã hoàn thành).
         */
        bool has_cycle_directed() {
            assert(is_directed && "Graph must be directed for has_cycle_directed!");
            vector<int> color(n + 1, 0);

            auto dfs = [&](auto self, int u) -> bool {
                color[u] = 1; // Tô xám
                for (int v : adj[u]) {
                    if (color[v] == 1) return true; // Cạnh ngược -> có chu trình!
                    if (color[v] == 0 && self(self, v)) return true;
                }
                color[u] = 2; // Tô đen
                return false;
            };

            for (int i = 1; i <= n; ++i) {
                if (color[i] == 0) {
                    if (dfs(dfs, i)) return true;
                }
            }
            return false;
        }

        /**
         * @brief Sắp xếp tô-pô bằng thuật toán Kahn (BFS và Bán bậc vào).
         * @return Thứ tự topo (rỗng nếu đồ thị có chu trình).
         */
        vector<int> topological_sort_kahn() {
            assert(is_directed && "Topological sort only applies to DAG!");
            vector<int> in_degree(n + 1, 0);
            for (int u = 1; u <= n; ++u) {
                for (int v : adj[u]) {
                    in_degree[v]++;
                }
            }

            queue<int> q;
            for (int i = 1; i <= n; ++i) {
                if (in_degree[i] == 0) q.push(i);
            }

            vector<int> topo;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                topo.push_back(u);

                for (int v : adj[u]) {
                    if (--in_degree[v] == 0) {
                        q.push(v);
                    }
                }
            }

            if ((int)topo.size() < n) return {}; // Có chu trình
            return topo;
        }

        /**
         * @brief Đếm số lượng thành phần liên thông trên đồ thị vô hướng.
         */
        int count_connected_components() {
            vector<bool> visited(n + 1, false);
            int count = 0;

            auto dfs = [&](auto self, int u) -> void {
                visited[u] = true;
                for (int v : adj[u]) {
                    if (!visited[v]) self(self, v);
                }
            };

            for (int i = 1; i <= n; ++i) {
                if (!visited[i]) {
                    count++;
                    dfs(dfs, i);
                }
            }
            return count;
        }
    };

    /**
     * @brief Euler Tour và Quản lý thời gian vào/ra (tin/tout) trên Cây.
     * Cho phép kiểm tra đỉnh u có phải tổ tiên của v hay không trong O(1).
     */
    class TreeEulerTour {
    public:
        int n;
        int timer;
        vector<int> tin, tout;
        vector<vector<int>> adj;

        TreeEulerTour(int vertices) : n(vertices), timer(0), tin(vertices + 1), tout(vertices + 1), adj(vertices + 1) {}

        void add_edge(int u, int v) {
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void dfs_timer(int u, int p = 0) {
            tin[u] = ++timer;
            for (int v : adj[u]) {
                if (v != p) {
                    dfs_timer(v, u);
                }
            }
            tout[u] = ++timer;
        }

        void build(int root = 1) {
            timer = 0;
            dfs_timer(root, 0);
        }

        /**
         * @brief Kiểm tra u có phải là tổ tiên của v hay không trong O(1).
         */
        bool is_ancestor(int u, int v) const {
            return tin[u] <= tin[v] && tout[u] >= tout[v];
        }
    };

    /**
     * @brief Thuật toán Rùa và Thỏ của Floyd tìm chu trình trên Đồ thị Hàm (Functional Graph).
     * Mỗi đỉnh u có duy nhất 1 cạnh đi tới f(u).
     * Độ phức tạp: O(N) thời gian, O(1) bộ nhớ phụ.
     */
    struct FunctionalGraphFloyd {
        /**
         * @param start Đỉnh xuất phát
         * @param f Hàm chuyển trạng thái f(u)
         * @return pair<start_of_cycle, length_of_cycle>
         */
        static pair<int, int> find_cycle(int start, const vector<int>& f) {
            int tortoise = f[start];
            int hare = f[f[start]];

            // Bước 1: Cho thỏ chạy nhanh gấp đôi rùa đến khi gặp nhau
            while (tortoise != hare) {
                tortoise = f[tortoise];
                hare = f[f[hare]];
            }

            // Bước 2: Tìm đỉnh bắt đầu chu trình (giao điểm)
            int mu = 0;
            tortoise = start;
            while (tortoise != hare) {
                tortoise = f[tortoise];
                hare = f[hare];
                mu++;
            }
            int cycle_start = tortoise;

            // Bước 3: Đếm độ dài chu trình
            int lambda = 1;
            hare = f[tortoise];
            while (tortoise != hare) {
                hare = f[hare];
                lambda++;
            }

            return {cycle_start, lambda};
        }
    };
}

void run_unit_tests() {
    using namespace GraphTraversal;

    // 1. Kiểm thử BFS đường đi ngắn nhất và truy vết
    {
        Graph g(6, false);
        g.add_edge(1, 2);
        g.add_edge(1, 3);
        g.add_edge(2, 4);
        g.add_edge(3, 4);
        g.add_edge(4, 5);
        g.add_edge(5, 6);

        auto [dist, parent] = g.bfs(1);
        assert(dist[1] == 0);
        assert(dist[2] == 1);
        assert(dist[4] == 2);
        assert(dist[6] == 4);

        vector<int> path = g.restore_path(1, 6, parent);
        assert(path.size() == 5);
        assert(path.front() == 1 && path.back() == 6);
    }

    // 2. Kiểm thử Multi-source BFS
    {
        Graph g(5, false);
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_edge(3, 4);
        g.add_edge(4, 5);

        // Đặt nguồn tại 1 và 5
        vector<int> sources = {1, 5};
        vector<int> dist = g.multi_source_bfs(sources);
        assert(dist[1] == 0);
        assert(dist[2] == 1);
        assert(dist[3] == 2); // 3 cách 1 hai bước, cách 5 hai bước
        assert(dist[4] == 1);
        assert(dist[5] == 0);
    }

    // 3. Kiểm thử 0-1 BFS
    {
        Graph g(4, true); // Đồ thị có hướng
        g.add_edge_01(1, 2, 1);
        g.add_edge_01(1, 3, 0);
        g.add_edge_01(3, 4, 0);
        g.add_edge_01(2, 4, 0);

        vector<int> dist = g.bfs_01(1);
        // Đường đi 1 -> 3 -> 4 có tổng chi phí 0 + 0 = 0
        assert(dist[4] == 0);
        assert(dist[2] == 1);
        assert(dist[3] == 0);
    }

    // 4. Kiểm thử Bipartite Check
    {
        // Đồ thị tam giác 1-2-3 (chu trình lẻ 3 đỉnh) -> Không phải hai phía
        Graph g_odd(3, false);
        g_odd.add_edge(1, 2);
        g_odd.add_edge(2, 3);
        g_odd.add_edge(3, 1);
        assert(g_odd.check_bipartite().first == false);

        // Đồ thị hình vuông 1-2-3-4 (chu trình chẵn 4 đỉnh) -> Là hai phía
        Graph g_even(4, false);
        g_even.add_edge(1, 2);
        g_even.add_edge(2, 3);
        g_even.add_edge(3, 4);
        g_even.add_edge(4, 1);
        assert(g_even.check_bipartite().first == true);
    }

    // 5. Kiểm thử Phát hiện Chu trình có hướng và Topo Sort
    {
        Graph dag(4, true);
        dag.add_edge(1, 2);
        dag.add_edge(1, 3);
        dag.add_edge(2, 4);
        dag.add_edge(3, 4);
        assert(!dag.has_cycle_directed());

        vector<int> topo = dag.topological_sort_kahn();
        assert(topo.size() == 4);
        assert(topo[0] == 1);
        assert(topo[3] == 4);

        // Thêm cạnh ngược 4 -> 1 tạo chu trình
        dag.add_edge(4, 1);
        assert(dag.has_cycle_directed());
        assert(dag.topological_sort_kahn().empty());
    }

    // 6. Kiểm thử TreeEulerTour is_ancestor
    {
        TreeEulerTour tree(5);
        // Cây: 1 -> 2 -> 4; 1 -> 3 -> 5
        tree.add_edge(1, 2);
        tree.add_edge(2, 4);
        tree.add_edge(1, 3);
        tree.add_edge(3, 5);
        tree.build(1);

        assert(tree.is_ancestor(1, 4));
        assert(tree.is_ancestor(1, 5));
        assert(tree.is_ancestor(2, 4));
        assert(!tree.is_ancestor(2, 5));
        assert(!tree.is_ancestor(4, 1));
    }

    // 7. Kiểm thử Floyd Cycle Finder trên Functional Graph
    {
        // 1 -> 2 -> 3 -> 4 -> 2 (chu trình 2-3-4 độ dài 3)
        vector<int> f = {0, 2, 3, 4, 2};
        auto [c_start, c_len] = FunctionalGraphFloyd::find_cycle(1, f);
        assert(c_start == 2);
        assert(c_len == 3);
    }

    cout << "✅ [GraphTraversal] Tất cả các Unit Tests đều vượt qua thành công!\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_unit_tests();
    return 0;
}
