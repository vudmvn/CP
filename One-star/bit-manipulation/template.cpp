/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Phép toán Bit (Bitwise Operations) & Cấu trúc Dữ liệu Bitset
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <bitset>
#include <bit>
#include <cstdint>
#include <functional>

using namespace std;

namespace BitUtils {

    // ==========================================
    // 1. CÁC THAO TÁC CƠ BẢN TRÊN TỪNG BIT (64-bit)
    // ==========================================

    /**
     * @brief Kiểm tra bit thứ k của x có bằng 1 hay không
     */
    inline bool checkBit(uint64_t x, int k) {
        return (x >> k) & 1ULL;
    }

    /**
     * @brief Bật bit thứ k của x lên 1
     */
    inline uint64_t setBit(uint64_t x, int k) {
        return x | (1ULL << k);
    }

    /**
     * @brief Tắt bit thứ k của x về 0
     */
    inline uint64_t clearBit(uint64_t x, int k) {
        return x & ~(1ULL << k);
    }

    /**
     * @brief Đảo bit thứ k của x (0 -> 1, 1 -> 0)
     */
    inline uint64_t toggleBit(uint64_t x, int k) {
        return x ^ (1ULL << k);
    }

    /**
     * @brief Lấy bit 1 thấp nhất (Lowest Set Bit - LSB)
     * Trả về giá trị 2^k tương ứng với bit 1 nhỏ nhất của x (ví dụ: x = 12 (1100_2) -> trả về 4 (0100_2))
     */
    inline uint64_t lowestSetBit(uint64_t x) {
        return x & (-x);
    }

    /**
     * @brief Xóa bit 1 thấp nhất của x (Thuật toán Brian Kernighan)
     */
    inline uint64_t clearLowestSetBit(uint64_t x) {
        return x & (x - 1ULL);
    }

    /**
     * @brief Kiểm tra x có phải là lũy thừa của 2 hay không (2^k)
     */
    inline bool isPowerOfTwo(uint64_t x) {
        return x > 0 && (x & (x - 1ULL)) == 0;
    }

    // ==========================================
    // 2. CÁC HÀM TỐI ƯU PHẦN CỨNG (AN TOÀN TRÁNH UB KHI X = 0)
    // ==========================================

    /**
     * @brief Đếm số bit 1 trong biểu diễn nhị phân 64-bit
     */
    inline int popcount(uint64_t x) {
        return std::popcount(x); // Chuẩn C++20 (hoặc __builtin_popcountll(x))
    }

    /**
     * @brief Đếm số bit 0 ở đầu tính từ MSB (Count Leading Zeros)
     * Đã xử lý an toàn: Trả về 64 nếu x == 0 (tránh Undefined Behavior của __builtin_clzll)
     */
    inline int countLeadingZeros(uint64_t x) {
        return (x == 0) ? 64 : std::countl_zero(x);
    }

    /**
     * @brief Đếm số bit 0 ở đuôi tính từ LSB (Count Trailing Zeros)
     * Đã xử lý an toàn: Trả về 64 nếu x == 0 (tránh Undefined Behavior của __builtin_ctzll)
     */
    inline int countTrailingZeros(uint64_t x) {
        return (x == 0) ? 64 : std::countr_zero(x);
    }

    /**
     * @brief Tính sàn logarit cơ số 2: floor(log2(x))
     * Vị trí của bit 1 cao nhất (MSB) tính từ 0
     */
    inline int log2Floor(uint64_t x) {
        if (x == 0) return -1;
        return 63 - std::countl_zero(x);
    }

    // ==========================================
    // 3. DUYỆT TẬP CON CỦA BITMASK (SUBMASK ENUMERATION)
    // ==========================================

    /**
     * @brief Duyệt toàn bộ các tập con (submasks) của một mask M theo thứ tự giảm dần
     * Độ phức tạp tổng khi duyệt mọi mask độ dài N là O(3^N)
     */
    void forEachSubmask(uint64_t mask, const function<void(uint64_t)>& callback) {
        for (uint64_t sub = mask; sub > 0; sub = (sub - 1ULL) & mask) {
            callback(sub);
        }
        callback(0); // Tập con rỗng
    }

} // namespace BitUtils

// ==========================================
// 4. CÁC BÀI TOÁN KINH ĐIỂN VỚI std::bitset
// ==========================================

namespace BitsetApplications {

    const int MAX_WEIGHT = 1000005;

    /**
     * @brief Giải bài toán Knapsack / Subset Sum trong O(N * W / 64)
     * @param weights Danh sách trọng số các phần tử
     * @param maxW Trọng số tối đa cần kiểm tra
     * @return bitset đại diện cho tất cả các tổng có thể tạo ra
     */
    bitset<MAX_WEIGHT> subsetSum(const vector<int>& weights, int maxW) {
        bitset<MAX_WEIGHT> dp;
        dp.reset();
        dp[0] = 1; // Tổng 0 luôn tạo được từ tập rỗng
        for (int w : weights) {
            if (w <= maxW) {
                dp |= (dp << w); // Dịch bitset song song 64-bit
            }
        }
        return dp;
    }

