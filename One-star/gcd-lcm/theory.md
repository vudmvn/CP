# Lý thuyết chuyên sâu: Ước chung lớn nhất (GCD), Bội chung nhỏ nhất (LCM) & Thuật toán Euclid

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---


> **Chuyên mục**: Lý thuyết số cơ bản (Elementary Number Theory)  
> **Tài liệu tham khảo kinh điển**:  
> - *Introduction to Algorithms (CLRS)* – Chapter 31: Number-Theoretic Algorithms  
> - *Concrete Mathematics* (Ronald L. Graham, Donald E. Knuth, Oren Patashnik) – Chapter 4: Number Theory  
> - *Guide to Competitive Programming* (Antti Laaksonen) – Chapter 21: Number theory  
> - *CP-Algorithms* – Euclidean Algorithm, Extended Euclidean Algorithm & Linear Diophantine Equations  

---

## 1. Bản chất toán học của GCD và LCM

### 1.1. Định nghĩa
Cho hai số nguyên $a, b$ (không đồng thời bằng $0$):
- **Ước chung lớn nhất (Greatest Common Divisor - $\gcd(a, b)$)** là số nguyên dương $d$ lớn nhất sao cho $d \mid a$ và $d \mid b$.
- **Bội chung nhỏ nhất (Least Common Multiple - $\operatorname{lcm}(a, b)$)** là số nguyên dương $m$ nhỏ nhất sao cho $a \mid m$ và $b \mid m$ (với $a, b \neq 0$).
- Hai số $a, b$ được gọi là **nguyên tố cùng nhau (coprime / relatively prime)** nếu $\gcd(a, b) = 1$.

### 1.2. Góc nhìn từ Phân tích thừa số nguyên tố (Prime Factorization)
Theo **Định lý cơ bản của Số học (Fundamental Theorem of Arithmetic)**, mọi số nguyên dương $a, b > 1$ đều phân tích duy nhất thành tích các thừa số nguyên tố:
$$a = \prod_{i=1}^{k} p_i^{\alpha_i}, \quad b = \prod_{i=1}^{k} p_i^{\beta_i} \quad (\alpha_i, \beta_i \ge 0)$$

Khi đó:
$$\gcd(a, b) = \prod_{i=1}^{k} p_i^{\min(\alpha_i, \beta_i)}$$
$$\operatorname{lcm}(a, b) = \prod_{i=1}^{k} p_i^{\max(\alpha_i, \beta_i)}$$

### 1.3. Mối quan hệ đối ngẫu kinh điển
Vì với mọi số thực $x, y$, ta luôn có:
$$\min(x, y) + \max(x, y) = x + y$$
Do đó:
$$\sum_{i=1}^k [\min(\alpha_i, \beta_i) + \max(\alpha_i, \beta_i)] \cdot \ln(p_i) = \sum_{i=1}^k (\alpha_i + \beta_i) \cdot \ln(p_i)$$
Suy ra đẳng thức đối ngẫu cốt lõi:
$$\gcd(a, b) \times \operatorname{lcm}(a, b) = |a \times b|$$

> [!IMPORTANT]
> **Hệ quả tính toán**:
> $$\operatorname{lcm}(a, b) = \frac{|a \times b|}{\gcd(a, b)} = \left(\frac{|a|}{\gcd(a, b)}\right) \times |b|$$
> Luôn chia trước khi nhân để triệt tiêu nguy cơ tràn số (overflow)!

---

## 2. Thuật toán Euclid (Euclidean Algorithm)

### 2.1. Bổ đề Euclid
Với mọi cặp số nguyên $a, b$ ($b \neq 0$):
$$\gcd(a, b) = \gcd(b, a \bmod b)$$

**Chứng minh tính đúng đắn**:
Giả sử $a = q \cdot b + r$ với $r = a \bmod b$ ($0 \le r < |b|$).
- Giả sử $d$ là ước chung của $a$ và $b$ ($d \mid a$ và $d \mid b$).  
  Khi đó $r = a - q \cdot b$, vì $d \mid a$ và $d \mid q \cdot b$ nên $d \mid r$. Như vậy $d$ cũng là ước chung của $b$ và $r$.
- Ngược lại, giả sử $c$ là ước chung của $b$ và $r$ ($c \mid b$ và $c \mid r$).  
  Khi đó $a = q \cdot b + r$, vì $c \mid q \cdot b$ và $c \mid r$ nên $c \mid a$. Như vậy $c$ cũng là ước chung của $a$ và $b$.

Tập các ước chung của $(a, b)$ hoàn toàn trùng khớp với tập các ước chung của $(b, a \bmod b)$. Do đó số lớn nhất trong hai tập hợp phải bằng nhau: $\gcd(a, b) = \gcd(b, a \bmod b)$. $\blacksquare$

### 2.2. Thuật toán đệ quy
```cpp
long long gcd(long long a, long long b) {
    return b == 0 ? std::abs(a) : gcd(b, a % b);
}
```

