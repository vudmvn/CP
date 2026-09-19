/**
 * @file template.cpp
 * @brief Chuyên đề: Thuật toán Mo (Mo's Algorithm & Range Query Decomposition)
 * @author Duc-Minh Vu (SLSCM Lab - FDA, Đại học Kinh tế Quốc dân - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * 
 * Mã nguồn chuẩn C++20 bao gồm 4 biến thể hoàn chỉnh:
 * 1. MoStandard: Mo tiêu chuẩn với tối ưu sắp xếp chẵn lẻ (Odd-Even Sorting).
 * 2. Mo3D: Mo có thao tác cập nhật (3D Mo with Updates - O(N^(5/3))).
 * 3. MoTree: Mo trên cây qua trải phẳng Euler Tour (Tree Mo via Euler Tour).
 * 4. MoRollback: Mo không cần thao tác xóa kết hợp DSU có hoàn tác (Rollback Mo).
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

using namespace std;

// ============================================================================
// 1. MO'S ALGORITHM TIÊU CHUẨN VỚI TỐI ƯU CHẴN LẺ (ODD-EVEN SORTING)
// ============================================================================
namespace StandardMo {
    struct Query {
        int l, r, id;
        int block;
        bool operator<(const Query& other) const {
            if (block != other.block)
                return block < other.block;
            // Sắp xếp chẵn lẻ (Odd-Even Sorting):
            // Khối chẵn R tăng dần, khối lẻ R giảm dần để giảm 50% số bước di chuyển con trỏ R
            return (block & 1) ? (r < other.r) : (r > other.r);
        }
    };

    /**
     * @brief Giải bài toán đếm số phần tử phân biệt (Distinct Elements Query) trên mảng
     * @param a Mảng đầu vào kích thước N (0-indexed)
     * @param queries Danh sách các truy vấn dạng {L, R}
     * @return vector<int> Danh sách kết quả cho từng truy vấn
     */
    vector<int> solve_distinct_queries(const vector<int>& a, const vector<pair<int, int>>& queries) {
        int n = a.size();
        int q = queries.size();
        if (q == 0) return {};

        int block_size = max(1, (int)(n / sqrt(q)));
        vector<Query> qs(q);
        for (int i = 0; i < q; ++i) {
            qs[i] = {queries[i].first, queries[i].second, i, queries[i].first / block_size};
        }

        sort(qs.begin(), qs.end());

        // Tìm giá trị lớn nhất để cấp phát mảng tần số
        int max_val = 0;
        for (int x : a) max_val = max(max_val, x);
        vector<int> freq(max_val + 1, 0);

        int distinct_count = 0;
        auto add = [&](int idx) {
            int val = a[idx];
            if (freq[val] == 0) distinct_count++;
            freq[val]++;
        };

        auto remove = [&](int idx) {
            int val = a[idx];
            freq[val]--;
            if (freq[val] == 0) distinct_count--;
        };

        vector<int> ans(q);
        int currL = 0, currR = -1;

        for (const auto& query : qs) {
            // Quy tắc an toàn: Luôn MỞ RỘNG trước, THU HẸP sau
            while (currL > query.l) add(--currL);
            while (currR < query.r) add(++currR);
            while (currL < query.l) remove(currL++);
            while (currR > query.r) remove(currR--);

            ans[query.id] = distinct_count;
        }

        return ans;
    }
}

// ============================================================================
// 2. MO CÓ THAO TÁC CẬP NHẬT (3D MO WITH UPDATES - O(N^(5/3)))
// ============================================================================
namespace MoWithUpdates {
    struct Query3D {
        int l, r, t, id;
        int bL, bR;
        bool operator<(const Query3D& other) const {
            if (bL != other.bL) return bL < other.bL;
            if (bR != other.bR) return bR < other.bR;
            return t < other.t;
        }
    };

    struct UpdateOp {
        int pos;
        int old_val;
        int new_val;
    };

