/**
 * @file template.cpp
 * @brief Thư viện Thuật toán Chuẩn: Kỹ Thuật Slope Trick (Tối Ưu Hóa Hàm Lồi Trong DP)
 * 
 * Chuyên đề: Competitive Programming Handbook
 * Đơn vị: SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * Soạn thảo: Được hỗ trợ và soạn thảo bởi Agentic AI tool
 * Bản quyền (c) 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.
 * 
 * Các thuật toán & mô hình cài đặt:
 *  1. SlopeTrick: Cấu trúc dữ liệu đầy đủ duy trì hàm lồi tuyến tính từng khúc f(x).
 *     - Hỗ trợ: cộng |x - a|, max(0, x - a), max(0, a - x), prefix_min, suffix_min, shift_range.
 *  2. StrictlyIncreasingTransformSolver: Biến đổi mảng thành dãy tăng nghiêm ngặt với chi phí min (CF 713C / CF 13C).
 *  3. StockTradingSolver: Bài toán mua bán cổ phiếu tối ưu (Buy Low Sell High - CF 867E).
 */

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>

using namespace std;

// ============================================================================
// 1. CẤU TRÚC DỮ LIỆU SLOPE TRICK ĐẦY ĐỦ (GENERAL-PURPOSE SLOPE TRICK)
// ============================================================================

class SlopeTrick {
private:
    static constexpr long long INF = 2e18;

    long long f_min;       // Giá trị cực tiểu toàn cục của f(x)
    long long shift_L;     // Độ lệch cho các điểm gãy bên trái
    long long shift_R;     // Độ lệch cho các điểm gãy bên phải

    // L: Hàng đợi ưu tiên cực đại (Max-Heap) lưu các điểm gãy phía slope <= 0
    priority_queue<long long> L;
    // R: Hàng đợi ưu tiên cực tiểu (Min-Heap) lưu các điểm gãy phía slope >= 0
    priority_queue<long long, vector<long long>, greater<long long>> R;

public:
    SlopeTrick() : f_min(0), shift_L(0), shift_R(0) {}

    /**
     * @brief Lấy giá trị cực tiểu toàn cục của hàm f(x).
     */
    long long get_min() const {
        return f_min;
    }

    /**
     * @brief Lấy đoạn [l_max, r_min] nơi hàm số đạt cực tiểu (vùng độ dốc = 0).
     */
    pair<long long, long long> get_min_range() const {
        long long l_max = L.empty() ? -INF : L.top() + shift_L;
        long long r_min = R.empty() ? INF : R.top() + shift_R;
        return {l_max, r_min};
    }

    /**
     * @brief Cộng hằng số c vào hàm: f(x) <- f(x) + c.
     */
    void add_const(long long c) {
        f_min += c;
    }

    /**
     * @brief Cộng hàm nhánh phải: f(x) <- f(x) + max(0LL, x - a).
     * @note Làm tăng hệ số góc thêm +1 tại các điểm x > a.
     */
    void add_x_minus_a(long long a) {
        long long l_max = L.empty() ? -INF : L.top() + shift_L;
        if (a < l_max) {
            f_min += l_max - a;
            L.pop();
            L.push(a - shift_L);
            R.push(l_max - shift_R);
        } else {
            R.push(a - shift_R);
        }
    }

    /**
     * @brief Cộng hàm nhánh trái: f(x) <- f(x) + max(0LL, a - x).
     * @note Làm giảm hệ số góc đi 1 tại các điểm x < a.
     */
    void add_a_minus_x(long long a) {
        long long r_min = R.empty() ? INF : R.top() + shift_R;
        if (a > r_min) {
            f_min += a - r_min;
            R.pop();
            R.push(a - shift_R);
            L.push(r_min - shift_L);
        } else {
            L.push(a - shift_L);
        }
    }

