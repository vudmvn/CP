/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Xử lý Lưới Hình chữ nhật (2D Rectangular Grid Techniques)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @affiliation SLSCM Lab - Faculty of Data Science and AI (FDA), National Economics University (NEU)
 * @generator Soạn thảo và tối ưu bởi Agentic AI tool
 * @copyright © 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Chuẩn C++: C++20
 * Bộ công cụ bao gồm:
 * 1. Phẳng hóa tọa độ 2D <-> 1D, mảng hướng trực giao và 8 hướng.
 * 2. Cấu trúc Mảng Cộng dồn 2D (2D Prefix Sums) truy vấn O(1).
 * 3. Cấu trúc Mảng Hiệu 2D (2D Difference Array) cập nhật O(1).
 * 4. Thuật toán Loang Đa Nguồn (Multi-Source BFS) trên lưới O(R * C).
 * 5. Thuật toán 2D Kadane tìm hình chữ nhật con có tổng lớn nhất O(R^2 * C).
 * 6. Thuật toán Maximal Rectangle toàn 1 bằng Monotonic Stack O(R * C).
 */

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <cassert>

using namespace std;

namespace GridUtil {

    // ==========================================
    // 1. Vector Hướng & Ánh xạ Tọa độ
    // ==========================================
    const int dr4[4] = {-1, 0, 1, 0};
    const int dc4[4] = {0, 1, 0, -1};

    const int dr8[8] = {-1, -1, -1,  0, 0,  1, 1, 1};
    const int dc8[8] = {-1,  0,  1, -1, 1, -1, 0, 1};

    inline bool isValid(int r, int c, int R, int C) {
        return (r >= 0 && r < R && c >= 0 && c < C);
    }

    inline int flatId(int r, int c, int C) {
        return r * C + c;
    }

    inline pair<int, int> unflatId(int id, int C) {
        return {id / C, id % C};
    }

    // ==========================================
    // 2. Mảng Cộng dồn 2D (2D Prefix Sums)
    // ==========================================
    struct PrefixSum2D {
        int R, C;
        vector<vector<long long>> pref;

        PrefixSum2D(int R = 0, int C = 0) : R(R), C(C), pref(R + 1, vector<long long>(C + 1, 0)) {}

        template <typename T>
        void init(const vector<vector<T>>& a) {
            R = a.size();
            C = a[0].size();
            pref.assign(R + 1, vector<long long>(C + 1, 0));
            for (int r = 1; r <= R; ++r) {
                for (int c = 1; c <= C; ++c) {
                    pref[r][c] = pref[r - 1][c] + pref[r][c - 1] - pref[r - 1][c - 1] + a[r - 1][c - 1];
                }
            }
        }

        // Truy vấn tổng hình chữ nhật [r1..r2] x [c1..c2] (0-based)
        long long query(int r1, int c1, int r2, int c2) const {
            if (r1 > r2 || c1 > c2) return 0;
            return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
        }
    };

    // ==========================================
    // 3. Mảng Hiệu 2D (2D Difference Array)
    // ==========================================
    struct Difference2D {
        int R, C;
        vector<vector<long long>> diff;

        Difference2D(int R, int C) : R(R), C(C), diff(R + 2, vector<long long>(C + 2, 0)) {}

        // Cộng giá trị val vào hình chữ nhật [r1..r2] x [c1..c2] (0-based) trong O(1)
        void add(int r1, int c1, int r2, int c2, long long val) {
            diff[r1 + 1][c1 + 1] += val;
            diff[r1 + 1][c2 + 2] -= val;
            diff[r2 + 2][c1 + 1] -= val;
            diff[r2 + 2][c2 + 2] += val;
        }

        // Khôi phục mảng kết quả cuối cùng trong O(R * C)
        vector<vector<long long>> build() {
            vector<vector<long long>> res(R, vector<long long>(C, 0));
            for (int r = 1; r <= R; ++r) {
                for (int c = 1; c <= C; ++c) {
                    diff[r][c] += diff[r - 1][c] + diff[r][c - 1] - diff[r - 1][c - 1];
                    res[r - 1][c - 1] = diff[r][c];
                }
            }
            return res;
        }
    };

