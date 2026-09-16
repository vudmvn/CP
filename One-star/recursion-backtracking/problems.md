# Tuyển tập bài tập: Đệ quy & Thuật toán Quay lui

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, VNOJ, Codeforces, LeetCode, AtCoder**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm phân tích ý tưởng và cẩm nang cắt tỉa (Pruning hints).

---

## 🟢 Cấp độ 1: Cơ bản & Nhập môn (Easy / Direct Application)
*Mục tiêu*: Thành thạo mô hình `Choose -> Explore -> Unchoose`, đệ quy sinh tập con $O(2^N)$, hoán vị $O(N!)$, và bàn cờ $N$-Queens.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Apple Division**](https://cses.fi/problemset/task/1623) | CSES | Sinh tập con $O(2^N)$ | Chia $N \le 20$ quả táo thành 2 nhóm sao cho chênh lệch tổng trọng lượng nhỏ nhất. Mỗi quả táo có 2 lựa chọn (nhóm 1 hoặc nhóm 2). Không gian $2^{20} \approx 10^6$ duyệt an toàn trong $0.05\text{s}$. |
| 2 | [**Creating Strings**](https://cses.fi/problemset/task/1622) | CSES | Sinh hoán vị có phần tử lặp | Sinh tất cả các xâu phân biệt tạo từ các ký tự của xâu ban đầu (độ dài $\le 8$). Sắp xếp xâu ban đầu và áp dụng kỹ thuật cắt tỉa trùng lặp `if (i > 0 && s[i] == s[i-1] && !used[i-1]) continue;`. |
| 3 | [**Chessboard and Queens**](https://cses.fi/problemset/task/1624) | CSES | $N$-Queens có chướng ngại vật | Đặt 8 quân hậu lên bàn cờ $8 \times 8$ có một số ô bị đánh dấu `*` không được đặt. Kết hợp quay lui $N$-Queens chuẩn với điều kiện kiểm tra ô hợp lệ. |
| 4 | [**Gray Code**](https://cses.fi/problemset/task/2205) | CSES | Đệ quy phản xạ | Sinh dãy $2^N$ xâu nhị phân độ dài $N$ sao cho hai xâu liên tiếp chỉ khác nhau đúng 1 bit. Xây dựng đệ quy: Mã Gray bậc $N$ gồm mã bậc $N-1$ ghép thêm tiền tố `'0'`, nối với mã bậc $N-1$ đảo ngược thứ tự ghép thêm tiền tố `'1'`. |
| 5 | [**Tower of Hanoi**](https://cses.fi/problemset/task/2165) | CSES | Đệ quy chia để trị | Bài toán Tháp Hà Nội $N$ đĩa. Chuyển $N-1$ đĩa từ $A \to B$ qua $C$, chuyển đĩa lớn nhất từ $A \to C$, chuyển $N-1$ đĩa từ $B \to C$ qua $A$. Số bước tối thiểu $2^N - 1$. |

---

## 🟡 Cấp độ 2: Vận dụng & Cắt tỉa nhánh cận (Medium / Pruning)
*Mục tiêu*: Áp dụng cắt tỉa tính khả thi và tối ưu, sử dụng bitmask tăng tốc, xử lý bài toán Sudoku và bài toán đường đi.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Grid Paths**](https://cses.fi/problemset/task/1625) | CSES | Cắt tỉa hình học cực mạnh | Tìm số đường đi 48 bước đi qua toàn bộ 49 ô của bàn cờ $7 \times 7$. Bắt buộc dùng 3 kỹ thuật cắt tỉa: Cận đích đến sớm, Cắt tỉa chia đôi lưới (nếu phía trước bị chặn nhưng 2 bên trống $\to$ dừng ngay). |
| 7 | [**Sudoku Solver**](https://leetcode.com/problems/sudoku-solver/) | LeetCode 37 | Bitmask, MRV Heuristic | Điền các số $1 \dots 9$ vào bảng Sudoku $9 \times 9$. Tối ưu tốc độ bằng cách luôn chọn ô có ít số hợp lệ nhất để thử trước (Fail-first principle). |
| 8 | [**Preparing Olympiad**](https://codeforces.com/problemset/problem/550/B) | Codeforces | Quay lui tập con, Cắt tỉa | Chọn tập bài toán từ $N \le 15$ bài sao cho tổng độ khó $\in [l, r]$ và chênh lệch $\max - \min \ge x$. Sắp xếp mảng ban đầu để dễ dàng cắt tỉa khi tổng vượt quá $r$. |
| 9 | [**ORXOR**](https://atcoder.jp/contests/abc197/tasks/abc197_c) | AtCoder | Chia đoạn, Bitwise | Cho dãy $N \le 20$ số. Chia dãy thành các đoạn liên tiếp, mỗi đoạn lấy OR, sau đó lấy XOR các kết quả. Duyệt $2^{N-1}$ cách đặt vách ngăn phân đoạn bằng quay lui nhị phân. |
| 10 | [**BHouse**](https://oj.vnoi.info/problem/bhouse) | VNOJ | DFS / Backtracking trên lưới | Tìm đường đi trên lưới thỏa mãn các điều kiện đặc biệt, kết hợp mảng đánh dấu `visited` và khôi phục trạng thái. |

---

## 🔴 Cấp độ 3: Nâng cao & Meet-in-the-middle (Hard / Advanced ICPC)
*Mục tiêu*: Làm chủ kỹ thuật Meet-in-the-middle biến $O(2^N) \to O(2^{N/2})$ khi $N \approx 40$, kết hợp nhánh cận TSP.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng |
| :---: | :--- | :---: | :--- | :--- |
| 11 | [**Meet in the Middle**](https://cses.fi/problemset/task/1628) | CSES | Meet-in-the-middle | Cho $N \le 40$ số, đếm số tập con có tổng bằng $X$. Chia thành 2 nửa kích thước 20. Sinh $2^{20}$ tổng nửa đầu (sort lại), sinh $2^{20}$ tổng nửa sau và tìm kiếm nhị phân `equal_range` trong $O(N \cdot 2^{N/2})$. |
| 12 | [**Maximum Subsequence**](https://codeforces.com/problemset/problem/888/E) | Codeforces | Meet-in-the-middle, Modulo | Cho $N \le 35$ số, tìm tập con có tổng modulo $m$ lớn nhất. Chia 2 nửa, sinh mảng số dư $A$ và $B$. Với mỗi $b \in B$, tìm $a \in A$ lớn nhất sao cho $a + b < m$ hoặc $a + b < 2m$ bằng `upper_bound`. |
| 13 | [**Traveling Salesperson (TSP)**](https://oj.vnoi.info/problem/qbticket) | VNOJ / CSES | Branch and Bound | Tìm chu trình qua tất cả các thành phố với tổng chi phí nhỏ nhất. Áp dụng nhánh cận: Nếu chi phí hiện tại + Cận dưới cây khung nhỏ nhất (MST) $\ge \text{best\_ans} \implies$ Cắt tỉa ngay. |
| 14 | [**MKJUMPS - Making Jumps**](https://www.spoj.com/problems/MKJUMPS/) | SPOJ | Knight Tour Backtracking | Quân mã đi tuần trên bàn cờ khuyết ô. Tìm số ô không thể ghé thăm nhỏ nhất. Dùng quay lui thử 8 hướng nhảy của quân mã kết hợp đếm số ô còn lại. |

---

## 💡 Bí quyết luyện tập & Debug thuật toán Quay lui
1. **Vẽ cây trạng thái trên giấy với $N$ nhỏ ($N = 3, 4$)**: Luôn theo dõi các biến được thay đổi ở bước `Choose` và đảm bảo chúng được trả lại chính xác ở bước `Unchoose`.
2. **Kiểm tra Base Case đầu tiên**: Đảm bảo điều kiện dừng không bao giờ bị bỏ sót và luôn có lệnh `return;`.
3. **Đo thời gian chạy (Benchmark)**: Dùng `clock()` hoặc `std::chrono` để kiểm tra số lần hàm đệ quy được gọi. Nếu số lần gọi $> 10^8$, bạn **bắt buộc phải bổ sung điều kiện cắt tỉa** hoặc đổi sang Quy hoạch động.

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