    /**
     * @brief Mo có cập nhật đếm số phần tử phân biệt
     */
    class Mo3DManager {
    private:
        vector<int> a;
        int n;
        int block_size;
        vector<int> freq;
        int distinct_count;

        void add(int idx) {
            int val = a[idx];
            if (freq[val] == 0) distinct_count++;
            freq[val]++;
        }

        void remove(int idx) {
            int val = a[idx];
            freq[val]--;
            if (freq[val] == 0) distinct_count--;
        }

        void apply_update(const UpdateOp& u, int currL, int currR) {
            // Nếu vị trí thay đổi đang nằm trong đoạn [currL, currR], cập nhật trạng thái
            if (u.pos >= currL && u.pos <= currR) {
                remove(u.pos);
                a[u.pos] = u.new_val;
                add(u.pos);
            } else {
                a[u.pos] = u.new_val;
            }
        }

        void rollback_update(const UpdateOp& u, int currL, int currR) {
            if (u.pos >= currL && u.pos <= currR) {
                remove(u.pos);
                a[u.pos] = u.old_val;
                add(u.pos);
            } else {
                a[u.pos] = u.old_val;
            }
        }

    public:
        Mo3DManager(vector<int> initial_arr, int max_val) 
            : a(initial_arr), n(initial_arr.size()), distinct_count(0) {
            // Kích thước khối tối ưu cho Mo 3D là N^(2/3)
            block_size = max(1, (int)pow(n, 2.0 / 3.0));
            freq.assign(max_val + 1, 0);
        }

        vector<int> process(vector<Query3D> queries, const vector<UpdateOp>& updates) {
            for (auto& q : queries) {
                q.bL = q.l / block_size;
                q.bR = q.r / block_size;
            }
            sort(queries.begin(), queries.end());

            vector<int> ans(queries.size());
            int currL = 0, currR = -1, currT = 0;

            for (const auto& q : queries) {
                // Di chuyển con trỏ không gian L, R
                while (currL > q.l) add(--currL);
                while (currR < q.r) add(++currR);
                while (currL < q.l) remove(currL++);
                while (currR > q.r) remove(currR--);

                // Di chuyển con trỏ thời gian T
                while (currT < q.t) {
                    apply_update(updates[currT], currL, currR);
                    currT++;
                }
                while (currT > q.t) {
                    currT--;
                    rollback_update(updates[currT], currL, currR);
                }

                ans[q.id] = distinct_count;
            }

            return ans;
        }
    };
}

// ============================================================================
// 3. MO TRÊN CÂY QUA EULER TOUR (TREE MO VIA EULER TOUR)
// ============================================================================
namespace TreeMo {
    struct TreeQuery {
        int l, r, id, lca;
        int block;
        bool operator<(const TreeQuery& other) const {
            if (block != other.block)
                return block < other.block;
            return (block & 1) ? (r < other.r) : (r > other.r);
        }
    };

    class TreeMoSolver {
    private:
        int n;
        vector<vector<int>> adj;
        vector<int> val;
        vector<int> tin, tout, euler;
        int timer;
        vector<vector<int>> up;
        int LOGN;

        void dfs(int u, int p) {
            tin[u] = ++timer;
            euler[timer] = u;
            up[u][0] = p;
            for (int j = 1; j < LOGN; ++j) {
                up[u][j] = up[up[u][j - 1]][j - 1];
            }
            for (int v : adj[u]) {
                if (v != p) dfs(v, u);
            }
            tout[u] = ++timer;
            euler[timer] = u;
        }

        bool is_ancestor(int u, int v) {
            return tin[u] <= tin[v] && tout[u] >= tout[v];
        }

        int get_lca(int u, int v) {
            if (is_ancestor(u, v)) return u;
            if (is_ancestor(v, u)) return v;
            for (int j = LOGN - 1; j >= 0; --j) {
                if (!is_ancestor(up[u][j], v)) {
                    u = up[u][j];
                }
            }
            return up[u][0];
        }

