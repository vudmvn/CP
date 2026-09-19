/**
 * @file template.cpp
 * @brief Thư viện Thuật toán Chuẩn: Kỹ Thuật Đảo Nhãn Quy Hoạch Động (DP State-Value Inversion)
 * 
 * Chuyên đề: Competitive Programming Handbook
 * Đơn vị: SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * Soạn thảo: Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * Bản quyền (c) 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.
 * 
 * Các thuật toán & mô hình cài đặt:
 *  1. Knapsack2Solver: Ba lô 0-1 trọng số khổng lồ (W <= 1e18, sum(V) <= 1e5) - AtCoder DP Task E.
 *     - Hỗ trợ tái dựng vết các món đồ được chọn.
 *  2. LISInversion: Dãy con tăng dài nhất O(N log N) bằng kỹ thuật đảo vai trò (dp[len] = min_tail).
 *     - Hỗ trợ khôi phục trọn vẹn dãy con tối ưu.
 *  3. ConstrainedShortestPath: Tìm đường đi ngắn nhất với ràng buộc tổng chi phí <= C_max.
 *     - Đảo nhãn: dp[u][cost] = khoảng cách nhỏ nhất.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>
#include <tuple>

using namespace std;

// ============================================================================
// 1. BA LÔ TRỌNG SỐ LỚN (KNAPSACK 2 - VALUE INVERSION)
// ============================================================================

namespace Knapsack2Solver {
    const long long INF = 1e18;

    struct Item {
        long long weight;
        int value;
    };

    /**
     * @brief Tìm tổng giá trị lớn nhất của các đồ vật có tổng trọng lượng <= W.
     * @param items Danh sách các món đồ (weight, value)
     * @param W Trọng lượng tối đa (có thể lên tới 10^18)
     * @return Giá trị lớn nhất đạt được
     */
    long long solve(const vector<Item>& items, long long W) {
        int sum_v = 0;
        for (const auto& item : items) {
            sum_v += item.value;
        }

        // dp[v]: Trọng lượng nhỏ nhất để đạt được tổng giá trị đúng bằng v
        vector<long long> dp(sum_v + 1, INF);
        dp[0] = 0;

        for (const auto& item : items) {
            for (int v = sum_v; v >= item.value; --v) {
                if (dp[v - item.value] != INF) {
                    dp[v] = min(dp[v], dp[v - item.value] + item.weight);
                }
            }
        }

        // Lùi từ tổng giá trị lớn nhất về 0 để tìm giá trị tối ưu
        for (int v = sum_v; v >= 0; --v) {
            if (dp[v] <= W) {
                return v;
            }
        }
        return 0;
    }

    /**
     * @brief Giải Knapsack 2 và tái dựng vết các đồ vật được chọn.
     */
    pair<int, vector<int>> solve_with_reconstruction(const vector<Item>& items, long long W) {
        int n = items.size();
        int sum_v = 0;
        for (const auto& item : items) sum_v += item.value;

        // Lưu bảng 2 chiều để truy vết: dp_table[i][v]
        vector<vector<long long>> dp(n + 1, vector<long long>(sum_v + 1, INF));
        dp[0][0] = 0;

        for (int i = 0; i < n; ++i) {
            for (int v = 0; v <= sum_v; ++v) {
                // Không chọn đồ vật i
                dp[i + 1][v] = min(dp[i + 1][v], dp[i][v]);
                // Chọn đồ vật i
                if (v >= items[i].value && dp[i][v - items[i].value] != INF) {
                    dp[i + 1][v] = min(dp[i + 1][v], dp[i][v - items[i].value] + items[i].weight);
                }
            }
        }

        // Tìm v tối ưu
        int best_v = 0;
        for (int v = sum_v; v >= 0; --v) {
            if (dp[n][v] <= W) {
                best_v = v;
                break;
            }
        }

        // Truy vết ngược
        vector<int> chosen_indices;
        int curr_v = best_v;
        for (int i = n - 1; i >= 0; --i) {
            if (curr_v >= items[i].value && 
                dp[i][curr_v - items[i].value] != INF && 
                dp[i + 1][curr_v] == dp[i][curr_v - items[i].value] + items[i].weight) {
                chosen_indices.push_back(i);
                curr_v -= items[i].value;
            }
        }
        reverse(chosen_indices.begin(), chosen_indices.end());

        return {best_v, chosen_indices};
    }
}

