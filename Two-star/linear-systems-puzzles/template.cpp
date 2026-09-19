/**
 * @file template.cpp
 * @brief Chuẩn thuật toán Hệ Phương trình Tuyến tính: Biến đổi Cấu hình & Trò chơi Lưới / Xoay lật (C++20)
 * @author Duc-Minh Vu (SLSCM Lab - FDA - NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <bitset>
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace std;

// ==============================================================================
// 1. HỆ PHƯƠNG TRÌNH TUYẾN TÍNH TRÊN TRƯỜNG NHỊ PHÂN F2 (GF(2) - PHÉP XOR)
// ==============================================================================
namespace LinearSystemGF2 {

    constexpr int MAX_VARS = 1005; // Số ẩn tối đa (hỗ trợ lưới lên tới ~31x31)

    struct ResultGF2 {
        int status;       // -1: Vô nghiệm, 0: Nghiệm duy nhất, k > 0: Có 2^k nghiệm
        int min_ops;      // Số thao tác cực tiểu (nếu giải tìm min operations)
        bitset<MAX_VARS> solution; // Nghiệm tìm được
    };

    /**
     * @brief Khử Gauss trên GF(2) giải hệ A * X = B (mod 2)
     * @param n Số phương trình
     * @param m Số ẩn số
     * @param a Ma trận mở rộng [A | B], kích thước n, mỗi hàng có bit thứ m là vế phải B
     * @return ResultGF2 chứa trạng thái nghiệm và vector nghiệm
     */
    ResultGF2 solve(int n, int m, vector<bitset<MAX_VARS>> a) {
        int rank = 0;
        vector<int> pivot_col(n, -1);
        vector<bool> is_pivot(m, false);

        for (int col = 0; col < m && rank < n; ++col) {
            int pivot = rank;
            while (pivot < n && !a[pivot].test(col)) {
                pivot++;
            }
            if (pivot == n) continue;

            swap(a[rank], a[pivot]);
            pivot_col[rank] = col;
            is_pivot[col] = true;

            for (int i = 0; i < n; ++i) {
                if (i != rank && a[i].test(col)) {
                    a[i] ^= a[rank];
                }
            }
            rank++;
        }

        // Kiểm tra tính tương thích (Inconsistency)
        for (int i = rank; i < n; ++i) {
            if (a[i].test(m)) {
                return {-1, -1, bitset<MAX_VARS>()}; // Vô nghiệm
            }
        }

        bitset<MAX_VARS> ans;
        for (int i = 0; i < rank; ++i) {
            if (a[i].test(m)) {
                ans.set(pivot_col[i]);
            }
        }

        int free_vars = m - rank;
        return {free_vars, (int)ans.count(), ans};
    }

    /**
     * @brief Tìm nghiệm có tổng số thao tác nhỏ nhất (Min Hamming Weight) khi có biến tự do
     * @details Duyệt đệ quy / nhánh cận trên tối đa 20 biến tự do (O(2^k * rank))
     */
    ResultGF2 solveMinWeight(int n, int m, vector<bitset<MAX_VARS>> a) {
        int rank = 0;
        vector<int> pivot_col;
        vector<int> free_cols;
        vector<bool> is_pivot(m, false);

        for (int col = 0; col < m && rank < n; ++col) {
            int pivot = rank;
            while (pivot < n && !a[pivot].test(col)) pivot++;
            if (pivot == n) continue;

            swap(a[rank], a[pivot]);
            pivot_col.push_back(col);
            is_pivot[col] = true;

            for (int i = 0; i < n; ++i) {
                if (i != rank && a[i].test(col)) {
                    a[i] ^= a[rank];
                }
            }
            rank++;
        }

        for (int i = rank; i < n; ++i) {
            if (a[i].test(m)) return {-1, -1, bitset<MAX_VARS>()};
        }

        for (int col = 0; col < m; ++col) {
            if (!is_pivot[col]) free_cols.push_back(col);
        }

        int k = free_cols.size();
        if (k > 20) {
            // Nếu số biến tự do quá lớn, trả về nghiệm cơ sở
            bitset<MAX_VARS> ans;
            for (int i = 0; i < rank; ++i) {
                if (a[i].test(m)) ans.set(pivot_col[i]);
            }
            return {k, (int)ans.count(), ans};
        }

        int best_cost = 1e9;
        bitset<MAX_VARS> best_ans;

        // Duyệt toàn bộ 2^k trạng thái của các biến tự do
        int total_states = 1 << k;
        for (int mask = 0; mask < total_states; ++mask) {
            bitset<MAX_VARS> cur;
            for (int bit = 0; bit < k; ++bit) {
                if ((mask >> bit) & 1) {
                    cur.set(free_cols[bit]);
                }
            }

            // Tính giá trị của các biến trụ (pivot variables)
            for (int i = 0; i < rank; ++i) {
                int col = pivot_col[i];
                int val = a[i].test(m);
                for (int bit = 0; bit < k; ++bit) {
                    if (a[i].test(free_cols[bit]) && cur.test(free_cols[bit])) {
                        val ^= 1;
                    }
                }
                if (val) cur.set(col);
            }

            int cost = cur.count();
            if (cost < best_cost) {
                best_cost = cost;
                best_ans = cur;
            }
        }

        return {k, best_cost, best_ans};
    }
}