    public:
        TreeMoSolver(int n, const vector<int>& values, const vector<pair<int, int>>& edges) 
            : n(n), val(values) {
            adj.resize(n + 1);
            for (const auto& [u, v] : edges) {
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            LOGN = 1;
            while ((1 << LOGN) <= n) LOGN++;
            up.assign(n + 1, vector<int>(LOGN, 0));
            tin.assign(n + 1, 0);
            tout.assign(n + 1, 0);
            euler.assign(2 * n + 1, 0);
            timer = 0;

            dfs(1, 1);
        }

        vector<int> solve_path_distinct(const vector<pair<int, int>>& path_queries) {
            int q = path_queries.size();
            int m = 2 * n; // Kích thước mảng Euler Tour
            int block_size = max(1, (int)(m / sqrt(q)));

            vector<TreeQuery> qs(q);
            for (int i = 0; i < q; ++i) {
                int u = path_queries[i].first;
                int v = path_queries[i].second;
                if (tin[u] > tin[v]) swap(u, v);
                int lca = get_lca(u, v);

                if (lca == u) {
                    // Trường hợp u là tổ tiên của v: [tin[u], tin[v]]
                    qs[i] = {tin[u], tin[v], i, 0, tin[u] / block_size};
                } else {
                    // Trường hợp u không là tổ tiên của v: [tout[u], tin[v]] + đỉnh LCA
                    qs[i] = {tout[u], tin[v], i, lca, tout[u] / block_size};
                }
            }

            sort(qs.begin(), qs.end());

            int max_val = 0;
            for (int i = 1; i <= n; ++i) max_val = max(max_val, val[i]);
            vector<int> freq(max_val + 1, 0);
            vector<bool> vis(n + 1, false);
            int distinct_count = 0;

            auto toggle = [&](int node) {
                int v_val = val[node];
                if (vis[node]) {
                    // Đỉnh xuất hiện lần 2: Loại bỏ khỏi đường đi
                    freq[v_val]--;
                    if (freq[v_val] == 0) distinct_count--;
                    vis[node] = false;
                } else {
                    // Đỉnh xuất hiện lần 1: Thêm vào đường đi
                    if (freq[v_val] == 0) distinct_count++;
                    freq[v_val]++;
                    vis[node] = true;
                }
            };

            vector<int> ans(q);
            int currL = 1, currR = 0;

            for (const auto& query : qs) {
                while (currL > query.l) toggle(euler[--currL]);
                while (currR < query.r) toggle(euler[++currR]);
                while (currL < query.l) toggle(euler[currL++]);
                while (currR > query.r) toggle(euler[currR--]);

                // Nếu cần xét thêm đỉnh LCA
                if (query.lca != 0) toggle(query.lca);
                ans[query.id] = distinct_count;
                if (query.lca != 0) toggle(query.lca); // Hoàn trả trạng thái LCA
            }

            return ans;
        }
    };
}

// ============================================================================
// 4. MO KHÔNG CẦN XÓA / ROLLBACK MO (MO WITHOUT DELETIONS)
// ============================================================================
namespace RollbackMo {
    /**
     * @brief Cấu trúc DSU có cơ chế Rollback (DSU with rollback)
     */
    struct DSUWithRollback {
        vector<int> parent;
        vector<int> sz;
        int components;
        struct Op {
            int u, v;
            bool merged;
        };
        vector<Op> history;

        DSUWithRollback(int n) : parent(n + 1), sz(n + 1, 1), components(n) {
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int u) {
            while (u != parent[u]) u = parent[u];
            return u;
        }

        bool unite(int u, int v) {
            int root_u = find(u);
            int root_v = find(v);
            if (root_u == root_v) {
                history.push_back({root_u, root_v, false});
                return false;
            }
            if (sz[root_u] < sz[root_v]) swap(root_u, root_v);
            parent[root_v] = root_u;
            sz[root_u] += sz[root_v];
            components--;
            history.push_back({root_u, root_v, true});
            return true;
        }

