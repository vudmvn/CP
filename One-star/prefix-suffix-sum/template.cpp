/**
 * @file template.cpp
 * @brief Chuẩn C++20 Template cho Mảng tiền tố, Hậu tố & Mảng hiệu trong Competitive Programming.
 * @author Duc-Minh Vu (SLSCM Lab - Faculty of Data Science and Artificial Intelligence, NEU)
 * @copyright Copyright (c) 2026 Duc-Minh Vu. All rights reserved.
 * National Economics University (NEU) - Faculty of Data Science and Artificial Intelligence (FDA)
 * SLSCM Lab (Smart Logistics and Supply Chain Management)
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

using namespace std;

// ==========================================
// 1. MẢNG CỘNG DỒN 1 CHIỀU (1D PREFIX SUM)
// ==========================================

struct PrefixSum1D {
    int n;
    vector<long long> pref;

    PrefixSum1D(int _n = 0) { init(_n); }
    template <typename T>
    PrefixSum1D(const vector<T>& a) { build(a); }

    void init(int _n) {
        n = _n;
        pref.assign(n + 1, 0);
    }

    template <typename T>
    void build(const vector<T>& a) {
        n = a.size();
        pref.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            pref[i] = pref[i - 1] + a[i - 1];
        }
    }

    // Truy vấn tổng đoạn [l, r] (1-indexed) trong O(1)
    long long query(int l, int r) const {
        assert(1 <= l && l <= r && r <= n);
        return pref[r] - pref[l - 1];
    }
};

// ==========================================
// 2. MẢNG HIỆU 1 CHIỀU (1D DIFFERENCE ARRAY)
// ==========================================

struct DiffArray1D {
    int n;
    vector<long long> diff;

    DiffArray1D(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        diff.assign(n + 2, 0);
    }

    // Cộng giá trị v vào đoạn [l, r] (1-indexed) trong O(1)
    void add(int l, int r, long long v) {
        assert(1 <= l && l <= r && r <= n);
        diff[l] += v;
        diff[r + 1] -= v;
    }

    // Khôi phục mảng ban đầu sau khi thực hiện tất cả các cập nhật trong O(N)
    vector<long long> build() const {
        vector<long long> res(n + 1, 0);
        long long cur = 0;
        for (int i = 1; i <= n; ++i) {
            cur += diff[i];
            res[i] = cur;
        }
        return res;
    }
};

// ==========================================
// 3. MẢNG CỘNG DỒN 2 CHIỀU (2D PREFIX SUM)
// ==========================================

struct PrefixSum2D {
    int r, c;
    vector<vector<long long>> pref;

    PrefixSum2D(int _r = 0, int _c = 0) { init(_r, _c); }
    template <typename T>
    PrefixSum2D(const vector<vector<T>>& a) { build(a); }

    void init(int _r, int _c) {
        r = _r;
        c = _c;
        pref.assign(r + 1, vector<long long>(c + 1, 0));
    }

    template <typename T>
    void build(const vector<vector<T>>& a) {
        r = a.size();
        c = a[0].size();
        pref.assign(r + 1, vector<long long>(c + 1, 0));
        for (int i = 1; i <= r; ++i) {
            for (int j = 1; j <= c; ++j) {
                pref[i][j] = pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1] + a[i - 1][j - 1];
            }
        }
    }

    // Truy vấn tổng hình chữ nhật con [r1, c1] đến [r2, c2] (1-indexed) trong O(1)
    long long query(int r1, int c1, int r2, int c2) const {
        assert(1 <= r1 && r1 <= r2 && r2 <= r);
        assert(1 <= c1 && c1 <= c2 && c2 <= c);
        return pref[r2][c2] - pref[r1 - 1][c2] - pref[r2][c1 - 1] + pref[r1 - 1][c1 - 1];
    }
};

// ==========================================
// 4. MẢNG HIỆU 2 CHIỀU (2D DIFFERENCE ARRAY)
// ==========================================

struct DiffArray2D {
    int r, c;
    vector<vector<long long>> diff;

    DiffArray2D(int _r = 0, int _c = 0) { init(_r, _c); }

    void init(int _r, int _c) {
        r = _r;
        c = _c;
        diff.assign(r + 2, vector<long long>(c + 2, 0));
    }

    // Cộng giá trị v vào hình chữ nhật con [r1, c1] đến [r2, c2] (1-indexed) trong O(1)
    void add(int r1, int c1, int r2, int c2, long long v) {
        assert(1 <= r1 && r1 <= r2 && r2 <= r);
        assert(1 <= c1 && c1 <= c2 && c2 <= c);
        diff[r1][c1] += v;
        diff[r1][c2 + 1] -= v;
        diff[r2 + 1][c1] -= v;
        diff[r2 + 1][c2 + 1] += v;
    }

    // Khôi phục toàn bộ ma trận sau tất cả các cập nhật trong O(R * C)
    vector<vector<long long>> build() const {
        vector<vector<long long>> res(r + 1, vector<long long>(c + 1, 0));
        for (int i = 1; i <= r; ++i) {
            for (int j = 1; j <= c; ++j) {
                res[i][j] = res[i - 1][j] + res[i][j - 1] - res[i - 1][j - 1] + diff[i][j];
            }
        }
        return res;
    }
};

// ==========================================
// 5. PREFIX & SUFFIX GCD (LOẠI BỎ 1 PHẦN TỬ)
// ==========================================

struct PrefixSuffixGCD {
    int n;
    vector<long long> pref, suff;

    template <typename T>
    PrefixSuffixGCD(const vector<T>& a) {
        n = a.size();
        pref.assign(n + 2, 0);
        suff.assign(n + 2, 0);

        for (int i = 1; i <= n; ++i) pref[i] = std::gcd(pref[i - 1], a[i - 1]);
        for (int i = n; i >= 1; --i) suff[i] = std::gcd(suff[i + 1], a[i - 1]);
    }

    // Tính GCD của toàn bộ mảng khi loại bỏ phần tử thứ idx (1-indexed) trong O(1)
    long long query_without(int idx) const {
        assert(1 <= idx && idx <= n);
        return std::gcd(pref[idx - 1], suff[idx + 1]);
    }
};

// ==========================================
// 6. MAIN DEMO
// ==========================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Demo 1D Prefix Sum
    cout << "=== 1. DEMO 1D PREFIX SUM ===\n";
    vector<int> a = {3, 2, 4, 5, 1, 1, 5, 3}; // n = 8
    PrefixSum1D p1d(a);
    cout << "Tong doan [2, 4] = " << p1d.query(2, 4) << " (ky vong: 2+4+5 = 11)\n";
    cout << "Tong doan [1, 8] = " << p1d.query(1, 8) << " (ky vong: 24)\n\n";

    // 2. Demo 1D Difference Array
    cout << "=== 2. DEMO 1D DIFFERENCE ARRAY ===\n";
    DiffArray1D d1d(5);
    d1d.add(1, 3, 2); // +2 vao [1, 3] -> [2, 2, 2, 0, 0]
    d1d.add(2, 5, 3); // +3 vao [2, 5] -> [2, 5, 5, 3, 3]
    auto res1d = d1d.build();
    cout << "Mang ket qua sau cac cap nhat: ";
    for (int i = 1; i <= 5; ++i) cout << res1d[i] << " ";
    cout << "\n\n";

    // 3. Demo 2D Prefix Sum
    cout << "=== 3. DEMO 2D PREFIX SUM ===\n";
    vector<vector<int>> mat = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    PrefixSum2D p2d(mat);
    cout << "Tong hcn con [2, 2] den [3, 3] = " << p2d.query(2, 2, 3, 3) 
         << " (ky vong: 5+6+8+9 = 28)\n\n";

    // 4. Demo Prefix & Suffix GCD
    cout << "=== 4. DEMO PREFIX & SUFFIX GCD ===\n";
    vector<long long> gcd_arr = {12, 18, 24, 60};
    PrefixSuffixGCD ps_gcd(gcd_arr);
    for (int i = 1; i <= 4; ++i) {
        cout << "Loai bo phan tu vi tri " << i << " (" << gcd_arr[i - 1] 
             << ") -> GCD con lai = " << ps_gcd.query_without(i) << "\n";
    }

    return 0;
}