// ==============================================================================
// 2. HỆ PHƯƠNG TRÌNH TUYẾN TÍNH TRÊN VÀNH Z_m (MODULO BẤT KỲ)
// ==============================================================================
namespace LinearSystemMod {

    long long extGCD(long long a, long long b, long long &x, long long &y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        long long x1, y1;
        long long g = extGCD(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return g;
    }

    /**
     * @brief Khử Gauss trên vành Z_mod giải A * X = B (mod mod)
     * @param n Số phương trình
     * @param m Số ẩn
     * @param a Ma trận n x (m + 1), cột m là vế phải
     * @param mod Cơ số modulo (ví dụ: 4 cho phép xoay 90 độ)
     * @param ans Vector nghiệm tìm được
     * @return true nếu có nghiệm, false nếu vô nghiệm
     */
    bool solve(int n, int m, vector<vector<long long>> a, int mod, vector<long long>& ans) {
        ans.assign(m, 0);
        int rank = 0;
        vector<int> pivot_col;

        for (int col = 0; col < m && rank < n; ++col) {
            int pivot = -1;
            for (int i = rank; i < n; ++i) {
                a[i][col] = (a[i][col] % mod + mod) % mod;
                if (a[i][col] != 0 && (pivot == -1 || gcd(a[i][col], mod) < gcd(a[pivot][col], mod))) {
                    pivot = i;
                }
            }
            if (pivot == -1) continue;

            swap(a[rank], a[pivot]);

            // Dùng thuật toán Euclid giữa hàng rank và các hàng bên dưới
            for (int i = rank + 1; i < n; ++i) {
                while (a[i][col] != 0) {
                    long long q = a[rank][col] / a[i][col];
                    for (int j = col; j <= m; ++j) {
                        a[rank][j] = (a[rank][j] - q * a[i][j]) % mod;
                        if (a[rank][j] < 0) a[rank][j] += mod;
                    }
                    swap(a[rank], a[i]);
                }
            }

            pivot_col.push_back(col);
            rank++;
        }

        // Kiểm tra vô nghiệm
        for (int i = rank; i < n; ++i) {
            if ((a[i][m] % mod + mod) % mod != 0) return false;
        }

        // Thế ngược (Back-substitution)
        for (int i = rank - 1; i >= 0; --i) {
            int col = pivot_col[i];
            long long sum = a[i][m];
            for (int j = col + 1; j < m; ++j) {
                sum = (sum - a[i][j] * ans[j]) % mod;
                if (sum < 0) sum += mod;
            }
            long long x, y;
            long long g = extGCD(a[i][col], mod, x, y);
            if (sum % g != 0) return false; // Vô nghiệm

            long long step = mod / g;
            ans[col] = (x * (sum / g)) % step;
            if (ans[col] < 0) ans[col] += step;
        }

        return true;
    }
}

// ==============================================================================
// 3. ỨNG DỤNG GIẢI TRÒ CHƠI LƯỚI: LIGHTS OUT 2D & BÀI TOÁN XOAY HÌNH
// ==============================================================================
namespace GridPuzzleSolver {

