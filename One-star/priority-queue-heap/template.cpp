/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Hàng đợi Ưu tiên & Cấu trúc Đống (Binary Heap & Priority Queue)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstdint>
#include <cassert>

using namespace std;

// ============================================================================
// 1. CÀI ĐẶT THỦ CÔNG BINARY HEAP TRÊN MẢNG 1D (THUẦN TÚY C++)
// ============================================================================

template<typename T, typename Compare = less<T>>
class CustomBinaryHeap {
private:
    vector<T> heap; // 1-indexed: Gốc tại index 1
    Compare comp;

    void swim(int k) {
        while (k > 1 && comp(heap[k / 2], heap[k])) {
            swap(heap[k / 2], heap[k]);
            k = k / 2;
        }
    }

    void sink(int k, int n) {
        while (2 * k <= n) {
            int j = 2 * k; // Con trái
            if (j < n && comp(heap[j], heap[j + 1])) ++j; // j là con có độ ưu tiên cao hơn
            if (!comp(heap[k], heap[j])) break;
            swap(heap[k], heap[j]);
            k = j;
        }
    }

public:
    CustomBinaryHeap() {
        heap.push_back(T{}); // Phần tử bù vị trí index 0
    }

    bool empty() const {
        return heap.size() <= 1;
    }

    int size() const {
        return (int)heap.size() - 1;
    }

    void push(const T& val) {
        heap.push_back(val);
        swim(size());
    }

    T top() const {
        assert(!empty());
        return heap[1];
    }

    void pop() {
        assert(!empty());
        swap(heap[1], heap.back());
        heap.pop_back();
        if (!empty()) {
            sink(1, size());
        }
    }

    /**
     * @brief Thuật toán Build Heap tuyến tính O(N) từ một mảng có sẵn
     */
    void buildHeap(const vector<T>& arr) {
        heap.assign(arr.size() + 1, T{});
        for (size_t i = 0; i < arr.size(); ++i) {
            heap[i + 1] = arr[i];
        }
        int n = size();
        for (int i = n / 2; i >= 1; --i) {
            sink(i, n);
        }
    }

    /**
     * @brief Thuật toán HeapSort O(N log N) in-place
     */
    static void heapSort(vector<T>& a) {
        int n = a.size();
        if (n <= 1) return;

        // Đưa về 1-indexed trong vector tạm thời
        vector<T> h(n + 1);
        for (int i = 0; i < n; ++i) h[i + 1] = a[i];

        Compare cmp;
        auto localSink = [&](int k, int len) {
            while (2 * k <= len) {
                int j = 2 * k;
                if (j < len && cmp(h[j], h[j + 1])) ++j;
                if (!cmp(h[k], h[j])) break;
                swap(h[k], h[j]);
                k = j;
            }
        };

        // 1. Build Max-Heap O(N)
        for (int i = n / 2; i >= 1; --i) {
            localSink(i, n);
        }

        // 2. Trích xuất lần lượt từ cuối về đầu O(N log N)
        for (int len = n; len > 1; --len) {
            swap(h[1], h[len]);
            localSink(1, len - 1);
        }

        for (int i = 0; i < n; ++i) a[i] = h[i + 1];
    }
};

// ============================================================================
// 2. DUY TRÌ TRUNG VỊ ĐỘNG BẰNG 2 HEAPS (RUNNING MEDIAN)
// ============================================================================

class RunningMedian {
private:
    priority_queue<long long> max_heap; // Nửa nhỏ
    priority_queue<long long, vector<long long>, greater<long long>> min_heap; // Nửa lớn

public:
    void addNum(long long num) {
        if (max_heap.empty() || num <= max_heap.top()) {
            max_heap.push(num);
        } else {
            min_heap.push(num);
        }

        // Tái cân bằng: size(max_heap) luôn bằng hoặc nhiều hơn size(min_heap) đúng 1 phần tử
        if (max_heap.size() > min_heap.size() + 1) {
            min_heap.push(max_heap.top());
            max_heap.pop();
        } else if (min_heap.size() > max_heap.size()) {
            max_heap.push(min_heap.top());
            min_heap.pop();
        }
    }

    double findMedian() const {
        assert(!max_heap.empty());
        if (max_heap.size() > min_heap.size()) {
            return (double)max_heap.top();
        }
        return (max_heap.top() + min_heap.top()) / 2.0;
    }

    int size() const {
        return max_heap.size() + min_heap.size();
    }
};

// ============================================================================
// 3. PRIORITY QUEUE HỖ TRỢ XÓA TRỄ BẤT KỲ (REMOVABLE / LAZY PQ)
// ============================================================================

template<typename T, typename Compare = less<T>>
class RemovablePQ {
private:
    priority_queue<T, vector<T>, Compare> pq;
    priority_queue<T, vector<T>, Compare> del_pq;

    void clean() {
        while (!del_pq.empty() && !pq.empty() && pq.top() == del_pq.top()) {
            pq.pop();
            del_pq.pop();
        }
    }

public:
    void push(const T& val) {
        pq.push(val);
    }

