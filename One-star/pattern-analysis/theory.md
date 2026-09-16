# Lý thuyết chuyên sâu: Nhận dạng Quy luật & Công thức đóng (Pattern Analysis & Closed-form Formulas)

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---

## 1. Bản chất toán học của Bài toán Quy luật (Pattern Finding)

### 1.1. Khái niệm & Mục tiêu
Trong Competitive Programming, có một lớp bài toán lớn đưa ra quy tắc sinh dãy hoặc bảng số vô hạn rất trực quan, kèm theo yêu cầu:  
**"Tìm giá trị của phần tử thứ $K$"** hoặc **"Tìm giá trị tại tọa độ $(y, x)$"** với $K, x, y \le 10^{12} \dots 10^{18}$.

- **Mô phỏng ngây thơ (Simulation)**: Tạo mảng hoặc chạy vòng lặp tuần tự từng bước từ $1$ đến $K$. Tốn $O(K)$ thời gian $\implies$ Bị **TLE** hoặc **MLE** ngay lập tức khi $K > 10^7$.
- **Mục tiêu của Pattern Analysis**: Tìm ra ánh xạ toán học trực tiếp:
  $$f: (N, K) \mapsto \text{Ans}$$
  để tính ra đáp án chỉ bằng các phép toán số học cơ bản trong thời gian $O(1)$ hoặc $O(\log K)$ mà không cần lưu trữ hay duyệt qua các phần tử trước đó.

---

### 1.2. Các công cụ toán học nền tảng

#### A. Hàm sàn (Floor) và Hàm trần (Ceiling)
- **Hàm sàn $\lfloor x \rfloor$**: Số nguyên lớn nhất không vượt quá $x$.
- **Hàm trần $\lceil x \rceil$**: Số nguyên nhỏ nhất không nhỏ hơn $x$.
- **Trong ngôn ngữ C++**:
  - Khi chia 2 số nguyên dương `a / b`, phép chia mặc định bị cắt cụt (truncation) về $0$, tương đương với hàm sàn:
    $$\lfloor a / b \rfloor = a / b \quad (\text{với } a, b \in \mathbb{Z}^+)$$
  - Để tính hàm trần với số nguyên dương mà không dùng thư viện số thực `ceil()` (tránh lỗi sai số dấu phẩy động):
    $$\lceil a / b \rceil = \left\lfloor \frac{a + b - 1}{b} \right\rfloor = \frac{a + b - 1}{b}$$

#### B. Cấp số cộng (Arithmetic Progression)
Dãy số có khoảng cách giữa hai phần tử liên tiếp luôn bằng hằng số $d$:
$$u_n = u_1 + (n - 1)d$$
- **Số lượng phần tử trong đoạn $[A, B]$ với bước nhảy $d$**:
  $$\text{count} = \left\lfloor \frac{B - A}{d} \right\rfloor + 1$$
- **Tổng $n$ số hạng đầu tiên**:
  $$S_n = \frac{n(u_1 + u_n)}{2} = \frac{n[2u_1 + (n - 1)d]}{2}$$

---

## 2. Phân loại 5 Mô hình Quy luật Kinh điển trong CP

### 2.1. Mô hình 1: Phân đoạn cấp số cộng (Block Partitioning)

#### Đặc trưng nhận diện:
Dãy số gồm $N$ phần tử được chia thành $M$ khối liên tiếp $(B_1, B_2, \dots, B_M)$. Mỗi khối là một cấp số cộng có quy luật riêng.

#### Phương pháp giải tổng quát:
1. **Xác định kích thước của từng khối**: $S_1, S_2, \dots, S_M$.
2. **Xác định vị trí $K$ nằm ở khối nào**:
   - Nếu $K \le S_1 \implies K$ thuộc khối $B_1$.
   - Nếu $S_1 < K \le S_1 + S_2 \implies K$ thuộc khối $B_2$, với vị trí tương đối bên trong khối là $k' = K - S_1$.
