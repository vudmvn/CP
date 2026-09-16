# Tuyển tập bài tập: Ước chung lớn nhất (GCD), Bội chung nhỏ nhất (LCM) & Thuật toán Euclid

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, Codeforces, AtCoder, VNOJ, LeetCode**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm hướng dẫn phương pháp giải chi tiết.

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo tính toán GCD, LCM an toàn tràn số, tính chất số lẻ/chẵn kề nhau và áp dụng trực tiếp thuật toán Euclid.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Common Divisors**](https://cses.fi/problemset/task/1081) | CSES | Sieve GCD / Đếm bội | Cho mảng $N$ số ($A_i \le 10^6$). Tìm cặp $(i \neq j)$ có $gcd(A_i, A_j)$ lớn nhất. Đếm tần số xuất hiện của các phần tử, duyệt $g$ từ $10^6$ về $1$ và đếm số bội của $g$ trong $O(M \ln M)$. |
| 2 | [**Maximum GCD**](https://codeforces.com/problemset/problem/1370/A) | Codeforces 1370A | Math / Quy luật | Tìm cặp số $1 \le a < b \le n$ có $gcd(a, b)$ lớn nhất. Để GCD lớn nhất, chọn $b$ chẵn lớn nhất $\le n$ và $a = b / 2$. Đáp án luôn là $\lfloor n / 2 \rfloor$. Độ phức tạp $O(1)$. |
| 3 | [**Relatively Prime Pairs**](https://codeforces.com/problemset/problem/1051/B) | Codeforces 1051B | Number Theory | Cho đoạn $[L, R]$ có độ dài chẵn $R - L + 1$. Ghép toàn bộ thành các cặp nguyên tố cùng nhau. Nhận xét: Hai số tự nhiên liên tiếp luôn nguyên tố cùng nhau ($gcd(k, k+1) = 1$). Ghép các cặp $(2i, 2i+1)$. |
| 4 | [**Coprime**](https://codeforces.com/problemset/problem/1742/D) | Codeforces 1742D | Math / Bảng ước | Tìm cặp $(i, j)$ có $gcd(A_i, A_j) = 1$ sao cho $i + j$ lớn nhất. Vì $A_i \le 1000$, lưu chỉ số lớn nhất của từng giá trị trong mảng $idx[x]$. Duyệt mọi cặp giá trị $(x, y) \in [1, 1000]$ có $gcd(x, y) = 1$ để cập nhật $\max(idx[x] + idx[y])$. |
| 5 | [**Coprime Array**](https://codeforces.com/problemset/problem/660/A) | Codeforces 660A | Greedy / Construction | Chèn ít số nhất vào giữa các phần tử liền kề của mảng để mọi cặp kề nhau đều nguyên tố cùng nhau. Nếu $gcd(A_i, A_{i+1}) > 1$, chỉ cần chèn đúng số $1$ vào giữa vì $gcd(A_i, 1) = 1$. |

---

## 🟡 Cấp độ 2: Vận dụng & Biến thể đặc biệt (Medium / Intermediate)
*Mục tiêu*: Làm chủ bổ đề biến đổi hiệu $gcd(a, b) = gcd(a, b-a)$, rút gọn phân số, Prefix & Suffix GCD, và ứng dụng tính chất $O(\log)$ của GCD tiền tố.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Row GCD**](https://codeforces.com/problemset/problem/1458/A) | Codeforces 1458A | Bổ đề biến đổi hiệu | Cho 2 mảng $A$ ($N$ phần tử) và $B$ ($M$ phần tử). Với mỗi $B_j$, tính $\gcd(A_1 + B_j, \dots, A_n + B_j)$. Áp dụng $\gcd(x, y) = \gcd(x, \lvert y - x \rvert)$, biến đổi thành $\gcd(A_1 + B_j, G)$ với $G = \gcd(\lvert A_2 - A_1 \rvert, \dots, \lvert A_n - A_1 \rvert)$. Tiền xử lý $G$ trong $O(N)$, trả lời mỗi query $O(\log)$. |
| 7 | [**Diluc and Kaeya**](https://codeforces.com/problemset/problem/1536/C) | Codeforces 1536C | Tỉ lệ tối giản / Hash Map | Chia tiền tố thành nhiều đoạn con liên tiếp nhất có cùng tỉ lệ `'D'` : `'K'`. Rút gọn tỉ lệ về dạng tối giản $\left(\frac{cnt_D}{g}, \frac{cnt_K}{g}\right)$ với $g = \gcd(cnt_D, cnt_K)$. Dùng `map` đếm số lần xuất hiện của cặp tỉ số tối giản này. |
| 8 | [**GCD on Blackboard**](https://atcoder.jp/contests/abc125/tasks/abc125_c) | AtCoder ABC 125 C | Prefix & Suffix GCD | Thay thế đúng 1 số trong mảng để GCD của mảng là lớn nhất. Xây dựng mảng `prefGCD` và `suffGCD`. Khi loại bỏ phần tử $A_i$, GCD của mảng còn lại là $\gcd(\text{prefGCD}[i-1], \text{suffGCD}[i+1])$. Độ phức tạp $O(N \log(\max A))$. |
| 9 | [**Nastia and a Good Array**](https://codeforces.com/problemset/problem/1521/B) | Codeforces 1521B | Number Theory / Construction | Biến đổi mảng sao cho mọi cặp kề nhau có $\gcd = 1$. Tìm phần tử nhỏ nhất trong mảng $A_{pos}$, biến đổi các vị trí khác thành $A_{pos} + \lvert i - pos \rvert$, đảm bảo hai số kề nhau hơn kém nhau 1 đơn vị nên luôn nguyên tố cùng nhau. |
| 10 | [**Array Elimination**](https://codeforces.com/problemset/problem/1602/C) | Codeforces 1602C | Bitwise + GCD | Tìm tất cả các số $K$ để có thể đưa toàn bộ mảng về 0 bằng thao tác trừ bitwise AND trên $K$ phần tử. Đếm số lượng bit 1 tại mỗi vị trí từ 0 đến 29: $C_b$. Thao tác thực hiện được khi và chỉ khi $K$ là ước chung của tất cả các $C_b > 0$. Lấy $gcd$ của tất cả các $C_b$. |
| 11 | [**Common Divisors (VNOJ)**](https://oj.vnoi.info/problem/fcb003_gcd) | VNOJ | Phân tích ước số | Tìm số ước chung của một tập hợp các số. Số ước chung của dãy $A_1, \dots, A_n$ chính là số ước của $G = gcd(A_1, \dots, A_n)$. Tính $G$ rồi đếm ước trong $O(\sqrt{G})$. |

---

## 🔴 Cấp độ 3: Nâng cao & Đại số vô định (Hard / Advanced ICPC)
*Mục tiêu*: Thuần thục thuật toán Euclid mở rộng, giải phương trình Diophantine tuyến tính, đếm đoạn con có GCD bằng $X$ trong $O(N \log(\max A))$.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**The Football Season**](https://codeforces.com/problemset/problem/1244/C) | Codeforces 1244C | Linear Diophantine | Đội bóng thắng $x$ trận (mỗi trận được $w$ điểm), hòa $y$ trận (mỗi trận được $d$ điểm), thua $z$ trận, tổng cộng có $n$ trận và $p$ điểm: $w \cdot x + d \cdot y = p$ với $x + y \le n$. Dùng Extended GCD giải nghiệm riêng, dịch nghiệm $y \in [0, w-1]$ để tìm nghiệm không âm. |
| 13 | [**CGCDSSQ**](https://codeforces.com/problemset/problem/475/D) | Codeforces 475D | GCD Subarrays / Log Steps | Cho mảng $N$ số và $Q$ truy vấn: Có bao nhiêu đoạn con liên tiếp có $gcd = X$? Dùng tính chất: Danh sách GCD của các đoạn con kết thúc tại vị trí $i$ chỉ có tối đa $\log_2(\max A) \le 30$ giá trị phân biệt. Tiền xử lý đếm toàn bộ đoạn con bằng bảng băm trong $O(N \log^2(\max A))$ và trả lời mỗi truy vấn trong $O(1)$. |
| 14 | [**GCD - Extreme (I & II)**](https://www.spoj.com/problems/GCDEX/) | SPOJ / VNOJ | Euler's Totient + Sieve | Tính $\sum_{i=1}^{N-1} \sum_{j=i+1}^N gcd(i, j)$ với $N \le 10^6$. Đổi biến theo ước chung $g$: $\sum_{g=1}^N g \cdot \sum_{k=2}^{\lfloor N/g \rfloor} \varphi(k)$. Tiền xử lý mảng phi hàm Euler bằng sàng nguyên tố và mảng tiền tố trong $O(N \log N)$. |
| 15 | [**Prefix GCD**](https://atcoder.jp/contests/agc018/tasks/agc018_c) | AtCoder AGC | Greedy / GCD Properties | Sắp xếp mảng để tổng các GCD tiền tố là nhỏ nhất hoặc lớn nhất. Tận dụng tính chất GCD tiền tố giảm theo bội số để tham lam chọn phần tử tiếp theo làm giảm GCD nhanh nhất. |
| 16 | [**Equator**](https://codeforces.com/problemset/problem/962/A) | Codeforces | Diophantine Boundaries | Mô hình hóa các bài toán cân bằng tài nguyên, giao điểm bước nhảy trên đường tròn modulo $N$ bằng nghiệm của phương trình đồng dư $a \cdot x \equiv b \pmod M$. |

---

## 💡 Lời khuyên vàng khi giải bài toán GCD & LCM

1. **Cẩn trọng tuyệt đối với phép nhân LCM**:
   - Biểu thức `a * b / gcd(a, b)` luôn tiềm ẩn nguy cơ Overflow `long long`. Luôn viết `(a / std::gcd(a, b)) * b`.
   - Nếu phải tính LCM của nhiều hơn 2 số, hãy kiểm tra ngưỡng giới hạn (Threshold) trước khi nhân; nếu vượt quá giới hạn đề bài, gán ngay giá trị vô cùng ($\infty$) để tránh tràn số vòng tròn âm.

2. **Quy tắc bất biến hiệu trong GCD**:
   - Khi gặp biểu thức có dạng $gcd(A_1 + x, A_2 + x, \dots, A_n + x)$, ngay lập tức nghĩ tới việc trừ bớt phần tử đầu tiên $A_1 + x$:
     $$gcd(A_1 + x, |A_2 - A_1|, |A_3 - A_1|, \dots, |A_n - A_1|)$$
   - Kỹ thuật này giúp tách rời biến số $x$ ra khỏi phần mảng tĩnh, giảm độ phức tạp từ $O(N \log)$ xuống $O(\log)$ cho mỗi truy vấn.

3. **Tận dụng tính chất giảm nhanh của GCD tiền tố**:
   - Khi nới rộng một đoạn con sang phải, giá trị GCD của đoạn chỉ có thể giữ nguyên hoặc giảm đi ít nhất 2 lần.
   - Vì thế, trên bất kỳ mảng nào, số lượng giá trị GCD khác nhau của các đoạn con bắt đầu tại một vị trí $L$ cố định không bao giờ vượt quá $60$. Điều này cho phép kết hợp Binary Search + Sparse Table để "nhảy cóc" qua các đoạn có cùng giá trị GCD.

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