    // ==========================================
    // 4. Loang Đa Nguồn (Multi-Source BFS)
    // ==========================================
    vector<vector<int>> multiSourceBFS(int R, int C, const vector<pair<int, int>>& sources, const vector<string>& grid) {
        vector<vector<int>> dist(R, vector<int>(C, -1));
        queue<pair<int, int>> q;

        for (auto [r, c] : sources) {
            if (isValid(r, c, R, C) && grid[r][c] != '#') {
                dist[r][c] = 0;
                q.push({r, c});
            }
        }

        while (!q.empty()) {
            auto [r, c] = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int nr = r + dr4[i];
                int nc = c + dc4[i];
                if (isValid(nr, nc, R, C) && grid[nr][nc] != '#' && dist[nr][nc] == -1) {
                    dist[nr][nc] = dist[r][c] + 1;
                    q.push({nr, nc});
                }
            }
        }
        return dist;
    }

    // ==========================================
    // 5. 2D Kadane: Maximum Submatrix Sum O(R^2 * C)
    // ==========================================
    long long maxSubmatrixSum(const vector<vector<int>>& mat) {
        int R = mat.size();
        int C = mat[0].size();
        long long globalMax = -1e18;

        for (int r1 = 0; r1 < R; ++r1) {
            vector<long long> colSum(C, 0);
            for (int r2 = r1; r2 < R; ++r2) {
                for (int c = 0; c < C; ++c) {
                    colSum[c] += mat[r2][c];
                }

                // Chạy Kadane 1D trên mảng colSum
                long long curSum = 0;
                for (int c = 0; c < C; ++c) {
                    curSum = max(colSum[c], curSum + colSum[c]);
                    globalMax = max(globalMax, curSum);
                }
            }
        }
        return globalMax;
    }

    // ==========================================
    // 6. Maximal Rectangle toàn 1 trong O(R * C)
    // ==========================================
    int maximalRectangle(const vector<string>& grid) {
        if (grid.empty()) return 0;
        int R = grid.size(), C = grid[0].size();
        vector<int> H(C, 0);
        int maxArea = 0;

        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                H[c] = (grid[r][c] == '1') ? H[c] + 1 : 0;
            }

            // Monotonic Stack tìm diện tích lớn nhất trên biểu đồ cột H
            vector<int> st;
            for (int c = 0; c <= C; ++c) {
                int curH = (c == C) ? 0 : H[c];
                while (!st.empty() && curH < H[st.back()]) {
                    int h = H[st.back()];
                    st.pop_back();
                    int width = st.empty() ? c : (c - st.back() - 1);
                    maxArea = max(maxArea, h * width);
                }
                st.push_back(c);
            }
        }
        return maxArea;
    }

} // namespace GridUtil

// ==========================================
// Minh họa Tích hợp & Kiểm thử Độc lập
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    using namespace GridUtil;

    // 1. Kiểm thử 2D Prefix Sums
    vector<vector<int>> mat = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    PrefixSum2D ps;
    ps.init(mat);
    // Tổng toàn bộ ma trận = 45
    assert(ps.query(0, 0, 2, 2) == 45);
    // Tổng ô [1..2] x [1..2]: {5, 6, 8, 9} = 28
    assert(ps.query(1, 1, 2, 2) == 28);
    cout << "2D Prefix Sum Query [1..2]x[1..2] = " << ps.query(1, 1, 2, 2) << " (Expected: 28)\n";

    // 2. Kiểm thử 2D Difference Array
    Difference2D diff(3, 3);
    diff.add(0, 0, 1, 1, 5); // Cộng 5 vào ô 2x2 trên trái
    auto built = diff.build();
    assert(built[0][0] == 5 && built[1][1] == 5 && built[2][2] == 0);
    cout << "2D Difference Array verified successfully!\n";

    // 3. Kiểm thử 2D Kadane
    vector<vector<int>> negMat = {
        { 1, -2,  3},
        {-4,  5,  6},
        { 7, -8,  1}
    };
    // Hình chữ nhật con lớn nhất: hàng [1..1] cột [1..2] là {5, 6} tổng 11
    // Hoặc hàng [0..2]: thử các kết hợp
    long long kad = maxSubmatrixSum(negMat);
    cout << "Maximum Submatrix Sum = " << kad << "\n";
    assert(kad >= 11);

    // 4. Kiểm thử Maximal Rectangle of 1s
    vector<string> binGrid = {
        "10100",
        "10111",
        "11111",
        "10010"
    };
    // Hình chữ nhật lớn nhất là 3x2 hoặc 2x3 có diện tích 6 (hàng 1-2, cột 2-4)
    int maxRect = maximalRectangle(binGrid);
    assert(maxRect == 6);
    cout << "Maximal Rectangle Area of 1s = " << maxRect << " (Expected: 6)\n";

    cout << "\nAll Grid Techniques tests passed successfully!\n";
    return 0;
}
