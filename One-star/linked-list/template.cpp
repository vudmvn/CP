/**
 * @file template.cpp
 * @brief Thư viện mẫu chuẩn C++20: Danh sách Liên kết & Cấu trúc Dữ liệu Động (Linked List)
 * @author Duc-Minh Vu (Vũ Đức Minh)
 * @organization SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)
 * @details Được hỗ trợ và soạn thảo bởi Agentic AI tool.
 *          Bản quyền © 2026 Duc-Minh Vu. Mọi quyền được bảo lưu.
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cassert>

using namespace std;

// ============================================================================
// 1. CẤU TRÚC NODE CƠ BẢN VÀ CÁC THAO TÁC TIỆN ÍCH
// ============================================================================

namespace DynamicLinkedList {

    struct ListNode {
        int val;
        ListNode* next;
        ListNode(int x = 0) : val(x), next(nullptr) {}
        ListNode(int x, ListNode* nxt) : val(x), next(nxt) {}
    };

    /**
     * @brief Tạo danh sách liên kết từ std::vector
     */
    ListNode* createList(const vector<int>& nums) {
        ListNode dummy(0);
        ListNode* curr = &dummy;
        for (int x : nums) {
            curr->next = new ListNode(x);
            curr = curr->next;
        }
        return dummy.next;
    }

    /**
     * @brief Chuyển danh sách liên kết thành std::vector để kiểm tra
     */
    vector<int> toVector(ListNode* head) {
        vector<int> res;
        while (head != nullptr) {
            res.push_back(head->val);
            head = head->next;
        }
        return res;
    }

    /**
     * @brief Giải phóng bộ nhớ danh sách liên kết
     */
    void freeList(ListNode* head) {
        while (head != nullptr) {
            ListNode* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    // ========================================================================
    // 2. KỸ THUẬT CON TRỎ NHANH - CHẬM (FLOYD'S TORTOISE & HARE)
    // ========================================================================

    /**
     * @brief Tìm nút trung điểm trong một lượt duyệt O(N) thời gian, O(1) bộ nhớ
     * Nếu độ dài chẵn, trả về nút thứ 2 trong cặp trung tâm
     */
    ListNode* findMiddle(ListNode* head) {
        if (!head) return nullptr;
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    /**
     * @brief Kiểm tra xem danh sách có chứa chu trình hay không
     */
    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) return true;
        }
        return false;
    }

    /**
     * @brief Tìm nút bắt đầu chu trình (Floyd's Algorithm II)
     * @return Con trỏ đến nút bắt đầu chu trình, hoặc nullptr nếu không có chu trình
     */
    ListNode* detectCycleStart(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        bool hasLoop = false;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
            if (slow == fast) {
                hasLoop = true;
                break;
            }
        }
        if (!hasLoop) return nullptr;

        ListNode* ptr1 = head;
        ListNode* ptr2 = slow;
        while (ptr1 != ptr2) {
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
        return ptr1;
    }

    // ========================================================================
    // 3. CÁC THUẬT TOÁN ĐẢO NGƯỢC (REVERSE LINKED LIST)
    // ========================================================================

    /**
     * @brief Đảo ngược toàn bộ danh sách liên kết in-place O(N) thời gian, O(1) bộ nhớ
     */
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* curr = head;
        while (curr != nullptr) {
            ListNode* nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }
        return prev;
    }

    /**
     * @brief Đảo ngược các nút từ vị trí left đến right (1-indexed)
     */
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        if (!head || left == right) return head;

        ListNode dummy(0, head);
        ListNode* prev = &dummy;

        for (int i = 1; i < left; ++i) {
            prev = prev->next;
        }

        ListNode* curr = prev->next;
        for (int i = 0; i < right - left; ++i) {
            ListNode* nxt = curr->next;
            curr->next = nxt->next;
            nxt->next = prev->next;
            prev->next = nxt;
        }

        return dummy.next;
    }

    /**
     * @brief Đảo ngược danh sách liên kết theo từng nhóm k phần tử (Reverse Nodes in k-Group)
     */
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* check = head;
        for (int i = 0; i < k; ++i) {
            if (!check) return head; // Không đủ k phần tử, giữ nguyên
            check = check->next;
        }

        ListNode* prev = nullptr;
        ListNode* curr = head;
        for (int i = 0; i < k; ++i) {
            ListNode* nxt = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nxt;
        }

        head->next = reverseKGroup(curr, k);
        return prev;
    }

    // ========================================================================
    // 4. HỢP NHẤT DANH SÁCH (MERGE LISTS)
    // ========================================================================

    /**
     * @brief Hợp nhất hai danh sách liên kết đã sắp xếp O(N + M)
     */
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val <= l2->val) {
                tail->next = l1;
                l1 = l1->next;
            } else {
                tail->next = l2;
                l2 = l2->next;
            }
            tail = tail->next;
        }
        tail->next = (l1 ? l1 : l2);
        return dummy.next;
    }

    /**
     * @brief Hợp nhất K danh sách đã sắp xếp O(N log K) bằng Min-Heap
     */
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) {
            return a->val > b->val;
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);

        for (ListNode* l : lists) {
            if (l) pq.push(l);
        }

        ListNode dummy(0);
        ListNode* tail = &dummy;

        while (!pq.empty()) {
            ListNode* smallest = pq.top();
            pq.pop();

            tail->next = smallest;
            tail = tail->next;

            if (smallest->next) {
                pq.push(smallest->next);
            }
        }
        return dummy.next;
    }

} // namespace DynamicLinkedList