// ============================================================================
// 2. DÃY CON TĂNG DÀI NHẤT (LIS - INVERSION O(N log N))
// ============================================================================

namespace LISInversion {
    /**
     * @brief Tìm độ dài và dãy con tăng dài nhất (Strictly Increasing Subsequence).
     * @param a Mảng số nguyên
     * @return Dãy con tăng dài nhất
     */
    vector<int> get_lis(const vector<int>& a) {
        int n = a.size();
        if (n == 0) return {};

        // tails[len]: Giá trị phần tử kết thúc nhỏ nhất của một LIS có độ dài len
        vector<int> tails;
        // tail_indices[len]: Chỉ số trong mảng a của phần tử kết thúc LIS độ dài len
        vector<int> tail_indices;
        // parent[i]: Lưu chỉ số phần tử đứng trước a[i] trong LIS tối ưu
        vector<int> parent(n, -1);

        for (int i = 0; i < n; ++i) {
            // Dùng lower_bound tìm vị trí đầu tiên >= a[i]
            auto it = lower_bound(tails.begin(), tails.end(), a[i]);
            int idx = it - tails.begin();

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

        // Khôi phục dãy con từ phần tử kết thúc của LIS dài nhất
        vector<int> lis;
        int curr = tail_indices.back();
        while (curr != -1) {
            lis.push_back(a[curr]);
            curr = parent[curr];
        }
        reverse(lis.begin(), lis.end());
        return lis;
    }
}

// ============================================================================
// 3. ĐƯỜNG ĐI NGẮN NHẤT CÓ RÀNG BUỘC CHI PHÍ (CONSTRAINED SHORTEST PATH)
// ============================================================================

namespace ConstrainedShortestPath {
    const long long INF = 1e18;

    struct Edge {
        int to;
        long long dist;
        int cost;
    };

