# 🎯 Tuyển tập Bài tập Thực hành: Các Thuật toán Sắp xếp & Đếm Nghịch thế (Inversions)

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Biên soạn:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---

## 📊 Bảng tổng hợp Danh mục 16 Bài tập Phân cấp

| STT | Mã bài (Click mở đề) | Tên bài toán | Nguồn | Cấp độ | Kỹ thuật trọng tâm | Độ phức tạp |
| :---: | :--- | :--- | :---: | :---: | :--- | :---: |
| 1 | [`CSES-1621`](https://cses.fi/problemset/task/1621) | [**Distinct Numbers**](https://cses.fi/problemset/task/1621) | CSES | ⭐ Cơ bản | Sắp xếp & Đếm phân biệt | $O(N \log N)$ |
| 2 | [`CF-1360B`](https://codeforces.com/problemset/problem/1360/B) | [**Honest Coach**](https://codeforces.com/problemset/problem/1360/B) | Codeforces | ⭐ Cơ bản | Sắp xếp, Hiệu 2 phần tử kề | $O(N \log N)$ |
| 3 | [`CSES-1090`](https://cses.fi/problemset/task/1090) | [**Ferris Wheel**](https://cses.fi/problemset/task/1090) | CSES | ⭐ Cơ bản | Sắp xếp + Hai con trỏ tham lam | $O(N \log N)$ |
| 4 | [`CF-1490B`](https://codeforces.com/problemset/problem/1490/B) | [**01 Game**](https://codeforces.com/problemset/problem/1373/B) | Codeforces | ⭐ Cơ bản | Đếm chẵn lẻ, Trò chơi đối kháng | $O(N)$ |
| 5 | [`CF-1512A`](https://codeforces.com/problemset/problem/1512/A) | [**Spy Detected!**](https://codeforces.com/problemset/problem/1512/A) | Codeforces | ⭐ Cơ bản | Phân phối tần suất, Đếm số | $O(N)$ |
| 6 | [`SPOJ-INVCNT`](https://www.spoj.com/problems/INVCNT/) | [**Inversion Count**](https://www.spoj.com/problems/INVCNT/) | SPOJ | ⭐⭐ Trung cấp | Đếm nghịch thế Merge Sort / BIT | $O(N \log N)$ |
| 7 | [`CSES-1163`](https://cses.fi/problemset/task/1163) | [**Traffic Lights**](https://cses.fi/problemset/task/1163) | CSES | ⭐⭐ Trung cấp | `std::set` sắp xếp động | $O(N \log N)$ |
| 8 | [`CSES-1164`](https://cses.fi/problemset/task/1164) | [**Room Allocation**](https://cses.fi/problemset/task/1164) | CSES | ⭐⭐ Trung cấp | Sắp xếp sự kiện + Min-Heap | $O(N \log N)$ |
| 9 | [`CF-911D`](https://codeforces.com/problemset/problem/911/D) | [**Inversion Counting**](https://codeforces.com/problemset/problem/911/D) | Codeforces | ⭐⭐ Trung cấp | Tính chẵn lẻ nghịch thế khi đảo đoạn | $O(N^2 + Q)$ |
| 10 | [`CF-1585D`](https://codeforces.com/problemset/problem/1585/D) | [**Yet Another Sorting Problem**](https://codeforces.com/problemset/problem/1585/D) | Codeforces | ⭐⭐ Trung cấp | Bất biến chẵn lẻ của 3-cycle | $O(N \log N)$ |
| 11 | [`CSES-2216`](https://cses.fi/problemset/task/2216) | [**Collecting Numbers I**](https://cses.fi/problemset/task/2216) | CSES | ⭐⭐ Trung cấp | Nghịch thế vị trí trong hoán vị | $O(N)$ |
| 12 | [`CSES-2217`](https://cses.fi/problemset/task/2217) | [**Collecting Numbers II**](https://cses.fi/problemset/task/2217) | CSES | ⭐⭐⭐ Nâng cao | Cập nhật hoán đổi vị trí trong $O(1)$ | $O(M)$ |
| 13 | [`CF-1430E`](https://codeforces.com/problemset/problem/1430/E) | [**String Reversal**](https://codeforces.com/problemset/problem/1430/E) | Codeforces | ⭐⭐⭐ Nâng cao | Đảo chuỗi bằng số swap kề tối thiểu | $O(N \log N)$ |
| 14 | [`LC-493`](https://leetcode.com/problems/reverse-pairs/) | [**Reverse Pairs**](https://leetcode.com/problems/reverse-pairs/) | LeetCode / VNOI | ⭐⭐⭐ Nâng cao | Đếm cặp $A[i] > 2 \cdot A[j]$ | $O(N \log N)$ |
| 15 | [`CSES-2163`](https://cses.fi/problemset/task/2163) | [**Josephus Problem II**](https://cses.fi/problemset/task/2163) | CSES | ⭐⭐⭐ Nâng cao | Fenwick Tree / PBDS Order Statistics | $O(N \log N)$ |
| 16 | [`CF-1359D`](https://codeforces.com/problemset/problem/1359/D) | [**Yet Another Yet Another Task**](https://codeforces.com/problemset/problem/1359/D) | Codeforces | ⭐⭐⭐ Nâng cao | Sắp xếp giá trị tối đa, Kadane | $O(30 \cdot N)$ |

---

# 🟢 MỨC ĐỘ 1: KHỞI ĐỘNG & NỀN TẢNG (WARM-UP)

### 1. CSES 1621 - Distinct Numbers
- **Link đề bài:** [CSES 1621](https://cses.fi/problemset/task/1621)
- **Tóm tắt:** Cho dãy $N$ số nguyên ($N \le 2 \cdot 10^5$). Đếm số lượng giá trị phân biệt.
- **Ý tưởng:** Sắp xếp mảng tăng dần bằng `std::sort` trong $O(N \log N)$. Sau đó đếm số lần $A[i] \ne A[i-1]$, hoặc dùng `std::unique`:
```cpp
sort(a.begin(), a.end());
int distinct_cnt = unique(a.begin(), a.end()) - a.begin();
```
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(1)$.

### 2. Codeforces 1360B - Honest Coach
- **Link đề bài:** [Codeforces 1360B](https://codeforces.com/problemset/problem/1360/B)
- **Tóm tắt:** Chia $N$ vận động viên thành 2 đội sao cho chênh lệch sức mạnh nhỏ nhất giữa 1 người đội A và 1 người đội B là cực tiểu.
- **Ý tưởng:** Sau khi sắp xếp mảng tăng dần, hai phần tử có độ chênh lệch nhỏ nhất chắc chắn là **hai phần tử kề nhau** $A[i] - A[i-1]$. Kết quả là $\min_{i=1}^{N-1} (A[i] - A[i-1])$.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(1)$.

### 3. CSES 1090 - Ferris Wheel
- **Link đề bài:** [CSES 1090](https://cses.fi/problemset/task/1090)
- **Tóm tắt:** $N$ đứa trẻ với cân nặng $p_i$. Mỗi cabin chở tối đa 2 người và tổng cân nặng $\le X$. Tìm số cabin ít nhất.
- **Ý tưởng:** Sắp xếp cân nặng tăng dần. Dùng hai con trỏ: ghép người nặng nhất còn lại ($R$) với người nhẹ nhất còn lại ($L$). Nếu $p_L + p_R \le X$ thì cho cả 2 lên cùng cabin ($L++, R--$); ngược lại người nặng nhất $R$ phải đi một mình ($R--$).
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(1)$.

### 4. Codeforces 1373B - 01 Game
- **Link đề bài:** [Codeforces 1373B](https://codeforces.com/problemset/problem/1373/B)
- **Tóm tắt:** Hai người chơi luân phiên xóa một cặp ký tự khác nhau đứng cạnh nhau (`01` hoặc `10`). Ai không còn nước đi là người thua.
- **Ý tưởng:** Bất kể xóa ở vị trí nào, mỗi lượt chơi luôn tiêu hao đúng một chữ số 0 và một chữ số 1. Tổng số lượt chơi tối đa luôn bằng $\min(\text{count}(0), \text{count}(1))$. Nếu số lượt chơi là số lẻ thì người đi trước thắng, ngược lại người đi sau thắng.
- **Độ phức tạp:** Thời gian: $O(N)$, Bộ nhớ: $O(1)$.

### 5. Codeforces 1512A - Spy Detected!
- **Link đề bài:** [Codeforces 1512A](https://codeforces.com/problemset/problem/1512/A)
- **Tóm tắt:** Tìm vị trí của phần tử duy nhất có giá trị khác biệt với tất cả các phần tử còn lại trong mảng.
- **Ý tưởng:** Đếm tần suất hoặc so sánh $A[i]$ với hai đầu mảng để xác định phần tử thiểu số.
- **Độ phức tạp:** Thời gian: $O(N)$, Bộ nhớ: $O(1)$.

---

# 🟡 MỨC ĐỘ 2: TRUNG CẤP (ĐẾM NGHỊCH THẾ & BẤT BIẾN HOÁN VỊ)

### 6. SPOJ INVCNT - Inversion Count
- **Link đề bài:** [SPOJ INVCNT](https://www.spoj.com/problems/INVCNT/)
- **Tóm tắt:** Cho mảng $N$ số nguyên ($N \le 2 \cdot 10^5$). Đếm số cặp nghịch thế $i < j$ và $A[i] > A[j]$.
- **Ý tưởng:** Áp dụng thuật toán Merge Sort đếm nghịch thế hoặc Fenwick Tree kết hợp nén tọa độ. Lưu ý biến đếm phải dùng `long long`.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 7. CSES 1163 - Traffic Lights
- **Link đề bài:** [CSES 1163](https://cses.fi/problemset/task/1163)
- **Tóm tắt:** Đèn giao thông được thêm dần vào đoạn đường độ dài $X$. Sau mỗi lần thêm đèn, in ra độ dài đoạn đường không có đèn lớn nhất.
- **Ý tưởng:** Dùng `std::set` để lưu tọa độ các đèn đã đặt, và `std::multiset` để lưu độ dài các khoảng cách. Dùng `upper_bound` tìm 2 đèn kề bên để xóa khoảng cũ và chèn 2 khoảng mới trong $O(\log N)$.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 8. CSES 1164 - Room Allocation
- **Link đề bài:** [CSES 1164](https://cses.fi/problemset/task/1164)
- **Tóm tắt:** Có $N$ khách với khoảng thời gian lưu trú $[a_i, b_i]$. Tìm số phòng ít nhất và chỉ định phòng cho từng khách.
- **Ý tưởng:** Sắp xếp các khách theo thời gian đến $a_i$. Dùng một hàng đợi ưu tiên (Min-Heap) lưu các phòng đang có người ở theo thời gian trả phòng $b_i$. Nếu khách mới đến sau thời gian trả phòng của phòng sớm nhất, ta tái sử dụng phòng đó; ngược lại mở phòng mới.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 9. Codeforces 911D - Inversion Counting
- **Link đề bài:** [Codeforces 911D](https://codeforces.com/problemset/problem/911/D)
- **Tóm tắt:** Cho một hoán vị $N$ số. Có $M$ truy vấn: mỗi truy vấn đảo ngược đoạn con $[l, r]$. Xác định sau mỗi truy vấn, số nghịch thế của toàn bộ hoán vị là chẵn hay lẻ.
- **Ý tưởng:** Khi đảo ngược một đoạn độ dài $L = r - l + 1$, số cặp phần tử trong đoạn là $\binom{L}{2} = \frac{L(L-1)}{2}$. Mỗi cặp trước đó là nghịch thế sẽ thành không nghịch thế và ngược lại. Do đó, tính chẵn lẻ của số lượng nghịch thế thay đổi:
$$\Delta \text{Parity} \equiv \frac{L(L-1)}{2} \pmod 2$$
Ta chỉ cần tính tính chẵn lẻ ban đầu bằng Merge Sort trong $O(N \log N)$, sau đó mỗi truy vấn trả lời trong **$O(1)$**!
- **Độ phức tạp:** Thời gian: $O(N \log N + M)$, Bộ nhớ: $O(N)$.

### 10. Codeforces 1585D - Yet Another Sorting Problem
- **Link đề bài:** [Codeforces 1585D](https://codeforces.com/problemset/problem/1585/D)
- **Tóm tắt:** Mỗi bước được chọn 3 phần tử phân biệt và xoay vòng dịch chuyển (Cyclic shift): $A[i] \to A[j] \to A[k] \to A[i]$. Hỏi có thể sắp xếp mảng tăng dần được không?
- **Ý tưởng:**
  - Một phép xoay 3 phần tử tương đương với **2 phép hoán đổi liên tiếp** $\implies$ Tính chẵn lẻ của số lượng nghịch thế **không bao giờ thay đổi** (Bất biến chẵn lẻ)!
  - Nếu mảng có ít nhất 2 phần tử trùng nhau ($A[i] == A[j]$): Ta có thể tự do tráo đổi 2 phần tử trùng nhau này để đảo tính chẵn lẻ $\implies$ Luôn **YES**.
  - Nếu tất cả phần tử phân biệt: Mảng sắp xếp được khi và chỉ khi số nghịch thế ban đầu là **số chẵn** ($\text{Inv} \pmod 2 == 0$).
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 11. CSES 2216 - Collecting Numbers I
- **Link đề bài:** [CSES 2216](https://cses.fi/problemset/task/2216)
- **Tóm tắt:** Thu thập các số từ $1 \to N$ theo thứ tự tăng dần bằng cách quét mảng từ trái sang phải nhiều vòng. Tìm số vòng quét ít nhất.
- **Ý tưởng:** Gọi `pos[x]` là vị trí của số $x$ trong mảng. Nếu `pos[x] > pos[x + 1]`, để nhặt được số $x + 1$ sau khi đã nhặt số $x$, ta bắt buộc phải bắt đầu một vòng quét mới!  
Số vòng quét = $1 + \sum_{x=1}^{N-1} [\text{pos}[x] > \text{pos}[x+1]]$.
- **Độ phức tạp:** Thời gian: $O(N)$, Bộ nhớ: $O(N)$.

---

# 🔴 MỨC ĐỘ 3: NÂNG CAO & NGHỊCH THẾ ĐỘNG (ADVANCED INVERSIONS)

### 12. CSES 2217 - Collecting Numbers II
- **Link đề bài:** [CSES 2217](https://cses.fi/problemset/task/2217)
- **Tóm tắt:** Giống bài trên nhưng có thêm $M$ truy vấn hoán đổi giá trị $A[u]$ và $A[v]$. Cập nhật số vòng quét sau mỗi lần tráo đổi.
- **Ý tưởng:** Khi tráo đổi 2 phần tử có giá trị $x$ và $y$, chỉ có mối quan hệ giữa các cặp $(x-1, x)$, $(x, x+1)$, $(y-1, y)$, $(y, y+1)$ bị ảnh hưởng. Ta dùng một `std::set` chứa các cặp có nguy cơ thay đổi, trừ đi đóng góp cũ của chúng trước khi swap, thực hiện swap, rồi cộng lại đóng góp mới trong $O(1)$!
- **Độ phức tạp:** Thời gian: $O(N + M)$, Bộ nhớ: $O(N)$.

### 13. Codeforces 1430E - String Reversal
- **Link đề bài:** [Codeforces 1430E](https://codeforces.com/problemset/problem/1430/E)
- **Tóm tắt:** Cho chuỗi $S$. Mỗi bước được hoán đổi 2 ký tự kề nhau. Tìm số bước ít nhất để biến $S$ thành chuỗi đảo ngược của nó.
- **Ý tưởng:**
  - Để tối thiểu hóa số phép swap kề, ký tự `'a'` đầu tiên của chuỗi đích nên được ghép với ký tự `'a'` đầu tiên của chuỗi gốc (chiến thuật tham lam).
  - Ta xây dựng mảng hoán vị $P$ ánh xạ mỗi vị trí trong $S$ tới vị trí đích của nó trong chuỗi đảo ngược.
  - Số phép swap kề tối thiểu chính bằng **Số lượng nghịch thế của hoán vị $P$**! Tính bằng Fenwick Tree trong $O(N \log N)$.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 14. LeetCode 493 / VNOI - Reverse Pairs
- **Link đề bài:** [LeetCode 493](https://leetcode.com/problems/reverse-pairs/)
- **Tóm tắt:** Đếm số cặp chỉ số $i < j$ sao cho $A[i] > 2 \cdot A[j]$.
- **Ý tưởng:**
  - Áp dụng Merge Sort: Khi 2 nửa $[L, mid]$ và $[mid+1, R]$ đã được sắp xếp, dùng 2 con trỏ chạy trên 2 nửa để đếm số cặp thỏa mãn $A[i] > 2 \cdot A[j]$ trong $O(R - L)$ trước khi thực hiện trộn mảng thông thường.
  - Hoặc dùng Fenwick Tree nén tọa độ trên tập giá trị $\{A[i], 2 \cdot A[i]\}$.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 15. CSES 2163 - Josephus Problem II
- **Link đề bài:** [CSES 2163](https://cses.fi/problemset/task/2163)
- **Tóm tắt:** Vòng tròn $N$ người, mỗi lượt loại người thứ $K$ tiếp theo ($N \le 2 \cdot 10^5, K \le 10^9$). In ra thứ tự những người bị loại.
- **Ý tưởng:** Dùng Cây Fenwick kết hợp Chặt nhị phân (Binary Lifting trên BIT) để tìm và xóa phần tử thứ $p$ còn sống trong vòng $O(\log N)$, hoặc dùng `__gnu_pbds::tree` (Policy-Based Data Structure) với hàm `find_by_order`.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

### 16. Codeforces 1359D - Yet Another Yet Another Task
- **Link đề bài:** [Codeforces 1359D](https://codeforces.com/problemset/problem/1359/D)
- **Tóm tắt:** Tìm đoạn con liên tiếp có $\sum A_k - \max(A_k)$ đạt giá trị lớn nhất. Biết $-30 \le A_i \le 30$.
- **Ý tưởng:** Vì miền giá trị tối đa $M \in [-30, 30]$ rất nhỏ (chỉ 61 giá trị), ta cố định $\max = M$ từ 0 đến 30. Với mỗi giá trị $\max$, tất cả các phần tử $> \max$ bị coi như rào cản chia mảng thành các đoạn độc lập. Áp dụng thuật toán Kadane tìm đoạn con có tổng lớn nhất trên mỗi đoạn trong $O(N)$.
- **Độ phức tạp:** Thời gian: $O(30 \cdot N)$, Bộ nhớ: $O(1)$.

---

<div align="center">

<a href="https://www.neu.edu.vn" target="_blank"><img src="../../assets/logos/NEU_Logo.png" alt="NEU Logo" height="50" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.fda.neu.edu.vn" target="_blank"><img src="../../assets/logos/FDA_Logo.png" alt="FDA Logo" height="50" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.facebook.com/slscm.lab" target="_blank"><img src="../../assets/logos/SLSCM_logo.jpg" alt="SLSCM Logo" height="50" /></a>

<br/><br/>

**Competitive Programming Handbook**  
*SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA) — National Economics University (NEU)*  
*Tài liệu được soạn thảo và tối ưu bởi Agentic AI tool*  
© 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.

</div>
