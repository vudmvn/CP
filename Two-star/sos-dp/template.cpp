/**
 * @file template.cpp
 * @brief Production-Ready Sum Over Subsets DP (SOS DP / Fast Zeta & Möbius Transform) Frameworks in C++20
 * @author Duc-Minh Vu
 * @institution SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), National Economics University (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * 
 * Standard C++20 implementations:
 * 1. SOS_Subsets: Fast Zeta Transform on Subsets (F[mask] = sum_{sub in mask} A[sub]) and its Möbius inverse.
 * 2. SOS_Supersets: Fast Zeta Transform on Supersets (G[mask] = sum_{mask in super} A[super]) and its Möbius inverse.
 * 3. SOS_Top2: Semiring extension maintaining top 2 elements for disjoint pairs (x & y = 0) with max sum.
 * 4. BitwiseConvolution: Bitwise OR and AND convolution using SOS DP in O(N * 2^N).
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>

namespace cp::sos {

// =========================================================================
// 1. Fast Zeta & Möbius Transform on Subsets (SOS DP Subsets)
// =========================================================================

/**
 * @brief Forward Zeta Transform on Subsets: F[mask] = sum_{sub in mask} A[sub]
 * @param dp 1D array of size 2^N
 * @param n Number of bits (dimension of hypercube)
 */
template <typename T>
void zeta_subsets(std::vector<T>& dp, int n) {
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (mask & (1 << i)) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
}

/**
 * @brief Inverse Möbius Transform on Subsets: A[mask] = sum_{sub in mask} (-1)^{|mask \ sub|} F[sub]
 * @param dp 1D array of size 2^N
 * @param n Number of bits
 */
template <typename T>
void mobius_subsets(std::vector<T>& dp, int n) {
    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (mask & (1 << i)) {
                dp[mask] -= dp[mask ^ (1 << i)];
            }
        }
    }
}

// =========================================================================
// 2. Fast Zeta & Möbius Transform on Supersets (SOS DP Supersets)
// =========================================================================

/**
 * @brief Forward Zeta Transform on Supersets: G[mask] = sum_{mask in super} A[super]
 * @param dp 1D array of size 2^N
 * @param n Number of bits
 */
template <typename T>
void zeta_supersets(std::vector<T>& dp, int n) {
    for (int i = 0; i < n; ++i) {
        for (int mask = (1 << n) - 1; mask >= 0; --mask) {
            if (!(mask & (1 << i))) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
}

/**
 * @brief Inverse Möbius Transform on Supersets: A[mask] = sum_{mask in super} (-1)^{|super \ mask|} G[super]
 * @param dp 1D array of size 2^N
 * @param n Number of bits
 */
template <typename T>
void mobius_supersets(std::vector<T>& dp, int n) {
    for (int i = 0; i < n; ++i) {
        for (int mask = (1 << n) - 1; mask >= 0; --mask) {
            if (!(mask & (1 << i))) {
                dp[mask] -= dp[mask ^ (1 << i)];
            }
        }
    }
}

// =========================================================================
// 3. Semiring Extension: Top-2 Elements for Disjoint Mask Pairing
// =========================================================================
struct Top2 {
    int max1 = -1, id1 = -1;
    int max2 = -1, id2 = -1;

    void insert(int val, int id) {
        if (id == -1) return;
        if (val > max1) {
            if (id != id1) {
                max2 = max1;
                id2 = id1;
            }
            max1 = val;
            id1 = id;
        } else if (val > max2 && id != id1) {
            max2 = val;
            id2 = id;
        }
    }

    void merge(const Top2& other) {
        if (other.id1 != -1) insert(other.max1, other.id1);
        if (other.id2 != -1) insert(other.max2, other.id2);
    }
};

/**
 * @brief Compute Top-2 elements for each submask in O(N * 2^N)
 * @param a Input values for each mask
 * @param n Number of bits
 * @return std::vector<Top2> where result[mask] contains top 2 values across all sub in mask
 */
std::vector<Top2> sos_top2_subsets(const std::vector<int>& a, int n) {
    int total_masks = 1 << n;
    std::vector<Top2> dp(total_masks);
    for (int mask = 0; mask < total_masks; ++mask) {
        dp[mask].insert(a[mask], mask);
    }

    for (int i = 0; i < n; ++i) {
        for (int mask = 0; mask < total_masks; ++mask) {
            if (mask & (1 << i)) {
                dp[mask].merge(dp[mask ^ (1 << i)]);
            }
        }
    }
    return dp;
}

// =========================================================================
// 4. Bitwise Convolution (OR and AND Convolution) via SOS DP
// =========================================================================

/**
 * @brief Bitwise OR Convolution: C[k] = sum_{i | j = k} A[i] * B[j]
 * @param a First operand array of size 2^N
 * @param b Second operand array of size 2^N
 * @param n Number of bits
 * @return std::vector<long long> Result array C of size 2^N
 */
std::vector<long long> or_convolution(std::vector<long long> a, std::vector<long long> b, int n) {
    zeta_subsets(a, n);
    zeta_subsets(b, n);
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] *= b[i];
    }
    mobius_subsets(a, n);
    return a;
}

