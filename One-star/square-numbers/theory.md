# Lý thuyết chuyên sâu: Số chính phương & Số không chứa thừa số chính phương (Square-Free Numbers)

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---


> **Chuyên mục**: Lý thuyết số & Kỹ thuật đếm (Number Theory & Combinatorics)  
> **Tài liệu tham khảo kinh điển**:  
> - *An Introduction to the Theory of Numbers* (G. H. Hardy, E. M. Wright) – Chapter 16: The Arithmetical Functions  
> - *Concrete Mathematics* (Graham, Knuth, Patashnik) – Chapter 4: Number Theory  
> - *Guide to Competitive Programming* (Antti Laaksonen) – Chapter 21: Number Theory  
> - *CP-Algorithms* – Möbius function & Multiplicative functions  

---

## 1. Số chính phương (Perfect Square)

### 1.1. Định nghĩa & Phân tích thừa số nguyên tố
Một số nguyên $n \ge 0$ được gọi là **số chính phương (perfect square)** nếu tồn tại một số nguyên $k \in \mathbb{Z}$ sao cho:
$$n = k^2$$

Xét phân tích tiêu chuẩn của $n > 0$ ra thừa số nguyên tố:
$$n = \prod_{i=1}^m p_i^{\alpha_i} = p_1^{\alpha_1} p_2^{\alpha_2} \dots p_m^{\alpha_m} \quad (\alpha_i \ge 1)$$

> [!IMPORTANT]
> **Định lý đặc trưng số mũ**:
> $n$ là số chính phương khi và chỉ khi **tất cả các số mũ $\alpha_i$ đều là số chẵn**:
> $$n = k^2 \iff \forall i \in \{1, \dots, m\}, \quad \alpha_i \equiv 0 \pmod 2$$
> Khi đó: $k = \prod_{i=1}^m p_i^{\alpha_i / 2}$.

---

### 1.2. Định lý Vàng: Tính chẵn lẻ của số lượng ước số
Hàm số lượng ước số của $n$ ký hiệu là $d(n)$ hoặc $\tau(n)$:
$$d(n) = \prod_{i=1}^m (\alpha_i + 1)$$

> [!NOTE]
> **Định lý về ước số lẻ**:
> Số nguyên dương $n$ có **số lượng ước số là số LẺ** khi và chỉ khi $n$ là **số chính phương**.
> $$d(n) \equiv 1 \pmod 2 \iff n = k^2$$

**Chứng minh tính chất 2 chiều**:
- **Chiều thuận ($\Rightarrow$)**: Nếu $d(n)$ là số lẻ, thì tích $\prod_{i=1}^m (\alpha_i + 1)$ là số lẻ. Tích các số nguyên là lẻ khi và chỉ khi *mọi* thừa số $(\alpha_i + 1)$ đều lẻ $\iff$ mọi $\alpha_i$ đều chẵn $\iff n$ là số chính phương.
- **Chiều hình học / ghép cặp**: Với mọi số nguyên dương $n$, các ước số của $n$ luôn xuất hiện theo từng cặp $(d, \frac{n}{d})$.
  - Nếu $d \neq \frac{n}{d}$, mỗi cặp đóng góp đúng $2$ ước số (chẵn).
  - Để tổng số ước là số lẻ, bắt buộc phải có ít nhất một cặp bị suy biến thành $d = \frac{n}{d} \iff d^2 = n$. Điều này chỉ xảy ra khi $n$ là số chính phương! $\blacksquare$

---

### 1.3. Tính chất thặng dư chính phương (Quadratic Residues)
Số chính phương khi chia cho các số nhỏ chỉ có thể nhận một tập hợp hữu hạn các số dư:
1. **Modulo 3**: $k^2 \equiv 0$ hoặc $1 \pmod 3$ (không bao giờ dư 2).
2. **Modulo 4**: $k^2 \equiv 0$ (nếu $k$ chẵn) hoặc $1 \pmod 4$ (nếu $k$ lẻ).
3. **Modulo 8**: $k^2 \equiv 0, 1$ hoặc $4 \pmod 8$.
4. **Chữ số tận cùng (Modulo 10)**: Chỉ có thể kết thúc bằng $0, 1, 4, 5, 6, 9$ (không bao giờ tận cùng bằng $2, 3, 7, 8$).

