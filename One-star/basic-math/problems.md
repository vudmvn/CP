# Tuyển tập bài tập: Kiến thức Toán học Nền tảng & Đại số Rời rạc

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, Codeforces, AtCoder, VNOJ**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm hướng dẫn phương pháp giải chi tiết.

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo phép chia trần/sàn, kiểm tra tính chẵn lẻ (Parity), áp dụng công thức cấp số cộng và bài toán chia kẹo Euler cơ bản.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Distributing Apples**](https://cses.fi/problemset/task/1715) | CSES | Stars and Bars / Math | Chia $M$ quả táo cho $N$ đứa trẻ. Áp dụng trực tiếp Định lý chia kẹo Euler dạng 1: số cách chia là $\binom{M + N - 1}{N - 1} \pmod{10^9+7}$. Dùng mảng giai thừa và nghịch đảo giai thừa $O(N + M)$. |
| 2 | [**Even Odds**](https://codeforces.com/problemset/problem/318/A) | Codeforces 318A | Parity / Math | Dãy số gồm các số lẻ tăng dần rồi đến các số chẵn tăng dần từ $1$ đến $N$. Số lượng số lẻ là $k_{odd} = (N + 1) / 2$. Nếu vị trí $K \le k_{odd}$, đáp án là $2K - 1$; ngược lại là $2(K - k_{odd})$. Độ phức tạp $O(1)$. |
| 3 | [**Domino piling**](https://codeforces.com/problemset/problem/50/A) | Codeforces 50A | Math / Floor Division | Đặt các thanh domino $2 \times 1$ vào bảng $M \times N$. Mỗi thanh chiếm 2 ô vuông. Số thanh tối đa luôn là phần nguyên sàn $\lfloor (M \times N) / 2 \rfloor$. |
| 4 | [**Candies and Two Sisters**](https://codeforces.com/problemset/problem/1335/A) | Codeforces 1335A | Math / Combinatorics | Chia $n$ chiếc kẹo cho 2 chị em sao cho $a > b > 0$ và $a + b = n$. Số cách chọn $a$ từ $\lfloor n/2 \rfloor + 1$ đến $n-1$, đáp án luôn là $(n - 1) / 2$ bằng phép chia nguyên. |
| 5 | [**Watermelon**](https://codeforces.com/problemset/problem/4/A) | Codeforces 4A | Parity / Math | Chia quả dưa nặng $w$ kg thành 2 phần đều có khối lượng chẵn. Tổng hai số chẵn luôn là số chẵn, do đó $w$ phải là số chẵn và $w > 2$ (vì $2 = 1 + 1$ là hai số lẻ). |

---

## 🟡 Cấp độ 2: Vận dụng & Biến thể đặc biệt (Medium / Intermediate)
*Mục tiêu*: Sử dụng biến đổi Manhattan $\leftrightarrow$ Chebyshev, công thức Dây giày tính diện tích đa giác, và hoán vị lặp.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Polygon Area**](https://cses.fi/problemset/task/2191) | CSES | Shoelace Formula / Geometry | Cho tọa độ $N$ đỉnh của đa giác. Tính $2 \times \text{diện tích}$ của đa giác. Áp dụng trực tiếp công thức Dây giày (Shoelace): $2S = \lvert \sum_{i=1}^n (x_i y_{i+1} - x_{i+1} y_i) \rvert$ dùng kiểu `long long`. |
| 7 | [**Point Location Test**](https://cses.fi/problemset/task/2189) | CSES | Cross Product / Geometry | Kiểm tra điểm $P_3$ nằm bên trái, bên phải hay nằm trên đường thẳng đi qua $P_1 \to P_2$. Tính tích có hướng (Cross Product) của vector $\vec{P_1 P_2} \times \vec{P_1 P_3} = (x_2 - x_1)(y_3 - y_1) - (y_2 - y_1)(x_3 - x_1)$. Dấu của tích quyết định hướng. |
| 8 | [**Creating Strings II**](https://cses.fi/problemset/task/1716) | CSES | Multinomial Combinatorics | Đếm số lượng hoán vị khác nhau của một chuỗi ký tự. Công thức hoán vị lặp: $\frac{N!}{c_a! c_b! \dots c_z!} \pmod{10^9+7}$ với $c_x$ là số lần xuất hiện của ký tự $x$. |
| 9 | [**Max Manhattan Distance**](https://atcoder.jp/contests/abc178/tasks/abc178_e) | AtCoder ABC 178 E | Chebyshev Transform | Cho $N$ điểm, tìm $\max(\lvert x_i - x_j \rvert + \lvert y_i - y_j \rvert)$. Chuyển đổi sang hệ tọa độ Chebyshev $u = x + y, v = x - y$. Khoảng cách lớn nhất là $\max(\max u - \min u, \max v - \min v)$ tính trong $O(N)$. |
| 10 | [**Apple Division**](https://cses.fi/problemset/task/1623) | CSES | Bitmask / Optimization | Chia $N \le 20$ quả táo thành 2 nhóm sao cho chênh lệch tổng khối lượng là nhỏ nhất. Duyệt toàn bộ $2^N$ cấu hình nhị phân bằng bitmask, cập nhật $\min \lvert \text{sum}_1 - \text{sum}_2 \rvert$ trong $O(2^N)$. |
| 11 | [**Number Spiral**](https://cses.fi/problemset/task/1071) | CSES | Math / Square Layers | Xác định số tại tọa độ $(Y, X)$ trong bảng số xoắn ốc vô tận. Xét lớp ngoài cùng $M = \max(Y, X)$, giá trị đường chéo chính là $M^2 - M + 1$. Căn cứ vào tính chẵn lẻ của $M$ để tịnh tiến trong $O(1)$. |

---

## 🔴 Cấp độ 3: Nâng cao & Định lý Pick (Hard / Advanced ICPC)
*Mục tiêu*: Thành thạo Định lý Pick kết hợp Shoelace, kiểm tra điểm thuộc đa giác, và đếm cặp nguyên tố cùng nhau bằng Bao hàm - loại trừ.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**Polygon Lattice Points**](https://cses.fi/problemset/task/2193) | CSES | Pick's Theorem + GCD | Tính số điểm nguyên trên biên ($B$) và bên trong ($I$) đa giác. Số điểm trên cạnh nối $(x_1, y_1)$ và $(x_2, y_2)$ là $\gcd(\lvert x_1 - x_2 \rvert, \lvert y_1 - y_2 \rvert)$. Tính $B$ bằng tổng $\gcd$ các cạnh. Tính diện tích $S$ bằng Shoelace. Áp dụng Định lý Pick: $I = S - B/2 + 1$. |
| 13 | [**Point in Polygon**](https://cses.fi/problemset/task/2192) | CSES | Ray Casting / Geometry | Kiểm tra điểm nằm trong, ngoài hay trên cạnh đa giác. Bắn một tia ngang từ điểm khảo sát sang phải vô tận và đếm số lần tia cắt các cạnh của đa giác. Nếu số lần cắt là số lẻ $\implies$ nằm trong; nếu chẵn $\implies$ nằm ngoài. |
| 14 | [**Counting Coprime Pairs**](https://cses.fi/problemset/task/2417) | CSES | Inclusion-Exclusion | Cho mảng $N$ số. Đếm số cặp $(i < j)$ có $\gcd(A_i, A_j) = 1$. Lấy tổng số cặp $\binom{N}{2}$ trừ đi số cặp có ước chung $> 1$ bằng nguyên lý bao hàm - loại trừ trên các ước nguyên tố phân biệt của từng số trong $O(N \sqrt{\max A})$. |
| 15 | [**Exponentiation II**](https://cses.fi/problemset/task/1095) | CSES | Fermat's Little Theorem / Towers | Tính $a^{b^c} \pmod{10^9+7}$. Vì $P = 10^9+7$ là số nguyên tố, theo định lý Fermat nhỏ, số mũ $b^c$ được thu gọn theo modulo $\varphi(P) = P - 1 = 10^9+6$. Tính $E = b^c \bmod (10^9+6)$ trước, sau đó tính $a^E \bmod (10^9+7)$. |
| 16 | [**Manhattan Triangle**](https://atcoder.jp/contests/arc065/tasks/arc065_b) | AtCoder ARC | Manhattan Metric / Hashing | Đếm số lượng bộ 3 điểm tạo thành tam giác đều trong metric khoảng cách Manhattan (khoảng cách giữa mọi cặp điểm trong bộ ba đều bằng nhau). Kết hợp xoay tọa độ 45 độ và cấu trúc bảng băm để tìm kiếm các điểm thỏa mãn trong $O(N \log N)$. |

---

## 💡 Lời khuyên vàng khi áp dụng Toán học trong CP

1. **Khử hoàn toàn số thực khi chia làm tròn**:
   - Thay vì `ceil((double)a / b)`, luôn viết `(a + b - 1) / b` với $a \ge 0, b > 0$.
   - Điều này giúp code an toàn $100\%$ trước các lỗi sai số làm tròn $0.99999999999$ vốn có thể khiến bài thi dính `Wrong Answer`.

2. **Quy tắc tính diện tích nguyên (2*S)**:
   - Khi áp dụng công thức Shoelace, luôn giữ giá trị $2 \cdot S$ dưới dạng số nguyên (`long long` hoặc `__int128_t`), chỉ chia cho 2 khi in ra kết quả cuối cùng hoặc giữ nguyên $2S$ nếu đề bài yêu cầu.

3. **Kỹ thuật xoay trục 45 độ đối với khoảng cách Manhattan**:
   - Bất cứ khi nào gặp bài toán tối ưu khoảng cách Manhattan $|x_1 - x_2| + |y_1 - y_2|$, hãy nghĩ ngay đến phép đổi tọa độ:
     $$(x', y') = (x + y, x - y)$$
   - Việc tách rời 2 chiều độc lập giúp đưa bài toán từ $O(N^2)$ về $O(N)$ hoặc cho phép dùng Segment Tree 1D độc lập trên từng trục.

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

