/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Queue, Deque, 0-1 BFS & Monotonic Deque
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

// ============================================================================
// 1. HÀNG ĐỢI VÒNG TRÊN MẢNG TĨNH (FAST CIRCULAR QUEUE CHO ICPC)
// ============================================================================

template<typename T, int CAPACITY>
class FastCircularQueue {
private:
    T data[CAPACITY];
    int head = 0;
    int tail = 0;
    int count = 0;

public:
    FastCircularQueue() : head(0), tail(0), count(0) {}

    void clear() {
        head = tail = count = 0;
    }

    bool empty() const {
        return count == 0;
    }

    bool full() const {
        return count == CAPACITY;
    }

    int size() const {
        return count;
    }

    void push(const T& val) {
        assert(!full());
        data[tail] = val;
        tail = (tail + 1 == CAPACITY) ? 0 : tail + 1;
        ++count;
    }

    void pop() {
        assert(!empty());
        head = (head + 1 == CAPACITY) ? 0 : head + 1;
        --count;
    }

    const T& front() const {
        assert(!empty());
        return data[head];
    }
};

// ============================================================================
// 2. MIN-QUEUE O(1) AMORTIZED (DÙNG 2 NGĂN XẾP)
// ============================================================================

template<typename T>
class MinQueue {
private:
    stack<pair<T, T>> s_in;  // {val, min_from_bottom}
    stack<pair<T, T>> s_out; // {val, min_from_bottom}

    void transfer() {
        if (s_out.empty()) {
            while (!s_in.empty()) {
                T val = s_in.top().first;
                s_in.pop();
                T mn = s_out.empty() ? val : min(val, s_out.top().second);
                s_out.push({val, mn});
            }
        }
    }

public:
    void push(const T& val) {
        T mn = s_in.empty() ? val : min(val, s_in.top().second);
        s_in.push({val, mn});
    }

    void pop() {
        assert(!empty());
        transfer();
        s_out.pop();
    }

    T front() {
        assert(!empty());
        transfer();
        return s_out.top().first;
    }

    bool empty() const {
        return s_in.empty() && s_out.empty();
    }

    int size() const {
        return (int)(s_in.size() + s_out.size());
    }

    T getMin() {
        assert(!empty());
        if (s_in.empty()) return s_out.top().second;
        if (s_out.empty()) return s_in.top().second;
        return min(s_in.top().second, s_out.top().second);
    }
};

// ============================================================================
// 3. THUẬT TOÁN 0-1 BFS ĐƯỜNG ĐI NGẮN NHẤT O(V + E)
// ============================================================================

namespace ZeroOneBFS {
    const int INF = 1e9 + 7;

    /**
     * @brief Tìm đường đi ngắn nhất trên đồ thị có trọng số cạnh chỉ gồm 0 và 1
     * @param n Số đỉnh (1-indexed)
     * @param src Đỉnh xuất phát
     * @param adj Danh sách kề lưu {đỉnh kề v, trọng số w (0 hoặc 1)}
     * @return Vector khoảng cách ngắn nhất từ src đến mọi đỉnh
     */
    vector<int> shortestPath(int n, int src, const vector<vector<pair<int, int>>>& adj) {
        vector<int> dist(n + 1, INF);
        deque<int> dq;

        dist[src] = 0;
        dq.push_back(src);

        while (!dq.empty()) {
            int u = dq.front();
            dq.pop_front();

            for (const auto& [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    if (w == 0) {
                        dq.push_front(v); // Cạnh 0: ưu tiên xét ngay lập tức
                    } else {
                        dq.push_back(v);  // Cạnh 1: xét sau
                    }
                }
            }
        }
        return dist;
    }
}

// ============================================================================
// 4. HÀNG ĐỢI ĐƠN ĐIỆU (MONOTONIC DEQUE - SLIDING WINDOW)
// ============================================================================

namespace MonotonicQueue {

    /**
     * @brief Tìm giá trị lớn nhất trong mọi cửa sổ trượt kích thước k
     * @return Vector kết quả kích thước n - k + 1, thời gian O(N)
     */
    template<typename T>
    vector<T> slidingWindowMaximum(const vector<T>& a, int k) {
        int n = a.size();
        if (n == 0 || k <= 0) return {};
        if (k > n) k = n;

        deque<int> dq; // Lưu chỉ số index, giá trị a[index] giảm dần
        vector<T> result;

        for (int i = 0; i < n; ++i) {
            // 1. Loại bỏ phần tử trượt ra khỏi cửa sổ [i - k + 1, i]
            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            // 2. Duy trì tính đơn điệu giảm nghiêm ngặt
            while (!dq.empty() && a[dq.back()] <= a[i]) {
                dq.pop_back();
            }
            dq.push_back(i);

            // 3. Ghi nhận đáp án khi cửa sổ đã đủ kích thước k
            if (i >= k - 1) {
                result.push_back(a[dq.front()]);
            }
        }
        return result;
    }