    /**
     * @brief Cộng hàm trị tuyệt đối: f(x) <- f(x) + |x - a|.
     */
    void add_abs(long long a) {
        long long l_max = L.empty() ? -INF : L.top() + shift_L;
        long long r_min = R.empty() ? INF : R.top() + shift_R;

        if (a < l_max) {
            f_min += l_max - a;
            L.pop();
            L.push(a - shift_L);
            L.push(a - shift_L);
            R.push(l_max - shift_R);
        } else if (a > r_min) {
            f_min += a - r_min;
            R.pop();
            R.push(a - shift_R);
            R.push(a - shift_R);
            L.push(r_min - shift_L);
        } else {
            L.push(a - shift_L);
            R.push(a - shift_R);
        }
    }

    /**
     * @brief Tiền tố cực tiểu: f(x) <- min_{y <= x} f(y).
     * @note Làm phẳng toàn bộ các đoạn có độ dốc > 0 (xóa sạch R).
     */
    void prefix_min() {
        while (!R.empty()) R.pop();
    }

    /**
     * @brief Hậu tố cực tiểu: f(x) <- min_{y >= x} f(y).
     * @note Làm phẳng toàn bộ các đoạn có độ dốc < 0 (xóa sạch L).
     */
    void suffix_min() {
        while (!L.empty()) L.pop();
    }

    /**
     * @brief Dịch chuyển và co dãn cửa sổ cực tiểu: f(x) <- min_{y in [x - b, x - a]} f(y).
     * @param a Độ dịch chuyển của biên trái (y <= x - a)
     * @param b Độ dịch chuyển của biên phải (y >= x - b)
     */
    void shift_range(long long a, long long b) {
        shift_L += a;
        shift_R += b;
    }

    /**
     * @brief Dịch chuyển tịnh tiến hàm số sang phải c đơn vị: f(x) <- f(x - c).
     */
    void shift_x(long long c) {
        shift_range(c, c);
    }
};

// ============================================================================
// 2. BIẾN ĐỔI DÃY THÀNH TĂNG NGHIÊM NGẶT (CODEFORCES 713C / 13C)
// ============================================================================

namespace StrictlyIncreasingTransformSolver {
    /**
     * @brief Tìm chi phí tối thiểu để biến đổi A thành dãy tăng nghiêm ngặt (A_1 < A_2 < ... < A_N).
     * @param a Mảng ban đầu
     * @return Tổng chênh lệch nhỏ nhất sum |a[i] - x[i]|
     * @complexity O(N log N) thời gian, O(N) bộ nhớ.
     */
    long long solve(const vector<long long>& a) {
        int n = a.size();
        if (n <= 1) return 0;

        // Đổi biến b[i] = a[i] - i để đưa về bài toán dãy không giảm
        priority_queue<long long> L;
        long long min_cost = 0;

        for (int i = 0; i < n; ++i) {
            long long b_i = a[i] - i;
            if (!L.empty() && L.top() > b_i) {
                min_cost += L.top() - b_i;
                L.pop();
                L.push(b_i);
            }
            L.push(b_i);
        }

        return min_cost;
    }

    /**
     * @brief Biến đổi thành dãy không giảm (Non-decreasing: A_1 <= A_2 <= ... <= A_N).
     * @note Codeforces 13C - Sequence
     */
    long long solve_non_decreasing(const vector<long long>& a) {
        priority_queue<long long> L;
        long long min_cost = 0;

        for (long long val : a) {
            if (!L.empty() && L.top() > val) {
                min_cost += L.top() - val;
                L.pop();
                L.push(val);
            }
            L.push(val);
        }

        return min_cost;
    }
}

// ============================================================================
// 3. MUA BÁN CỔ PHIẾU TỐI ƯU (BUY LOW SELL HIGH - CODEFORCES 867E)
// ============================================================================

namespace StockTradingSolver {
    /**
     * @brief Tính lợi nhuận tối đa khi mỗi ngày có thể mua 1, bán 1 hoặc không làm gì.
     * @param prices Giá cổ phiếu từng ngày
     * @return Lợi nhuận cực đại
     * @complexity O(N log N)
     */
    long long max_profit(const vector<long long>& prices) {
        priority_queue<long long, vector<long long>, greater<long long>> min_heap;
        long long total_profit = 0;

        for (long long p : prices) {
            if (!min_heap.empty() && min_heap.top() < p) {
                total_profit += p - min_heap.top();
                min_heap.pop();
                // Đẩy p vào như một cơ hội "chuyển giao" (nếu sau này có giá cao hơn nữa mua lại)
                min_heap.push(p);
            }
            // Đẩy p vào như một cơ hội mua mới
            min_heap.push(p);
        }

        return total_profit;
    }
}

