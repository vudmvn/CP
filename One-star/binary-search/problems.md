# Tuyển tập Bài tập: Tìm kiếm Nhị phân & Tìm kiếm Tam phân (Binary & Ternary Search)

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---

> Tuyển tập 16 bài tập chọn lọc kinh điển từ các nền tảng **CSES, VNOI, Codeforces, AtCoder** được phân cấp từ nền tảng đến biến hóa nâng cao, rèn luyện phản xạ nhận diện tính đơn điệu và thiết kế hàm kiểm tra khả thi (`check(mid)`).

---

## 📊 Bảng tổng hợp bài tập phân cấp

| STT | Tên bài toán | Nguồn | Cấp độ | Chủ đề cốt lõi |
| :---: | :--- | :---: | :---: | :--- |
| 1 | [Binary Search](https://codeforces.com/edu/course/2/lesson/6/1/practice/contest/283911/problem/A) | Codeforces Edu | 🟢 Dễ | Tìm kiếm cơ bản trên mảng đã sắp |
| 2 | [Closest to the Left](https://codeforces.com/edu/course/2/lesson/6/1/practice/contest/283911/problem/B) | Codeforces Edu | 🟢 Dễ | Mô phỏng `upper_bound` và số $\le X$ |
| 3 | [Fast Search](https://codeforces.com/edu/course/2/lesson/6/1/practice/contest/283911/problem/D) | Codeforces Edu | 🟢 Dễ | Đếm số lượng phần tử trong đoạn $[L, R]$ |
| 4 | [Square Root](https://cses.fi/problemset/) | Classic | 🟢 Dễ | Tìm căn bậc hai nguyên bằng BS |
| 5 | [Ropes](https://codeforces.com/edu/course/2/lesson/6/2/practice/contest/283932/problem/B) | Codeforces Edu | 🟡 Trung bình | Chặt nhị phân trên số thực $\mathbb{R}$ |
| 6 | [Factory Machines](https://cses.fi/problemset/task/1620) | CSES | 🟡 Trung bình | Chặt nhị phân trên không gian nghiệm thời gian |
| 7 | [Array Division](https://cses.fi/problemset/task/1085) | CSES | 🟡 Trung bình | Cực tiểu hóa tổng đoạn lớn nhất (Min-Max) |
| 8 | [Aggressive Cows](https://www.spoj.com/problems/AGGRCOW/) | SPOJ / VNOI | 🟡 Trung bình | Cực đại hóa khoảng cách nhỏ nhất (Max-Min) |
| 9 | [Multiplication Table](https://cses.fi/problemset/task/2422) | CSES | 🟡 Trung bình | Tìm số thứ $K$ trong bảng nhân $N \times N$ |
| 10 | [Equation](https://codeforces.com/edu/course/2/lesson/6/2/practice/contest/283932/problem/E) | Codeforces Edu | 🟡 Trung bình | Giải phương trình $x^2 + \sqrt{x} = C$ |
| 11 | [Subarray Sums I](https://cses.fi/problemset/task/1660) | CSES | 🟡 Trung bình | Prefix Sum kết hợp `std::lower_bound` |
| 12 | [K-th Not Divisible by n](https://codeforces.com/problemset/problem/1352/C) | Codeforces | 🟡 Trung bình | Đếm số không chia hết bằng toán & BS |
| 13 | [Weakness and Poorness](https://codeforces.com/problemset/problem/578/C) | Codeforces | 🔴 Nâng cao | Tìm kiếm tam phân trên hàm lồi số thực |
| 14 | [Restorer Distance](https://codeforces.com/problemset/problem/1355/E) | Codeforces | 🔴 Nâng cao | Ternary Search tối ưu chi phí làm bằng cột |
| 15 | [Maximum Average Segment](https://codeforces.com/edu/course/2/lesson/6/4/practice/contest/285083/problem/A) | Codeforces Edu | 🔴 Nâng cao | Trừ trung bình $A_i - M$ kết hợp Prefix Min |
| 16 | [Pair Selection](https://codeforces.com/edu/course/2/lesson/6/4/practice/contest/285083/problem/C) | Codeforces Edu | 🔴 Nâng cao | Tối ưu hóa phân thức $\sum a_i / \sum b_i$ |

---

## 💡 Phân tích & Hướng dẫn tư duy chi tiết

### Bài 1: Factory Machines (CSES 1620)
- **Đề bài**: Có $N$ máy sản xuất ($N \le 2 \cdot 10^5$), máy thứ $i$ mất $k_i$ giây ($k_i \le 10^9$) để tạo 1 sản phẩm. Cần làm ít nhất $T$ sản phẩm ($T \le 10^9$). Tìm thời gian ngắn nhất.
- **Tư duy**:
  - *Hint 1*: Trong thời gian cố định $M$ giây, mỗi máy làm được bao nhiêu sản phẩm?
  - *Hint 2*: Tổng sản phẩm trong $M$ giây là $f(M) = \sum \lfloor M / k_i \rfloor$. Rõ ràng $M$ càng lớn thì $f(M)$ càng tăng $\implies$ Tính đơn điệu!
  - *Hint 3*: Khoảng tìm kiếm: $low = 1$, $high = \min(k_i) \cdot T \le 10^{18}$. Cần chú ý dùng kiểu dữ liệu `long long` và ngắt sớm trong vòng lặp tính tổng nếu tổng đã đạt $T$ để tránh tràn số!
- **Độ phức tạp**: $O(N \log(10^{18}))$.

---

### Bài 2: Array Division (CSES 1085)
- **Đề bài**: Chia mảng $N$ phần tử thành $K$ đoạn con liên tiếp sao cho tổng lớn nhất của một đoạn con là nhỏ nhất.
- **Tư duy**:
  - *Hint 1*: Đây là dạng bài kinh điển **Cực tiểu hóa giá trị cực đại (Min-Max)**.
  - *Hint 2*: Giả sử tổng mỗi đoạn không được vượt quá $S$. Ta có thể tham lam gom nhiều phần tử nhất có thể vào đoạn hiện tại trước khi bắt đầu đoạn mới.
  - *Hint 3*: Nếu số đoạn cần dùng $\le K$, giá trị $S$ là khả thi $\implies$ Thu hẹp tìm $S$ nhỏ hơn. Khoảng tìm kiếm: $[\max(A_i), \sum A_i]$.
- **Độ phức tạp**: $O(N \log(\sum A_i))$.

---

### Bài 3: Aggressive Cows (SPOJ / VNOI)
- **Đề bài**: Có $N$ chuồng bò tại tọa độ $x_1, x_2, \dots, x_N$. Cần xếp $C$ con bò vào các chuồng sao cho khoảng cách nhỏ nhất giữa 2 con bò bất kỳ là lớn nhất có thể.
- **Tư duy**:
  - *Hint 1*: Dạng bài **Cực đại hóa giá trị cực tiểu (Max-Min)**.
  - *Hint 2*: Sắp xếp tọa độ các chuồng tăng dần. Chặt nhị phân khoảng cách tối thiểu $D \in [1, x_N - x_1]$.
  - *Hint 3*: Hàm `check(D)`: Con bò đầu tiên luôn đặt ở $x_1$. Các con bò tiếp theo chỉ đặt vào chuồng có khoảng cách so với chuồng con bò trước đó $\ge D$. Nếu xếp đủ $C$ con bò $\implies$ Khả thi, tìm $D$ lớn hơn (`low = mid + 1`).
- **Độ phức tạp**: $O(N \log N + N \log(\max X))$.

---

### Bài 4: Multiplication Table (CSES 2422)
- **Đề bài**: Bảng nhân kích thước $N \times N$ ($N \le 10^6$, $N$ lẻ), ô $(i, j)$ có giá trị $i \cdot j$. Khi sắp xếp tất cả $N^2$ số theo thứ tự tăng dần, tìm số ở vị trí chính giữa (trung vị, chỉ số $(N^2 + 1) / 2$).
- **Tư duy**:
  - *Hint 1*: $N^2 \le 10^{12}$, không thể sinh mảng rồi sắp xếp.
  - *Hint 2*: Chặt nhị phân giá trị trung vị $M \in [1, N^2]$. Đếm xem có bao nhiêu số trong bảng $\le M$.
  - *Hint 3*: Ở hàng thứ $i$, các giá trị là $i \cdot 1, i \cdot 2, \dots, i \cdot N$. Số lượng phần tử $\le M$ ở hàng $i$ chính là $\min(N, \lfloor M / i \rfloor)$.
  - *Hint 4*: Tổng các phần tử $\le M$ trong cả bảng tính được trong $O(N)$ bằng vòng lặp từ $i = 1 \dots N$.
- **Độ phức tạp**: $O(N \log(N^2))$.

---

### Bài 5: Weakness and Poorness (Codeforces 578C)
- **Đề bài**: Cho mảng $A$ gồm $N$ số nguyên. Trừ mỗi phần tử đi số thực $x$: $A'_i = A_i - x$. "Poorness" của mảng là độ lớn tuyệt đối lớn nhất của tổng một đoạn con liên tiếp: $f(x) = \max_{1 \le i \le j \le N} \lvert \sum_{k=i}^j A'_k \rvert$. Tìm $x$ để $f(x)$ đạt giá trị nhỏ nhất.
- **Tư duy**:
  - *Hint 1*: Hàm $f(x)$ là hàm lồi (Convex function) đơn đỉnh dạng đáy chén (có duy nhất một điểm cực tiểu toàn cục).
  - *Hint 2*: Áp dụng **Tìm kiếm tam phân (Ternary Search)** trên số thực $x \in [-10^4, 10^4]$ trong 100 vòng lặp.
  - *Hint 3*: Tại mỗi bước, với giá trị $x$, tính tổng đoạn con lớn nhất và nhỏ nhất của mảng $A_i - x$ bằng thuật toán Kadane trong $O(N)$.
- **Độ phức tạp**: $O(100 \times N)$.

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
