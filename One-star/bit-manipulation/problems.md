# 🎯 Tuyển tập Bài tập Thực hành: Phép toán Bit & Cấu trúc Dữ liệu Bitset

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Biên soạn:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---

## 📊 Bảng tổng hợp Danh mục 16 Bài tập Phân cấp

| STT | Mã bài | Tên bài toán | Nguồn | Cấp độ | Kỹ thuật trọng tâm | Độ phức tạp |
| :---: | :--- | :--- | :---: | :---: | :--- | :---: |
| 1 | [`CSES-1617`](https://cses.fi/problemset/task/1617) | [**Bit Strings**](https://cses.fi/problemset/task/1617) | CSES | ⭐ Cơ bản | Lũy thừa nhị phân, Shift bit | $O(\log N)$ |
| 2 | [`CF-1527A`](https://codeforces.com/problemset/problem/1527/A) | [**And Then There Were K**](https://codeforces.com/problemset/problem/1527/A) | Codeforces | ⭐ Cơ bản | Bit cao nhất (MSB), Phép AND | $O(1)$ |
| 3 | [`CF-1514B`](https://codeforces.com/problemset/problem/1514/B) | [**AND 0, Sum Big**](https://codeforces.com/problemset/problem/1514/B) | Codeforces | ⭐ Cơ bản | Tổ hợp bit, Tính chất AND | $O(K \log N)$ |
| 4 | [`CF-1362A`](https://codeforces.com/problemset/problem/1362/A) | [**Johnny and Ancient Computer**](https://codeforces.com/problemset/problem/1362/A) | Codeforces | ⭐ Cơ bản | Phép dịch bit, Đếm trailing zeros | $O(1)$ |
| 5 | [`CSES-1622`](https://cses.fi/problemset/task/1622) | [**Creating Strings**](https://cses.fi/problemset/task/1622) | CSES | ⭐ Cơ bản | Sinh tổ hợp, Quản lý bitmask | $O(N \cdot N!)$ |
| 6 | [`CSES-1623`](https://cses.fi/problemset/task/1623) | [**Apple Division**](https://cses.fi/problemset/task/1623) | CSES | ⭐⭐ Trung cấp | Duyệt $2^N$ tập con bằng bitmask | $O(N \cdot 2^N)$ |
| 7 | [`CF-1097B`](https://codeforces.com/problemset/problem/1097/B) | [**Petr and a Combination Lock**](https://codeforces.com/problemset/problem/1097/B) | Codeforces | ⭐⭐ Trung cấp | Bitmask brute force 360 độ | $O(2^N)$ |
| 8 | [`ABC-147C`](https://atcoder.jp/contests/abc147/tasks/abc147_c) | [**HonestOrUnkind2**](https://atcoder.jp/contests/abc147/tasks/abc147_c) | AtCoder | ⭐⭐ Trung cấp | Kiểm tra tính nhất quán bitmask | $O(N^2 \cdot 2^N)$ |
| 9 | [`CSES-2165`](https://cses.fi/problemset/task/2165) | [**Tower of Hanoi**](https://cses.fi/problemset/task/2165) | CSES | ⭐⭐ Trung cấp | Quy luật nhị phân đĩa tháp | $O(2^N)$ |
| 10 | [`CSES-1090`](https://cses.fi/problemset/task/1090) | [**Ferris Wheel (Bitwise)**](https://cses.fi/problemset/task/1090) | CSES | ⭐⭐ Trung cấp | Tham lam hai con trỏ trên bit | $O(N \log N)$ |
| 11 | [`CSES-1653`](https://cses.fi/problemset/task/1653) | [**Elevator Rides**](https://cses.fi/problemset/task/1653) | CSES | ⭐⭐⭐ Nâng cao | Bitmask DP nền tảng | $O(N \cdot 2^N)$ |
| 12 | [`CSES-2134`](https://cses.fi/problemset/task/2134) | [**Reachable Nodes**](https://cses.fi/problemset/task/2134) | CSES | ⭐⭐⭐ Nâng cao | DAG Reachability với `bitset` | $O(\frac{N(N+M)}{64})$ |
| 13 | [`CSES-2143`](https://cses.fi/problemset/task/2143) | [**Reachability Queries**](https://cses.fi/problemset/task/2143) | CSES | ⭐⭐⭐ Nâng cao | Nén SCC + `bitset` trên DAG | $O(\frac{V^2}{64} + Q)$ |
| 14 | [`ABC-258G`](https://atcoder.jp/contests/abc258/tasks/abc258_g) | [**Triangle**](https://atcoder.jp/contests/abc258/tasks/abc258_g) | AtCoder | ⭐⭐⭐ Nâng cao | Đếm tam giác ma trận kề `bitset` | $O(\frac{N^3}{64})$ |
| 15 | [`CF-1093E`](https://codeforces.com/problemset/problem/1093/E) | [**Intersection of Permutations**](https://codeforces.com/problemset/problem/1093/E) | Codeforces | ⭐⭐⭐ Nâng cao | Bit-parallelism, Chia khối | $O(\frac{N \sqrt{N}}{64})$ |
| 16 | [`CSES-1654`](https://cses.fi/problemset/task/1654) | [**Bit Problem**](https://cses.fi/problemset/task/1654) | CSES | ⭐⭐⭐ Nâng cao | SOS DP (Sum Over Subsets) | $O(N \cdot 2^N)$ |

---

# 🟢 MỨC ĐỘ 1: KHỞI ĐỘNG & THAO TÁC CƠ BẢN (WARM-UP)

### 1. CSES 1617 - Bit Strings
- **Link đề bài:** [CSES 1617](https://cses.fi/problemset/task/1617)
- **Tóm tắt:** Đếm số lượng chuỗi nhị phân độ dài $N$ modulo $10^9 + 7$.
- **Ý tưởng:** Mỗi vị trí có $2$ lựa chọn $\implies$ Kết quả là $2^N \pmod{10^9 + 7}$. Áp dụng thuật toán lũy thừa nhị phân (Binary Exponentiation).
- **Độ phức tạp:** Thời gian: $O(\log N)$, Bộ nhớ: $O(1)$.

### 2. Codeforces 1527A - And Then There Were K
- **Link đề bài:** [Codeforces 1527A](https://codeforces.com/problemset/problem/1527/A)
- **Tóm tắt:** Cho số nguyên $n$. Tìm số nguyên $k$ lớn nhất sao cho $n \ \& \ (n-1) \ \& \ \dots \ \& \ k = 0$.
- **Ý tưởng:** Giá trị AND liên tiếp sẽ giảm dần về $0$ khi và chỉ khi bit $1$ cao nhất (MSB) của $n$ bị triệt tiêu thành $0$. Bit này lần đầu tiên bằng $0$ tại giá trị $2^p - 1$ với $2^p$ là lũy thừa lớn nhất của $2$ mà $\le n$.
$$\implies k = 2^{\lfloor \log_2 n \rfloor} - 1 = (1 \ll (63 - \text{\_\_builtin\_clzll}(n))) - 1$$
- **Độ phức tạp:** Thời gian: $O(1)$, Bộ nhớ: $O(1)$.

### 3. Codeforces 1514B - AND 0, Sum Big
- **Link đề bài:** [Codeforces 1514B](https://codeforces.com/problemset/problem/1514/B)
- **Tóm tắt:** Tìm số mảng gồm $n$ số nguyên không âm có độ dài $k$ bit sao cho tổng mảng lớn nhất và AND của cả mảng bằng $0$.
- **Ý tưởng:** Để tổng lớn nhất, mỗi bit từ $0$ đến $k-1$ phải xuất hiện ở càng nhiều phần tử càng tốt. Nhưng để AND bằng $0$, mỗi bit phải có ít nhất một phần tử mang bit $0$. Để tối đa hóa tổng, mỗi bit trong số $k$ bit sẽ có đúng 1 phần tử mang bit $0$ và $n-1$ phần tử còn lại mang bit $1$. Có $n$ cách chọn phần tử nhận bit $0$ cho mỗi bit $\implies$ Kết quả là $n^k \pmod{10^9 + 7}$.
- **Độ phức tạp:** Thời gian: $O(\log K)$, Bộ nhớ: $O(1)$.

### 4. Codeforces 1362A - Johnny and Ancient Computer
- **Link đề bài:** [Codeforces 1362A](https://codeforces.com/problemset/problem/1362/A)
- **Tóm tắt:** Từ số $a$, mỗi bước có thể nhân hoặc chia cho $2, 4, 8$ (tương đương dịch trái/phải $1, 2, 3$ bit). Tìm số bước ít nhất biến $a$ thành $b$.
- **Ý tưởng:** Nếu $a > b$, tráo đổi $a$ và $b$. Khi đó $b$ phải chia hết cho $a$, và tỉ số $b / a$ phải là lũy thừa của $2$. Nếu không, in ra $-1$. Giả sử $b / a = 2^k$. Mỗi bước ta dịch tối đa $3$ bit $\implies$ Số bước tối thiểu là $\lceil k / 3 \rceil = (k + 2) / 3$.
- **Độ phức tạp:** Thời gian: $O(1)$, Bộ nhớ: $O(1)$.

### 5. CSES 1622 - Creating Strings
- **Link đề bài:** [CSES 1622](https://cses.fi/problemset/task/1622)
- **Tóm tắt:** Cho xâu ký tự $S$ ($|S| \le 8$). Sinh toàn bộ các hoán vị phân biệt theo thứ tự từ điển.
- **Ý tưởng:** Sử dụng kỹ thuật quay lui quản lý trạng thái các ký tự đã dùng thông qua một bitmask $8$-bit (`used_mask | (1 << i)`).
- **Độ phức tạp:** Thời gian: $O(N \cdot N!)$, Bộ nhớ: $O(N)$.

---

# 🟡 MỨC ĐỘ 2: TRUNG CẤP & BIỂU DIỄN BITMASK (SUBSETS & MASKS)

### 6. CSES 1623 - Apple Division
- **Link đề bài:** [CSES 1623](https://cses.fi/problemset/task/1623)
- **Tóm tắt:** Chia $n$ quả táo ($n \le 20$) thành $2$ nhóm sao cho chênh lệch tổng khối lượng giữa $2$ nhóm là nhỏ nhất.
- **Ý tưởng:** Với $n \le 20$, số tập con là $2^{20} \approx 10^6$. Duyệt mask từ $0$ đến $(1 \ll n) - 1$. Với mỗi mask, nếu bit thứ $i$ bằng $1$ thì quả táo $i$ thuộc nhóm $1$, ngược lại thuộc nhóm $2$. Tính chênh lệch và cập nhật giá trị nhỏ nhất.
- **Độ phức tạp:** Thời gian: $O(n \cdot 2^n)$, Bộ nhớ: $O(n)$.

### 7. Codeforces 1097B - Petr and a Combination Lock
- **Link đề bài:** [Codeforces 1097B](https://codeforces.com/problemset/problem/1097/B)
- **Tóm tắt:** Cho $n \le 15$ góc xoay. Mỗi góc có thể xoay xuôi chiều (+) hoặc ngược chiều (-) kim đồng hồ. Kiểm tra xem có cách xoay nào đưa ổ khóa về vị trí ban đầu (tổng góc xoay chia hết cho $360^\circ$) hay không.
- **Ý tưởng:** Dùng bitmask độ dài $n$ để biểu diễn chiều xoay ($1$ là $+$, $0$ là $-$). Duyệt toàn bộ $2^n \le 2^{15} = 32768$ trạng thái.
- **Độ phức tạp:** Thời gian: $O(n \cdot 2^n)$, Bộ nhớ: $O(n)$.

### 8. AtCoder abc147_c - HonestOrUnkind2
- **Link đề bài:** [AtCoder abc147_c](https://atcoder.jp/contests/abc147/tasks/abc147_c)
- **Tóm tắt:** Cho $N \le 15$ người, mỗi người có thể là người thật thà hoặc kẻ nói dối. Mỗi người đưa ra một số lời khai về những người khác. Tìm số lượng người thật thà lớn nhất sao cho không có mâu thuẫn.
- **Ý tưởng:** Giả định tập người thật thà tương ứng với một bitmask $M \in [0, 2^N - 1]$. Kiểm tra nếu người $i$ là thật thà (bit thứ $i$ bật), mọi lời khai của người $i$ phải khớp hoàn toàn với trạng thái của $M$. Nếu hợp lệ, lấy `__builtin_popcount(M)` cực đại.
- **Độ phức tạp:** Thời gian: $O(N^2 \cdot 2^N)$, Bộ nhớ: $O(N^2)$.

### 9. CSES 2165 - Tower of Hanoi
- **Link đề bài:** [CSES 2165](https://cses.fi/problemset/task/2165)
- **Tóm tắt:** In ra dãy $2^n - 1$ bước di chuyển ngắn nhất giải bài toán Tháp Hà Nội $n$ đĩa.
- **Ý tưởng:** Thuật toán đệ quy cổ điển hoặc quy luật nhị phân: Bước thứ $k$ ($1 \le k < 2^n$) di chuyển chiếc đĩa thứ $\text{ctz}(k) + 1$ (chính là vị trí của bit 1 thấp nhất của $k$).
- **Độ phức tạp:** Thời gian: $O(2^n)$, Bộ nhớ: $O(n)$.

### 10. CSES 1090 - Ferris Wheel
- **Link đề bài:** [CSES 1090](https://cses.fi/problemset/task/1090)
- **Tóm tắt:** Có $n$ đứa trẻ với cân nặng $p_i$ và các cabin chứa tối đa $2$ đứa trẻ với tổng trọng lượng $\le x$. Tìm số cabin tối thiểu.
- **Ý tưởng:** Sắp xếp mảng tăng dần, dùng hai con trỏ hoặc kết hợp quản lý ghép cặp tối ưu bằng cấu trúc bitmask nếu $N$ nhỏ.
- **Độ phức tạp:** Thời gian: $O(N \log N)$, Bộ nhớ: $O(N)$.

---

# 🔴 MỨC ĐỘ 3: NÂNG CAO & TĂNG TỐC BITSET (BIT-PARALLELISM)

### 11. CSES 1653 - Elevator Rides
- **Link đề bài:** [CSES 1653](https://cses.fi/problemset/task/1653)
- **Tóm tắt:** Có $n \le 20$ người, thang máy chịu tải tối đa $X$. Tìm số chuyến thang máy ít nhất để đưa toàn bộ $n$ người lên tầng.
- **Ý tưởng (Bitmask DP):**
  - Gọi `dp[mask]` là một cặp `(rides, last_weight)` thể hiện số chuyến ít nhất và trọng lượng của chuyến cuối cùng để phục vụ tập người `mask`.
  - Chuyển trạng thái: Với mỗi người $p$ chưa có trong `mask` (bit $p$ bằng $0$), ta thêm người $p$ vào `mask`:
    - Nếu `last_weight + w[p] <= X`: Cùng chuyến $\implies$ `(rides, last_weight + w[p])`.
    - Ngược lại: Mở chuyến mới $\implies$ `(rides + 1, w[p])`.
- **Độ phức tạp:** Thời gian: $O(n \cdot 2^n)$, Bộ nhớ: $O(2^n)$.

### 12. CSES 2134 - Reachable Nodes
- **Link đề bài:** [CSES 2134](https://cses.fi/problemset/task/2134)
- **Tóm tắt:** Cho đồ thị có hướng không chu trình (DAG) gồm $N \le 50000$ đỉnh và $M \le 150000$ cạnh. Với mỗi đỉnh $u$, đếm số lượng đỉnh có thể đến được từ $u$.
- **Ý tưởng:**
  - Định nghĩa `bitset<50005> reach[50005]`.
  - Sắp xếp topo các đỉnh của DAG. Duyệt ngược từ cuối danh sách topo về đầu:
    $$\text{reach}[u] = \{u\} \cup \bigcup_{(u, v) \in E} \text{reach}[v] \implies \mathbf{\text{reach}[u] \ \vert= \text{reach}[v]}$$
  - Phép OR trên bitset được tăng tốc 64 lần. Nếu bộ nhớ quá lớn ($50000 \times 50000$ bits $\approx 312 \text{ MB}$), chia làm nhiều đợt (chunks) 10000 đỉnh.
- **Độ phức tạp:** Thời gian: $O\left(\frac{N(N + M)}{64}\right)$, Bộ nhớ: $O\left(\frac{N \times \text{Chunk}}{64}\right)$.

### 13. CSES 2143 - Reachability Queries
- **Link đề bài:** [CSES 2143](https://cses.fi/problemset/task/2143)
- **Tóm tắt:** Cho đồ thị có hướng tổng quát $N, M \le 50000$ và $Q \le 100000$ truy vấn: đỉnh $u$ có đến được đỉnh $v$ hay không.
- **Ý tưởng:**
  1. Dùng thuật toán Tarjan / Kosaraju nén các thành phần liên thông mạnh (SCC) thành một Đồ thị có hướng không chu trình (DAG).
  2. Áp dụng `std::bitset` trên DAG của các SCC như bài 12.
  3. Trả lời mỗi truy vấn trong $O(1)$ bằng cách kiểm tra: `reach[scc[u]].test(scc[v])`.
- **Độ phức tạp:** Thời gian: $O\left(V + E + \frac{V_{scc}^2}{64} + Q\right)$, Bộ nhớ: $O\left(\frac{V_{scc}^2}{64}\right)$.

### 14. AtCoder abc258_g - Triangle
- **Link đề bài:** [AtCoder abc258_g](https://atcoder.jp/contests/abc258/tasks/abc258_g)
- **Tóm tắt:** Cho đồ thị vô hướng $N \le 3000$ đỉnh dưới dạng ma trận kề nhị phân. Đếm số bộ ba $(i, j, k)$ với $i < j < k$ sao cho có cạnh nối giữa cả 3 đỉnh.
- **Ý tưởng:**
  - Lưu hàng thứ $i$ của ma trận kề bằng `bitset<3005> adj[3005]`.
  - Duyệt qua mọi cặp cạnh $(i, j)$ với $i < j$. Nếu có cạnh nối $i - j$, số đỉnh $k > j$ tạo tam giác với $i, j$ chính là số bit 1 trong:
    $$(\text{adj}[i] \ \& \ \text{adj}[j]) \text{ sau vị trí } j$$
  - Tạo mặt nạ `mask` bỏ qua các đỉnh $\le j$: `(adj[i] & adj[j] & (all_ones << (j + 1))).count()`.
- **Độ phức tạp:** Thời gian: $O\left(\frac{N^3}{64}\right) \approx \frac{2.7 \times 10^{10}}{64} \approx 4.2 \times 10^8$ thao tác (vừa vặn Time Limit 3.0s).

### 15. Codeforces 1093E - Intersection of Permutations
- **Link đề bài:** [Codeforces 1093E](https://codeforces.com/problemset/problem/1093/E)
- **Tóm tắt:** Đếm số phần tử chung nằm trong khoảng chỉ số $[l_a, r_a]$ của hoán vị $A$ và $[l_b, r_b]$ của hoán vị $B$.
- **Ý tưởng:** Áp dụng kỹ thuật chia khối (Square Root Decomposition) kết hợp `std::bitset` để tính giao của hai tập hợp chỉ số trong thời gian cực nhanh.
- **Độ phức tạp:** Thời gian: $O\left(\frac{Q \cdot N}{\sqrt{N} \cdot 64}\right)$.

### 16. CSES 1654 - Bit Problem
- **Link đề bài:** [CSES 1654](https://cses.fi/problemset/task/1654)
- **Tóm tắt:** Cho $N \le 2 \times 10^5$ số nguyên $x_i < 2^{20}$. Với mỗi số $x$, đếm:
  1. Số lượng $y$ sao cho $x \ \vert \ y = x$ ($y$ là tập con của $x$).
  2. Số lượng $y$ sao cho $x \ \& \ y = x$ ($y$ là tập cha của $x$).
  3. Số lượng $y$ sao cho $x \ \& \ y \ne 0$.
- **Ý tưởng:** Sử dụng thuật toán **SOS DP (Sum Over Subsets Dynamic Programming)** trên không gian $2^{20}$:
  - $F[\text{mask}][i]$ là tổng giá trị của các tập con chỉ khác `mask` ở $i$ bit đầu tiên.
  - Phân tích phần bù (De Morgan) để giải quyết truy vấn $x \ \& \ y \ne 0$.
- **Độ phức tạp:** Thời gian: $O(20 \cdot 2^{20}) \approx 2 \times 10^7$ thao tác, Bộ nhớ: $O(2^{20})$.

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
