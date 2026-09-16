# Tuyển tập bài tập: Mảng tiền tố, Hậu tố & Mảng hiệu

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, VNOJ, Codeforces, LeetCode, AtCoder**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm phân tích ý tưởng (hints/editorials).

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo mảng cộng dồn 1D, 2D và mảng tiền tố phép toán XOR.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Static Range Sum Queries**](https://cses.fi/problemset/task/1646) | CSES | 1D Prefix Sum | Cho dãy $N$ số và $Q$ truy vấn tổng đoạn $[L, R]$. Tiền xử lý $P[i] = P[i-1] + A[i]$, trả lời mỗi truy vấn trong $O(1)$ bằng $P[R] - P[L-1]$. Lưu ý dùng `long long`. |
| 2 | [**Forest Queries**](https://cses.fi/problemset/task/1652) | CSES | 2D Prefix Sum | Cho lưới rừng $N \times N$ với các cây `*`. Tính số cây trong hình chữ nhật con $[y_1, x_1]$ đến $[y_2, x_2]$. Xây dựng mảng cộng dồn 2D và truy vấn trong $O(1)$ bằng nguyên lý bù trừ. |
| 3 | [**Range Xor Queries**](https://cses.fi/problemset/task/1650) | CSES | Prefix XOR | Truy vấn tổng XOR trên đoạn $[L, R]$. Xây dựng mảng $X[i] = X[i-1] \oplus A[i]$, trả lời trong $O(1)$ bằng $X[R] \oplus X[L-1]$. |
| 4 | [**Range Sum Query - Immutable**](https://leetcode.com/problems/range-sum-query-immutable/) | LeetCode 303 | 1D Prefix Sum | Thiết kế cấu trúc dữ liệu trả lời tổng đoạn trong $O(1)$. Khởi tạo mảng tiền tố kích thước $N + 1$. |
| 5 | [**CSUM - Tính tổng tiền tố**](https://oj.vnoi.info/problem/csum) | VNOJ | 1D Prefix Sum | Áp dụng mảng tiền tố cơ bản để xử lý hàng loạt truy vấn tính tổng. |

---

## 🟡 Cấp độ 2: Vận dụng & Biến thể đặc biệt (Medium / Intermediate)
*Mục tiêu*: Sử dụng Prefix Sum kết hợp Hash Map, mảng hiệu 1D, Prefix/Suffix Max, và Prefix/Suffix GCD.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Subarray Sums II**](https://cses.fi/problemset/task/1661) | CSES | Prefix Sum + Hash Map | Đếm số đoạn con có tổng bằng $X$ (mảng có thể chứa số âm). Đưa về đếm cặp $(L, R)$ thỏa $P[R] - P[L-1] = X \iff P[L-1] = P[R] - X$. Dùng `map<long long, int>` đếm trong $O(N \log N)$. |
| 7 | [**Subarray Divisibility**](https://cses.fi/problemset/task/1662) | CSES | Prefix Sum Modulo $N$ | Đếm số đoạn con có tổng chia hết cho $N$. Điều kiện: $P[R] \equiv P[L-1] \pmod N$. Đếm tần số xuất hiện của các số dư $(P[i] \bmod N + N) \bmod N$, cộng dồn $\binom{\text{cnt}}{2}$. |
| 8 | [**GCD on Blackboard**](https://atcoder.jp/contests/abc125/tasks/abc125_c) | AtCoder | Prefix & Suffix GCD | Thay thế đúng 1 số trong $N$ số để $\gcd$ của mảng là lớn nhất. Xây dựng mảng `prefGCD` và `suffGCD`, duyệt bỏ từng số $i$, $\gcd$ còn lại là $\gcd(\text{prefGCD}[i-1], \text{suffGCD}[i+1])$ trong $O(1)$. |
| 9 | [**Trapping Rain Water**](https://leetcode.com/problems/trapping-rain-water/) | LeetCode 42 | Prefix & Suffix Max | Tính lượng nước mưa hứng được giữa các thanh đứng. Tại mỗi vị trí $i$, lượng nước bằng $\min(\text{prefMax}[i], \text{suffMax}[i]) - H[i]$. |
| 10 | [**Karen and Coffee**](https://codeforces.com/problemset/problem/816/B) | Codeforces | 1D Difference Array | Cho $N$ công thức cà phê khuyên dùng nhiệt độ trong đoạn $[l_i, r_i]$. Dùng mảng hiệu đánh dấu $+1$ tại $l_i$ và $-1$ tại $r_i + 1$, sau đó dùng mảng tiền tố đếm số nhiệt độ đạt chuẩn. |
| 11 | [**Greg and Array**](https://codeforces.com/problemset/problem/295/A) | Codeforces | Mảng hiệu 2 tầng | Cho $M$ thao tác cộng đoạn, và $K$ truy vấn thực hiện lại các thao tác từ $x$ đến $y$. Dùng mảng hiệu thứ nhất đếm số lần mỗi thao tác được thực hiện, sau đó dùng mảng hiệu thứ hai cập nhật lên mảng gốc. |

---

## 🔴 Cấp độ 3: Nâng cao & 2D Grid (Hard / Advanced ICPC)
*Mục tiêu*: Làm chủ mảng hiệu 2D trên ma trận, mảng cộng dồn trên cây và kỹ thuật dồn trạng thái.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**Range Sum Query 2D**](https://leetcode.com/problems/range-sum-query-2d-immutable/) | LeetCode 304 | 2D Prefix Sum | Thiết kế cấu trúc truy vấn hình chữ nhật con $O(1)$ cho ma trận tĩnh $M \times N$. |
| 13 | [**Two-dimensional Difference**](https://atcoder.jp/contests/abc014/tasks/abc014_3) | AtCoder | 2D Difference Array | Cho nhiều hình chữ nhật cập nhật cộng dồn giá trị. Dùng mảng hiệu 2D đánh dấu 4 góc trong $O(1)$, sau đó chạy 2D prefix sum để khôi phục trong $O(R \times C)$. |
| 14 | [**Nuske vs Elodea**](https://atcoder.jp/contests/arc077/tasks/arc077_c) | AtCoder | 2D Prefix Sum, Đồ thị phẳng | Đếm số thành phần liên thông trong rừng hình chữ nhật con. Áp dụng công thức Euler cho rừng cây: $\text{Số TPLT} = V - E$. Đếm số đỉnh $V$ và số cạnh ngang/dọc $E$ bằng mảng cộng dồn 2D. |
| 15 | [**Good Subarrays**](https://codeforces.com/problemset/problem/1398/C) | Codeforces | Prefix Sum biến đổi | Đếm số đoạn con có tổng bằng độ dài: $\sum_{i=L}^R A_i = R - L + 1 \iff P[R] - P[L-1] = R - L + 1 \iff P[R] - R = P[L-1] - (L-1)$. Đặt $B_i = P_i - i$, bài toán quy về tìm số cặp $B[R] = B[L-1]$. |

---

## 💡 Lời khuyên vàng khi cài đặt Mảng tiền tố & Mảng hiệu
1. **Luôn dùng 1-indexed**: Bắt đầu mảng từ chỉ số $1$ với $P[0] = 0$. Điều này giúp loại bỏ hoàn toàn các câu lệnh kiểm tra biên `if (L == 0)` và triệt tiêu lỗi `IndexOutOfBounds`.
2. **Kích thước mảng hiệu**: Luôn khai báo kích thước mảng hiệu tối thiểu là $N + 2$ (hoặc $(R + 2) \times (C + 2)$ với 2D) để phép cập nhật `D[r + 1] -= V` không bị tràn mảng khi $r = N$.
3. **Mảng tiền tố của Mảng hiệu = Mảng ban đầu**: Mối quan hệ đối ngẫu này là nền tảng cho hàng trăm bài toán xử lý truy vấn đoạn trong ICPC.

---

<div align="center">

<img src="../../assets/logos/NEU_Logo.png" alt="NEU Logo" height="50" />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<img src="../../assets/logos/FDA_Logo.png" alt="FDA Logo" height="50" />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<img src="../../assets/logos/SLSCM_logo.jpg" alt="SLSCM Logo" height="50" />

<br/><br/>

**Competitive Programming Handbook**  
*SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA) — National Economics University (NEU)*  
*Tài liệu được soạn thảo và tối ưu bởi Agentic AI tool*  
© 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.

</div>