// ============================================================================
// HỆ THỐNG KIỂM THỬ TỰ ĐỘNG (UNIT TESTS)
// ============================================================================

void run_all_tests() {
    cerr << "=== BAT DAU KIEM THU SLOPE TRICK ===" << endl;

    // 1. Kiểm thử SlopeTrick: Tìm trung vị của {1, 2, 5}
    {
        SlopeTrick st;
        st.add_abs(1);
        st.add_abs(2);
        st.add_abs(5);
        // Cực tiểu tại x = 2, giá trị = |2-1| + |2-2| + |2-5| = 1 + 0 + 3 = 4
        assert(st.get_min() == 4);
        auto [l, r] = st.get_min_range();
        assert(l == 2 && r == 2);
        cerr << "[PASS] Test 1: SlopeTrick co ban (Tong khoang cach tuyet doi & Trung vi)" << endl;
    }

    // 2. Kiểm thử CF 713C (Sonya and Problem Wihtout a Legend)
    {
        // Sample 1: [2, 1, 5, 11, 5, 9, 11] -> ans = 9
        vector<long long> a1 = {2, 1, 5, 11, 5, 9, 11};
        assert(StrictlyIncreasingTransformSolver::solve(a1) == 9);

        // Sample 2: [5, 4, 3, 2, 1] -> ans = 12
        vector<long long> a2 = {5, 4, 3, 2, 1};
        assert(StrictlyIncreasingTransformSolver::solve(a2) == 12);
        cerr << "[PASS] Test 2: CF 713C Strictly Increasing Transform" << endl;
    }

    // 3. Kiểm thử CF 13C (Sequence - Non-decreasing)
    {
        // [3, 2, -4, 5, 7, 1] -> biến đổi thành không giảm
        vector<long long> a3 = {3, 2, -4, 5, 7, 1};
        long long ans3 = StrictlyIncreasingTransformSolver::solve_non_decreasing(a3);
        // Đáp án tối ưu là 13
        assert(ans3 == 13);
        cerr << "[PASS] Test 3: CF 13C Non-decreasing Sequence" << endl;
    }

    // 4. Kiểm thử CF 867E (Buy Low Sell High)
    {
        // [10, 5, 4, 7, 9, 12] -> ans = 12 (mua 5 ban 9, mua 4 ban 12: 4 + 8 = 12)
        vector<long long> p1 = {10, 5, 4, 7, 9, 12};
        assert(StockTradingSolver::max_profit(p1) == 12);

        // [1, 2, 10, 9] -> ans = 16 (mua 1 ban 10, mua 2 ban 9: 9 + 7 = 16)
        vector<long long> p2 = {1, 2, 10, 9};
        assert(StockTradingSolver::max_profit(p2) == 16);
        cerr << "[PASS] Test 4: CF 867E Stock Trading Solver" << endl;
    }

    // 5. Kiểm thử Thao tác Prefix Min & Shift Range
    {
        SlopeTrick st;
        st.add_abs(5); // f(x) = |x - 5|, f_min = 0 tai x = 5
        assert(st.get_min() == 0);

        st.prefix_min(); // lam phang ben phai: f(x) = max(0, 5 - x)
        auto [l, r] = st.get_min_range();
        assert(l == 5);

        // Dịch chuyển cửa sổ [-2, 3]: y in [x - 3, x - (-2)]
        st.shift_range(-2, 3);
        auto [nl, nr] = st.get_min_range();
        assert(nl == 5 + (-2) && nl == 3);
        cerr << "[PASS] Test 5: SlopeTrick Prefix Min & Shift Range" << endl;
    }

    cerr << "=== TAT CA KIEM THU SLOPE TRICK DA HOAN TAT XUAT SAC! ===" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_all_tests();

    return 0;
}
