/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Đệ quy & Quay lui (Recursion & Backtracking) trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <string>

using namespace std;

// ==========================================
// 1. SINH HOÁN VỊ XỬ LÝ TRÙNG LẶP (PERMUTATIONS)
// ==========================================

namespace Permutations {
    /**
     * @brief Sinh tất cả các hoán vị phân biệt (kể cả khi mảng có phần tử trùng lặp)
     * @complexity O(N! * N)
     */
    void backtrack(vector<int>& a, vector<bool>& used, vector<int>& cur, vector<vector<int>>& result) {
        if (cur.size() == a.size()) {
            result.push_back(cur);
            return;
        }
        for (size_t i = 0; i < a.size(); ++i) {
            if (used[i]) continue;
            // Cắt tỉa trùng lặp: Nếu phần tử giống phần tử trước và phần tử trước chưa dùng -> bỏ qua
            if (i > 0 && a[i] == a[i - 1] && !used[i - 1]) continue;

            used[i] = true;
            cur.push_back(a[i]);
            backtrack(a, used, cur, result);
            cur.pop_back();
            used[i] = false;
        }
    }

    vector<vector<int>> generate(vector<int> a) {
        sort(a.begin(), a.end());
        vector<bool> used(a.size(), false);
        vector<int> cur;
        vector<vector<int>> result;
        backtrack(a, used, cur, result);
        return result;
    }
}

// ==========================================
// 2. GIẢI BÀI TOÁN N-QUEENS BẰNG PHÉP TOÁN BIT (BITWISE N-QUEENS)
// ==========================================

namespace NQueens {
    int total_solutions = 0;

    /**
     * @brief Đếm số cách xếp N quân hậu không ăn nhau trên bàn cờ N x N
     * @param row Hàng hiện tại đang xét
     * @param n Kích thước bàn cờ
     * @param cols Mặt nạ các cột đã bị chiếm
     * @param diag1 Mặt nạ các đường chéo chính (Tây Bắc - Đông Nam)
     * @param diag2 Mặt nạ các đường chéo phụ (Đông Bắc - Tây Nam)
     * @complexity Nhỏ hơn rất nhiều so với O(N!) nhờ cắt tỉa tức thời
     */
    void solve(int row, int n, int cols, int diag1, int diag2) {
        if (row == n) {
            total_solutions++;
            return;
        }
        // Tất cả vị trí bị tấn công ở hàng hiện tại
        int attacked = cols | diag1 | diag2;
        // Các vị trí còn trống và hợp lệ trong phạm vi n cột
        int available = ((1 << n) - 1) & (~attacked);

        while (available > 0) {
            // Lấy bit 1 thấp nhất (cột trống đầu tiên)
            int bit = available & (-available);
            available ^= bit; // Thử vị trí này

            solve(row + 1, n, cols | bit, (diag1 | bit) << 1, (diag2 | bit) >> 1);
        }
    }

    int count_solutions(int n) {
        total_solutions = 0;
        solve(0, n, 0, 0, 0);
        return total_solutions;
    }
}

// ==========================================
// 3. GIẢI SUDOKU 9x9 BẰNG BITMASK & MRV HEURISTIC
// ==========================================

namespace Sudoku {
    int row_mask[9], col_mask[9], box_mask[9];
    int board[9][9];

    inline int get_box(int r, int c) { return (r / 3) * 3 + (c / 3); }