        int snapshot() const {
            return history.size();
        }

        void rollback(int target_size) {
            while ((int)history.size() > target_size) {
                Op op = history.back();
                history.pop_back();
                if (op.merged) {
                    components++;
                    sz[op.u] -= sz[op.v];
                    parent[op.v] = op.v;
                }
            }
        }
    };

    struct EdgeQuery {
        int l, r, id;
    };

    /**
     * @brief Giải bài toán đếm số thành phần liên thông khi xét tập cạnh trong đoạn [L, R]
     */
    vector<int> count_components_range(int num_nodes, const vector<pair<int, int>>& edges, const vector<pair<int, int>>& queries) {
        int m = edges.size();
        int q = queries.size();
        if (q == 0) return {};

        int block_size = max(1, (int)(m / sqrt(q)));
        int num_blocks = (m + block_size - 1) / block_size;

        vector<vector<EdgeQuery>> block_queries(num_blocks);
        vector<int> ans(q);

        for (int i = 0; i < q; ++i) {
            int l = queries[i].first;
            int r = queries[i].second;
            int bl = l / block_size;
            int br = r / block_size;
            if (bl == br) {
                // Truy vấn nằm hoàn toàn trong 1 khối: Brute-force trực tiếp
                DSUWithRollback temp_dsu(num_nodes);
                for (int j = l; j <= r; ++j) {
                    temp_dsu.unite(edges[j].first, edges[j].second);
                }
                ans[i] = temp_dsu.components;
            } else {
                block_queries[bl].push_back({l, r, i});
            }
        }

        // Xử lý từng khối L
        for (int b = 0; b < num_blocks; ++b) {
            if (block_queries[b].empty()) continue;

            // Sắp xếp các truy vấn cùng khối theo R tăng dần
            sort(block_queries[b].begin(), block_queries[b].end(), [](const EdgeQuery& a, const EdgeQuery& b_q) {
                return a.r < b_q.r;
            });

            DSUWithRollback dsu(num_nodes);
            int r_boundary = (b + 1) * block_size - 1;
            int currR = r_boundary;

            for (const auto& qry : block_queries[b]) {
                // 1. Mở rộng con trỏ R tiến về phía trước (không bao giờ lùi)
                while (currR < qry.r) {
                    currR++;
                    dsu.unite(edges[currR].first, edges[currR].second);
                }

                // 2. Lưu lại vết của DSU trước khi tạm thêm đoạn con trỏ L
                int snap = dsu.snapshot();

                // 3. Tạm thời thêm các phần tử từ r_boundary về l
                for (int j = qry.l; j <= r_boundary; ++j) {
                    dsu.unite(edges[j].first, edges[j].second);
                }

                ans[qry.id] = dsu.components;

                // 4. Hoàn tác (Rollback) toàn bộ thao tác của con trỏ L
                dsu.rollback(snap);
            }
        }

        return ans;
    }
}

// ============================================================================
// HÀM KIỂM THỬ ĐƠN VỊ TOÀN DIỆN (UNIT TESTS)
// ============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== BẮT ĐẦU KIỂM THỬ THUẬT TOÁN MO ===" << "\n";

    // 1. Test Mo tiêu chuẩn (Distinct elements)
    {
        vector<int> a = {1, 3, 1, 2, 3, 4, 2};
        vector<pair<int, int>> queries = {
            {0, 2}, // [1, 3, 1] -> 2
            {1, 4}, // [3, 1, 2, 3] -> 3
            {0, 6}, // [1, 3, 1, 2, 3, 4, 2] -> 4
            {3, 5}  // [2, 3, 4] -> 3
        };
        vector<int> expected = {2, 3, 4, 3};
        vector<int> res = StandardMo::solve_distinct_queries(a, queries);
        assert(res == expected);
        cout << "[OK] Test 1: Standard Mo (Distinct Elements) Passed!" << "\n";
    }

