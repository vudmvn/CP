# 🎯 Tuyển tập Bài tập: Kỹ thuật Băm (Hashing) & Anti-Hash

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Biên soạn:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.

---

## 📌 Bảng Tổng hợp Bài tập Tuyển chọn

| STT | Bài toán | Nền tảng | Độ khó | Kỹ thuật trọng tâm |
| :---: | :--- | :---: | :---: | :--- |
| 1 | [String Matching](https://cses.fi/problemset/task/1753) | CSES 1753 | ⭐ Cơ bản | So khớp chuỗi mẫu bằng thuật toán Rabin-Karp $O(N + M)$ |
| 2 | [Find First Occurrence](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/) | LeetCode 28 | ⭐ Cơ bản | Cài đặt Rolling Hash cơ bản tìm kiếm chuỗi con |
| 3 | [Repeated DNA Sequences](https://leetcode.com/problems/repeated-dna-sequences/) | LeetCode 187 | ⭐ Cơ bản | Rolling Hash trên cửa sổ trượt độ dài 10 cố định |
| 4 | [Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome/) | LeetCode 214 | ⭐ Cơ bản | Băm xuôi băm ngược tìm tiền tố Palindrome dài nhất |
| 5 | [Longest Happy Prefix](https://leetcode.com/problems/longest-happy-prefix/) | LeetCode 1392 | ⭐ Cơ bản | Rolling Hash so khớp tiền tố và hậu tố dài nhất |
| 6 | [Word Combinations](https://cses.fi/problemset/task/1731) | CSES 1731 | ⭐ Cơ bản | DP ghép từ điển kết hợp Rolling Hash / Trie |
| 7 | [Finding Borders](https://cses.fi/problemset/task/1732) | CSES 1732 | ⭐⭐ Trung cấp | Tìm mọi độ dài tiền tố bằng hậu tố bằng $O(1)$ Hash |
| 8 | [Finding Periods](https://cses.fi/problemset/task/1733) | CSES 1733 | ⭐⭐ Trung cấp | Kiểm tra chu kỳ xâu bằng so sánh các khối Hash |
| 9 | [Good Substrings](https://codeforces.com/problemset/problem/271/D) | Codeforces 271D | ⭐⭐ Trung cấp | Đếm xâu con phân biệt thỏa mãn điều kiện bằng Hash |
| 10 | [Compress Words](https://codeforces.com/problemset/problem/1200/E) | Codeforces 1200E | ⭐⭐ Trung cấp | Ghép các từ liên tiếp bằng đoạn chồng chập dài nhất |
| 11 | [Longest Duplicate Substring](https://leetcode.com/problems/longest-duplicate-substring/) | LeetCode 1044 | ⭐⭐ Trung cấp | Chặt nhị phân độ dài + Double Hashing chống va chạm |
| 12 | [Palindrome Queries](https://cses.fi/problemset/task/2420) | CSES 2420 | ⭐⭐ Trung cấp | Segment Tree kết hợp Băm xuôi băm ngược động |
| 13 | [Three Occurrences](https://codeforces.com/problemset/problem/1418/G) | Codeforces 1418G | ⭐⭐⭐ Nâng cao | XOR Hashing / Zobrist Hash đếm mảng con tần suất chia hết cho 3 |
| 14 | [Palindromic Characteristics](https://codeforces.com/problemset/problem/835/D) | Codeforces 835D | ⭐⭐⭐ Nâng cao | DP bậc Palindrome $k$ kết hợp $O(1)$ Substring Hashing |
| 15 | [Finding Patterns](https://cses.fi/problemset/task/2102) | CSES 2102 | ⭐⭐⭐ Nâng cao | Đa mẫu so khớp: Phân nhóm độ dài + Hash Table |
| 16 | [PALINY - Xâu Đối xứng Dài nhất](https://oj.vnoi.info/problem/paliny) | VNOI PALINY | ⭐⭐⭐ Nâng cao | Chặt nhị phân độ dài tâm Palindrome + Rolling Hash |

---

# 📝 HƯỚNG DẪN GIẢI CHI TIẾT & GỢI Ý CHIẾN THUẬT

## 1. Nhóm Bài tập Cơ bản (6 bài)

### Bài 1: [CSES 1753 - String Matching](https://cses.fi/problemset/task/1753)
- **Đề bài tóm tắt**: Cho xâu văn bản $S$ và xâu mẫu $P$ ($|S|, |P| \le 10^6$). Đếm số lần $P$ xuất hiện trong $S$.
- **Phân tích & Thuật toán**:
  - Áp dụng thuật toán **Rabin-Karp**:
    - Tiền xử lý băm mảng tiền tố của $S$ và tính mã băm của $P$ trong $O(|S| + |P|)$.
    - Sử dụng Double Hashing hoặc Mersenne 61-bit Hash với **Base ngẫu nhiên** để tránh bị hack.
    - Quét mọi đoạn con độ dài $|P|$ của $S$ trong $O(1)$, tăng biến đếm nếu mã băm khớp.
  - Độ phức tạp: $O(|S| + |P|)$ thời gian, $O(|S|)$ không gian.

---

### Bài 2: [LeetCode 28 - Find First Occurrence](https://leetcode.com/problems/find-the-index-of-the-first-occurrence-in-a-string/)
- **Đề bài tóm tắt**: Tìm chỉ số xuất hiện đầu tiên của chuỗi `needle` trong chuỗi `haystack`.
- **Phân tích & Thuật toán**:
  - Cài đặt cơ bản Rolling Hash: Duyệt qua từng vị trí $i$, so sánh `getHash(i, i + m - 1) == needleHash`.
  - Trả về vị trí $i$ đầu tiên tìm thấy.

---

### Bài 3: [LeetCode 187 - Repeated DNA Sequences](https://leetcode.com/problems/repeated-dna-sequences/)
- **Đề bài tóm tắt**: Cho chuỗi DNA chỉ gồm các ký tự `A, C, G, T`. Tìm tất cả các chuỗi con độ dài 10 xuất hiện nhiều hơn 1 lần.
- **Phân tích & Thuật toán**:
  - Vì độ dài cố định $K = 10$, ta có thể mã hóa mỗi ký tự bằng 2 bit: `A -> 00, C -> 01, G -> 10, T -> 11`.
  - Cửa sổ trượt 10 ký tự tương đương với một số nguyên 20 bit:
    $$\text{mask} = ((\text{mask} \ll 2) | \text{char\_val}) \ \& \ ((1 \ll 20) - 1)$$
  - Dùng bảng băm hoặc mảng đếm tần suất đánh dấu các mask xuất hiện.
  - Độ phức tạp: $O(N)$ thời gian, $O(N)$ bộ nhớ.

---

### Bài 4: [LeetCode 214 - Shortest Palindrome](https://leetcode.com/problems/shortest-palindrome/)
- **Đề bài tóm tắt**: Thêm các ký tự vào đầu xâu $S$ để tạo thành xâu đối xứng ngắn nhất có thể.
- **Phân tích & Thuật toán**:
  - Bài toán quy về: **Tìm tiền tố Palindrome dài nhất** của $S$.
  - Duyệt $i$ từ $0$ đến $N - 1$:
    - Tính mã băm xuôi: $hash_{fwd} = (hash_{fwd} \cdot B + S[i]) \pmod M$.
    - Tính mã băm ngược: $hash_{rev} = (hash_{rev} + S[i] \cdot B^i) \pmod M$.
    - Nếu $hash_{fwd} == hash_{rev}$, đoạn $S[0 \dots i]$ là một Palindrome!
  - Lấy $i$ lớn nhất thỏa mãn, đảo ngược phần đuôi còn lại $S[i+1 \dots N-1]$ và ghép lên đầu $S$.
  - Độ phức tạp: $O(N)$ thời gian, $O(1)$ phụ trợ.

---

### Bài 5: [LeetCode 1392 - Longest Happy Prefix](https://leetcode.com/problems/longest-happy-prefix/)
- **Đề bài tóm tắt**: Tìm xâu tiền tố dài nhất khác toàn bộ chuỗi $S$ mà cũng đồng thời là hậu tố của $S$.
- **Phân tích & Thuật toán**:
  - Duyệt độ dài $len$ từ $1$ đến $N - 1$:
    - So sánh mã băm của tiền tố $S[0 \dots len - 1]$ và hậu tố $S[N - len \dots N - 1]$.
    - Lưu lại độ dài lớn nhất có mã băm trùng nhau.
  - Độ phức tạp: $O(N)$ thời gian.

---

### Bài 6: [CSES 1731 - Word Combinations](https://cses.fi/problemset/task/1731)
- **Đề bài tóm tắt**: Cho xâu $S$ độ dài $N$ và từ điển gồm $K$ từ. Đếm số cách ghép các từ trong từ điển để tạo thành xâu $S$ (modulo $10^9+7$).
- **Phân tích & Thuật toán**:
  - Quy hoạch động: $DP[i]$ là số cách ghép xâu con $S[i \dots N - 1]$.
  - Công thức: $DP[i] = \sum DP[i + len]$ với mọi từ trong từ điển có độ dài $len$ khớp với $S[i \dots i + len - 1]$.
  - Sử dụng cây Trie hoặc bảng băm lưu mã băm của các từ điển phân loại theo độ dài để kiểm tra khớp trong $O(1)$.

---

## 2. Nhóm Bài tập Trung cấp (6 bài)

### Bài 7: [CSES 1732 - Finding Borders](https://cses.fi/problemset/task/1732)
- **Đề bài tóm tắt**: Cho xâu $S$. Tìm mọi độ dài $L$ ($1 \le L < N$) sao cho tiền tố độ dài $L$ bằng hậu tố độ dài $L$.
- **Phân tích & Thuật toán**:
  - Tiền xử lý băm tiền tố $S$.
  - Duyệt $L$ từ $1$ đến $N - 1$:
    - So sánh `getHash(0, L - 1) == getHash(N - L, N - 1)`.
    - Nếu bằng nhau, in $L$.
  - Độ phức tạp: $O(N)$ thời gian.

---

### Bài 8: [CSES 1733 - Finding Periods](https://cses.fi/problemset/task/1733)
- **Đề bài tóm tắt**: Một xâu có chu kỳ $P$ nếu nó có thể được biểu diễn dưới dạng lặp lại của tiền tố độ dài $P$ (phần cuối có thể bị cắt ngắn). Tìm mọi chu kỳ $P$ có thể của xâu.
- **Phân tích & Thuật toán**:
  - Với mỗi độ dài $P \in [1, N]$:
    - Ta cần kiểm tra xem $S[0 \dots N - 1 - P]$ có bằng $S[P \dots N - 1]$ hay không!
    - So sánh mã băm trong $O(1)$: `getHash(0, N - 1 - P) == getHash(P, N - 1)`.
    - Nếu bằng nhau, $P$ là một chu kỳ hợp lệ!
  - Độ phức tạp: $O(N)$ thời gian, duyệt cực kỳ đơn giản và thanh lịch.

---

### Bài 9: [Codeforces 271D - Good Substrings](https://codeforces.com/problemset/problem/271/D)
- **Đề bài tóm tắt**: Cho xâu $S$ và định nghĩa các ký tự tốt/xấu. Đếm số lượng **xâu con phân biệt** chứa không quá $K$ ký tự xấu.
- **Phân tích & Thuật toán**:
  - Duyệt mọi vị trí bắt đầu $i$ từ $0$ đến $N - 1$:
    - Duy trì số lượng ký tự xấu `bad_count` khi mở rộng $j$ từ $i$ đến $N - 1$.
    - Nếu `bad_count > K`, dừng mở rộng.
    - Ngược lại, lấy mã băm của $S[i \dots j]$ đẩy vào một `set` (hoặc cấu trúc băm kép).
  - Kết quả là số lượng mã băm phân biệt thu được.
  - Độ phức tạp: $O(N^2 \log N)$ hoặc $O(N^2)$ với Custom Hash.

---

### Bài 10: [Codeforces 1200E - Compress Words](https://codeforces.com/problemset/problem/1200/E)
- **Đề bài tóm tắt**: Cho $N$ từ. Lần lượt ghép từng từ vào chuỗi kết quả: nếu từ mới có tiền tố trùng với hậu tố của chuỗi hiện có thì chồng chập đoạn dài nhất đó lên nhau.
- **Phân tích & Thuật toán**:
  - Khi ghép từ mới $W$ vào chuỗi kết quả $R$:
    - Chiều dài chồng chập tối đa là $L_{max} = \min(|W|, |R|)$.
    - Duyệt $len$ từ $L_{max}$ giảm về $1$: so sánh mã băm hậu tố của $R$ và tiền tố của $W$.
    - Đoạn dài nhất khớp nhau sẽ được bỏ qua, chỉ nối phần đuôi còn lại của $W$ vào $R$.
  - Cập nhật tiếp mảng băm tiền tố của $R$.
  - Tổng độ phức tạp: $O(\sum |W_i|)$.

---

### Bài 11: [LeetCode 1044 - Longest Duplicate Substring](https://leetcode.com/problems/longest-duplicate-substring/)
- **Đề bài tóm tắt**: Tìm xâu con xuất hiện ít nhất 2 lần trong xâu $S$ có độ dài lớn nhất.
- **Phân tích & Thuật toán**:
  - Tính chất đơn điệu: Nếu tồn tại xâu con trùng lặp độ dài $L$, thì luôn tồn tại xâu con trùng lặp độ dài $< L$.
  - **Chặt nhị phân độ dài $L$** trong khoảng $[1, N - 1]$:
    - Với độ dài $L$: Dùng Rolling Hash quét qua mọi xâu con độ dài $L$, lưu mã băm vào `SafeUnorderedMap` hoặc `set`.
    - Nếu phát hiện mã băm đã xuất hiện $\implies$ Tìm thấy xâu trùng lặp, mở rộng miền tìm kiếm $L$.
  - Bắt buộc dùng **Double Hashing với Base ngẫu nhiên** để vượt qua các test cases chống băm ác ý trên LeetCode.
  - Độ phức tạp: $O(N \log N)$.

---

### Bài 12: [CSES 2420 - Palindrome Queries](https://cses.fi/problemset/task/2420)
- **Đề bài tóm tắt**: Cho xâu $S$. Hỗ trợ $Q$ truy vấn:
  1. Thay đổi ký tự tại vị trí $k$ thành $c$.
  2. Kiểm tra đoạn con $S[a \dots b]$ có phải là Palindrome hay không.
- **Phân tích & Thuật toán**:
  - Kết hợp **Segment Tree** và **Rolling Hash**:
    - Xây dựng 2 cây Segment Tree: Cây 1 lưu mã băm xuôi, Cây 2 lưu mã băm ngược.
    - Mỗi nút quản lý đoạn $[L, R]$, lưu giá trị băm của đoạn đó.
    - Thao tác cập nhật ký tự: Cập nhật điểm trên Segment Tree trong $O(\log N)$.
    - Truy vấn Palindrome: Lấy mã băm xuôi và ngược của đoạn $[a, b]$, so sánh trong $O(\log N)$.
  - Độ phức tạp: $O((N + Q) \log N)$.

---

## 3. Nhóm Bài tập Nâng cao (4 bài)

### Bài 13: [Codeforces 1418G - Three Occurrences](https://codeforces.com/problemset/problem/1418/G)
- **Đề bài tóm tắt**: Cho mảng $N$ phần tử. Đếm số mảng con liên tiếp mà mỗi giá trị xuất hiện trong mảng con đó đều có số lần xuất hiện chia hết cho $3$.
- **Phân tích & Thuật toán**:
  - Kỹ thuật **Zobrist / Hash Vector Modulo 3**:
    - Với mỗi giá trị phân biệt $x$, sinh ngẫu nhiên 2 số 64-bit $w_1(x), w_2(x)$.
    - Biểu diễn số lần xuất hiện modulo 3:
      - Lần $1 \pmod 3$: gán trọng số $+w_1(x)$.
      - Lần $2 \pmod 3$: gán trọng số $+w_2(x)$.
      - Lần $0 \pmod 3$: trừ đi $(w_1(x) + w_2(x))$ để triệt tiêu về $0$.
    - Mảng con $[L, R]$ thỏa mãn khi mã băm tiền tố $H[R] == H[L - 1]$.
  - Kết hợp kỹ thuật Hai con trỏ loại bỏ các đoạn có phần tử xuất hiện $> 3$ lần.
  - Độ phức tạp: $O(N)$.

---

### Bài 14: [Codeforces 835D - Palindromic Characteristics](https://codeforces.com/problemset/problem/835/D)
- **Đề bài tóm tắt**: Một xâu là $1$-palindrome nếu nó là palindrome. Nó là $k$-palindrome nếu nó là palindrome và nửa đầu cũng như nửa sau của nó là $(k-1)$-palindrome. Đếm số lượng $k$-palindrome với mọi $1 \le k \le N$.
- **Phân tích & Thuật toán**:
  - Quy hoạch động: $DP[L][R]$ là cấp độ palindrome của xâu con $S[L \dots R]$.
  - Dùng Rolling Hash để kiểm tra tính Palindrome của $S[L \dots R]$ trong $O(1)$ và so sánh nửa đầu $S[L \dots mid]$ với nửa sau.
  - Nếu $S[L \dots R]$ là Palindrome:
    $$DP[L][R] = DP[L][L + len / 2 - 1] + 1$$
  - Độ phức tạp: $O(N^2)$ thời gian và bộ nhớ.

---

### Bài 15: [CSES 2102 - Finding Patterns](https://cses.fi/problemset/task/2102)
- **Đề bài tóm tắt**: Cho xâu $S$ và $K$ xâu mẫu $P_1, \dots, P_k$. Với mỗi xâu mẫu, xác định xem nó có xuất hiện trong $S$ hay không.
- **Phân tích & Thuật toán**:
  - Gom nhóm các xâu mẫu có cùng độ dài. Vì tổng độ dài các xâu mẫu $\le 5 \cdot 10^5$, số lượng độ dài phân biệt không quá $O(\sqrt{\sum |P|}) \approx 1000$.
  - Với mỗi độ dài $L$:
    - Dùng Rolling Hash quét xâu $S$ để lấy tất cả các xâu con độ dài $L$ đẩy vào `SafeUnorderedMap`.
    - Kiểm tra các mẫu độ dài $L$ trong $O(1)$.
  - Độ phức tạp: $O(|S| \sqrt{\sum |P|} + \sum |P|)$.

---

### Bài 16: [VNOI - PALINY (Xâu Con Đối xứng Dài nhất)](https://oj.vnoi.info/problem/paliny)
- **Đề bài tóm tắt**: Tìm độ dài xâu con đối xứng liên tiếp dài nhất trong xâu $S$ ($N \le 5 \cdot 10^4$).
- **Phân tích & Thuật toán**:
  - Xét riêng hai trường hợp Palindrome có độ dài lẻ và độ dài chẵn.
  - Với mỗi tâm $i$: **Chặt nhị phân bán kính mở rộng $R$**:
    - Dùng Rolling Hash kiểm tra xem nửa trái mở rộng có khớp với nửa phải mở rộng trong $O(1)$ hay không.
  - Tìm bán kính lớn nhất cho từng tâm và cập nhật kết quả.
  - Độ phức tạp: $O(N \log N)$.

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