/**
 * @brief Bitwise AND Convolution: C[k] = sum_{i & j = k} A[i] * B[j]
 * @param a First operand array of size 2^N
 * @param b Second operand array of size 2^N
 * @param n Number of bits
 * @return std::vector<long long> Result array C of size 2^N
 */
std::vector<long long> and_convolution(std::vector<long long> a, std::vector<long long> b, int n) {
    zeta_supersets(a, n);
    zeta_supersets(b, n);
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] *= b[i];
    }
    mobius_supersets(a, n);
    return a;
}

} // namespace cp::sos

// =========================================================================
// Verification & Unit Tests
// =========================================================================
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Test 1: Subsets Zeta & Möbius Inversion Roundtrip
    // N = 3 (8 masks: 0..7)
    // A = [1, 2, 4, 8, 16, 32, 64, 128]
    {
        const int n = 3;
        std::vector<long long> a = {1, 2, 4, 8, 16, 32, 64, 128};
        std::vector<long long> original = a;

        // Zeta transform
        cp::sos::zeta_subsets(a, n);

        // Mask 0 (000): sum is a[0] = 1
        assert(a[0] == 1);
        // Mask 3 (011): sum of subsets {000, 001, 010, 011} = 1 + 2 + 4 + 8 = 15
        assert(a[3] == 15);
        // Mask 7 (111): sum of all 8 elements = 1+2+4+8+16+32+64+128 = 255
        assert(a[7] == 255);

        // Möbius inverse should restore original array exactly
        cp::sos::mobius_subsets(a, n);
        for (int mask = 0; mask < (1 << n); ++mask) {
            assert(a[mask] == original[mask]);
        }
    }

    // Test 2: Supersets Zeta & Möbius Inversion Roundtrip
    {
        const int n = 3;
        std::vector<long long> a = {1, 2, 4, 8, 16, 32, 64, 128};
        std::vector<long long> original = a;

        cp::sos::zeta_supersets(a, n);

        // Mask 7 (111): only superset is 7 itself -> 128
        assert(a[7] == 128);
        // Mask 0 (000): every mask is superset -> sum of all = 255
        assert(a[0] == 255);

        // Möbius inverse restores original array
        cp::sos::mobius_supersets(a, n);
        for (int mask = 0; mask < (1 << n); ++mask) {
            assert(a[mask] == original[mask]);
        }
    }

    // Test 3: SOS Top-2 for Disjoint Masks
    // Masks:
    // a[1] (001) = 10
    // a[2] (010) = 20
    // a[4] (100) = 30
    // a[6] (110) = 40
    // All others 0
    {
        const int n = 3;
        std::vector<int> a(8, 0);
        a[1] = 10;
        a[2] = 20;
        a[4] = 30;
        a[6] = 40;

        auto top2 = cp::sos::sos_top2_subsets(a, n);

        // Subsets of 7 (111): best values are 40 (id 6) and 30 (id 4)
        assert(top2[7].max1 == 40 && top2[7].id1 == 6);
        assert(top2[7].max2 == 30 && top2[7].id2 == 4);

        // Find max disjoint pair: for each mask x, complement is ~x = 7 ^ x
        // x = 1 (001): complement is 6 (110). Best in complement is a[6] = 40. Sum = 10 + 40 = 50.
        int max_disjoint_sum = 0;
        for (int mask = 0; mask < 8; ++mask) {
            int comp = 7 ^ mask;
            if (top2[comp].id1 != -1) {
                max_disjoint_sum = std::max(max_disjoint_sum, a[mask] + top2[comp].max1);
            }
        }
        assert(max_disjoint_sum == 50); // a[1] + a[6] = 10 + 40 = 50
    }

    // Test 4: OR Convolution Verification
    // A = [1, 2], B = [3, 4] for n = 1
    // OR table:
    // 0 | 0 = 0 -> A[0]*B[0] = 1*3 = 3
    // 0 | 1 = 1 -> A[0]*B[1] = 1*4 = 4
    // 1 | 0 = 1 -> A[1]*B[0] = 2*3 = 6
    // 1 | 1 = 1 -> A[1]*B[1] = 2*4 = 8
    // Result C[0] = 3, C[1] = 4 + 6 + 8 = 18
    {
        const int n = 1;
        std::vector<long long> a = {1, 2};
        std::vector<long long> b = {3, 4};
        auto c = cp::sos::or_convolution(a, b, n);
        assert(c[0] == 3);
        assert(c[1] == 18);
    }

    std::cout << "SOS DP & Fast Zeta/Mobius Templates: 100% assertions PASSED!\n";
    return 0;
}
