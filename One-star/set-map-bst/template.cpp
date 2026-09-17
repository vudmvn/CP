/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Set, Map, Balanced BST & GNU PBDS Order Statistic Tree
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <chrono>
#include <cstdint>
#include <cassert>
#include <optional>

// Thư viện GNU C++ Policy-Based Data Structures
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// ============================================================================
// 1. TIỆN ÍCH CHUẨN CHO STD::SET & STD::MULTISET
// ============================================================================

namespace SetUtils {

    /**
     * @brief Xóa đúng 1 bản sao của giá trị val trong multiset (thay vì xóa sạch)
     * @return true nếu tìm thấy và xóa thành công, false nếu giá trị không tồn tại
     */
    template<typename T>
    bool eraseSingle(multiset<T>& ms, const T& val) {
        auto it = ms.find(val);
        if (it != ms.end()) {
            ms.erase(it);
            return true;
        }
        return false;
    }

    /**
     * @brief Tìm phần tử lớn nhất strictly nhỏ hơn val (< val)
     */
    template<typename T>
    optional<T> getStrictPredecessor(const set<T>& s, const T& val) {
        auto it = s.lower_bound(val);
        if (it == s.begin()) return nullopt;
        return *prev(it);
    }

    /**
     * @brief Tìm phần tử lớn nhất <= val
     */
    template<typename T>
    optional<T> getFloor(const set<T>& s, const T& val) {
        auto it = s.upper_bound(val);
        if (it == s.begin()) return nullopt;
        return *prev(it);
    }

    /**
     * @brief Tìm phần tử nhỏ nhất >= val
     */
    template<typename T>
    optional<T> getCeil(const set<T>& s, const T& val) {
        auto it = s.lower_bound(val);
        if (it == s.end()) return nullopt;
        return *it;
    }
}

// ============================================================================
// 2. CẤU TRÚC QUẢN LÝ CÁC ĐOẠN RỜI RẠC ĐỘNG (DYNAMIC INTERVAL SET)
// ============================================================================

class IntervalSet {
private:
    set<pair<long long, long long>> intervals; // Lưu các cặp [L, R] không giao nhau
    long long totalCoveredLength;

public:
    IntervalSet() : totalCoveredLength(0) {}

    /**
     * @brief Thêm đoạn [L, R] và gộp các đoạn giao nhau trong O(log N) amortized
     */
    void addInterval(long long L, long long R) {
        if (L > R) return;

        auto it = intervals.upper_bound({L, 2e18});
        if (it != intervals.begin() && prev(it)->second >= L) {
            --it;
        }

        while (it != intervals.end() && it->first <= R) {
            L = min(L, it->first);
            R = max(R, it->second);
            totalCoveredLength -= (it->second - it->first + 1);
            it = intervals.erase(it);
        }

        intervals.insert({L, R});
        totalCoveredLength += (R - L + 1);
    }

    /**
     * @brief Kiểm tra xem điểm x có nằm trong bất kỳ đoạn nào không
     */
    bool contains(long long x) const {
        auto it = intervals.upper_bound({x, 2e18});
        if (it == intervals.begin()) return false;
        --it;
        return (it->first <= x && x <= it->second);
    }

    long long getTotalCovered() const {
        return totalCoveredLength;
    }

    int countDisjointIntervals() const {
        return (int)intervals.size();
    }
};

// ============================================================================
// 3. GNU PBDS: ORDER STATISTIC TREE (ORDERED SET & MULTISET)
// ============================================================================

template<typename T>
using pbds_set = tree<
    T,
    null_type,
    less<T>,
    rb_tree_tag,
    tree_order_statistics_node_update
>;

/**
 * @brief Ordered MultiSet chuẩn trong CP (dùng pair<T, int> để tránh bug lower_bound)
 */
template<typename T>
class OrderedMultiSet {
private:
    tree<
        pair<T, int>,
        null_type,
        less<pair<T, int>>,
        rb_tree_tag,
        tree_order_statistics_node_update
    > ost;
    int timer = 0;
    map<T, vector<int>> idMap;

public:
    void insert(const T& val) {
        int id = ++timer;
        ost.insert({val, id});
        idMap[val].push_back(id);
    }