    void erase(const T& val) {
        del_pq.push(val);
    }

    T top() {
        clean();
        assert(!empty());
        return pq.top();
    }

    void pop() {
        clean();
        assert(!empty());
        pq.pop();
    }

    bool empty() {
        clean();
        return pq.empty();
    }

    int size() {
        clean();
        return (int)pq.size() - (int)del_pq.size();
    }
};

// ============================================================================
// 4. HỢP NHẤT K DÃY ĐÃ SẮP XẾP (K-WAY MERGE) O(N log K)
// ============================================================================

template<typename T>
vector<T> kWayMerge(const vector<vector<T>>& arrays) {
    struct Element {
        T val;
        int arr_idx;
        int elem_idx;
        bool operator>(const Element& other) const {
            return val > other.val; // Min-Heap
        }
    };

    priority_queue<Element, vector<Element>, greater<Element>> min_pq;

    int totalElements = 0;
    for (int i = 0; i < (int)arrays.size(); ++i) {
        if (!arrays[i].empty()) {
            min_pq.push({arrays[i][0], i, 0});
            totalElements += arrays[i].size();
        }
    }

    vector<T> result;
    result.reserve(totalElements);

    while (!min_pq.empty()) {
        auto [val, arr_i, elem_i] = min_pq.top();
        min_pq.pop();
        result.push_back(val);

        if (elem_i + 1 < (int)arrays[arr_i].size()) {
            min_pq.push({arrays[arr_i][elem_i + 1], arr_i, elem_i + 1});
        }
    }

    return result;
}

// ============================================================================
// 5. THUẬT TOÁN DIJKSTRA ĐƯỜNG ĐI NGẮN NHẤT O((V + E) log V)
// ============================================================================

namespace Dijkstra {
    const long long INF = 1e18 + 7;

    struct Edge {
        int to;
        long long weight;
    };

    /**
     * @brief Tìm khoảng cách ngắn nhất từ đỉnh src đến mọi đỉnh khác
     * @param n Số đỉnh (1-indexed)
     * @param src Đỉnh nguồn
     * @param adj Danh sách kề
     * @return Vector khoảng cách (size n + 1)
     */
    vector<long long> shortestPath(int n, int src, const vector<vector<Edge>>& adj) {
        vector<long long> dist(n + 1, INF);
        // Min-Heap lưu {khoảng cách, đỉnh}
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            // Tối ưu quan trọng: Bỏ qua bản ghi lỗi thời trong heap
            if (d > dist[u]) continue;

            for (const auto& edge : adj[u]) {
                int v = edge.to;
                long long w = edge.weight;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }
}

// ============================================================================
// 6. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    // 1. Test CustomBinaryHeap (Max-Heap)
    CustomBinaryHeap<int> maxHeap;
    maxHeap.push(10);
    maxHeap.push(40);
    maxHeap.push(20);
    maxHeap.push(30);
    assert(maxHeap.top() == 40);
    maxHeap.pop();
    assert(maxHeap.top() == 30);

    // 2. Test BuildHeap & HeapSort
    vector<int> unsorted = {64, 34, 25, 12, 22, 11, 90};
    CustomBinaryHeap<int>::heapSort(unsorted);
    assert(is_sorted(unsorted.begin(), unsorted.end()));

    // 3. Test RunningMedian
    RunningMedian rm;
    rm.addNum(5);
    assert(rm.findMedian() == 5.0);
    rm.addNum(15);
    assert(rm.findMedian() == 10.0);
    rm.addNum(1);
    assert(rm.findMedian() == 5.0);
    rm.addNum(3);
    assert(rm.findMedian() == 4.0);

    // 4. Test RemovablePQ
    RemovablePQ<int> rem_pq;
    rem_pq.push(10);
    rem_pq.push(30);
    rem_pq.push(20);
    assert(rem_pq.top() == 30);
    rem_pq.erase(30); // Xóa 30
    assert(rem_pq.top() == 20);

    // 5. Test K-Way Merge
    vector<vector<int>> k_arrays = {
        {1, 4, 7},
        {2, 5, 8},
        {3, 6, 9}
    };
    auto merged = kWayMerge(k_arrays);
    vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(merged == expected);

    // 6. Test Dijkstra
    int n = 4;
    vector<vector<Dijkstra::Edge>> adj(n + 1);
    adj[1].push_back({2, 1});
    adj[2].push_back({3, 2});
    adj[1].push_back({3, 5});
    adj[3].push_back({4, 1});
    auto d = Dijkstra::shortestPath(n, 1, adj);
    assert(d[1] == 0);
    assert(d[2] == 1);
    assert(d[3] == 3); // Đi qua 1 -> 2 -> 3 (tổng 3) ngắn hơn 1 -> 3 (tổng 5)
    assert(d[4] == 4);

    cout << "==> ALL PRIORITY QUEUE & BINARY HEAP SELF-TESTS PASSED SUCCESSFULLY!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
