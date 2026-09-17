/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Lũy thừa nhị phân, Nhân nhanh Modulo 10^18 & Nhân ma trận tối ưu DP
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cassert>

using namespace std;

// ============================================================================
// 1. NHÂN NHANH SỐ LỚN MODULO 10^18 & LŨY THỪA NHỊ PHÂN
// ============================================================================

namespace FastMath {

    /**
     * @brief Nhân 2 số nguyên 64-bit a * b mod m (m <= 10^18) bằng __int128 trong O(1)
     * Phương pháp khuyến nghị dùng trên GCC / Clang 64-bit
     */
    inline uint64_t mulMod128(uint64_t a, uint64_t b, uint64_t m) {
        return (uint64_t)((unsigned __int128)a * b % m);
    }

    /**
     * @brief Nhân nhanh modulo 10^18 trong O(1) bằng ảo thuật số thực long double
     * Không cần __int128, dùng được trên MSVC x86/x64
     */
    inline uint64_t mulModFloat(uint64_t a, uint64_t b, uint64_t m) {
        uint64_t q = (long double)a * b / m;
        int64_t res = (int64_t)(a * b - q * m) % (int64_t)m;
        if (res < 0) res += m;
        return res;
    }

    /**
     * @brief Nhân nhanh Ấn Độ (Russian Peasant) trong O(log b)
     * An toàn tuyệt đối trên mọi kiến trúc, không lo tràn số
     */
    uint64_t mulModBinary(uint64_t a, uint64_t b, uint64_t m) {
        uint64_t res = 0;
        a %= m;
        while (b > 0) {
            if (b & 1) res = (res + a) % m;
            a = (a + a) % m;
            b >>= 1;
        }
        return res;
    }

    /**
     * @brief Lũy thừa nhị phân a^b mod m với a, m <= 10^18 trong O(log b)
     */
    uint64_t powerMod(uint64_t a, uint64_t b, uint64_t m) {
        uint64_t res = 1 % m;
        a %= m;
        while (b > 0) {
            if (b & 1) res = mulMod128(res, a, m);
            a = mulMod128(a, a, m);
            b >>= 1;
        }
        return res;
    }

} // namespace FastMath

// ============================================================================
// 2. CẤU TRÚC MA TRẬN ĐẠI SỐ CHUẨN (STANDARD MATRIX MODULO M)
// ============================================================================

template<int SZ>
struct Matrix {
    int n, m;
    long long mat[SZ][SZ];
    long long mod;

    Matrix(int _n = SZ, int _m = SZ, long long _mod = 1000000007LL) 
        : n(_n), m(_m), mod(_mod) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                mat[i][j] = 0;
    }

    // Tạo ma trận đơn vị Identity I
    static Matrix identity(int sz, long long _mod = 1000000007LL) {
        Matrix res(sz, sz, _mod);
        for (int i = 0; i < sz; ++i) res.mat[i][i] = 1;
        return res;
    }

    // Nhân hai ma trận A * B trong O(N^3) với thứ tự duyệt i-k-j tối ưu CPU Cache
    Matrix operator*(const Matrix& other) const {
        assert(m == other.n);
        Matrix res(n, other.m, mod);
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < m; ++k) {
                if (mat[i][k] == 0) continue;
                for (int j = 0; j < other.m; ++j) {
                    res.mat[i][j] = (res.mat[i][j] + (__int128)mat[i][k] * other.mat[k][j]) % mod;
                }
            }
        }
        return res;
    }

    // Lũy thừa ma trận M^exp mod MOD trong O(SZ^3 * log(exp))
    Matrix power(long long exp) const {
        assert(n == m);
        Matrix res = Matrix::identity(n, mod);
        Matrix base = *this;
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }
};

// ============================================================================
// 3. VÀNH NHIỆT ĐỚI (TROPICAL MIN-PLUS SEMIRING MATRIX)
// ============================================================================

template<int SZ>
struct TropicalMatrix {
    static constexpr long long INF = 4e18; // An toàn tránh tràn số khi cộng INF + weight
    int n;
    long long mat[SZ][SZ];

    TropicalMatrix(int _n = SZ) : n(_n) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                mat[i][j] = INF;
    }

    // Ma trận đơn vị Min-Plus: Đường chéo chính bằng 0, các ô còn lại là INF
    static TropicalMatrix identity(int sz) {
        TropicalMatrix res(sz);
        for (int i = 0; i < sz; ++i) res.mat[i][i] = 0;
        return res;
    }

    // Phép nhân Min-Plus: C[i][j] = min_k (A[i][k] + B[k][j])
    TropicalMatrix operator*(const TropicalMatrix& other) const {
        TropicalMatrix res(n);
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                if (mat[i][k] == INF) continue;
                for (int j = 0; j < n; ++j) {
                    if (other.mat[k][j] == INF) continue;
                    res.mat[i][j] = min(res.mat[i][j], mat[i][k] + other.mat[k][j]);
                }
            }
        }
        return res;
    }

    // Lũy thừa ma trận Min-Plus trong O(SZ^3 * log(exp))
    TropicalMatrix power(long long exp) const {
        TropicalMatrix res = TropicalMatrix::identity(n);
        TropicalMatrix base = *this;
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }
};

// ============================================================================
// 4. ỨNG DỤNG MẪU TRONG THI ĐẤU CP
// ============================================================================