    // 2. Test Mo có cập nhật (3D Mo with Updates)
    {
        vector<int> a = {1, 2, 3, 1, 2};
        MoWithUpdates::Mo3DManager manager(a, 10);
        // Ban đầu: [1, 2, 3, 1, 2]
        // Query 0 (time 0): [0, 2] -> {1, 2, 3} -> 3
        // Update 0: a[1] = 5 -> mảng thành [1, 5, 3, 1, 2]
        // Query 1 (time 1): [0, 2] -> {1, 5, 3} -> 3
        // Update 1: a[0] = 5 -> mảng thành [5, 5, 3, 1, 2]
        // Query 2 (time 2): [0, 2] -> {5, 5, 3} -> 2
        vector<MoWithUpdates::Query3D> qs = {
            {0, 2, 0, 0, 0, 0},
            {0, 2, 1, 1, 0, 0},
            {0, 2, 2, 2, 0, 0}
        };
        vector<MoWithUpdates::UpdateOp> updates = {
            {1, 2, 5},
            {0, 1, 5}
        };
        vector<int> res = manager.process(qs, updates);
        vector<int> expected = {3, 3, 2};
        assert(res == expected);
        cout << "[OK] Test 2: Mo with Updates (3D Mo) Passed!" << "\n";
    }

    // 3. Test Mo trên cây (Tree Mo via Euler Tour)
    {
        // Cây:
        //      1 (val=1)
        //     / \
        // 2(val=2) 3(val=1)
        //   /   \
        // 4(val=3) 5(val=2)
        int n = 5;
        vector<int> values = {0, 1, 2, 1, 3, 2};
        vector<pair<int, int>> edges = {{1, 2}, {1, 3}, {2, 4}, {2, 5}};
        TreeMo::TreeMoSolver solver(n, values, edges);

        vector<pair<int, int>> queries = {
            {4, 5}, // path 4-2-5: values {3, 2, 2} -> distinct = 2
            {4, 3}, // path 4-2-1-3: values {3, 2, 1, 1} -> distinct = 3
            {1, 5}, // path 1-2-5: values {1, 2, 2} -> distinct = 2
            {2, 2}  // path 2: values {2} -> distinct = 1
        };
        vector<int> expected = {2, 3, 2, 1};
        vector<int> res = solver.solve_path_distinct(queries);
        assert(res == expected);
        cout << "[OK] Test 3: Tree Mo (Euler Tour Distinct on Path) Passed!" << "\n";
    }

    // 4. Test Mo không cần xóa (Rollback Mo with DSU)
    {
        int num_nodes = 5;
        // Danh sách cạnh (0-indexed):
        // 0: (1, 2)
        // 1: (2, 3)
        // 2: (4, 5)
        // 3: (1, 3) (cạnh thừa)
        // 4: (3, 4) (nối 2 thành phần lớn)
        vector<pair<int, int>> edges = {
            {1, 2},
            {2, 3},
            {4, 5},
            {1, 3},
            {3, 4}
        };
        vector<pair<int, int>> queries = {
            {0, 1}, // Cạnh 0, 1: thành phần {1,2,3}, {4}, {5} -> 3
            {0, 2}, // Cạnh 0, 1, 2: thành phần {1,2,3}, {4,5} -> 2
            {2, 3}, // Cạnh 2, 3: thành phần {4,5}, {1,3}, {2} -> 3
            {0, 4}  // Cạnh 0..4: tất cả liên thông -> 1
        };
        vector<int> expected = {3, 2, 3, 1};
        vector<int> res = RollbackMo::count_components_range(num_nodes, edges, queries);
        assert(res == expected);
        cout << "[OK] Test 4: Rollback Mo (DSU Range Components) Passed!" << "\n";
    }

    cout << "=== TẤT CẢ 4 BỘ TEST THUẬT TOÁN MO ĐÃ VƯỢT QUA THÀNH CÔNG ===" << "\n";
    return 0;
}
