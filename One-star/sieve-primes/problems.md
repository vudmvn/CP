# Tuyển tập bài tập: Sàng số nguyên tố & Thừa số nguyên tố

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, VNOJ, Codeforces, AtCoder, SPOJ, Library Checker**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm phân tích ý tưởng (hints/editorials).

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo Sàng Eratosthenes cổ điển, Sàng tuyến tính $O(N)$, mảng SPF đếm ước và sàng đoạn.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Counting Divisors**](https://cses.fi/problemset/task/1713) | CSES | Sàng SPF, Đếm ước | Có $n \le 10^5$ truy vấn, mỗi truy vấn cho $x \le 10^6$. Dùng Sàng tuyến tính tiền xử lý mảng `spf` trong $O(N)$, trả lời mỗi truy vấn đếm ước $d(x) = \prod (a_i + 1)$ trong $O(\log x)$. |
| 2 | [**Common Divisors**](https://cses.fi/problemset/task/1081) | CSES | Đếm phân phối, Ước chung | Cho $N$ số $\le 10^6$. Tìm $\gcd$ lớn nhất của 2 số bất kỳ. Đếm tần số xuất hiện của mỗi số, duyệt $g$ từ $10^6$ lùi về 1, đếm số lượng bội số của $g$. Nếu có $\ge 2$ bội số $\implies$ $g$ là đáp án. |
| 3 | [**Prime Generator**](https://www.spoj.com/problems/PRIME1/) | SPOJ | Sàng đoạn $[L, R]$ | Tìm các số nguyên tố trong $[L, R]$ với $R \le 10^9, R - L \le 10^5$. Sàng các số nguyên tố $\le \sqrt{R} \approx 31622$, sau đó dùng các số này gạch bỏ hợp số trên đoạn $[L, R]$. |
| 4 | [**T-primes**](https://codeforces.com/problemset/problem/230/B) | Codeforces | Tính chất ước số | Một số có đúng 3 ước số dương khi và chỉ khi nó là bình phương của một số nguyên tố ($x = p^2$). Lấy căn bậc hai $p = \text{round}(\sqrt{x})$, kiểm tra $p^2 == x$ và $p$ có phải là số nguyên tố bằng Sàng Eratosthenes. |
| 5 | [**PNUMBER - Tìm số nguyên tố**](https://oj.vnoi.info/problem/pnumber) | VNOJ | Sàng Eratosthenes | Liệt kê các số nguyên tố trong đoạn $[A, B]$ với $A, B \le 2 \times 10^5$. Áp dụng Sàng Eratosthenes cơ bản. |

---

## 🟡 Cấp độ 2: Vận dụng & Tối ưu hóa (Medium / Intermediate)
*Mục tiêu*: Sử dụng mảng SPF để biến đổi đồ thị bài toán, kết hợp bao hàm loại trừ, kiểm tra nguyên tố cùng nhau.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Enlarge GCD**](https://codeforces.com/problemset/problem/1034/A) | Codeforces | Sàng SPF, GCD mảng | Cho mảng $N$ số. Xóa ít phần tử nhất để GCD của mảng tăng lên. Chia tất cả các số cho $g = \gcd(A_1, \dots, A_n)$. Dùng mảng `spf` phân tích các thừa số nguyên tố nguyên thủy của mỗi số, đếm thừa số nào xuất hiện nhiều nhất. |
| 7 | [**Prime Multiples**](https://cses.fi/problemset/task/2185) | CSES | Bao hàm - Loại trừ, Số nguyên tố | Đếm số nguyên $\le N \le 10^{18}$ chia hết cho ít nhất một trong $K \le 20$ số nguyên tố cho trước. Duyệt $2^K$ tập con bằng bitmask, áp dụng Nguyên lý Bao hàm - Loại trừ: cộng với tập lẻ, trừ với tập chẵn. Chú ý chống tràn số khi nhân. |
| 8 | [**Divisor Analysis**](https://cses.fi/problemset/task/2182) | CSES | Phân tích số học, Fermat nhỏ | Cho $N = \prod p_i^{k_i}$. Tính số lượng, tổng và tích các ước số modulo $10^9+7$. Riêng phần tích các ước số, số mũ cần được chia đôi khéo léo hoặc tính theo modulo $10^9+6$. |
| 9 | [**Coprime**](https://atcoder.jp/contests/abc177/tasks/abc177_e) | AtCoder | Pairwise Coprime, SPF | Kiểm tra xem mảng là pairwise coprime hay setwise coprime. Dùng mảng `spf` để phân tích thừa số nguyên tố của từng số; nếu có bất kỳ ước nguyên tố nào xuất hiện $\ge 2$ lần trong các số khác nhau $\implies$ không thể là pairwise coprime. |
| 10 | [**Colliders**](https://codeforces.com/problemset/problem/154/B) | Codeforces | Mảng SPF, Bật/tắt trạng thái | Bật hoặc tắt các máy gia tốc $1 \dots N$. Máy chỉ bật được nếu nguyên tố cùng nhau với tất cả các máy đang bật. Dùng `spf` lưu các ước nguyên tố của số đang bật để kiểm tra xung đột trong $O(\log N)$. |
| 11 | [**The Number of Pairs**](https://codeforces.com/problemset/problem/1499/D) | Codeforces | Sàng tuyến tính, Hàm $2^{\omega(n)}$ | Đếm số cặp $(a, b)$ thỏa mãn $c \cdot \operatorname{lcm}(a, b) - d \cdot \gcd(a, b) = x$. Đặt $g = \gcd(a, b)$, đưa về việc đếm số ước nguyên tố phân biệt $\omega(k)$ và tính $2^{\omega(k)}$ bằng sàng tuyến tính. |

---

## 🔴 Cấp độ 3: Nâng cao & Ứng dụng hàm nhân tính (Hard / Advanced ICPC)
*Mục tiêu*: Tiền xử lý hàm Möbius $\mu(n)$ bằng Sàng Euler, thuật toán Miller-Rabin và đếm cặp nguyên tố cùng nhau.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**Counting Coprime Pairs**](https://cses.fi/problemset/task/2417) | CSES | Hàm Möbius $\mu(n)$, Sàng Euler | Đếm số cặp $(A_i, A_j)$ có $\gcd = 1$ trong $N \le 10^5$ số $\le 10^6$. Tiền xử lý hàm Möbius $\mu(x)$ bằng Sàng tuyến tính. Đếm tần số bội số của mỗi số, đáp án bằng $\sum \mu(d) \binom{\text{cnt}[d]}{2}$. |
| 13 | [**Coprime Subsequences**](https://codeforces.com/problemset/problem/803/F) | Codeforces | Möbius Inversion, Lũy thừa | Đếm số dãy con có $\gcd$ bằng 1. Áp dụng công thức nghịch đảo Möbius: $\sum_{d=1}^{10^5} \mu(d) (2^{\text{cnt}[d]} - 1) \pmod{10^9+7}$. |
| 14 | [**Multiplicity**](https://codeforces.com/problemset/problem/1061/C) | Codeforces | DP, Sàng phân tích ước | Đếm số dãy con đẹp sao cho phần tử thứ $i$ chia hết cho $i$. Dùng mảng `spf` hoặc tiền xử lý danh sách ước số của từng số, cập nhật mảng DP lùi để tránh bị ghi đè trạng thái. |
| 15 | [**Factorize**](https://judge.yosupo.jp/problem/factorize) | Library Checker | Miller-Rabin, Pollard's Rho | Phân tích số nguyên lên tới $10^{18}$ ra thừa số nguyên tố. Kết hợp Miller-Rabin để test nguyên tố và thuật toán Pollard's Rho tìm ước không tầm thường trong $O(N^{1/4})$. |
| 16 | [**FACTCG2 - Medium Factorization**](https://www.spoj.com/problems/FACTCG2/) | SPOJ | Sàng SPF siêu tốc | Phân tích hàng trăm nghìn số $\le 10^7$ ra thừa số nguyên tố. Cài đặt Sàng tuyến tính Euler tối ưu để đạt thời gian chạy nhanh nhất. |

---

## 💡 Bí quyết làm chủ chuyên đề Số nguyên tố
1. **Mặc định dùng Sàng tuyến tính Euler**: Trừ khi bộ nhớ bị giới hạn cực ngặt (khi đó dùng Sàng cổ điển + `bitset`), trong mọi bài toán đếm ước hay số học, hãy khởi tạo mảng `spf` bằng Sàng tuyến tính $O(N)$.
2. **Luôn nhớ công thức số lượng ước**: $d(N) = \prod (a_i + 1)$. Với $N \le 10^6$, số lượng ước tối đa chỉ là **$240$** (đạt được tại $N = 720720$). Với $N \le 10^9$, max $d(N) = 1344$. Với $N \le 10^{18}$, max $d(N) = 103680$. Điều này có nghĩa là các thuật toán duyệt qua toàn bộ ước số chạy cực kỳ nhanh!

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