### 2.3. Phân tích độ phức tạp & Định lý Lamé
- **Định lý Lamé (Gabriel Lamé, 1844)**: Số bước chia lấy dư khi thực hiện thuật toán Euclid trên hai số $a > b > 0$ **không vượt quá 5 lần số chữ số thập phân của $b$**.
- **Trường hợp xấu nhất (Worst-case)**: Xảy ra khi hai số $a$ và $b$ là hai số Fibonacci liên tiếp:
  $$a = F_{k+1}, \quad b = F_k$$
  Khi đó $F_{k+1} \bmod F_k = F_{k-1}$, thuật toán duyệt qua toàn bộ dãy Fibonacci lùi về $F_1, F_0$ với đúng $k$ bước.
  Vì $F_k \approx \frac{\phi^k}{\sqrt{5}}$ (với $\phi = \frac{1 + \sqrt{5}}{2} \approx 1.618$), số bước $k = O(\log_\phi b)$.
- **Độ phức tạp thời gian**:
  $$T(a, b) = O(\log(\min(a, b)))$$
  Với số nguyên 64-bit (`long long` $\le 10^{18}$), thuật toán Euclid chạy không quá **90 bước chia** — tức diễn ra trong chưa đầy $0.1$ microsecond!

---

## 3. Thuật toán Euclid mở rộng (Extended Euclidean Algorithm)

### 3.1. Định lý Bézout
Cho hai số nguyên $a, b$ không đồng thời bằng $0$. Luôn tồn tại hai số nguyên $x, y \in \mathbb{Z}$ sao cho:
$$a \cdot x + b \cdot y = \gcd(a, b)$$
Giá trị $\gcd(a, b)$ chính là số nguyên dương nhỏ nhất có thể biểu diễn dưới dạng tổ hợp tuyến tính $a \cdot x + b \cdot y$.

### 3.2. Thuật toán tìm nghiệm Bézout $(x, y)$
Thuật toán Euclid mở rộng vừa tính $\gcd(a, b)$ vừa truy vết ngược lại để tìm một nghiệm nguyên cụ thể $(x, y)$.

**Quy nạp từ bước cơ sở**:
- **Trường hợp cơ sở**: Khi $b = 0$, ta có $\gcd(a, 0) = a$. Phương trình trở thành:
  $$a \cdot x + 0 \cdot y = a \implies x = 1, \quad y = 0$$
- **Bước quy nạp**: Giả sử qua lời gọi đệ quy $(b, a \bmod b)$, ta đã tìm được $(x_1, y_1)$ thỏa:
  $$b \cdot x_1 + (a \bmod b) \cdot y_1 = g$$
  Biết rằng $a \bmod b = a - \lfloor a / b \rfloor \cdot b$. Thay vào phương trình trên:
  $$b \cdot x_1 + (a - \lfloor a / b \rfloor \cdot b) \cdot y_1 = g$$
  $$\iff a \cdot y_1 + b \cdot (x_1 - \lfloor a / b \rfloor \cdot y_1) = g$$
  So sánh với dạng gốc $a \cdot x + b \cdot y = g$, ta suy ra nghiệm của bước hiện tại:
  $$\begin{cases} x = y_1 \\ y = x_1 - \lfloor a / b \rfloor \cdot y_1 \end{cases}$$

### 3.3. Độ phức tạp
Tương tự thuật toán Euclid thông thường:
- **Thời gian**: $O(\log(\min(a, b)))$
- **Không gian**: $O(\log(\min(a, b)))$ theo ngăn xếp đệ quy (hoặc $O(1)$ nếu lặp).

---

## 4. Phương trình Diophantine tuyến tính ($a \cdot x + b \cdot y = c$)

### 4.1. Điều kiện có nghiệm
Phương trình vô định tuyến tính hai ẩn:
$$a \cdot x + b \cdot y = c \quad (a, b, c \in \mathbb{Z})$$
có nghiệm nguyên $(x, y)$ khi và chỉ khi:
$$\gcd(a, b) \mid c$$

### 4.2. Tìm một nghiệm riêng $(x_0, y_0)$
Đặt $g = \gcd(a, b)$. Nếu $c$ không chia hết cho $g$, phương trình vô nghiệm.  
Nếu $g \mid c$, dùng Euclid mở rộng giải phương trình Bézout tìm $(x_g, y_g)$ thỏa $a \cdot x_g + b \cdot y_g = g$.  
Nhân cả hai vế với $\frac{c}{g}$:
$$a \cdot \underbrace{\left(x_g \cdot \frac{c}{g}\right)}_{x_0} + b \cdot \underbrace{\left(y_g \cdot \frac{c}{g}\right)}_{y_0} = c$$
Vậy một nghiệm riêng là:
$$x_0 = x_g \cdot \frac{c}{g}, \quad y_0 = y_g \cdot \frac{c}{g}$$

### 4.3. Họ nghiệm tổng quát
Mọi nghiệm nguyên $(x, y)$ của phương trình đều có dạng:
$$\begin{cases} x = x_0 + k \cdot \dfrac{b}{g} \\ y = y_0 - k \cdot \dfrac{a}{g} \end{cases} \quad (k \in \mathbb{Z})$$

