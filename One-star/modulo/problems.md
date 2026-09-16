# Tuyển tập bài tập: Số học Modulo & Nghịch đảo Modulo

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nguồn uy tín (**CSES, VNOJ, Codeforces, AtCoder, ICPC Regional**) và được phân cấp từ **Cơ bản** đến **Nâng cao**, kèm phân tích ý tưởng (Idea/Editorial hints).

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo các phép tính cộng, trừ, nhân, lũy thừa nhị phân, xử lý số âm và tránh tràn số.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Exponentiation**](https://cses.fi/problemset/task/1095) | CSES | Binary Exponentiation | Tính $a^b \pmod{10^9+7}$. Sử dụng lũy thừa nhị phân lặp bit trong $O(\log b)$. Chú ý trường hợp biên $0^0 = 1$. |
| 2 | [**Đếm đi các bạn ơiiii**](https://oj.vnoi.info/problem/bedao_m22_b) | VNOJ | Phép chia & Modulo | Ứng dụng quy tắc chia hết và tính chất đồng dư để đếm số lượng phần tử thỏa mãn. |
| 3 | [**Competitive Programmer**](https://codeforces.com/problemset/problem/1266/A) | Codeforces | Tính chất chia hết | Kiểm tra hoán vị các chữ số có chia hết cho $60$ (tức chia hết cho $2, 3, 4, 5$). Dùng quy tắc tổng các chữ số và chữ số tận cùng. |
| 4 | [**Phi hàm Euler (ETF)**](https://oj.vnoi.info/problem/etf) | VNOJ / SPOJ | Euler Totient Function | Tính $\varphi(n)$ với $n \le 10^6$. Phân tích thừa số nguyên tố $n = p_1^{a_1} \dots p_k^{a_k}$ và áp dụng công thức tích $\varphi(n) = n \prod (1 - 1/p_i)$ trong $O(\sqrt{n})$. |
| 5 | [**Creating Strings II**](https://cses.fi/problemset/task/1715) | CSES | Combinatorics, Modulo Inverse | Đếm số hoán vị khác nhau của chuỗi: $\frac{N!}{c_a! c_b! \dots c_z!} \pmod{10^9+7}$. Tiền xử lý giai thừa và nhân nghịch đảo bằng Fermat nhỏ. |
| 6 | [**Distributing Apples**](https://cses.fi/problemset/task/1716) | CSES | Bài toán chia kẹo Euler | Chia $m$ quả táo cho $n$ đứa trẻ: Công thức tổ hợp lặp $\binom{n+m-1}{m} \pmod{10^9+7}$. |

---

## 🟡 Cấp độ 2: Vận dụng & Trung cấp (Medium / Intermediate)
*Mục tiêu*: Sử dụng nghịch đảo modulo khi chia, Fermat nhỏ trên số mũ, giải phương trình đồng dư tuyến tính với Euclid mở rộng.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 7 | [**Exponentiation II**](https://cses.fi/problemset/task/1712) | CSES | Fermat nhỏ, Số mũ tầng tháp | Tính $a^{b^c} \pmod{10^9+7}$. Cần tính số mũ $E = b^c \pmod{10^9+6}$ trước, sau đó mới tính $a^E \pmod{10^9+7}$. Chú ý trường hợp $a=0, b=0, c=0$. |
| 8 | [**Số nguyên**](https://oj.vnoi.info/problem/integer7) | VNOJ | Extended Euclid, Diophantus | Giải phương trình đồng dư tuyến tính $ax \equiv b \pmod m$ hoặc $ax + by = c$. Tìm nghiệm nguyên dương nhỏ nhất. |
| 9 | [**Throne**](https://atcoder.jp/contests/abc186/tasks/abc186_e) | AtCoder | Linear Congruence Equation | Tìm bước nhảy để về vị trí 0 trên vòng tròn $N$ ghế: giải $(S + K \cdot x) \equiv 0 \pmod N \iff K \cdot x \equiv -S \pmod N$. Rút gọn với $\gcd(K, N)$ rồi dùng nghịch đảo modulo. |
| 10 | [**Divisor Analysis**](https://cses.fi/problemset/task/2182) | CSES | Phân tích ước số, Fermat nhỏ | Cho $N = \prod p_i^{k_i}$. Tính số lượng ước số, tổng các ước số và tích các ước số modulo $10^9+7$. Riêng tích các ước số có số mũ cần được tính modulo $(10^9+6)$ hoặc chia đôi số mũ cẩn thận. |
| 11 | [**Christmas Party**](https://cses.fi/problemset/task/1717) | CSES | Số Derangement (Hoán vị mất tiêu) | Đếm số cách tặng quà sao cho không ai nhận quà của chính mình: Công thức truy hồi $D_n = (n-1)(D_{n-1} + D_{n-2}) \pmod{10^9+7}$. |
| 12 | [**Beautiful Numbers**](https://codeforces.com/problemset/problem/300/C) | Codeforces | Tổ hợp, Modulo | Đếm số lượng số đẹp có $n$ chữ số ($n \le 10^6$) chỉ gồm 2 chữ số $a, b$. Duyệt số lượng chữ số $a$ từ $0$ đến $n$, kiểm tra tổng các chữ số và cộng dồn $\binom{n}{k} \pmod{10^9+7}$. |
| 13 | [**Tổ hợp chẵn**](https://oj.vnoi.info/problem/bedao_oi4_b) | VNOJ | Tính chất đồng dư tổ hợp | Vận dụng tính chất modulo 2 hoặc định lý Lucas để xác định tính chẵn lẻ của các hệ số nhị thức. |

---

## 🔴 Cấp độ 3: Nâng cao & Thách thức (Hard / Advanced ICPC)
*Mục tiêu*: Áp dụng Định lý Thặng dư Trung Hoa (CRT / ExCRT), Định lý Lucas, Baby-step Giant-step, Modulo không nguyên tố.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 14 | [**Remainders Game**](https://codeforces.com/problemset/problem/687/B) | Codeforces | BCNN, CRT, Tính chất đồng dư | Kiểm tra xem từ các số dư $x \bmod c_i$ có thể suy ra $x \bmod k$ duy nhất hay không. Quy về kiểm tra $k \mid \operatorname{lcm}(c_1, c_2, \dots, c_n)$. |
| 15 | [**Congruence Equations**](https://codeforces.com/problemset/problem/919/E) | Codeforces | CRT, Fermat nhỏ, Chu kỳ | Giải phương trình $n \cdot a^n \equiv b \pmod p$ với $n \le x$. Nhận xét: $n \pmod p$ có chu kỳ $p$, còn $a^n \pmod p$ có chu kỳ $p-1$. Vì $\gcd(p, p-1) = 1$, dùng CRT gộp 2 chu kỳ thành một chu kỳ độ dài $p(p-1)$. |
| 16 | [**Two Chandeliers**](https://codeforces.com/problemset/problem/1500/B) | Codeforces | Extended CRT, Chặt nhị phân | Hai vòng đèn chu kỳ $n$ và $m$. Tìm ngày thứ $k$ mà hai đèn khác màu nhau. Dùng Extended CRT để tìm các ngày trùng màu trong một chu kỳ $\operatorname{lcm}(n, m)$, kết hợp chặt nhị phân kết quả. |
| 17 | [**Two Arithmetic Progressions**](https://codeforces.com/problemset/problem/710/D) | Codeforces | ExCRT, Diophantus | Tìm số lượng số nguyên thuộc đoạn $[l, r]$ thuộc đồng thời hai cấp số cộng $a_1 \cdot k + b_1$ và $a_2 \cdot m + b_2$. Đưa về giải hệ đồng dư bằng thuật toán Euclid mở rộng. |
| 18 | [**Discrete Logarithm**](https://judge.yosupo.jp/problem/discrete_logarithm_mod) | Library Checker | Baby-step Giant-step | Tìm $x$ nhỏ nhất sao cho $a^x \equiv b \pmod m$. Sử dụng thuật toán Baby-step Giant-step tổng quát hóa cho cả trường hợp $\gcd(a, m) > 1$ trong $O(\sqrt{m})$. |

---

## 💡 Lời khuyên khi luyện tập
1. **Bắt đầu từ Cấp độ 1**: Giải hết các bài CSES Exponentiation, Creating Strings II, Distributing Apples để làm quen với struct `Mint` và hàm `power`.
2. **Chinh phục Cấp độ 2**: Đặc biệt chú ý bài **CSES Exponentiation II** và **Divisor Analysis** để hiểu tường tận sự khác biệt giữa modulo cơ số và modulo số mũ.
3. **Thử sức với Cấp độ 3**: Các bài Codeforces 919E và 1500B là các bài toán kinh điển trong kỳ thi ICPC Regional ứng dụng Định lý Thặng dư Trung Hoa (CRT).

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