    /**
     * @brief Tìm độ dài đường đi ngắn nhất từ start_node đến end_node có tổng chi phí <= max_cost.
     * @note Đảo nhãn: dp[u][c] là độ dài ngắn nhất tới đỉnh u với chi phí c.
     */
    long long find_shortest_path(int n, const vector<vector<Edge>>& adj, int start_node, int end_node, int max_cost) {
        // dp[u][c]: Độ dài nhỏ nhất tới đỉnh u với chi phí đúng bằng c
        vector<vector<long long>> dp(n + 1, vector<long long>(max_cost + 1, INF));
        
        // Priority Queue: {dist, u, cost}
        using State = tuple<long long, int, int>;
        priority_queue<State, vector<State>, greater<State>> pq;

        dp[start_node][0] = 0;
        pq.emplace(0, start_node, 0);

        while (!pq.empty()) {
            auto [d, u, c] = pq.top();
            pq.pop();

            if (d > dp[u][c]) continue;
            if (u == end_node) continue; // Tiếp tục tìm các nhánh chi phí khác

            for (const auto& edge : adj[u]) {
                int next_cost = c + edge.cost;
                if (next_cost <= max_cost) {
                    if (dp[edge.to][next_cost] > d + edge.dist) {
                        dp[edge.to][next_cost] = d + edge.dist;
                        pq.emplace(dp[edge.to][next_cost], edge.to, next_cost);
                    }
                }
            }
        }

        long long ans = INF;
        for (int c = 0; c <= max_cost; ++c) {
            ans = min(ans, dp[end_node][c]);
        }
        return (ans == INF ? -1 : ans);
    }
}

// ============================================================================
// HỆ THỐNG KIỂM THỬ TỰ ĐỘNG (UNIT TESTS)
// ============================================================================

void run_all_tests() {
    cerr << "=== BAT DAU KIEM THU DP STATE-VALUE INVERSION ===" << endl;

    // 1. Kiểm thử Knapsack 2 cơ bản (AtCoder DP Task E)
    {
        // 3 món: (w=5, v=2), (w=4, v=3), (w=7, v=4). W = 10 -> Đáp án: 5 (chọn món 1 và 2: w=9 <= 10, v=2+3=5)
        vector<Knapsack2Solver::Item> items = {
            {5, 2},
            {4, 3},
            {7, 4}
        };
        assert(Knapsack2Solver::solve(items, 10) == 5);

        // Kiểm thử truy vết
        auto [best_v, chosen] = Knapsack2Solver::solve_with_reconstruction(items, 10);
        assert(best_v == 5);
        assert(chosen.size() == 2);
        assert(chosen[0] == 0 && chosen[1] == 1); // Chọn món 0 và món 1
        cerr << "[PASS] Test 1: Knapsack 2 co ban & Truy vet nghiem" << endl;
    }

    // 2. Kiểm thử Knapsack 2 với trọng lượng siêu lớn (W = 10^14)
    {
        vector<Knapsack2Solver::Item> large_items = {
            {1000000000000LL, 100},  // 10^12
            {2000000000000LL, 200},  // 2*10^12
            {5000000000000LL, 500}   // 5*10^12
        };
        // W = 6*10^12 -> Chọn món 0 và 2: w = 6*10^12 <= W, v = 600
        assert(Knapsack2Solver::solve(large_items, 6000000000000LL) == 600);
        cerr << "[PASS] Test 2: Knapsack 2 voi trong luong sieu lon (W = 6*10^12)" << endl;
    }

    // 3. Kiểm thử LIS Inversion O(N log N)
    {
        vector<int> a = {10, 9, 2, 5, 3, 7, 101, 18};
        vector<int> lis = LISInversion::get_lis(a);
        // LIS hợp lệ có độ dài 4: ví dụ [2, 3, 7, 101] hoặc [2, 5, 7, 18]...
        assert(lis.size() == 4);
        for (size_t i = 1; i < lis.size(); ++i) {
            assert(lis[i] > lis[i - 1]);
        }
        cerr << "[PASS] Test 3: LIS Inversion O(N log N) chinh xac" << endl;
    }

    // 4. Kiểm thử Constrained Shortest Path
    {
        int n = 4;
        vector<vector<ConstrainedShortestPath::Edge>> adj(n + 1);
        // Đỉnh 1 -> Đỉnh 2: dist=10, cost=2
        adj[1].push_back({2, 10, 2});
        // Đỉnh 2 -> Đỉnh 4: dist=20, cost=3
        adj[2].push_back({4, 20, 3});
        // Đỉnh 1 -> Đỉnh 3: dist=5, cost=6
        adj[1].push_back({3, 5, 6});
        // Đỉnh 3 -> Đỉnh 4: dist=5, cost=5
        adj[3].push_back({4, 5, 5});

        // Đường 1 -> 2 -> 4: dist = 30, cost = 5
        // Đường 1 -> 3 -> 4: dist = 10, cost = 11

        // Với max_cost = 5: Chỉ đi được đường 1 -> 2 -> 4 (dist = 30)
        assert(ConstrainedShortestPath::find_shortest_path(n, adj, 1, 4, 5) == 30);
        // Với max_cost = 11: Đi được đường 1 -> 3 -> 4 ngắn hơn (dist = 10)
        assert(ConstrainedShortestPath::find_shortest_path(n, adj, 1, 4, 11) == 10);
        // Với max_cost = 4: Không tới được đỉnh 4 -> return -1
        assert(ConstrainedShortestPath::find_shortest_path(n, adj, 1, 4, 4) == -1);
        cerr << "[PASS] Test 4: Constrained Shortest Path hoat dong xuat sac" << endl;
    }

    cerr << "=== TAT CA KIEM THU DA HOAN TAT XUAT SAC! ===" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_all_tests();

    return 0;
}
