# Tuyển tập bài tập: Kỹ thuật Hai con trỏ & Cửa sổ trượt (Two Pointers & Sliding Window)

> **Biên soạn:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tuyển tập bài tập và hướng dẫn thuật toán ICPC / OLP.

---


> Tuyển tập bài tập được chọn lọc từ các nền tảng uy tín (**CSES, Codeforces, VNOJ, LeetCode, AtCoder**) và phân cấp từ **Cơ bản** đến **Nâng cao**, kèm hướng dẫn phương pháp giải và phân tích ý tưởng.

---

## 🟢 Cấp độ 1: Cơ bản & Nền tảng (Easy / Direct Application)
*Mục tiêu*: Thành thạo mô hình 2 con trỏ co từ 2 đầu mảng sắp xếp và cửa sổ trượt cơ bản.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 1 | [**Sum of Two Values**](https://cses.fi/problemset/task/1640) | CSES | Two Pointers đối hướng (2Sum) | Cho mảng $N$ số và mục tiêu $X$. Lưu chỉ số ban đầu, sắp xếp mảng tăng dần. Đặt $L = 1, R = N$: nếu $A[L] + A[R] = X$ tìm thấy; nếu $< X$ thì $L{+}{+}$; nếu $> X$ thì $R{-}{-}$. Độ phức tạp $O(N \log N)$. |
| 2 | [**Two Sum II - Input Array Is Sorted**](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/) | LeetCode 167 | Opposing Two Pointers | Mảng đã được sắp xếp sẵn. Co 2 con trỏ từ 2 biên vào giữa trong $O(N)$ thời gian và $O(1)$ bộ nhớ. |
| 3 | [**Books**](https://codeforces.com/problemset/problem/279/B) | Codeforces | Sliding Window (Tổng $\le T$) | Cho thời gian đọc $N$ cuốn sách và thời gian rảnh $t$. Duy trì cửa sổ $[L, R]$, mở rộng $R$ và cộng dồn thời gian. Khi tổng thời gian $> t$, tịnh tiến $L$ sang phải để loại bớt sách. Cập nhật kết quả lớn nhất $\max(R - L + 1)$ trong $O(N)$. |
| 4 | [**Remove Duplicates from Sorted Array**](https://leetcode.com/problems/remove-duplicates-from-sorted-array/) | LeetCode 26 | Fast/Slow Pointers | Dùng con trỏ chậm $slow$ ghi nhận vị trí phần tử không trùng lặp duy nhất tiếp theo, con trỏ nhanh $fast$ duyệt qua toàn bộ mảng. Thay đổi mảng in-place $O(1)$ memory. |
| 5 | [**Subarray Sums I**](https://cses.fi/problemset/task/1660) | CSES | Sliding Window (Mảng dương) | Cho mảng gồm các số nguyên **dương**, đếm số đoạn con liên tiếp có tổng đúng bằng $X$. Dùng 2 con trỏ cùng chiều: nới $R$ khi tổng $< X$, co $L$ khi tổng $> X$. |

---

## 🟡 Cấp độ 2: Vận dụng & Biến thể đặc biệt (Medium / Intermediate)
*Mục tiêu*: Áp dụng cho bài toán 3Sum/4Sum, tối ưu hóa diện tích/khoảng cách, và đếm đoạn con có điều kiện tần số.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 6 | [**Sum of Three Values**](https://cses.fi/problemset/task/1641) | CSES | 3Sum ($O(N^2)$) | Cố định chỉ số $i$ từ $1$ đến $N-2$, bài toán quy về tìm 2 số trong đoạn $[i+1, N]$ có tổng bằng $X - A[i]$. Áp dụng Two Pointers đối hướng giải trong $O(N)$, tổng thời gian toàn bài $O(N^2)$. |
| 7 | [**Container With Most Water**](https://leetcode.com/problems/container-with-most-water/) | LeetCode 11 | Opposing Two Pointers | Đặt $L = 1, R = N$. Diện tích nước $= (R - L) \times \min(H[L], H[R])$. Tịnh tiến con trỏ có độ cao thấp hơn vào trong vì việc giữ lại cột thấp hơn không thể tạo ra diện tích lớn hơn ở bất kỳ khoảng cách hẹp hơn nào. |
| 8 | [**Alice, Bob and Chocolate**](https://codeforces.com/problemset/problem/6/C) | Codeforces | Mô phỏng 2 con trỏ đối hướng | Hai người ăn thanh socola từ 2 phía với thời gian ăn mỗi thanh khác nhau. Duy trì thời điểm hoàn thành thanh hiện tại của mỗi người để quyết định ai ăn thanh tiếp theo. |
| 9 | [**Subarray Distinct Values**](https://cses.fi/problemset/task/2428) | CSES | Cửa sổ trượt + Đếm phân biệt | Đếm số đoạn con chứa tối đa $K$ giá trị phân biệt. Dùng cửa sổ $[L, R]$ kết hợp `map` đếm tần số: khi số loại giá trị $> K$, tăng $L$ và giảm tần số. Với mỗi $R$, số đoạn con hợp lệ kết thúc tại $R$ là $R - L + 1$. Tổng độ phức tạp $O(N \log N)$ hoặc $O(N)$ nếu nén tọa độ. |
| 10 | [**Longest Substring Without Repeating Characters**](https://leetcode.com/problems/longest-substring-without-repeating-characters/) | LeetCode 3 | Variable-size Window | Tìm độ dài chuỗi con không chứa ký tự lặp lại. Dùng mảng đánh dấu vị trí xuất hiện cuối cùng của từng ký tự ASCII để nhảy $L$ trực tiếp: $L = \max(L, \text{last}[s[R]] + 1)$. |
| 11 | [**Sum of Four Values**](https://cses.fi/problemset/task/1642) | CSES | 4Sum | Duyệt cặp $(i, j)$ cố định hoặc kết hợp lưu tổng từng cặp $(A[i] + A[j])$ vào bảng băm / cấu trúc mảng đã sắp xếp và truy vấn cặp bù tương ứng. |

---

## 🔴 Cấp độ 3: Nâng cao & Kỹ thuật chuyển bài (Hard / Advanced ICPC)
*Mục tiêu*: Thành thạo kỹ thuật giảm bài $\text{Exact}(K) = \text{AtMost}(K) - \text{AtMost}(K-1)$, tính tổng độ dài đoạn con, và tối ưu hóa điều kiện phức tạp.

| STT | Tên bài | Nguồn | Dạng bài / Tags | Tóm tắt ý tưởng & Phương pháp |
| :---: | :--- | :---: | :--- | :--- |
| 12 | [**Minimum Window Substring**](https://leetcode.com/problems/minimum-window-substring/) | LeetCode 76 | Cửa sổ trượt đa tần số | Tìm xâu con ngắn nhất của $S$ chứa toàn bộ ký tự của $T$. Dùng biến `matched_chars` đếm số ký tự đã đạt đủ tần số yêu cầu. Khi đã đủ, liên tục co $L$ để thu hẹp độ dài nhỏ nhất có thể. |
| 13 | [**Enough Array**](https://atcoder.jp/contests/abc130/tasks/abc130_d) | AtCoder ABC 130 D | Two Pointers (Đoạn con tổng $\ge K$) | Cho mảng số nguyên dương, đếm số đoạn con có tổng $\ge K$. Với mỗi $L$, tìm vị trí $R$ nhỏ nhất sao cho $\sum_{i=L}^R A_i \ge K$. Khi đó tất cả các đoạn kết thúc từ $R$ đến $N$ đều hợp lệ, đóng góp $N - R + 1$ đoạn con. Vì $A_i > 0$, con trỏ $R$ luôn tăng đơn điệu. |
| 14 | [**Subarrays with K Different Integers**](https://leetcode.com/problems/subarrays-with-k-different-integers/) | LeetCode 992 | Kỹ thuật AtMost Reduction | Đếm số đoạn con chứa đúng $K$ số nguyên phân biệt. Chuyển đổi thành $\text{solve}(K) - \text{solve}(K-1)$, trong đó $\text{solve}(k)$ là số đoạn con có tối đa $k$ số phân biệt giải bằng Sliding Window cơ bản trong $O(N)$. |
| 15 | [**Segment with Small Spread**](https://codeforces.com/edu/course/2/lesson/9/2/practice/contest/279653/problem/F) | Codeforces EDU | Two Pointers + 2 Stacks | Đếm số đoạn con có $(\max - \min) \le K$. Vì thao tác xóa ở đầu cửa sổ không hỗ trợ trực tiếp việc tìm $\max/\min$ trong $O(1)$, ta sử dụng cấu trúc **Queue bằng 2 Stacks** (hoặc `multiset` trong $O(N \log N)$) để duy trì $\max$ và $\min$ trong $O(1)$ amortized. |
| 16 | [**Total Length**](https://codeforces.com/edu/course/2/lesson/9/2/practice/contest/279653/problem/E) | Codeforces EDU | Two Pointers + Tổng độ dài | Đếm tổng độ dài của tất cả các đoạn con có tổng $\le S$. Với cửa sổ $[L, R]$ hợp lệ, các đoạn con kết thúc tại $R$ có độ dài từ $1$ đến $R - L + 1$. Tổng độ dài đóng góp là $\frac{(R - L + 1)(R - L + 2)}{2}$. |

---

## 💡 Lời khuyên vàng khi giải bài toán Hai con trỏ & Cửa sổ trượt

1. **Kiểm tra tính đơn điệu trước tiên (Monotonicity Check)**:
   - Hai con trỏ chỉ hoạt động khi có tính chất đơn điệu: khi $R$ tăng thì $L$ chỉ có thể tăng hoặc giữ nguyên.
   - Nếu mảng chứa số âm (khiến tổng không đơn điệu), Two Pointers sẽ sai! Thay vào đó phải dùng **Prefix Sum + Hash Map**.

2. **Quy tắc tính số lượng đoạn con (Subarray Counting)**:
   - Đoạn con có tính chất **$\le K$**: Cố định $R$, số đoạn con kết thúc tại $R$ là $R - L + 1$.
   - Đoạn con có tính chất **$\ge K$**: Cố định $L$, tìm $R$ nhỏ nhất thỏa mãn, số đoạn con bắt đầu tại $L$ là $N - R + 1$.
   - Đoạn con có tính chất **$= K$**: Phân rã thành $\text{AtMost}(K) - \text{AtMost}(K - 1)$.

3. **Luôn lưu ý tràn số (Integer Overflow)**:
   - Tổng mảng trong cửa sổ có thể vượt quá $2 \times 10^9$, hãy dùng `long long` cho biến `current_sum`.
   - Kết quả đếm số lượng đoạn con có thể lên tới $\frac{N(N+1)}{2} \approx 2 \times 10^{10}$ khi $N = 2 \times 10^5$, bắt buộc biến đếm `ans` phải là `long long`.

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

