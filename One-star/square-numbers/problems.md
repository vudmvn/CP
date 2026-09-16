# Tuyển tập bài tập: Số chính phương & Số không chứa thừa số chính phương (Square-Free Numbers)

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**Codeforces, CSES, AtCoder, VNOJ, LeetCode, SPOJ**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm hướng dẫn phương pháp giải chi tiết.

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo kiểm tra số chính phương an toàn số 64-bit, kỹ thuật đếm số chính phương trong đoạn $[L, R]$, và bản chất ước số lẻ.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Bulb Switcher**](https://leetcode.com/problems/bulb-switcher/) | LeetCode 319 | Ước số lẻ / Math | Cho $N$ bóng đèn bật tắt qua $N$ vòng. Bóng thứ $k$ sáng khi và chỉ khi số ước của $k$ là số lẻ $\iff k$ là số chính phương. Đáp án là $\lfloor \sqrt{N} \rfloor$ trong $O(1)$. |
| 2 | [**Valid Perfect Square**](https://leetcode.com/problems/valid-perfect-square/) | LeetCode 367 | Binary Search / Math | Kiểm tra số nguyên dương $n$ có phải là số chính phương hay không mà không dùng hàm căn bậc hai có sẵn. Dùng Chặt nhị phân trên đoạn $[1, n]$ với $mid \times mid == n$. |
| 3 | [**Đếm số chính phương trong đoạn**](https://oj.vnoi.info/problem/fcb015_squares) | VNOJ | Math / Range Query | Đếm số lượng số chính phương trong đoạn $[A, B]$ với $A, B \le 10^{18}$. Số chính phương $k^2 \in [A, B] \iff \lceil \sqrt{A} \rceil \le k \le \lfloor \sqrt{B} \rfloor$. Đáp án là $\max(0LL, \lfloor \sqrt{B} \rfloor - \lceil \sqrt{A} \rceil + 1)$. Dùng `integer_sqrt` an toàn sai số. |
| 4 | [**Square String?**](https://codeforces.com/problemset/problem/1619/A) | Codeforces 1619A | Implementation / String | Chuỗi $S$ được gọi là chuỗi chính phương nếu nó được tạo thành từ một chuỗi lặp lại 2 lần: $S = T + T$. Kiểm tra độ dài $\vert S \vert$ có chẵn không và nửa đầu có trùng nửa sau không. |
| 5 | [**Almost Prime**](https://codeforces.com/problemset/problem/26/A) | Codeforces 26A | Sieve / Number Theory | Đếm các số $\le N$ có đúng 2 ước nguyên tố phân biệt. Dùng sàng số nguyên tố đếm số ước nguyên tố phân biệt của từng số trong $O(N \log \log N)$. |

---

## 🟡 Cấp độ 2: Vận dụng & Biến thể đặc biệt (Medium / Intermediate)
*Mục tiêu*: Sử dụng phân rã Square-Free Core $\text{core}(u) = \text{core}(v)$ để đếm cặp tích chính phương và chia đoạn tham lam.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Square-free division (easy version)**](https://codeforces.com/problemset/problem/1497/E1) | Codeforces 1497E1 | Square-Free Core / Greedy | Chia mảng thành ít đoạn nhất sao cho trong mỗi đoạn không có cặp nào có tích là số chính phương. Tích $A_i \cdot A_j = k^2 \iff \text{core}(A_i) = \text{core}(A_j)$. Thay mỗi số bằng core của nó, bài toán quy về chia mảng thành ít đoạn nhất không chứa phần tử trùng lặp (dùng `set` tham lam). |
| 7 | [**Count Square Pairs**](https://atcoder.jp/contests/abc254/tasks/abc254_d) | AtCoder ABC 254 D | Square-Free Core / Math | Đếm số cặp $(i, j) \in [1, N]^2$ sao cho $i \times j$ là số chính phương. Phân tích $i = \text{core}(i) \cdot a^2$, khi đó $j$ phải có dạng $\text{core}(i) \cdot b^2$. Với mỗi $i$, số lượng $j \le N$ hợp lệ chính là số lượng số $b$ sao cho $\text{core}(i) \cdot b^2 \le N \implies b \le \lfloor \sqrt{N / \text{core}(i)} \rfloor$. |
| 8 | [**Four Divisors**](https://leetcode.com/problems/four-divisors/) | LeetCode 1390 | Phân tích thừa số | Số có đúng 4 ước số chỉ có 2 dạng: $n = p^3$ ($p$ nguyên tố) hoặc $n = p \cdot q$ ($p \neq q$ nguyên tố, dạng square-free). Kiểm tra và tính tổng ước trong $O(\sqrt{n})$. |
| 9 | [**Square-Free Integers**](https://www.spoj.com/problems/SQFREE/) | SPOJ SQFREE | Hàm Möbius / Bao hàm bù trừ | Đếm số lượng số square-free trong đoạn $[1, N]$ với $N \le 10^{14}$. Áp dụng công thức $\sum_{k=1}^{\lfloor \sqrt{N} \rfloor} \mu(k) \lfloor N / k^2 \rfloor$. Sàng hàm $\mu$ tới $10^7$ trong $O(\sqrt{N})$. |
| 10 | [**Product of Three Numbers**](https://codeforces.com/problemset/problem/1294/C) | Codeforces 1294C | Greedy / Number Theory | Phân tích $N$ thành tích 3 số nguyên dương phân biệt $a \cdot b \cdot c > 1$. Tham lam tìm ước nhỏ nhất $a \ge 2$, sau đó tìm ước nhỏ nhất $b > a$ của $N / a$, và đặt $c = N / (a \cdot b)$. Kiểm tra $c > b$ và $c > 1$. |
| 11 | [**Square Difference**](https://codeforces.com/problemset/problem/1033/B) | Codeforces 1033B | Hằng đẳng thức đáng nhớ | Kiểm tra $a^2 - b^2$ có phải số nguyên tố không ($a, b \le 10^{11}$). Phân tích $a^2 - b^2 = (a - b)(a + b)$. Vì đây là số nguyên tố, bắt buộc thừa số nhỏ hơn phải bằng 1 $\iff a - b = 1$. Khi đó kiểm tra xem $a + b = 2a - 1$ có là số nguyên tố không. |

---

## 🔴 Cấp độ 3: Nâng cao & Bitmask Parity (Hard / Advanced ICPC)
*Mục tiêu*: Kết hợp Quy hoạch động, Chặt nhị phân trên hàm Möbius, và Biểu diễn Bitmask Parity giải quyết hệ phương trình số mũ.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**Square-free division (hard version)**](https://codeforces.com/problemset/problem/1497/E2) | Codeforces 1497E2 | Two Pointers + DP | Giống bản Easy nhưng cho phép sửa tối đa $k \le 20$ phần tử trong mảng. Đặt $dp[i][j]$ là số đoạn ít nhất để phân hoạch tiền tố $i$ dùng không quá $j$ lần sửa. Kết hợp Two Pointers tìm vị trí bắt đầu xa nhất cho từng số lần vi phạm trong $O(N \cdot k^2)$. |
| 13 | [**Square Subsets**](https://codeforces.com/problemset/problem/895/C) | Codeforces 895C | DP Bitmask / Gaussian Basis | Cho mảng $N$ số với $A_i \le 70$. Đếm số tập con khác rỗng có tích là một số chính phương. Chỉ có 19 số nguyên tố $\le 70$. Mỗi số $A_i$ được biểu diễn thành một bitmask 19-bit (bit thứ $j$ là parity của số mũ nguyên tố $p_j$). Tích là số chính phương $\iff$ tổng XOR của các mask bằng 0! Áp dụng DP cái túi theo bitmask $O(70 \cdot 2^{19})$ hoặc Đại số tuyến tính F2 Basis trong $O(N + 70 \cdot 19)$. |
| 14 | [**K-th Square-Free Number**](https://codeforces.com/gym/100814) | Codeforces Gym / SPOJ | Binary Search + Möbius | Tìm số square-free thứ $K$ ($K \le 10^{11}$). Chặt nhị phân nghiệm trong khoảng $[1, 2 \cdot 10^{11}]$, kiểm tra số lượng số square-free bằng hàm Möbius trong $O(\sqrt{mid})$. |
| 15 | [**Coprime Subsequences**](https://codeforces.com/problemset/problem/803/F) | Codeforces 803F | Nghịch đảo Möbius / DP | Cho mảng $N$ số ($A_i \le 10^5$). Đếm số dãy con có $\gcd = 1$. Đặt $cnt[g]$ là số phần tử chia hết cho $g$. Số dãy con có mọi phần tử chia hết cho $g$ là $2^{cnt[g]} - 1$. Theo công thức nghịch đảo Möbius: $\text{ans} = \sum_{g=1}^{\max A} \mu(g) (2^{cnt[g]} - 1) \pmod{10^9+7}$. |
| 16 | [**Count Square Subarrays**](https://codeforces.com/) | Codeforces / VNOJ | Hashing / Random XOR Basis | Đếm số đoạn con liên tiếp có tích là số chính phương. Gán mỗi số nguyên tố một mã hash 64-bit ngẫu nhiên. Mảng tiền tố XOR cho phép đưa bài toán về đếm cặp $prefXor[R] == prefXor[L-1]$ bằng `map` trong $O(N \log N)$. |

---

## 💡 Lời khuyên vàng khi giải bài toán Số chính phương & Square-Free

1. **Bảo vệ căn bậc hai nguyên trước sai số số thực**:
   - Khi kiểm tra $n \le 10^{18}$ có phải là số chính phương, tuyệt đối không viết `if (sqrt(n) * sqrt(n) == n)`.
   - Luôn sử dụng hàm `integer_sqrt` điều chỉnh lân cận: $r = \text{round}(\text{sqrtl}((long\ double)n))$, sau đó tinh chỉnh `while ((r+1)*(r+1) <= n) r++; while (r*r > n) r--;`.

2. **Chìa khóa Square-Free Core**:
   - Bất cứ khi nào đề bài yêu cầu "tích 2 số là số chính phương", hãy chuyển đổi mảng thành mảng các hạt nhân $\text{core}(A_i)$.
   - Bài toán lập tức biến thành bài toán đếm các phần tử có giá trị bằng nhau: $\text{core}(u) = \text{core}(v)$!

3. **Chuyển đổi bài toán tích chính phương sang Đại số tuyến tính XOR ($\mathbb{F}_2$)**:
   - Khi bài toán yêu cầu tìm tập con có tích là số chính phương với $A_i$ nhỏ ($\le 70$), phân tích mỗi số thành vector số mũ modulo 2.
   - Phép nhân các số tương đương với **phép cộng XOR các vector bit**.
   - Bài toán quy về tìm số nghiệm của hệ phương trình thuần nhất trên trường $\mathbb{F}_2$: Nếu có $N$ vector và không gian sinh có rank là $R$, số tập con hợp lệ luôn là $2^{N - R} - 1$.

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