    const int MAX_V = 2005;

    /**
     * @brief Tìm bao đóng bắc cầu (Transitive Closure / Reachability) của đồ thị có hướng
     * Độ phức tạp: O(V^3 / 64)
     * @param V Số đỉnh của đồ thị (1-indexed)
     * @param adj Ma trận kề dưới dạng mảng bitset
     */
    void transitiveClosure(int V, vector<bitset<MAX_V>>& adj) {
        for (int k = 1; k <= V; ++k) {
            for (int i = 1; i <= V; ++i) {
                if (adj[i].test(k)) {
                    adj[i] |= adj[k];
                }
            }
        }
    }

    /**
     * @brief Đếm số tam giác trong đồ thị vô hướng trong O(V^3 / 64)
     */
    long long countTriangles(int V, const vector<bitset<MAX_V>>& adj) {
        long long triangles_times_3 = 0;
        for (int u = 1; u <= V; ++u) {
            for (int v = u + 1; v <= V; ++v) {
                if (adj[u].test(v)) {
                    // Đếm số hàng xóm chung của cả u và v bằng phép AND bitset
                    triangles_times_3 += (adj[u] & adj[v]).count();
                }
            }
        }
        return triangles_times_3 / 3;
    }

} // namespace BitsetApplications

// ==========================================
// 5. CHƯƠNG TRÌNH KIỂM THỬ MẪU (TEST DRIVER)
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout << "=== KIỂM THỬ BIT UTILS & BITSET C++20 ===\n\n";

    // 1. Kiểm thử các hàm Bit cơ bản
    uint64_t x = 12; // 12 = 1100_2
    cout << "Số x = 12 (1100_2):\n";
    cout << "- Bit 0: " << BitUtils::checkBit(x, 0) << " (kỳ vọng: 0)\n";
    cout << "- Bit 2: " << BitUtils::checkBit(x, 2) << " (kỳ vọng: 1)\n";
    cout << "- Bit 3: " << BitUtils::checkBit(x, 3) << " (kỳ vọng: 1)\n";
    cout << "- Lowest Set Bit x & (-x): " << BitUtils::lowestSetBit(x) << " (kỳ vọng: 4)\n";
    cout << "- Xóa bit thấp nhất x & (x - 1): " << BitUtils::clearLowestSetBit(x) << " (kỳ vọng: 8)\n";
    cout << "- Có phải lũy thừa của 2? " << (BitUtils::isPowerOfTwo(x) ? "YES" : "NO") << "\n";
    cout << "- 16 có phải lũy thừa của 2? " << (BitUtils::isPowerOfTwo(16) ? "YES" : "NO") << "\n\n";

    // 2. Kiểm thử Hardware Intrinsics C++20
    uint64_t val = 42; // 42 = 101010_2 (3 bit 1)
    cout << "Số val = 42 (101010_2):\n";
    cout << "- Số bit 1 (popcount): " << BitUtils::popcount(val) << " (kỳ vọng: 3)\n";
    cout << "- Trailing zeros (ctz): " << BitUtils::countTrailingZeros(val) << " (kỳ vọng: 1)\n";
    cout << "- floor(log2(42)): " << BitUtils::log2Floor(val) << " (kỳ vọng: 5)\n\n";

    // 3. Kiểm thử Duyệt Submask
    uint64_t mask = 10; // 10 = 1010_2 (các bit 1 và 3)
    cout << "Duyệt toàn bộ submask của mask = 10 (1010_2):\n";
    BitUtils::forEachSubmask(mask, [](uint64_t sub) {
        cout << sub << " ";
    });
    cout << "\n(Kỳ vọng: 10, 8, 2, 0)\n\n";

    // 4. Kiểm thử Knapsack Subset Sum bằng Bitset
    vector<int> weights = {3, 5, 8};
    int maxW = 20;
    auto dp = BitsetApplications::subsetSum(weights, maxW);
    cout << "Các tổng trọng số có thể tạo ra từ {3, 5, 8} <= 20:\n";
    for (int w = 0; w <= maxW; ++w) {
        if (dp[w]) cout << w << " ";
    }
    cout << "\n(Kỳ vọng: 0 3 5 8 11 13 16)\n\n";

    // 5. Kiểm thử Đếm tam giác bằng Bitset
    int V = 4;
    vector<bitset<BitsetApplications::MAX_V>> adj(V + 1);
    // Tạo đồ thị K4 (đồ thị đầy đủ 4 đỉnh: 1-2, 1-3, 1-4, 2-3, 2-4, 3-4) -> có 4 tam giác C(4, 3) = 4
    for (int i = 1; i <= V; ++i) {
        for (int j = 1; j <= V; ++j) {
            if (i != j) adj[i].set(j);
        }
    }
    long long num_triangles = BitsetApplications::countTriangles(V, adj);
    cout << "Số tam giác trong đồ thị K4: " << num_triangles << " (kỳ vọng: 4)\n";

    return 0;
}