namespace MatrixDP {

    /**
     * @brief Tính số Fibonacci thứ N (F_0 = 0, F_1 = 1) modulo MOD trong O(log N)
     */
    long long getFibonacci(long long n, long long mod = 1000000007LL) {
        if (n <= 0) return 0;
        if (n == 1) return 1;

        Matrix<2> T(2, 2, mod);
        T.mat[0][0] = 1; T.mat[0][1] = 1;
        T.mat[1][0] = 1; T.mat[1][1] = 0;

        // V_n = T^(n-1) * V_1 với V_1 = [F_1, F_0]^T = [1, 0]^T
        Matrix<2> Tn = T.power(n - 1);
        return Tn.mat[0][0]; // Tn.mat[0][0] * 1 + Tn.mat[0][1] * 0
    }

    /**
     * @brief Giải hệ thức truy hồi tuyến tính bậc K:
     * A_n = c_1 * A_{n-1} + c_2 * A_{n-2} + ... + c_K * A_{n-K}
     * @param coeffs Mảng K hệ số [c_1, c_2, ..., c_K]
     * @param init_vals Mảng K giá trị khởi tạo [A_1, A_2, ..., A_K]
     * @param n Chỉ số cần tính (1-indexed)
     */
    long long solveLinearRecurrence(const vector<long long>& coeffs, 
                                   const vector<long long>& init_vals, 
                                   long long n, long long mod = 1000000007LL) {
        int K = coeffs.size();
        if (n <= K) return init_vals[n - 1] % mod;

        Matrix<50> T(K, K, mod);
        // Hàng 0 chứa các hệ số c_1, c_2, ..., c_K
        for (int j = 0; j < K; ++j) T.mat[0][j] = (coeffs[j] % mod + mod) % mod;
        // Các hàng tiếp theo dịch chuyển giá trị
        for (int i = 1; i < K; ++i) T.mat[i][i - 1] = 1;

        Matrix<50> Tn = T.power(n - K);
        long long ans = 0;
        // Nhân Tn với vector khởi tạo [A_K, A_{K-1}, ..., A_1]^T
        for (int j = 0; j < K; ++j) {
            long long val = init_vals[K - 1 - j] % mod;
            ans = (ans + (__int128)Tn.mat[0][j] * val) % mod;
        }
        return ans;
    }

} // namespace MatrixDP

// ============================================================================
// 5. CHƯƠNG TRÌNH KIỂM THỬ (TEST DRIVER)
// ============================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== KIỂM THỬ NHÂN NHANH 10^18 & NHÂN MA TRẬN C++20 ===\n\n";

    // 1. Kiểm thử nhân số lớn > 10^18 modulo 10^18
    uint64_t a = 123456789012345678ULL;
    uint64_t b = 987654321098765432ULL;
    uint64_t mod = 1000000000000000003ULL; // Số nguyên tố xấp xỉ 10^18

    uint64_t mul_128   = FastMath::mulMod128(a, b, mod);
    uint64_t mul_float = FastMath::mulModFloat(a, b, mod);
    uint64_t mul_bin   = FastMath::mulModBinary(a, b, mod);

    cout << "1. Kiểm thử nhân 2 số ~ 10^18 mod 10^18:\n";
    cout << "- Cách __int128: " << mul_128 << "\n";
    cout << "- Cách Float:    " << mul_float << "\n";
    cout << "- Cách Binary:   " << mul_bin << "\n";
    assert(mul_128 == mul_float && mul_128 == mul_bin);
    cout << "=> Kết quả 3 phương pháp hoàn toàn khớp nhau!\n\n";

    // 2. Kiểm thử Fibonacci lớn bằng Lũy thừa ma trận
    long long n_fib = 10;
    cout << "2. Số Fibonacci thứ " << n_fib << ": " 
         << MatrixDP::getFibonacci(n_fib) << " (kỳ vọng: 55)\n";
    
    long long big_n = 1000000000000000000LL; // 10^18
    cout << "Số Fibonacci thứ 10^18 mod 10^9+7: " 
         << MatrixDP::getFibonacci(big_n) << "\n\n";

    // 3. Kiểm thử Tropical Min-Plus Matrix: Đường đi ngắn nhất đúng K bước
    // Đồ thị 3 đỉnh: 1 -> 2 (trọng số 3), 2 -> 3 (trọng số 4), 3 -> 1 (trọng số 5)
    TropicalMatrix<3> W(3);
    W.mat[0][1] = 3;
    W.mat[1][2] = 4;
    W.mat[2][0] = 5;

    // Đường đi từ 1 -> 3 qua đúng 2 cạnh: 1 -> 2 -> 3 = 3 + 4 = 7
    TropicalMatrix<3> W2 = W.power(2);
    cout << "3. Đường đi ngắn nhất từ 1 -> 3 qua đúng 2 cạnh: " 
         << W2.mat[0][2] << " (kỳ vọng: 7)\n";

    // Đường đi từ 1 -> 3 qua đúng 5 cạnh: 1->2->3->1->2->3 = 3+4+5+3+4 = 19
    TropicalMatrix<3> W5 = W.power(5);
    cout << "Đường đi ngắn nhất từ 1 -> 3 qua đúng 5 cạnh: " 
         << W5.mat[0][2] << " (kỳ vọng: 19)\n";

    return 0;
}
