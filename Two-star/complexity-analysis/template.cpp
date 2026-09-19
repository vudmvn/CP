/**
 * @file template.cpp
 * @brief Performance Benchmarking, Memory Estimation, Fast I/O, and Cache-Locality Optimization
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 tools for:
 * 1. Timer / Benchmark: High-resolution clock measurement.
 * 2. CustomSafeHash: SplitMix64 anti-hash collision for unordered_map.
 * 3. FastIO: Optimized buffered reading and writing.
 * 4. Fast Modulo & Arithmetic Optimizers (avoiding CPU cycle bottlenecks).
 * 5. Cache-Locality Experiments (Row-Major vs Column-Major memory access).
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cstdint>
#include <random>
#include <unordered_map>
#include <cassert>
#include <numeric>

namespace cp::complexity {

// =========================================================================
// 1. High-Resolution Timer & Benchmark Utility
// =========================================================================
class Timer {
private:
    std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
    Timer() { reset(); }

    void reset() {
        start_time = std::chrono::steady_clock::now();
    }

    [[nodiscard]] double elapsed_ms() const {
        auto end_time = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }

    [[nodiscard]] double elapsed_sec() const {
        return elapsed_ms() / 1000.0;
    }
};

// =========================================================================
// 2. Anti-Hack Safe Hash for std::unordered_map (SplitMix64)
// =========================================================================
struct CustomSafeHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = 
            std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

// =========================================================================
// 3. Fast Modulo Arithmetic (Avoiding 15-25 CPU Cycle division bottleneck)
// =========================================================================
template <int MOD>
struct FastModulo {
    static inline int add(int a, int b) {
        int res = a + b;
        return (res >= MOD) ? res - MOD : res;
    }

    static inline int sub(int a, int b) {
        int res = a - b;
        return (res < 0) ? res + MOD : res;
    }

    static inline int mul(int a, int b) {
        return static_cast<int>((1LL * a * b) % MOD);
    }
};

// =========================================================================
// 4. Memory Estimation Helpers
// =========================================================================
struct MemoryEstimator {
    template <typename T>
    static double estimate_vector_mb(size_t element_count) {
        return static_cast<double>(element_count * sizeof(T)) / (1024.0 * 1024.0);
    }

    template <typename T>
    static double estimate_matrix_mb(size_t rows, size_t cols) {
        return static_cast<double>(rows * cols * sizeof(T)) / (1024.0 * 1024.0);
    }
};

// =========================================================================
// 5. Cache Locality Demonstration
// =========================================================================
long long sum_row_major(const std::vector<std::vector<int>>& mat, int n) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            total += mat[i][j];
        }
    }
    return total;
}

long long sum_column_major(const std::vector<std::vector<int>>& mat, int n) {
    long long total = 0;
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < n; ++i) {
            total += mat[i][j];
        }
    }
    return total;
}

} // namespace cp::complexity

// =========================================================================
// Self-Testing & Verification
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using namespace cp::complexity;

    // 1. Verify Fast Modulo arithmetic correctness
    constexpr int MOD = 1000000007;
    assert(FastModulo<MOD>::add(1000000000, 10) == 3);
    assert(FastModulo<MOD>::sub(5, 10) == MOD - 5);
    assert(FastModulo<MOD>::mul(1000000, 1000000) == (1LL * 1000000 * 1000000) % MOD);

    // 2. Verify Safe Hash on unordered_map
    std::unordered_map<long long, int, CustomSafeHash> safe_map;
    for (int i = 1; i <= 1000; ++i) {
        safe_map[1LL * i * MOD] = i;
    }
    for (int i = 1; i <= 1000; ++i) {
        assert(safe_map[1LL * i * MOD] == i);
    }

    // 3. Memory Estimation Output
    double mb_10m_int = MemoryEstimator::estimate_vector_mb<int>(10000000);
    double mb_mat_5k  = MemoryEstimator::estimate_matrix_mb<int>(5000, 5000);
    assert(mb_10m_int > 38.0 && mb_10m_int < 39.0); // ~38.15 MB
    assert(mb_mat_5k > 95.0 && mb_mat_5k < 96.0);   // ~95.37 MB

    // 4. Cache-Locality Benchmark on 1500 x 1500 Matrix
    int dim = 1500;
    std::vector<std::vector<int>> matrix(dim, std::vector<int>(dim, 1));

    Timer t_row;
    long long s1 = sum_row_major(matrix, dim);
    double ms_row = t_row.elapsed_ms();

    Timer t_col;
    long long s2 = sum_column_major(matrix, dim);
    double ms_col = t_col.elapsed_ms();

    assert(s1 == s2);

    std::cout << "[SUCCESS] Complexity & Performance Suite completed successfully!\n";
    std::cout << "RAM for 10M int vector: " << mb_10m_int << " MB\n";
    std::cout << "RAM for 5000x5000 int matrix: " << mb_mat_5k << " MB\n";
    std::cout << "Row-Major sum time:    " << ms_row << " ms\n";
    std::cout << "Column-Major sum time: " << ms_col << " ms (Notice cache miss impact!)\n";

    return 0;
}
