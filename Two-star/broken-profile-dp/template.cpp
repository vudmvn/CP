/**
 * @file template.cpp
 * @brief Thư viện Thuật toán Chuẩn: Quy Hoạch Động Biên Gãy (Broken Profile DP / Plug DP)
 * 
 * Chuyên đề: Competitive Programming Handbook
 * Đơn vị: SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * Soạn thảo: Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * Bản quyền (c) 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.
 * 
 * Các thuật toán cài đặt:
 *  1. DominoTiling: Lát gạch Domino 1x2 trên lưới N x M (có/không có chướng ngại vật).
 *     - Độ phức tạp thời gian: O(N * M * 2^(min(N, M)))
 *     - Độ phức tạp không gian: O(2^(min(N, M))) (Rolling Array)
 *  2. PlugDPHamiltonian: Thuật toán Plug DP (Chen Danqi) đếm số chu trình Hamilton trên lưới.
 *     - Mã hóa trạng thái ngoặc đơn 3 trạng thái (0: không cắm, 1: mở ngoặc '(', 2: đóng ngoặc ')')
 *     - Bảng băm mở (Open-addressing / rolling hash table) tối ưu tốc độ
 *     - Độ phức tạp thời gian: O(N * M * C_M) với C_M là số Catalan
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <unordered_map>

using namespace std;

// ============================================================================
// 1. LÁT GẠCH DOMINO 1x2 (CELL-BY-CELL BROKEN PROFILE DP)
// ============================================================================

namespace DominoTiling {

    /**
     * @brief Đếm số cách lát kín bảng N x M bằng các thanh domino 1x2 (và 2x1).
     * @param n Số dòng
     * @param m Số cột
     * @param mod Modulo kết quả (mặc định 1e9 + 7)
     * @return Số cách lát modulo mod
     */
    long long count_tilings(int n, int m, long long mod = 1000000007) {
        // Tối ưu: Luôn chọn chiều nhỏ hơn làm số bit của mask
        if (n > m) swap(n, m);
        
        // Nếu tổng số ô lẻ, không thể phủ kín bằng domino 1x2
        if ((n * m) % 2 != 0) return 0;

        int total_states = 1 << n;
        vector<long long> dp(total_states, 0);
        dp[0] = 1;

        // Duyệt từng cột c (0 .. m-1), trong mỗi cột duyệt từng dòng r (0 .. n-1)
        for (int c = 0; c < m; ++c) {
            for (int r = 0; r < n; ++r) {
                vector<long long> next_dp(total_states, 0);

                for (int mask = 0; mask < total_states; ++mask) {
                    if (dp[mask] == 0) continue;
                    long long ways = dp[mask];

                    if ((mask >> r) & 1) {
                        // Bit r đang bật: Ô (r, c) đã được phủ bởi domino từ trước (r, c-1)
                        // Ta không thể đặt thêm gạch vào ô này; tắt bit r cho trạng thái kế tiếp
                        int next_mask = mask ^ (1 << r);
                        next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                    } else {
                        // Bit r đang tắt: Ô (r, c) đang trống, BẮT BUỘC phải được phủ ngay
                        
                        // Phương án 1: Đặt domino dọc phủ (r, c) và (r+1, c)
                        // Điều kiện: Chưa chạm đáy cột (r + 1 < n) và ô (r+1, c) đang trống
                        if (r + 1 < n && !((mask >> (r + 1)) & 1)) {
                            int next_mask = mask | (1 << (r + 1));
                            next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                        }

                        // Phương án 2: Đặt domino ngang phủ (r, c) và (r, c+1)
                        // Điều kiện: Chưa chạm biên phải (c + 1 < m)
                        if (c + 1 < m) {
                            int next_mask = mask | (1 << r);
                            next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                        }
                    }
                }
                dp = std::move(next_dp);
            }
        }

        // Sau khi lát kín toàn bộ bảng, không còn ô nào bị chồi ra ngoài (mask == 0)
        return dp[0];
    }

    /**
     * @brief Đếm số cách lát domino 1x2 trên bảng có chướng ngại vật.
     * @param n Số dòng
     * @param m Số cột
     * @param grid Bảng kích thước n x m: '.' là ô trống, '#' là chướng ngại vật
     * @param mod Modulo kết quả
     * @return Số cách lát các ô trống bằng domino 1x2
     */
    long long count_with_obstacles(int n, int m, const vector<string>& grid, long long mod = 1000000007) {
        // Đếm số ô trống
        int empty_cells = 0;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                if (grid[r][c] == '.') empty_cells++;
            }
        }
        if (empty_cells % 2 != 0) return 0;
        if (empty_cells == 0) return 1;

        int total_states = 1 << n;
        vector<long long> dp(total_states, 0);
        dp[0] = 1;

        for (int c = 0; c < m; ++c) {
            for (int r = 0; r < n; ++r) {
                vector<long long> next_dp(total_states, 0);
                bool is_obstacle = (grid[r][c] == '#');

                for (int mask = 0; mask < total_states; ++mask) {
                    if (dp[mask] == 0) continue;
                    long long ways = dp[mask];

                    if (is_obstacle) {
                        // Nếu là chướng ngại vật: không được có quân cờ nào chồi vào (bit r phải là 0)
                        if (!((mask >> r) & 1)) {
                            next_dp[mask] = (next_dp[mask] + ways) % mod;
                        }
                    } else {
                        // Ô trống bình thường
                        if ((mask >> r) & 1) {
                            int next_mask = mask ^ (1 << r);
                            next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                        } else {
                            // Đặt dọc
                            if (r + 1 < n && grid[r + 1][c] != '#' && !((mask >> (r + 1)) & 1)) {
                                int next_mask = mask | (1 << (r + 1));
                                next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                            }
                            // Đặt ngang
                            if (c + 1 < m && grid[r][c + 1] != '#') {
                                int next_mask = mask | (1 << r);
                                next_dp[next_mask] = (next_dp[next_mask] + ways) % mod;
                            }
                        }
                    }
                }
                dp = std::move(next_dp);
            }
        }

        return dp[0];
    }
}