    // Tìm ô trống có ít lựa chọn hợp lệ nhất (Fail-First / Minimum Remaining Values)
    bool find_best_cell(int &best_r, int &best_c, int &best_mask) {
        int min_count = 10;
        best_r = -1;
        best_c = -1;

        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                if (board[r][c] == 0) {
                    int b = get_box(r, c);
                    int used = row_mask[r] | col_mask[c] | box_mask[b];
                    int available = 0x1FF & (~used); // Các số từ 1 đến 9 có thể điền
                    int cnt = __builtin_popcount(available);
                    if (cnt == 0) return false; // Không thể điền số nào -> Ngõ cụt
                    if (cnt < min_count) {
                        min_count = cnt;
                        best_r = r;
                        best_c = c;
                        best_mask = available;
                        if (cnt == 1) return true; // Tối ưu: Đã tìm thấy ô chỉ có 1 lựa chọn
                    }
                }
            }
        }
        return true;
    }

    bool solve() {
        int r, c, available;
        if (!find_best_cell(r, c, available)) return false;
        if (r == -1) return true; // Không còn ô trống nào -> Hoàn thành!

        int b = get_box(r, c);
        while (available > 0) {
            int bit = available & (-available);
            available ^= bit;
            int num = __builtin_ctz(bit) + 1; // Giá trị số từ 1 đến 9

            // Choose
            board[r][c] = num;
            row_mask[r] |= bit;
            col_mask[c] |= bit;
            box_mask[b] |= bit;

            // Explore
            if (solve()) return true;

            // Unchoose
            board[r][c] = 0;
            row_mask[r] ^= bit;
            col_mask[c] ^= bit;
            box_mask[b] ^= bit;
        }
        return false;
    }

    bool solve_sudoku(int input_board[9][9]) {
        for (int i = 0; i < 9; ++i) row_mask[i] = col_mask[i] = box_mask[i] = 0;
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) {
                board[r][c] = input_board[r][c];
                if (board[r][c] > 0) {
                    int bit = 1 << (board[r][c] - 1);
                    int b = get_box(r, c);
                    row_mask[r] |= bit;
                    col_mask[c] |= bit;
                    box_mask[b] |= bit;
                }
            }
        }
        return solve();
    }
}

// ==========================================
// 4. MEET-IN-THE-MIDDLE CHO BÀI TOÁN SUBSET SUM (N <= 40)
// ==========================================

namespace MeetInTheMiddle {
    /**
     * @brief Đếm số tập con có tổng bằng target với N lên tới 40
     * @complexity O(N * 2^(N/2))
     */
    long long count_subset_sum(const vector<long long>& a, long long target) {
        int n = a.size();
        int n1 = n / 2;
        int n2 = n - n1;

        vector<long long> left_sums;
        left_sums.reserve(1 << n1);
        for (int mask = 0; mask < (1 << n1); ++mask) {
            long long sum = 0;
            for (int i = 0; i < n1; ++i)
                if ((mask >> i) & 1) sum += a[i];
            left_sums.push_back(sum);
        }

        vector<long long> right_sums;
        right_sums.reserve(1 << n2);
        for (int mask = 0; mask < (1 << n2); ++mask) {
            long long sum = 0;
            for (int i = 0; i < n2; ++i)
                if ((mask >> i) & 1) sum += a[n1 + i];
            right_sums.push_back(sum);
        }

        sort(left_sums.begin(), left_sums.end());

        long long total = 0;
        for (long long r : right_sums) {
            long long need = target - r;
            auto [first, last] = equal_range(left_sums.begin(), left_sums.end(), need);
            total += (last - first);
        }
        return total;
    }
}

// ==========================================
// 5. DEMO MAIN
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Demo Permutations với mảng có phần tử trùng: {1, 2, 2}
    cout << "=== 1. PERMUTATIONS DEMO {1, 2, 2} ===\n";
    auto perms = Permutations::generate({1, 2, 2});
    for (const auto& p : perms) {
        cout << "[ ";
        for (int x : p) cout << x << " ";
        cout << "]\n";
    }
    cout << "Tong so hoan vi phan biet: " << perms.size() << "\n\n";

    // 2. Demo Bitwise N-Queens với N = 8
    cout << "=== 2. BITWISE N-QUEENS DEMO ===\n";
    for (int n = 4; n <= 10; ++n) {
        cout << "N = " << n << " -> So nghiem: " << NQueens::count_solutions(n) << "\n";
    }
    cout << "\n";

    // 3. Demo Meet-in-the-middle với Subset Sum
    cout << "=== 3. MEET-IN-THE-MIDDLE DEMO ===\n";
    vector<long long> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    long long S = 25;
    long long ways = MeetInTheMiddle::count_subset_sum(arr, S);
    cout << "So tap con co tong bang " << S << " tu mang 20 so: " << ways << "\n\n";

    // 4. Demo Sudoku Solver
    cout << "=== 4. SUDOKU SOLVER DEMO ===\n";
    int sample_sudoku[9][9] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    if (Sudoku::solve_sudoku(sample_sudoku)) {
        cout << "Giai Sudoku thanh cong:\n";
        for (int r = 0; r < 9; ++r) {
            for (int c = 0; c < 9; ++c) cout << Sudoku::board[r][c] << " ";
            cout << "\n";
        }
    } else {
        cout << "Sudoku vo nghiem!\n";
    }

    return 0;
}
