/**
 * @file template.cpp
 * @brief Kỹ thuật Xây dựng & Mô hình hóa Đồ thị (Graph Modeling & Construction Techniques)
 * @author Duc-Minh Vu
 * @affiliation SLSCM Lab - Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @date 2026
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * @note Soạn thảo và tối ưu bởi Agentic AI tool cho Olympic Tin học Sinh viên / ICPC.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cassert>

using namespace std;

namespace GraphModeling {

    const long long INF = 1e18;

    /**
     * @brief 1. Đồ thị Phân tầng (Layered Graph / Graph State Expansion).
     * Mở rộng trạng thái (u, k) với u trong [1, N] và k trong [0, K].
     */
    class LayeredGraph {
    public:
        int n, K;
        int total_nodes;
        vector<vector<pair<int, long long>>> adj;

        LayeredGraph(int vertices, int layers) 
            : n(vertices), K(layers), total_nodes(vertices * (layers + 1)), adj(total_nodes + 1) {}

        int get_id(int u, int k) const {
            return k * n + u;
        }

        pair<int, int> get_state(int id) const {
            return {(id - 1) % n + 1, (id - 1) / n};
        }

        // Cạnh thông thường trong cùng một tầng
        void add_edge(int u, int v, long long w, bool directed = false) {
            for (int k = 0; k <= K; ++k) {
                adj[get_id(u, k)].push_back({get_id(v, k), w});
                if (!directed) {
                    adj[get_id(v, k)].push_back({get_id(u, k), w});
                }
            }
        }

        // Cạnh chuyển tầng (ví dụ: dùng vé nhảy từ tầng k lên tầng k + dk)
        void add_layer_transition(int u, int v, long long w, int dk = 1, bool directed = true) {
            for (int k = 0; k + dk <= K; ++k) {
                adj[get_id(u, k)].push_back({get_id(v, k + dk), w});
                if (!directed) {
                    adj[get_id(v, k)].push_back({get_id(u, k + dk), w});
                }
            }
        }

        long long shortest_path(int s, int t) {
            vector<long long> dist(total_nodes + 1, INF);
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

            int start_id = get_id(s, 0);
            dist[start_id] = 0;
            pq.push({0, start_id});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();

                if (d > dist[u]) continue;

                for (const auto& [v, w] : adj[u]) {
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                    }
                }
            }

            long long ans = INF;
            for (int k = 0; k <= K; ++k) {
                ans = min(ans, dist[get_id(t, k)]);
            }
            return ans;
        }
    };

    /**
     * @brief 2. Đỉnh ảo Hội tụ (Hub Nodes / Star Graph Optimization).
     * Thay thế đồ thị clique K^2 cạnh giữa nhóm K đỉnh bằng 1 đỉnh Hub ảo và 2K cạnh.
     */
    class HubNetwork {
    public:
        int n;
        int next_virtual_node;
        vector<vector<pair<int, long long>>> adj;

        HubNetwork(int vertices) 
            : n(vertices), next_virtual_node(vertices + 1), adj(vertices + 1) {}

        /**
         * @brief Thêm một nhóm K đỉnh liên thông toàn phần với nhau.
         * Tạo 1 đỉnh ảo hub, nối u -> hub trọng số win, và hub -> v trọng số wout.
         */
        void add_clique_group(const vector<int>& group, long long w_in, long long w_out) {
            int hub = next_virtual_node++;
            if ((int)adj.size() <= hub) adj.resize(hub + 1);

            for (int u : group) {
                adj[u].push_back({hub, w_in});
                adj[hub].push_back({u, w_out});
            }
        }
    };

    /**
     * @brief 3. Cây phân đoạn tối ưu hóa Cạnh (Segment Tree to Graph).
     * Cho phép nối 1 đỉnh tới cả đoạn [L, R] các đỉnh khác chỉ với O(log N) cạnh.
     */
    class SegmentTreeGraph {
    private:
        int n;
        int node_count;
        vector<int> tree_node; // mapping từ chỉ số Segment Tree sang đỉnh đồ thị

    public:
        vector<vector<pair<int, long long>>> adj;

        SegmentTreeGraph(int vertices) : n(vertices) {
            node_count = n; // Các đỉnh từ 1 đến n là đỉnh thực tế
            tree_node.resize(4 * n + 1);
            adj.resize(n + 1);
            build(1, 1, n);
        }

        void build(int node, int l, int r) {
            if (l == r) {
                tree_node[node] = l; // Nút lá trỏ tới đỉnh thực tế l
                return;
            }
            // Tạo đỉnh ảo mới cho nút trung gian trên SegTree
            int v_id = ++node_count;
            tree_node[node] = v_id;
            if ((int)adj.size() <= node_count) adj.resize(node_count + 1);

            int mid = (l + r) / 2;
            build(node * 2, l, mid);
            build(node * 2 + 1, mid + 1, r);

            // Nối cạnh 0 từ nút cha xuống 2 nút con
            adj[v_id].push_back({tree_node[node * 2], 0});
            adj[v_id].push_back({tree_node[node * 2 + 1], 0});
        }

        /**
         * @brief Nối cạnh có hướng từ đỉnh u tới toàn bộ các đỉnh trong đoạn [ql, qr] với trọng số w.
         */
        void add_edge_to_range(int u, int ql, int qr, long long w) {
            add_range(1, 1, n, ql, qr, u, w);
        }

    private:
        void add_range(int node, int l, int r, int ql, int qr, int u, long long w) {
            if (ql <= l && r <= qr) {
                adj[u].push_back({tree_node[node], w});
                return;
            }
            int mid = (l + r) / 2;
            if (ql <= mid) add_range(node * 2, l, mid, ql, qr, u, w);
            if (qr > mid)  add_range(node * 2 + 1, mid + 1, r, ql, qr, u, w);
        }

    public:
        vector<long long> dijkstra(int s) {
            vector<long long> dist(node_count + 1, INF);
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
                        pq.push({dist[v], v});
                    }
                }
            }
            // Chỉ trả về khoảng cách tới các đỉnh thực tế [1, n]
            return vector<long long>(dist.begin(), dist.begin() + n + 1);
        }
    };

    /**
     * @brief 4. Hệ Ràng buộc Sai phân (Difference Constraints System: x_j - x_i <= w).
     * Giải bằng SPFA từ Siêu Nguồn S*.
     */
    class DifferenceConstraintsSolver {
    public:
        int n;
        vector<vector<pair<int, long long>>> adj;

        DifferenceConstraintsSolver(int variables) : n(variables), adj(variables + 1) {}

        // Thêm bất đẳng thức: x_v - x_u <= w (u -> v có trọng số w)
        void add_constraint(int u, int v, long long w) {
            adj[u].push_back({v, w});
        }

        /**
         * @return pair<is_feasible, solution_vector>
         */
        pair<bool, vector<long long>> solve() {
            int super_source = 0;
            vector<vector<pair<int, long long>>> g = adj;
            g.resize(n + 1);

            // Nối siêu nguồn 0 tới tất cả các biến với trọng số 0
            for (int i = 1; i <= n; ++i) {
                g[super_source].push_back({i, 0});
            }

            vector<long long> dist(n + 1, INF);
            vector<int> cnt(n + 1, 0);
            vector<bool> in_queue(n + 1, false);
            queue<int> q;

            dist[super_source] = 0;
            q.push(super_source);
            in_queue[super_source] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                in_queue[u] = false;

                for (const auto& [v, w] : g[u]) {
                    if (dist[u] + w < dist[v]) {
                        dist[v] = dist[u] + w;
                        if (!in_queue[v]) {
                            q.push(v);
                            in_queue[v] = true;
                            if (++cnt[v] > n + 1) {
                                return {false, {}}; // Tồn tại chu trình âm -> Vô nghiệm!
                            }
                        }
                    }
                }
            }

            return {true, vector<long long>(dist.begin() + 1, dist.end())};
        }
    };

    /**
     * @brief 5. Duyệt BFS trên Đồ thị Bù (Complement Graph BFS).
     * Đếm thành phần liên thông trên đồ thị bù trong O((V + E) log V).
     */
    class ComplementGraphBFS {
    public:
        int n;
        vector<set<int>> orig_adj;

        ComplementGraphBFS(int vertices) : n(vertices), orig_adj(vertices + 1) {}

        void add_original_edge(int u, int v) {
            orig_adj[u].insert(v);
            orig_adj[v].insert(u);
        }

        vector<vector<int>> get_connected_components() {
            set<int> unvisited;
            for (int i = 1; i <= n; ++i) unvisited.insert(i);

            vector<vector<int>> components;

            for (int i = 1; i <= n; ++i) {
                if (unvisited.find(i) == unvisited.end()) continue;

                vector<int> comp;
                queue<int> q;

                unvisited.erase(i);
                q.push(i);
                comp.push_back(i);

                while (!q.empty()) {
                    int u = q.front();
                    q.pop();

                    auto it = unvisited.begin();
                    while (it != unvisited.end()) {
                        int v = *it;
                        // Nếu không có cạnh giữa u và v trong đồ thị gốc -> có cạnh trong đồ thị bù!
                        if (orig_adj[u].find(v) == orig_adj[u].end()) {
                            comp.push_back(v);
                            q.push(v);
                            it = unvisited.erase(it); // Xóa ngay lập tức
                        } else {
                            ++it;
                        }
                    }
                }
                components.push_back(comp);
            }
            return components;
        }
    };
}