    /**
     * @brief Tìm giá trị nhỏ nhất trong mọi cửa sổ trượt kích thước k
     * @return Vector kết quả kích thước n - k + 1, thời gian O(N)
     */
    template<typename T>
    vector<T> slidingWindowMinimum(const vector<T>& a, int k) {
        int n = a.size();
        if (n == 0 || k <= 0) return {};
        if (k > n) k = n;

        deque<int> dq; // Lưu chỉ số index, giá trị a[index] tăng dần
        vector<T> result;

        for (int i = 0; i < n; ++i) {
            if (!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            while (!dq.empty() && a[dq.back()] >= a[i]) {
                dq.pop_back();
            }
            dq.push_back(i);

            if (i >= k - 1) {
                result.push_back(a[dq.front()]);
            }
        }
        return result;
    }

    /**
     * @brief Mẫu tối ưu Quy hoạch động bằng Monotonic Deque
     * Giải bài toán: DP[i] = max_{i - k <= j < i} (DP[j] + cost[j]) + weight[i] trong O(N)
     */
    vector<long long> optimizeDP(int n, int k, const vector<long long>& cost, const vector<long long>& weight) {
        vector<long long> dp(n, 0);
        deque<int> dq;

        dp[0] = weight[0];
        dq.push_back(0);

        for (int i = 1; i < n; ++i) {
            // Loại bỏ chỉ số j < i - k
            if (!dq.empty() && dq.front() < i - k) {
                dq.pop_front();
            }

            // Giá trị max DP[j] + cost[j] nằm ở đầu deque
            long long best_prev = dp[dq.front()] + cost[dq.front()];
            dp[i] = best_prev + weight[i];

            // Thêm i vào deque duy trì đơn điệu giảm của (dp[i] + cost[i])
            long long current_val = dp[i] + cost[i];
            while (!dq.empty() && (dp[dq.back()] + cost[dq.back()]) <= current_val) {
                dq.pop_back();
            }
            dq.push_back(i);
        }
        return dp;
    }
}

// ============================================================================
// 5. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    // 1. Test FastCircularQueue
    FastCircularQueue<int, 10> fcq;
    assert(fcq.empty());
    fcq.push(10);
    fcq.push(20);
    fcq.push(30);
    assert(fcq.size() == 3);
    assert(fcq.front() == 10);
    fcq.pop();
    assert(fcq.front() == 20);

    // 2. Test MinQueue O(1)
    MinQueue<int> mq;
    mq.push(5);
    mq.push(3);
    mq.push(7);
    assert(mq.getMin() == 3);
    assert(mq.front() == 5);
    mq.pop(); // Pop 5, còn lại {3, 7}
    assert(mq.getMin() == 3);
    mq.pop(); // Pop 3, còn lại {7}
    assert(mq.getMin() == 7);

    // 3. Test 0-1 BFS
    // Đồ thị: 1 --(0)--> 2 --(1)--> 3; 1 --(1)--> 3
    vector<vector<pair<int, int>>> adj(4);
    adj[1].push_back({2, 0});
    adj[2].push_back({3, 1});
    adj[1].push_back({3, 1});
    auto dist = ZeroOneBFS::shortestPath(3, 1, adj);
    assert(dist[1] == 0);
    assert(dist[2] == 0);
    assert(dist[3] == 1);

    // 4. Test Sliding Window Maximum
    vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};
    auto max_res = MonotonicQueue::slidingWindowMaximum(a, 3);
    // Cửa sổ: [1,3,-1]->3, [3,-1,-3]->3, [-1,-3,5]->5, [-3,5,3]->5, [5,3,6]->6, [3,6,7]->7
    vector<int> expected_max = {3, 3, 5, 5, 6, 7};
    assert(max_res == expected_max);

    // 5. Test Sliding Window Minimum
    auto min_res = MonotonicQueue::slidingWindowMinimum(a, 3);
    // Cửa sổ: [1,3,-1]->-1, [3,-1,-3]->-3, [-1,-3,5]->-3, [-3,5,3]->-3, [5,3,6]->3, [3,6,7]->3
    vector<int> expected_min = {-1, -3, -3, -3, 3, 3};
    assert(min_res == expected_min);

    // 6. Test DP Optimization
    vector<long long> cost = {0, 0, 0, 0};
    vector<long long> weight = {1, 2, 3, 4};
    auto dp_res = MonotonicQueue::optimizeDP(4, 2, cost, weight);
    // dp[0] = 1
    // dp[1] = dp[0] + 2 = 3
    // dp[2] = max(dp[0], dp[1]) + 3 = 3 + 3 = 6
    // dp[3] = max(dp[1], dp[2]) + 4 = 6 + 4 = 10
    vector<long long> expected_dp = {1, 3, 6, 10};
    assert(dp_res == expected_dp);

    cout << "==> ALL QUEUE, DEQUE & MONOTONIC QUEUE SELF-TESTS PASSED SUCCESSFULLY!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