// ============================================================================
// 5. THIẾT KẾ BỘ NHỚ ĐỆM LRU (LEAST RECENTLY USED CACHE) O(1)
// ============================================================================

class LRUCache {
private:
    struct DNode {
        int key;
        int value;
        DNode* prev;
        DNode* next;
        DNode(int k = 0, int v = 0) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    int capacity;
    int size;
    unordered_map<int, DNode*> cache;
    DNode* head; // Dummy Head (Most Recently Used - MRU)
    DNode* tail; // Dummy Tail (Least Recently Used - LRU)

    void addNode(DNode* node) {
        // Chèn ngay sau head (vị trí gần đây nhất)
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(DNode* node) {
        // Gỡ node khỏi danh sách
        DNode* prv = node->prev;
        DNode* nxt = node->next;
        prv->next = nxt;
        nxt->prev = prv;
    }

    void moveToHead(DNode* node) {
        removeNode(node);
        addNode(node);
    }

    DNode* popTail() {
        // Xóa phần tử ngay trước tail (phần tử lâu nhất không dùng)
        DNode* res = tail->prev;
        removeNode(res);
        return res;
    }

public:
    LRUCache(int cap) : capacity(cap), size(0) {
        head = new DNode();
        tail = new DNode();
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        DNode* curr = head;
        while (curr != nullptr) {
            DNode* nxt = curr->next;
            delete curr;
            curr = nxt;
        }
    }

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;
        DNode* node = it->second;
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            DNode* node = it->second;
            node->value = value;
            moveToHead(node);
        } else {
            DNode* newNode = new DNode(key, value);
            cache[key] = newNode;
            addNode(newNode);
            ++size;

            if (size > capacity) {
                DNode* tailNode = popTail();
                cache.erase(tailNode->key);
                delete tailNode;
                --size;
            }
        }
    }
};

// ============================================================================
// 6. DANH SÁCH LIÊN KẾT TRÊN MẢNG TĨNH (STATIC NODE POOL CHO ICPC)
// ============================================================================

namespace StaticLinkedList {
    const int MAXN = 100005;

    int val[MAXN];
    int nxt[MAXN];
    int prv[MAXN];
    int totalNodes = 0;

    void init() {
        totalNodes = 0;
        nxt[0] = 0;
        prv[0] = 0;
    }

    int allocate(int v) {
        int id = ++totalNodes;
        val[id] = v;
        nxt[id] = 0;
        prv[id] = 0;
        return id;
    }