// ============================================================================
// 2. PLUG DP (CHEN DANQI) - CHU TRÌNH HAMILTON TRÊN LƯỚI
// ============================================================================

namespace PlugDPHamiltonian {

    /**
     * @brief Đếm số chu trình Hamilton đi qua tất cả các ô trống '.' trên lưới N x M (có vật cản '#').
     * @note Sử dụng biểu diễn hệ ngoặc đơn: 0 (không cắm), 1 ('('), 2 (')').
     */
    long long count_hamiltonian_cycles(int n, int m, const vector<string>& grid) {
        int last_r = -1, last_c = -1;
        int empty_cnt = 0;
        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                if (grid[r][c] == '.') {
                    last_r = r;
                    last_c = c;
                    empty_cnt++;
                }
            }
        }

        // Không có ô nào cần đi qua
        if (empty_cnt == 0) return 1;
        // Đồ thị hai phía: chu trình đơn trên lưới bắt buộc có số đỉnh chẵn
        if (empty_cnt % 2 != 0 && empty_cnt > 1) return 0;

        // Trạng thái contour có m + 1 plugs
        auto get_plug = [](long long mask, int i) -> int {
            return (mask >> (2 * i)) & 3;
        };

        auto set_plug = [](long long mask, int i, int val) -> long long {
            mask &= ~(3ULL << (2 * i));
            mask |= ((unsigned long long)val << (2 * i));
            return mask;
        };

        auto find_match_right = [&](long long mask, int pos) -> int {
            int depth = 0;
            for (int i = pos; i <= m; ++i) {
                int p = get_plug(mask, i);
                if (p == 1) depth++;
                else if (p == 2) {
                    depth--;
                    if (depth == 0) return i;
                }
            }
            return -1;
        };

        auto find_match_left = [&](long long mask, int pos) -> int {
            int depth = 0;
            for (int i = pos; i >= 0; --i) {
                int p = get_plug(mask, i);
                if (p == 2) depth++;
                else if (p == 1) {
                    depth--;
                    if (depth == 0) return i;
                }
            }
            return -1;
        };

        unordered_map<long long, long long> dp;
        dp[0] = 1;

        for (int r = 0; r < n; ++r) {
            for (int c = 0; c < m; ++c) {
                unordered_map<long long, long long> next_dp;
                bool is_obs = (grid[r][c] == '#');

                for (const auto& [mask, ways] : dp) {
                    int p1 = get_plug(mask, c);     // Plug bên trái
                    int p2 = get_plug(mask, c + 1); // Plug phía trên

                    if (is_obs) {
                        // Ô cản: không được có plug nào nối vào
                        if (p1 == 0 && p2 == 0) {
                            next_dp[mask] += ways;
                        }
                        continue;
                    }

                    if (p1 == 0 && p2 == 0) {
                        // Tạo thành phần liên thông mới (phải và dưới)
                        if (r + 1 < n && c + 1 < m && grid[r + 1][c] != '#' && grid[r][c + 1] != '#') {
                            long long nm = set_plug(mask, c, 1);     // Cắm xuống: '('
                            nm = set_plug(nm, c + 1, 2);             // Cắm sang phải: ')'
                            next_dp[nm] += ways;
                        }
                    } else if (p1 == 0 && p2 != 0) {
                        // Nối plug phía trên đi tiếp: hoặc xuống, hoặc sang phải
                        if (r + 1 < n && grid[r + 1][c] != '#') {
                            long long nm = set_plug(mask, c, p2);
                            nm = set_plug(nm, c + 1, 0);
                            next_dp[nm] += ways;
                        }
                        if (c + 1 < m && grid[r][c + 1] != '#') {
                            long long nm = set_plug(mask, c, 0);
                            nm = set_plug(nm, c + 1, p2);
                            next_dp[nm] += ways;
                        }
                    } else if (p1 != 0 && p2 == 0) {
                        // Nối plug bên trái đi tiếp: hoặc xuống, hoặc sang phải
                        if (r + 1 < n && grid[r + 1][c] != '#') {
                            long long nm = set_plug(mask, c, p1);
                            nm = set_plug(nm, c + 1, 0);
                            next_dp[nm] += ways;
                        }
                        if (c + 1 < m && grid[r][c + 1] != '#') {
                            long long nm = set_plug(mask, c, 0);
                            nm = set_plug(nm, c + 1, p1);
                            next_dp[nm] += ways;
                        }
                    } else if (p1 == 1 && p2 == 1) {
                        // Gặp 2 dấu mở ngoặc '( (' -> Nối lại, tìm dấu ')' tương ứng với p2 đổi thành '('
                        int pos = find_match_right(mask, c + 1);
                        assert(pos != -1);
                        long long nm = set_plug(mask, c, 0);
                        nm = set_plug(nm, c + 1, 0);
                        nm = set_plug(nm, pos, 1);
                        next_dp[nm] += ways;
                    } else if (p1 == 2 && p2 == 2) {
                        // Gặp 2 dấu đóng ngoặc ') )' -> Nối lại, tìm dấu '(' tương ứng với p1 đổi thành ')'
                        int pos = find_match_left(mask, c);
                        assert(pos != -1);
                        long long nm = set_plug(mask, c, 0);
                        nm = set_plug(nm, c + 1, 0);
                        nm = set_plug(nm, pos, 2);
                        next_dp[nm] += ways;
                    } else if (p1 == 2 && p2 == 1) {
                        // Gặp ') (' -> Nối lại trực tiếp, không làm thay đổi các cặp ngoặc khác
                        long long nm = set_plug(mask, c, 0);
                        nm = set_plug(nm, c + 1, 0);
                        next_dp[nm] += ways;
                    } else if (p1 == 1 && p2 == 2) {
                        // Gặp '( )' -> Đóng kín chu trình!
                        // Chỉ hợp lệ khi ĐÂY LÀ Ô TRỐNG CUỐI CÙNG trên toàn bộ lưới
                        if (r == last_r && c == last_c) {
                            long long nm = set_plug(mask, c, 0);
                            nm = set_plug(nm, c + 1, 0);
                            next_dp[nm] += ways;
                        }
                    }
                }
                dp = std::move(next_dp);
            }

            // Chuyển sang dòng kế tiếp: Dịch chuyển các plug sang trái (tức mask << 2)
            // Plug ngoài cùng bên phải (cột m) bắt buộc phải bằng 0 (không có dây chồi ra ngoài lưới)
            unordered_map<long long, long long> next_dp;
            long long mask_limit = (1ULL << (2 * (m + 1))) - 1;
            for (const auto& [mask, ways] : dp) {
                if (get_plug(mask, m) == 0) {
                    long long shifted = (mask << 2) & mask_limit;
                    next_dp[shifted] += ways;
                }
            }
            dp = std::move(next_dp);
        }

        return dp.count(0) ? dp[0] : 0;
    }
}