> [!TIP]
> **Kỹ thuật tối ưu hóa trong CP**: Khi cần kiểm tra hàng triệu số xem có phải số chính phương hay không, kiểm tra các modulo trên bằng bitmask trước khi gọi hàm `sqrt()` có thể loại bỏ ngay **hơn 80%** trường hợp không thỏa mãn trong $O(1)$!

---

### 1.4. Phân rã Hạt nhân Square-free (Square-free Core Decomposition)
Mọi số nguyên dương $n$ đều có thể phân tích một cách **duy nhất** thành tích của một số không chứa thừa số chính phương và một số chính phương:
$$n = s \cdot k^2$$
Trong đó:
- $s$ được gọi là **Square-free Core** (phần lõi không chính phương): tích các thừa số nguyên tố có số mũ lẻ trong $n$.
- $k^2$ là thành phần chính phương lớn nhất là ước của $n$.

*Ví dụ*:
- $n = 72 = 2^3 \cdot 3^2 = 2 \cdot (2^1 \cdot 3^1)^2 = 2 \cdot 6^2 \implies \text{core}(72) = 2, k = 6$.
- $n = 75 = 3^1 \cdot 5^2 = 3 \cdot 5^2 \implies \text{core}(75) = 3, k = 5$.

> [!IMPORTANT]
> **Bổ đề tích hai số là số chính phương**:
> Cho hai số nguyên dương $u$ và $v$. Tích $u \cdot v$ là một số chính phương khi và chỉ khi **chúng có cùng Square-free Core**:
> $$u \cdot v = k^2 \iff \text{core}(u) = \text{core}(v)$$
> 
> **Ý nghĩa trong CP**: Thay vì kiểm tra từng cặp $O(N^2)$, ta chỉ cần chuẩn hóa mỗi phần tử $A_i$ thành $\text{core}(A_i)$, sau đó dùng bảng đếm tần số (`std::map` hoặc mảng trực tiếp) để đếm số cặp thỏa mãn trong $O(N)$ hoặc $O(N \log N)$!

---

## 2. Số không chứa thừa số chính phương (Square-Free Numbers)

### 2.1. Định nghĩa
Một số nguyên dương $n \ge 1$ được gọi là **Square-free (số không chứa thừa số chính phương)** nếu nó **không chia hết cho bất kỳ số chính phương nào lớn hơn 1**:
$$\forall p \in \mathbb{P}, \quad p^2 \nmid n$$

**Đặc trưng thừa số nguyên tố**:
$$n = p_1 p_2 \dots p_k \quad (\text{với } p_i \text{ là các số nguyên tố đôi một phân biệt})$$
Nói cách khác, mọi thừa số nguyên tố của $n$ đều có số mũ đúng bằng 1.

*Ví dụ*:
- Các số square-free nhỏ: $1, 2, 3, 5, 6, 7, 10, 11, 13, 14, 15, 17, 19, 21, 22, 23, 26, 29, 30, \dots$
- Các số không square-free: $4 = 2^2, 8 = 2^3, 9 = 3^2, 12 = 2^2 \cdot 3, 18 = 2 \cdot 3^2, \dots$

---

### 2.2. Hàm Möbius $\mu(n)$ (Möbius Function)
Hàm Möbius $\mu(n)$ là công cụ toán học tối thượng gắn liền với cấu trúc square-free:
$$\mu(n) = \begin{cases} 1 & \text{nếu } n = 1 \\ (-1)^k & \text{nếu } n = p_1 p_2 \dots p_k \text{ là square-free có } k \text{ ước nguyên tố phân biệt} \\ 0 & \text{nếu } n \text{ không square-free (tồn tại } p^2 \mid n) \end{cases}$$