    /**
     * @brief Giải trò chơi Lights Out kích thước R x C bằng Khử Gauss trên GF(2)
     * @param R Số hàng
     * @param C Số cột
     * @param grid Ma trận trạng thái hiện tại (1: Đang sáng, 0: Đang tắt)
     * @param solution Ma trận kết quả các ô cần bấm (1: Bấm, 0: Không bấm)
     * @return Số lượt bấm nhỏ nhất, hoặc -1 nếu vô nghiệm
     */
    int solveLightsOut(int R, int C, const vector<vector<int>>& grid, vector<vector<int>>& solution) {
        int total_cells = R * C;
        vector<bitset<LinearSystemGF2::MAX_VARS>> eq(total_cells);

        auto getID = [C](int r, int c) { return r * C + c; };

        // Thiết lập hệ phương trình
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                int eq_id = getID(r, c);
                // Ô (r, c) bị tác động bởi chính nó và 4 ô kề cạnh
                eq[eq_id].set(eq_id);
                if (r > 0)     eq[eq_id].set(getID(r - 1, c));
                if (r + 1 < R) eq[eq_id].set(getID(r + 1, c));
                if (c > 0)     eq[eq_id].set(getID(r, c - 1));
                if (c + 1 < C) eq[eq_id].set(getID(r, c + 1));

                // Vế phải: cần đảo trạng thái của ô (r, c) nếu nó đang bật (1 -> 0)
                if (grid[r][c] == 1) {
                    eq[eq_id].set(total_cells);
                }
            }
        }

        auto res = LinearSystemGF2::solveMinWeight(total_cells, total_cells, eq);
        if (res.status == -1) return -1;

        solution.assign(R, vector<int>(C, 0));
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                solution[r][c] = res.solution.test(getID(r, c));
            }
        }
        return res.min_ops;
    }

    /**
     * @brief Giải bài toán xoay mảnh ghép 4 hướng modulo 4
     * @param R Số hàng
     * @param C Số cột
     * @param grid Trạng thái góc quay hiện tại (0: 0°, 1: 90°, 2: 180°, 3: 270°)
     * @param solution Số lần xoay cần áp dụng tại mỗi ô
     * @return true nếu thành công, false nếu vô nghiệm
     */
    bool solveRotationPuzzle(int R, int C, const vector<vector<int>>& grid, vector<vector<int>>& solution) {
        int total_cells = R * C;
        vector<vector<long long>> a(total_cells, vector<long long>(total_cells + 1, 0));

        auto getID = [C](int r, int c) { return r * C + c; };

        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                int eq_id = getID(r, c);
                a[eq_id][eq_id] = 1;
                if (r > 0)     a[eq_id][getID(r - 1, c)] = 1;
                if (r + 1 < R) a[eq_id][getID(r + 1, c)] = 1;
                if (c > 0)     a[eq_id][getID(r, c - 1)] = 1;
                if (c + 1 < C) a[eq_id][getID(r, c + 1)] = 1;

                // Cần bù góc quay để đưa về hướng 0: (4 - grid[r][c]) % 4
                a[eq_id][total_cells] = (4 - grid[r][c]) % 4;
            }
        }

        vector<long long> ans;
        bool ok = LinearSystemMod::solve(total_cells, total_cells, a, 4, ans);
        if (!ok) return false;

        solution.assign(R, vector<int>(C, 0));
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                solution[r][c] = ans[getID(r, c)];
            }
        }
        return true;
    }
}

// ==============================================================================
// 4. KIỂM THỬ ĐƠN VỊ & CHẠY MẪU (UNIT TEST)
// ==============================================================================
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // Test 1: Lights Out 3x3 toàn bộ đèn đang bật
    vector<vector<int>> grid_3x3(3, vector<int>(3, 1));
    vector<vector<int>> solution_3x3;
    int min_ops = GridPuzzleSolver::solveLightsOut(3, 3, grid_3x3, solution_3x3);

    assert(min_ops == 5); // Bấm 4 góc và ô chính giữa
    cout << "[TEST 1 PASSED] Lights Out 3x3 min operations: " << min_ops << "\n";
    cout << "Solution Matrix (1 = Click):\n";
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cout << solution_3x3[r][c] << " ";
        }
        cout << "\n";
    }

    // Test 2: Rotation Puzzle 2x2 modulo 4
    // Ban đầu: góc [1, 2; 3, 0]
    vector<vector<int>> rot_grid = {
        {1, 2},
        {3, 0}
    };
    vector<vector<int>> rot_sol;
    bool rot_ok = GridPuzzleSolver::solveRotationPuzzle(2, 2, rot_grid, rot_sol);
    assert(rot_ok);
    cout << "\n[TEST 2 PASSED] Rotation Puzzle 2x2 solved successfully!\n";
    cout << "Rotation Solution Matrix (Steps to rotate):\n";
    for (int r = 0; r < 2; ++r) {
        for (int c = 0; c < 2; ++c) {
            cout << rot_sol[r][c] << " ";
        }
        cout << "\n";
    }

    cout << "\n==> All linear system puzzle tests passed!\n";
    return 0;
}
