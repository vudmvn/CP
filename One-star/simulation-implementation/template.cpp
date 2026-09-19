/**
 * @file template.cpp
 * @brief Chuẩn thuật toán và Framework Mô phỏng & Hiện thực hóa (Simulation & Implementation Framework)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @affiliation SLSCM Lab - Faculty of Data Science and AI (FDA), National Economics University (NEU)
 * @generator Soạn thảo và tối ưu bởi Agentic AI tool
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Chuẩn C++: C++20
 * Bộ công cụ bao gồm:
 * 1. Hằng số vector hướng (4-directions, 8-directions, Knight moves) và hàm kiểm tra biên.
 * 2. Biến đổi bảng 2D: Xoay 90°, 180°, 270° chiều kim đồng hồ, lật gương ngang/dọc.
 * 3. Bộ khung phát hiện chu kỳ cho mô phỏng bước nhảy K cực lớn (Cycle Detection for K <= 10^18).
 * 4. Bộ khung mô phỏng hướng sự kiện (Event-Driven Simulation) dùng priority_queue.
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include <tuple>
#include <cassert>

using namespace std;

namespace SimUtil {

    // ==========================================
    // 1. Vector Hướng Di chuyển & Thao tác Góc
    // ==========================================
    
    // 4 hướng trực giao: U (0), R (1), D (2), L (3) - Thứ tự chiều kim đồng hồ
    const int dx4[4] = {-1, 0, 1, 0};
    const int dy4[4] = {0, 1, 0, -1};
    const char dirChar4[4] = {'U', 'R', 'D', 'L'};

    inline int charToDir4(char c) {
        if (c == 'U' || c == 'N') return 0;
        if (c == 'R' || c == 'E') return 1;
        if (c == 'D' || c == 'S') return 2;
        if (c == 'L' || c == 'W') return 3;
        return -1;
    }

    inline int turnRight(int dir) { return (dir + 1) % 4; }
    inline int turnLeft(int dir)  { return (dir + 3) % 4; }
    inline int turnBack(int dir)  { return (dir + 2) % 4; }

    // 8 hướng bao gồm cả đường chéo
    const int dx8[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    const int dy8[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    // 8 nước đi của quân Mã trên bàn cờ vua
    const int kx[8] = {-2, -2, -1, -1,  1,  1,  2, 2};
    const int ky[8] = {-1,  1, -2,  2, -2,  2, -1, 1};

    // Kiểm tra tính hợp lệ trong biên ma trận [0, n-1] x [0, m-1]
    inline bool isValid(int r, int c, int n, int m) {
        return (r >= 0 && r < n && c >= 0 && c < m);
    }

    // ==========================================
    // 2. Biến đổi Hình học Ma trận 2D
    // ==========================================

    // Xoay ma trận 90 độ theo chiều kim đồng hồ
    template <typename T>
    vector<vector<T>> rotate90(const vector<vector<T>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<vector<T>> res(m, vector<T>(n));
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                res[c][n - 1 - r] = grid[r][c];
            }
        }
        return res;
    }

    // Xoay ma trận xâu ký tự 90 độ theo chiều kim đồng hồ
    vector<string> rotate90(const vector<string>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        vector<string> res(m, string(n, ' '));
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                res[c][n - 1 - r] = grid[r][c];
            }
        }
        return res;
    }

    // Lật ma trận theo trục ngang (Up-Down Flip)
    template <typename T>
    vector<vector<T>> flipHorizontal(vector<vector<T>> grid) {
        reverse(grid.begin(), grid.end());
        return grid;
    }

    // Lật ma trận theo trục dọc (Left-Right Flip)
    template <typename T>
    vector<vector<T>> flipVertical(vector<vector<T>> grid) {
        for (auto& row : grid) {
            reverse(row.begin(), row.end());
        }
        return grid;
    }

    // ==========================================
    // 3. Framework Phát hiện Chu kỳ cho K cực lớn
    // ==========================================

    /**
     * @brief Tìm trạng thái tại bước K (K <= 10^18) bằng cách phát hiện chu kỳ lặp.
     * @tparam State Kiểu dữ liệu biểu diễn trạng thái (cần hỗ trợ toán tử < để lưu trong std::map)
     * @tparam NextFunc Hàm chuyển trạng thái: State nextState(const State&)
     */
    template <typename State, typename NextFunc>
    State simulateWithCycle(State initialState, long long K, NextFunc getNext) {
        if (K == 0) return initialState;

        vector<State> history;
        map<State, int> stepMap;

        State cur = initialState;
        history.push_back(cur);
        stepMap[cur] = 0;

        for (long long step = 1; step <= K; ++step) {
            cur = getNext(cur);

            auto it = stepMap.find(cur);
            if (it != stepMap.end()) {
                // Phát hiện chu kỳ lặp
                int firstSeen = it->second;
                int cycleLength = (int)step - firstSeen;

                long long remaining = K - step;
                int finalIdx = firstSeen + (int)(remaining % cycleLength);
                return history[finalIdx];
            }

            stepMap[cur] = (int)step;
            history.push_back(cur);
        }

        return cur;
    }

    // ==========================================
    // 4. Mô hình Hướng Sự kiện (Event-Driven)
    // ==========================================

    struct Event {
        long long time;
        int type;       // Loại sự kiện do bài toán định nghĩa
        int id;         // ID đối tượng (khách hàng, hạt, xe, v.v.)

        // Ưu tiên thời gian nhỏ nhất (Min-heap)
        bool operator>(const Event& other) const {
            if (time != other.time) return time > other.time;
            return type > other.type;
        }
    };

} // namespace SimUtil

// ==========================================
// Minh họa Tích hợp & Kiểm thử (Demonstration)
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace SimUtil;

    // Ví dụ 1: Xoay ma trận xâu ký tự
    vector<string> grid = {
        "123",
        "456"
    };
    auto rot = rotate90(grid);
    cout << "Original 2x3 Grid rotated 90 deg CW (becomes 3x2):\n";
    for (const auto& row : rot) {
        cout << row << "\n";
    }

    // Ví dụ 2: Mô phỏng chu kỳ với K = 1,000,000,000 bước
    // Giả sử có một trạng thái chuyển vòng tròn: 0 -> 1 -> 2 -> 3 -> 1 -> 2 -> 3 ...
    auto nextFunc = [](int s) {
        if (s == 0) return 1;
        if (s == 1) return 2;
        if (s == 2) return 3;
        return 1; // 3 -> 1
    };

    long long K = 1000000000LL;
    int finalState = simulateWithCycle<int>(0, K, nextFunc);
    cout << "\nState after K = " << K << " steps starting from 0: " << finalState << "\n";
    // Giải thích: 0 (step 0) -> 1 (step 1) -> 2 (step 2) -> 3 (step 3) -> 1 (step 4, chu kỳ dài 3: 1, 2, 3)
    // K = 10^9: (10^9 - 1) % 3 = 999,999,999 % 3 = 0 -> Trạng thái 1.
    assert(finalState == 1);

    cout << "Simulation template initialized successfully!\n";
    return 0;
}