3. **Áp dụng công thức số hạng tổng quát của khối đó**:
   $$\text{Ans} = u_{\text{first}} + (k' - 1) \cdot d$$

*Ví dụ kinh điển*: **Dãy số chẵn lẻ (Even Odds)**
- Dãy $1 \dots N$ gồm các số chẵn trước, số lẻ sau:
  - Khối 1 (Chẵn): có $S_{\text{even}} = \lfloor N / 2 \rfloor$ số. Số thứ $k'$ là $2k'$.
  - Khối 2 (Lẻ): có $S_{\text{odd}} = \lceil N / 2 \rceil = (N + 1) / 2$ số. Số thứ $k'$ là $2k' - 1$.
  - Nếu $K \le S_{\text{even}} \implies \text{Ans} = 2K$.
  - Nếu $K > S_{\text{even}} \implies \text{Ans} = 2(K - S_{\text{even}}) - 1$.
  - Thời gian: $O(1)$.

---

### 2.2. Mô hình 2: Chu kỳ tuần hoàn (Periodic / Modulo Cycles)

#### Đặc trưng nhận diện:
Các giá trị lặp lại y hệt sau một chu kỳ cố định có độ dài $T$.

#### Phương pháp giải:
Sử dụng toán tử modulo `%` để quy vị trí $K$ (1-indexed) về vị trí chuẩn trong chu kỳ đầu tiên:
$$\text{pos} = (K - 1) \bmod T + 1 \quad (\text{với } \text{pos} \in [1, T])$$
Sau đó tra cứu trực tiếp trong mảng trạng thái chu kỳ:
$$\text{Ans} = \text{Cycle}[\text{pos}]$$

*Ví dụ*: Tìm chữ số tận cùng của $a^b$:
Dãy chữ số tận cùng của $a^1, a^2, a^3, \dots$ luôn có chu kỳ lặp lại với độ dài $T \le 4$. Ta chỉ cần tính số mũ tương đương trong chu kỳ là $b' = (b - 1) \bmod 4 + 1$.

---

### 2.3. Mô hình 3: Khối độ dài chữ số (Digit-Length Blocks)

#### Đặc trưng nhận diện:
Dãy tạo bằng cách viết liền các số nguyên liên tiếp: $12345678910111213141516\dots$ (CSES Digit Queries). Yêu cầu tìm chữ số thứ $K$ ($K \le 10^{18}$).

#### Phân tích cấu trúc bậc thang:
- Các số có $1$ chữ số ($1 \dots 9$): Có $9$ số $\implies 9 \times 1 = 9$ chữ số.
- Các số có $2$ chữ số ($10 \dots 99$): Có $90$ số $\implies 90 \times 2 = 180$ chữ số.
- Các số có $3$ chữ số ($100 \dots 999$): Có $900$ số $\implies 900 \times 3 = 2700$ chữ số.
- Tổng quát: Các số có $len$ chữ số có $count = 9 \times 10^{len-1}$ số, chiếm tổng cộng $total = len \times count$ chữ số.

#### Thuật toán giải quyết trong $O(\log_{10} K)$:
1. Duyệt $len = 1, 2, 3, \dots$, lấy $K$ trừ dần cho $total$ cho đến khi $K \le total$. Lúc này ta biết chữ số thứ $K$ thuộc về một số có đúng $len$ chữ số.
2. Tìm số nguyên cụ thể chứa chữ số cần tìm:
   $$\text{number} = 10^{len - 1} + \left\lfloor \frac{K - 1}{len} \right\rfloor$$
3. Tìm vị trí chữ số cụ thể trong số nguyên đó:
   $$\text{digit\_idx} = (K - 1) \bmod len$$
   Chuyển số `number` thành xâu ký tự và lấy ký tự tại vị trí `digit_idx`.

---

### 2.4. Mô hình 4: Tọa độ xoắn ốc trên lưới (Grid Spirals & Layers)

#### Đặc trưng nhận diện:
Bảng số vô hạn điền theo hình xoắn ốc (CSES Number Spiral) hoặc đường chéo ziczac. Yêu cầu tính giá trị tại ô $(y, x)$ với $y, x \le 10^9$.

#### Kỹ thuật phân tầng lớp vỏ (Layering):
Xét bảng số xoắn ốc CSES:
```text
(y, x) 1   2   3   4   5
1      1   2   9  10  25
2      4   3   8  11  24
3      5   6   7  12  23
4     16  15  14  13  22
5     17  18  19  20  21
```

1. **Xác định lớp vỏ (Layer)**: Ô $(y, x)$ thuộc lớp vỏ hình vuông kích thước $L \times L$ với:
   $$L = \max(y, x)$$
2. **Quan sát giá trị cực đại của lớp vỏ**:
   Lớp vỏ thứ $L$ luôn chứa số lớn nhất là $L^2$ tại một trong hai đầu mút của dải.
3. **Phân tích chiều đi của dải số**:
   - Nếu $L$ là số **chẵn**:
     - Số $L^2$ nằm ở hàng $L$, cột 1: $(L, 1)$.
     - Từ $(L, 1)$, dãy tăng dần sang phải đến $(L, L)$ rồi đi lên $(1, L)$.
     - Công thức:
       - Nếu $y = L \implies \text{Ans} = L^2 - (x - 1)$.
       - Nếu $x = L \implies \text{Ans} = (L - 1)^2 + y$.
   - Nếu $L$ là số **lẻ**:
     - Số $L^2$ nằm ở hàng 1, cột $L$: $(1, L)$.
     - Quy luật đảo chiều ngược lại:
       - Nếu $x = L \implies \text{Ans} = L^2 - (y - 1)$.
       - Nếu $y = L \implies \text{Ans} = (L - 1)^2 + x$.
4. Độ phức tạp: $O(1)$ phép tính số học!

---

### 2.5. Mô hình 5: Tự tương đồng & Phản xạ đối xứng (Fractals & Bitwise)

#### Đặc trưng nhận diện:
Dãy ký tự tăng gấp đôi kích thước sau mỗi bước theo quy tắc đối xứng (như chuỗi Thue-Morse, mã Gray, chuỗi đảo bit):
$$S_0 = \text{"0"}, \quad S_k = S_{k-1} + \overline{S_{k-1}}$$

#### Phương pháp giải:
- **Tiếp cận chia để trị $O(\log K)$**:
  - Xác định $K$ nằm ở nửa đầu ($K \le 2^{k-1}$) hay nửa sau ($K > 2^{k-1}$).
  - Nếu ở nửa sau: Đệ quy về $K' = K - 2^{k-1}$ và đảo ngược kết quả.
- **Tiếp cận Bitwise $O(1)$**:
  - Với chuỗi Thue-Morse: Ký tự tại vị trí $K$ (0-indexed) chính là số dư khi chia số lượng bit 1 của $K$ cho 2:
    $$\text{Ans} = \text{popcount}(K) \bmod 2 = \texttt{\_\_builtin\_popcountll}(K) \bmod 2$$

---

## 3. Nguyên lý phân tích trường hợp MECE

Khi giải bài toán quy luật, các nhánh rẽ phải tuân thủ nguyên lý **MECE (Mutually Exclusive and Collectively Exhaustive)**:
1. **Không trùng lặp (Mutually Exclusive)**: Không có bất kỳ giá trị $(N, K)$ nào thỏa mãn đồng thời 2 nhánh điều kiện khác nhau.
2. **Không bỏ sót (Collectively Exhaustive)**: Tất cả các trường hợp có thể xảy ra của đề bài đều được xử lý đầy đủ (đặc biệt là các trường hợp biên $N=1, K=1, K=N$).

---

## 4. Tài liệu tham khảo & Bài giảng đề xuất (References)

### 4.1. Giáo trình & Sách thuật toán kinh điển
1. **Competitive Programmer's Handbook** — *Antti Laaksonen*:
   - [Chapter 1: Introduction (Mathematics, Time Complexity, Number Spiral Walkthrough)](https://cses.fi/book/book.pdf).
2. **Concrete Mathematics: A Foundation for Computer Science** — *Ronald L. Graham, Donald E. Knuth, Oren Patashnik*:
   - *Chapter 3: Integer Functions (Floors and Ceilings, Modulo Arithmetic)* — Cẩm nang toán học nền tảng cho khoa học máy tính.
3. **Competitive Programming 4 (CP4 - Book 1)** — *Steven Halim*:
   - *Chapter 1 & 2: Ad Hoc Mathematics & Pattern Identification*.

### 4.2. Bài giảng & Chuyên đề trực tuyến (Online Resources & Wikis)
1. **VNOI Wiki**:
   - [Toán học cơ bản trong Tin học](https://wiki.vnoi.info/algo/basic/computational-complexity) — Phân tích các mô hình tư duy toán học và tối ưu độ phức tạp.
2. **CSES Problem Set Walkthroughs**:
   - [CSES Introductory Problems Analysis](https://cses.fi/problemset/) — Lời giải chi tiết cho Number Spiral, Digit Queries, Two Sets.
3. **Codeforces Catalog**:
   - [Math & Constructive Algorithms Tags](https://codeforces.com/problemset?tags=math) — Kho bài tập quy luật lớn nhất thế giới.

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