**Tính chất then chốt**:
1. **Nhân tính (Multiplicative)**: $\mu(a \cdot b) = \mu(a) \cdot \mu(b)$ khi $\gcd(a, b) = 1$.
2. **Tổng ước số**:
   $$\sum_{d \mid n} \mu(d) = [n = 1] = \begin{cases} 1 & \text{nếu } n = 1 \\ 0 & \text{nếu } n > 1 \end{cases}$$

---

### 2.3. Đếm số lượng Square-free $\le N$ bằng Nguyên lý Bao hàm - Loại trừ

Ta muốn đếm có bao nhiêu số $x \in [1, N]$ thỏa mãn $x$ là square-free.  
Theo nguyên lý bù trừ (Inclusion-Exclusion):
- Bắt đầu với tất cả $N$ số.
- Trừ đi các số chia hết cho $2^2, 3^2, 5^2, 7^2, \dots$ (bình phương của các số nguyên tố).
- Cộng lại các số chia hết cho $(2 \cdot 3)^2 = 6^2, (2 \cdot 5)^2 = 10^2, \dots$ (bình phương của tích 2 số nguyên tố phân biệt).
- Trừ đi các số chia hết cho $(2 \cdot 3 \cdot 5)^2 = 30^2, \dots$

Hệ số dấu đan xen chính là giá trị hàm Möbius $\mu(k)$!
$$\text{count\_sf}(N) = \sum_{k=1}^{\lfloor \sqrt{N} \rfloor} \mu(k) \left\lfloor \frac{N}{k^2} \right\rfloor$$

**Độ phức tạp tính toán**:
- Vòng lặp chạy từ $k = 1$ đến $\lfloor \sqrt{N} \rfloor$.
- Với mỗi $k$, phép tính mất $O(1)$ nếu đã tiền xử lý mảng $\mu(k)$.
- Tổng thời gian: $O(\sqrt{N})$.
- Với $N = 10^{14}$, $\sqrt{N} = 10^7$, tính toán chỉ mất chưa đầy $0.05$ giây!

---

### 2.4. Mật độ tiệm cận của số Square-free
Xác suất để một số nguyên ngẫu nhiên là square-free:
$$\lim_{N \to \infty} \frac{\text{count\_sf}(N)}{N} = \sum_{k=1}^\infty \frac{\mu(k)}{k^2} = \frac{1}{\zeta(2)} = \frac{6}{\pi^2} \approx 0.607927 \dots$$

> [!NOTE]
> Khoảng **$60.79\%$** các số nguyên dương là số square-free.  
> Điều này có nghĩa là số square-free thứ $K$ sẽ xấp xỉ nằm quanh vị trí $\frac{K}{0.6079} \approx 1.645 \cdot K$.  
> Nhận xét này giúp đặt cận trên cực kỳ chặt cho Chặt nhị phân tìm số square-free thứ $K$: $\text{high} \approx 2 \cdot K$.

---

## 3. Tổng hợp Bảng so sánh Thuật toán

| Bài toán | Phương pháp tối ưu | Độ phức tạp thời gian | Bộ nhớ |
| :--- | :--- | :---: | :---: |
| **Kiểm tra số chính phương 64-bit** | Bitmask lọc mod 64 + `sqrtl` / Binary Search | $O(1)$ | $O(1)$ |
| **Tìm Square-free Core của 1 số** | Thử ước $\le \sqrt{n}$ hoặc Sàng SPF | $O(\sqrt{n})$ hoặc $O(\log n)$ | $O(1)$ hoặc $O(\max A)$ |
| **Sàng hàm Möbius $\mu(n)$ tới $M$** | Sàng tuyến tính Euler (Linear Sieve) | $O(M)$ | $O(M)$ |
| **Đếm số Square-free $\le N$** | Công thức Bao hàm - loại trừ $\sum \mu(k) \lfloor N/k^2 \rfloor$ | $O(\sqrt{N})$ | $O(\sqrt{N})$ |
| **Tìm số Square-free thứ $K$** | Binary Search on Answer + Đếm SF | $O(\sqrt{K} \log K)$ | $O(\sqrt{K})$ |

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