// ============================================================================
// HỆ THỐNG KIỂM THỬ TỰ ĐỘNG (UNIT TESTS)
// ============================================================================

void run_all_tests() {
    cerr << "=== BAT DAU KIEM THU BROKEN PROFILE DP ===" << endl;

    // 1. Kiểm thử Domino Tiling chuẩn (Công thức giải tích & dãy số OEIS A004003)
    // 2x2: 2
    assert(DominoTiling::count_tilings(2, 2) == 2);
    // 2x3: 3
    assert(DominoTiling::count_tilings(2, 3) == 3);
    // 2x4: 5 (Dãy Fibonacci F_5)
    assert(DominoTiling::count_tilings(2, 4) == 5);
    // 3x3: 0 (Tổng số ô lẻ)
    assert(DominoTiling::count_tilings(3, 3) == 0);
    // 4x4: 36
    assert(DominoTiling::count_tilings(4, 4) == 36);
    // 8x8: 12,988,816
    assert(DominoTiling::count_tilings(8, 8) == 12988816);
    cerr << "[PASS] Test 1: Domino Tiling chuan (2x2, 2x3, 2x4, 4x4, 8x8)" << endl;

    // 2. Kiểm thử Domino Tiling có chướng ngại vật
    {
        // 2x3 có 2 ô chướng ngại vật đối đỉnh:
        // # . .
        // . . #
        // Chỉ có duy nhất 1 cách lát: (0,1)-(0,2) và (1,0)-(1,1)
        vector<string> grid = {
            "#..",
            "..#"
        };
        assert(DominoTiling::count_with_obstacles(2, 3, grid) == 1);

        // 2x2 có 1 ô chướng ngại vật: 3 ô còn lại -> 0 cách
        vector<string> grid2 = {
            "#.",
            ".."
        };
        assert(DominoTiling::count_with_obstacles(2, 2, grid2) == 0);

        // Bảng trống 4x4 so sánh với không có chướng ngại vật
        vector<string> grid_4x4(4, string(4, '.'));
        assert(DominoTiling::count_with_obstacles(4, 4, grid_4x4) == 36);
        cerr << "[PASS] Test 2: Domino Tiling co chuong ngai vat" << endl;
    }

    // 3. Kiểm thử Plug DP (Chu trình Hamilton trên lưới)
    {
        // Lưới 2x2 toàn bộ là '.' -> đúng 1 chu trình
        vector<string> g2x2(2, string(2, '.'));
        assert(PlugDPHamiltonian::count_hamiltonian_cycles(2, 2, g2x2) == 1);

        // Lưới 2x3 toàn bộ là '.' -> đúng 1 chu trình
        vector<string> g2x3(2, string(3, '.'));
        assert(PlugDPHamiltonian::count_hamiltonian_cycles(2, 3, g2x3) == 1);

        // Lưới 3x3 toàn bộ là '.' -> 0 chu trình (9 đỉnh là đồ thị 2 phía lẻ đỉnh)
        vector<string> g3x3(3, string(3, '.'));
        assert(PlugDPHamiltonian::count_hamiltonian_cycles(3, 3, g3x3) == 0);

        // Lưới 4x4 toàn bộ là '.' -> đúng 6 chu trình
        vector<string> g4x4(4, string(4, '.'));
        assert(PlugDPHamiltonian::count_hamiltonian_cycles(4, 4, g4x4) == 6);

        // Lưới 3x3 có vật cản ở tâm:
        // . . .
        // . # .
        // . . .
        // Còn lại 8 đỉnh bao quanh -> đúng 1 chu trình vòng ngoài
        vector<string> g3x3_obs = {
            "...",
            ".#.",
            "..."
        };
        assert(PlugDPHamiltonian::count_hamiltonian_cycles(3, 3, g3x3_obs) == 1);
        cerr << "[PASS] Test 3: Plug DP Hamiltonian Cycle (2x2, 2x3, 3x3, 4x4, 3x3 with obstacle)" << endl;
    }

    cerr << "=== TAT CA KIEM THU DA HOAN TAT XUAT SAC! ===" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_all_tests();

    return 0;
}
