# Tuyển tập bài tập: Nhận dạng Quy luật & Công thức đóng

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, Codeforces, VNOJ, AtCoder, LeetCode**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm phân tích ý tưởng (hints/editorials).

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Làm quen với việc tìm công thức đóng $O(1)$ thay cho vòng lặp, xử lý phân đoạn chẵn lẻ và bảng số.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Even Odds**](https://codeforces.com/problemset/problem/318/A) | Codeforces | Phân đoạn chẵn lẻ $O(1)$ | Dãy $1 \dots N$ gồm số chẵn trước, số lẻ sau. Tính $\text{num\_even} = \lfloor N/2 \rfloor$. Nếu $K \le \text{num\_even} \implies 2K$; ngược lại $\implies 2(K - \text{num\_even}) - 1$. |
| 2 | [**Number Spiral**](https://cses.fi/problemset/task/1071) | CSES | Bảng số xoắn ốc $O(1)$ | Tìm giá trị tại ô $(y, x)$ trên bảng xoắn ốc. Xác định tầng $L = \max(y, x)$. Giá trị lớn nhất tầng là $L^2$. Phân tích theo tính chẵn lẻ của $L$ để cộng/trừ khoảng cách trong $O(1)$. |
| 3 | [**Two Knights**](https://cses.fi/problemset/task/1072) | CSES | Tổ hợp & Hình học bàn cờ | Đếm số cách đặt 2 quân mã trên bàn cờ $k \times k$ không ăn nhau. Tổng số cách đặt: $\binom{k^2}{2}$. Trừ đi số cặp ăn nhau (mỗi ô $2 \times 3$ hoặc $3 \times 2$ có 2 cặp): trừ $4 \times (k-1)(k-2)$. |
| 4 | [**Elephant**](https://codeforces.com/problemset/problem/617/A) | Codeforces | Bước nhảy tối thiểu | Chú voi muốn đi đến điểm $x$ bằng các bước từ 1 đến 5. Số bước tối thiểu bằng công thức làm tròn lên: $\lceil x / 5 \rceil = (x + 4) / 5$. |
| 5 | [**Weird Algorithm**](https://cses.fi/problemset/task/1068) | CSES | Dãy Collatz $3n + 1$ | Nếu $n$ chẵn $n \to n/2$, nếu $n$ lẻ $n \to 3n+1$. Chú ý dùng `long long` vì giá trị trung gian có thể vượt quá $2 \times 10^9$. |

---

## 🟡 Cấp độ 2: Vận dụng & Biến đổi đại số (Medium / Intermediate)
*Mục tiêu*: Xử lý dãy số độ dài chữ số, phân tích trường hợp tối ưu chi phí, và chuỗi đệ quy.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Digit Queries**](https://cses.fi/problemset/task/2431) | CSES | Khối độ dài chữ số | Dãy viết liền các số $123456789101112...$. Trừ dần số chữ số của các nhóm có độ dài $1, 2, 3 \dots$ để tìm số nguyên cụ thể chứa chữ số thứ $K$ trong $O(\log_{10} K)$. |
| 7 | [**K-th Symbol in Grammar**](https://leetcode.com/problems/k-th-symbol-in-grammar/) | LeetCode 779 | Chuỗi Thue-Morse | Hàng 1 là `0`. Hàng sau sinh bằng cách thay `0 -> 01`, `1 -> 10`. Ký tự thứ $K$ (0-indexed) bằng số lượng bit 1 của $K$: `__builtin_popcountll(k) % 2`. |
| 8 | [**Road to Zero**](https://codeforces.com/problemset/problem/1342/A) | Codeforces | Phân tích trường hợp chi phí | Đưa $(x, y)$ về $(0, 0)$ với 2 thao tác: giảm 1 số tốn $a$, giảm cả 2 số tốn $b$. So sánh $2a$ và $b$: nếu $2a < b \implies$ chỉ dùng thao tác $a$; ngược lại dùng $b$ đưa về cùng giá trị rồi dùng $a$. |
| 9 | [**Cards**](https://codeforces.com/problemset/problem/1220/A) | Codeforces | Nhận dạng ký tự định danh | Xâu chứa các chữ cái bị xáo trộn của các từ "zero" và "one". Ký tự 'z' chỉ xuất hiện trong "zero", ký tự 'n' chỉ xuất hiện trong "one". Đếm số lượng chữ 'n' để in ra số 1, đếm chữ 'z' để in ra số 0. |
| 10 | [**Two Sets**](https://cses.fi/problemset/task/1075) | CSES | Phân hoạch tổng chẵn lẻ | Chia tập $\{1, 2, \dots, N\}$ thành 2 tập con có tổng bằng nhau. Tổng $S = N(N+1)/2$. Nếu $S$ lẻ $\implies$ Không thể chia; Nếu $S$ chẵn, nhóm từng cặp có tổng $N+1$ hoặc ghép đối xứng. |

---

## 🔴 Cấp độ 3: Nâng cao & Phân tích cấu trúc (Hard / Advanced ICPC)
*Mục tiêu*: Khai phá cấu trúc nhị phân, quy luật bitwise và bài toán trò chơi đối kháng.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 11 | [**Meaningless Operations**](https://codeforces.com/problemset/problem/1110/C) | Codeforces | Cấu trúc Bitwise, Ước số | Tìm $\gcd(a \oplus b, a \ \& \ b)$ lớn nhất với $0 < b < a$. Nếu $a$ chưa phải dạng $2^k - 1$, đáp án luôn là $2^{\lceil \log_2(a+1) \rceil} - 1$. Nếu $a = 2^k - 1$, đáp án là ước thực sự lớn nhất của $a$. |
| 12 | [**Maximum Sum on Even Positions**](https://codeforces.com/problemset/problem/1373/D) | Codeforces | Quy luật đảo đoạn | Cho đảo ngược một đoạn con bất kỳ để tối đa hóa tổng các vị trí chẵn. Quy về tìm đoạn con có tổng tăng thêm lớn nhất (bài toán Kadane) trên 2 dãy hiệu độ dài chẵn. |
| 13 | [**Gray Code**](https://cses.fi/problemset/task/2205) | CSES | Quy luật phản xạ đối xứng | Sinh mã Gray $N$ bit: Mã bậc $N$ được tạo bằng cách lấy mã bậc $N-1$ ghép thêm bit 0, nối với mã bậc $N-1$ đảo ngược thứ tự ghép thêm bit 1. Hoặc dùng công thức bitwise: $G(i) = i \oplus (i >> 1)$. |
| 14 | [**GCD on Blackboard**](https://atcoder.jp/contests/abc125/tasks/abc125_c) | AtCoder | Quy luật bù trừ GCD | Bỏ 1 số để GCD lớn nhất. Nhận dạng cấu trúc: đáp án là $\max_{i} \gcd(\text{pref}[i-1], \text{suff}[i+1])$. |

---

## 💡 Lời khuyên khi làm bài toán Quy luật
1. **Đừng bao giờ đoán mò khi chưa test tay**: Hãy luôn chạy thử code trâu sinh $10 - 20$ trường hợp đầu tiên để kiểm chứng giả thuyết.
2. **Kiểm tra kỹ giới hạn dữ liệu**: Nếu $N, K \le 10^{18}$, mọi biến tính toán trung gian phải dùng kiểu `long long` hoặc `unsigned long long`.
3. **Quy tắc chuyển đổi 0-indexed và 1-indexed**: Hầu hết các công thức chu kỳ $T$ chỉ hoạt động chuẩn xác khi đưa về 0-indexed: `pos = (k - 1) % T`.

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