    bool erase(const T& val) {
        auto it = idMap.find(val);
        if (it == idMap.end() || it->second.empty()) return false;
        int id = it->second.back();
        it->second.pop_back();
        if (it->second.empty()) idMap.erase(it);
        ost.erase({val, id});
        return true;
    }

    int size() const {
        return ost.size();
    }

    bool empty() const {
        return ost.empty();
    }

    /**
     * @brief Đếm số phần tử strictly nhỏ hơn val trong O(log N)
     */
    int order_of_key(const T& val) const {
        return ost.order_of_key({val, -1});
    }

    /**
     * @brief Tìm giá trị của phần tử nhỏ thứ k (0-indexed) trong O(log N)
     */
    T find_by_order(int k) const {
        assert(k >= 0 && k < (int)ost.size());
        return ost.find_by_order(k)->first;
    }
};

// ============================================================================
// 4. CUSTOM HASH CHO UNORDERED_MAP CHỐNG ANTI-HASH ATTACK TRÊN CODEFORCES
// ============================================================================

struct SafeHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

template<typename K, typename V>
using SafeUnorderedMap = unordered_map<K, V, SafeHash>;

// ============================================================================
// 5. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    // 1. Test multiset erase single
    multiset<int> ms = {5, 5, 5, 2, 8};
    bool deleted = SetUtils::eraseSingle(ms, 5);
    assert(deleted == true);
    assert(ms.count(5) == 2);
    assert(ms.size() == 4);

    // 2. Test floor, ceil, predecessor
    set<int> s = {10, 20, 30, 40};
    assert(SetUtils::getFloor(s, 25).value() == 20);
    assert(SetUtils::getCeil(s, 25).value() == 30);
    assert(SetUtils::getStrictPredecessor(s, 20).value() == 10);
    assert(SetUtils::getStrictPredecessor(s, 10) == nullopt);

    // 3. Test Dynamic Interval Set
    IntervalSet iset;
    iset.addInterval(1, 5);
    iset.addInterval(10, 15);
    assert(iset.countDisjointIntervals() == 2);
    assert(iset.getTotalCovered() == 11); // [1,5] là 5, [10,15] là 6 -> tổng 11
    assert(iset.contains(3) == true);
    assert(iset.contains(7) == false);

    // Gộp [4, 12] nối liền [1, 5] và [10, 15] thành [1, 15]
    iset.addInterval(4, 12);
    assert(iset.countDisjointIntervals() == 1);
    assert(iset.getTotalCovered() == 15);
    assert(iset.contains(7) == true);

    // 4. Test PBDS Ordered Set
    pbds_set<int> ost;
    ost.insert(10);
    ost.insert(20);
    ost.insert(30);
    ost.insert(40);
    ost.insert(50);

    // find_by_order (0-indexed)
    assert(*ost.find_by_order(0) == 10);
    assert(*ost.find_by_order(2) == 30);
    assert(*ost.find_by_order(4) == 50);

    // order_of_key: số phần tử < x
    assert(ost.order_of_key(25) == 2); // {10, 20}
    assert(ost.order_of_key(10) == 0);
    assert(ost.order_of_key(100) == 5);

    // 5. Test Ordered MultiSet
    OrderedMultiSet<int> oms;
    oms.insert(5);
    oms.insert(5);
    oms.insert(10);
    oms.insert(20);
    assert(oms.size() == 4);
    assert(oms.order_of_key(10) == 2); // Hai số 5
    assert(oms.find_by_order(0) == 5);
    assert(oms.find_by_order(1) == 5);
    assert(oms.find_by_order(2) == 10);
    oms.erase(5);
    assert(oms.size() == 3);
    assert(oms.order_of_key(10) == 1);

    // 6. Test SafeUnorderedMap
    SafeUnorderedMap<long long, int> safe_map;
    safe_map[1000000000000LL] = 42;
    assert(safe_map[1000000000000LL] == 42);

    cout << "==> ALL SET/MAP & BALANCED BST SELF-TESTS PASSED SUCCESSFULLY!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