    void insertAfter(int u, int v) {
        int id = allocate(v);
        nxt[id] = nxt[u];
        prv[id] = u;
        if (nxt[u]) prv[nxt[u]] = id;
        nxt[u] = id;
    }

    void erase(int u) {
        if (nxt[u]) prv[nxt[u]] = prv[u];
        if (prv[u]) nxt[prv[u]] = nxt[u];
    }
}

// ============================================================================
// 7. HÀM TỰ KIỂM THỬ TOÀN DIỆN (SELF-TEST)
// ============================================================================

void runSelfTest() {
    using namespace DynamicLinkedList;

    // Test 1: Tạo danh sách & Middle node
    ListNode* list1 = createList({1, 2, 3, 4, 5});
    ListNode* mid1 = findMiddle(list1);
    assert(mid1 != nullptr && mid1->val == 3);

    ListNode* list2 = createList({1, 2, 3, 4});
    ListNode* mid2 = findMiddle(list2);
    assert(mid2 != nullptr && mid2->val == 3);

    // Test 2: Phát hiện chu trình & Tìm điểm bắt đầu
    ListNode* headCycle = createList({1, 2, 3, 4, 5});
    // Nối node 5 -> node 3
    ListNode* tailC = headCycle;
    ListNode* node3 = nullptr;
    while (tailC->next) {
        if (tailC->val == 3) node3 = tailC;
        tailC = tailC->next;
    }
    tailC->next = node3; // Tạo chu trình
    assert(hasCycle(headCycle) == true);
    assert(detectCycleStart(headCycle) == node3);
    tailC->next = nullptr; // Phá chu trình để giải phóng bộ nhớ
    freeList(headCycle);

    // Test 3: Đảo ngược toàn bộ
    ListNode* listRev = createList({1, 2, 3, 4, 5});
    listRev = reverseList(listRev);
    assert(toVector(listRev) == (vector<int>{5, 4, 3, 2, 1}));
    freeList(listRev);

    // Test 4: Đảo ngược k-group (k = 2)
    ListNode* listK = createList({1, 2, 3, 4, 5});
    listK = reverseKGroup(listK, 2);
    assert(toVector(listK) == (vector<int>{2, 1, 4, 3, 5}));
    freeList(listK);

    // Test 5: Merge Two Sorted Lists
    ListNode* mA = createList({1, 2, 4});
    ListNode* mB = createList({1, 3, 4});
    ListNode* merged = mergeTwoLists(mA, mB);
    assert(toVector(merged) == (vector<int>{1, 1, 2, 3, 4, 4}));
    freeList(merged);

    // Test 6: LRU Cache
    LRUCache lru(2);
    lru.put(1, 1);
    lru.put(2, 2);
    assert(lru.get(1) == 1);       // Trả về 1
    lru.put(3, 3);                // Đào thải key 2 (vì key 1 vừa được get)
    assert(lru.get(2) == -1);      // Không tìm thấy
    lru.put(4, 4);                // Đào thải key 1
    assert(lru.get(1) == -1);      // Không tìm thấy
    assert(lru.get(3) == 3);       // Trả về 3
    assert(lru.get(4) == 4);       // Trả về 4

    // Test 7: Static Linked List
    StaticLinkedList::init();
    int node1 = StaticLinkedList::allocate(10);
    StaticLinkedList::insertAfter(node1, 20); // 10 -> 20
    StaticLinkedList::insertAfter(node1, 15); // 10 -> 15 -> 20
    int node15 = StaticLinkedList::nxt[node1];
    assert(StaticLinkedList::val[node15] == 15);
    int node20 = StaticLinkedList::nxt[node15];
    assert(StaticLinkedList::val[node20] == 20);
    StaticLinkedList::erase(node15);         // Xóa 15 -> 10 -> 20
    assert(StaticLinkedList::nxt[node1] == node20);

    freeList(list1);
    freeList(list2);

    cout << "==> ALL LINKED LIST SELF-TESTS PASSED SUCCESSFULLY!" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    runSelfTest();
    return 0;
}