### 4.4. Ứng dụng trong CP: Tìm nghiệm thỏa điều kiện
- **Nghiệm có $x$ dương nhỏ nhất**: Chọn $k$ sao cho $x > 0$ nhỏ nhất bằng phép chia lấy dư.
- **Đếm số nghiệm trong khoảng $x \in [x_{\min}, x_{\max}]$ và $y \in [y_{\min}, y_{\max}]$**: Giải hệ bất phương trình theo ẩn nguyên $k$, giao hai đoạn $[k_{1, \min}, k_{1, \max}] \cap [k_{2, \min}, k_{2, \max}]$ cho số lượng nghiệm nguyên hợp lệ trong $O(1)$.

---

## 5. Các Bổ đề & Tính chất "Vàng" trong Lập trình thi đấu

### 5.1. Bổ đề biến đổi hiệu (Difference Invariance)
$$\gcd(a, b) = \gcd(a, |b - a|) = \gcd(a, a + b)$$
Tổng quát hóa với dãy số:
$$\gcd(A_1, A_2, A_3, \dots, A_n) = \gcd(A_1, |A_2 - A_1|, |A_3 - A_1|, \dots, |A_n - A_1|)$$

> [!TIP]
> **Ứng dụng siêu kinh điển (Row GCD - Codeforces 1458A)**:
> Tính $\gcd(A_1 + x, A_2 + x, \dots, A_n + x)$ với nhiều giá trị $x$:
> Biến đổi thành $\gcd(A_1 + x, |A_2 - A_1|, |A_3 - A_1|, \dots, |A_n - A_1|)$.
> Nhận xét: Phần sau $G = \gcd(|A_2 - A_1|, \dots, |A_n - A_1|)$ hoàn toàn **cố định và không phụ thuộc vào $x$**!
> Ta tiền xử lý $G$ trong $O(N)$, mỗi truy vấn trả lời trong $O(\log)$ bằng $\gcd(A_1 + x, G)$.

### 5.2. Tính chất $O(\log(\max A))$ của dãy GCD tiền tố
Cho mảng số nguyên dương $A_1, A_2, \dots, A_n$. Xét dãy giá trị:
$$g_1 = A_1, \quad g_2 = \gcd(g_1, A_2), \quad \dots, \quad g_i = \gcd(g_{i-1}, A_i)$$
- Dãy $g_1, g_2, \dots, g_n$ là dãy **không tăng**: $g_1 \ge g_2 \ge \dots \ge g_n$.
- Mỗi khi $g_i < g_{i-1}$, thì $g_i$ phải là một ước thực sự của $g_{i-1}$, suy ra $g_i \le \frac{g_{i-1}}{2}$.
- Do đó, dãy GCD tiền tố **chỉ thay đổi giá trị tối đa $\lfloor \log_2(A_1) \rfloor \le 60$ lần**!

**Ứng dụng**:
- Đếm số lượng đoạn con có $\gcd = X$: Duy trì danh sách các giá trị GCD khác nhau kết thúc tại vị trí $i$. Tại mỗi bước, danh sách này có độ dài không quá $\approx 60$, giúp giải quyết bài toán trong $O(N \log(\max A))$.

### 5.3. Tính chất lũy thừa và Fibonacci
1. $\gcd(a^m - 1, a^n - 1) = a^{\gcd(m, n)} - 1$ với mọi số nguyên $a \ge 2$.
2. $\gcd(F_m, F_n) = F_{\gcd(m, n)}$ (với $F_n$ là số Fibonacci thứ $n$).

---

## 6. So sánh hiệu năng các phương pháp tính GCD

| Phương pháp | Ý tưởng chính | Ưu điểm | Nhược điểm | Độ phức tạp |
| :--- | :--- | :--- | :--- | :---: |
| **Trừ liên tiếp (Naive Euclid)** | $\gcd(a, b) = \gcd(a - b, b)$ | Cực kỳ đơn giản | Rất chậm khi $a \gg b$ (ví dụ $\gcd(10^{18}, 1)$ cần $10^{18}$ bước) | $O(\max(a, b))$ |
| **Chia dư (Standard Euclid)** | $\gcd(a, b) = \gcd(b, a \bmod b)$ | Nhanh, chuẩn mực, có sẵn trong `std::gcd` | Phép chia `%` trên CPU tốn $\approx 10-20$ chu kỳ lệnh | $O(\log(\min(a, b)))$ |
| **Binary GCD (Stein's Algorithm)** | Khử phép chia, chỉ dùng phép trừ và dịch bit (`>> 1`) | Không dùng phép `%`, cực nhanh trên vi xử lý | Cài đặt dài hơn Standard Euclid | $O(\log(\min(a, b)))$ |

```text
       Hiệu năng thực tế khi tính 10^7 cặp số ngẫu nhiên 64-bit:
       Standard std::gcd  : ~ 0.18s
       Binary GCD (Stein) : ~ 0.11s  (nhanh hơn ~ 40%)
```

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