void run_unit_tests() {
    using namespace GraphModeling;

    // 1. Kiểm thử LayeredGraph: 1 vé giảm giá chi phí = 0
    {
        LayeredGraph lg(3, 1);
        lg.add_edge(1, 2, 10);
        lg.add_edge(2, 3, 10);
        // Dùng 1 vé trên cạnh (1, 2) hoặc (2, 3)
        lg.add_layer_transition(1, 2, 0, 1);
        lg.add_layer_transition(2, 3, 0, 1);

        // Chi phí ngắn nhất: 1 -> 2 (dùng vé: 0) + 2 -> 3 (không dùng: 10) = 10
        assert(lg.shortest_path(1, 3) == 10);
    }

    // 2. Kiểm thử SegmentTreeGraph: Nối 1 tới đoạn [2, 4] chi phí 5
    {
        SegmentTreeGraph stg(4);
        stg.add_edge_to_range(1, 2, 4, 5);

        auto dist = stg.dijkstra(1);
        assert(dist[1] == 0);
        assert(dist[2] == 5);
        assert(dist[3] == 5);
        assert(dist[4] == 5);
    }

    // 3. Kiểm thử DifferenceConstraintsSolver
    {
        // Hệ:
        // x2 - x1 <= 3
        // x3 - x2 <= 2
        // x3 - x1 <= 6
        DifferenceConstraintsSolver dcs(3);
        dcs.add_constraint(1, 2, 3);
        dcs.add_constraint(2, 3, 2);
        dcs.add_constraint(1, 3, 6);

        auto [feasible, sol] = dcs.solve();
        assert(feasible == true);
        assert(sol.size() == 3);
        assert(sol[1] - sol[0] <= 3);
        assert(sol[2] - sol[1] <= 2);

        // Thêm chu trình âm: x1 - x3 <= -6 (x3 - x1 >= 6, nhưng từ trên x3 - x1 <= 5)
        dcs.add_constraint(3, 1, -6);
        auto [feasible_neg, sol_neg] = dcs.solve();
        assert(feasible_neg == false);
    }

    // 4. Kiểm thử ComplementGraphBFS
    {
        // Đồ thị gốc có 4 đỉnh: 1-2, 3-4
        // Đồ thị bù sẽ có các cạnh: (1,3), (1,4), (2,3), (2,4) -> Toàn bộ liên thông thành 1 thành phần!
        ComplementGraphBFS cg(4);
        cg.add_original_edge(1, 2);
        cg.add_original_edge(3, 4);

        auto comps = cg.get_connected_components();
        assert(comps.size() == 1);
        assert(comps[0].size() == 4);
    }

    cout << "✅ [GraphModeling] Tất cả các Unit Tests đều vượt qua thành công!\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_unit_tests();
    return 0;
}
