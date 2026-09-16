# 📚 Competitive Programming Curriculum & Lecture Index

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu.


> **Kho học liệu & Cẩm nang thuật toán Lập trình thi đấu cấp Đại học**  
> Dành cho sinh viên ôn luyện: **ICPC Vietnam**, **ICPC Asia Regional / World Finals**, **Olympic Tin học Sinh viên (OLP)**, **VNOI**, **Codeforces**, **AtCoder**, **CSES**.  
> 💡 *Khuyên đọc*: [**Cẩm nang Luyện tập & Phát triển Kỹ năng CP (`guidelines.md`)**](./guidelines.md) | [**Mục lục PDF & Templates (`index-pdf.md`)**](./index-pdf.md) — Tải toàn bộ bản in PDF và mã nguồn mẫu C++20.

---

## 🧭 Tổng quan tiến độ & Thống kê

| Cấp độ | Định hướng kỳ thi | Đã hoàn thiện | Dự kiến | Trạng thái |
| :---: | :--- | :---: | :---: | :---: |
| ⭐ **One-star** | Nền tảng ICPC, Vòng loại trường, OLP Không chuyên | **9** | 14 | 🟡 Đang hoàn thiện |
| ⭐⭐ **Two-star** | ICPC National, ICPC Regional Mid, OLP Chuyên Tin | **0** | 10 | ⏳ Lên kế hoạch |
| ⭐⭐⭐ **Three-star** | ICPC Regional Hard, World Finals, OLP Siêu Cúp | **0** | 8 | ⏳ Lên kế hoạch |

---

## 📑 Bảng tra cứu chuyên đề & Bài giảng chi tiết

Mỗi chuyên đề trong repo được chuẩn hóa thống nhất theo **Bộ 3 Tác chiến (3 Files phẳng)**:
- 📖 **Bài giảng & Lý thuyết toàn diện (`theory.md`)**: Cơ sở toán học, chứng minh, trực quan hóa, trace thuật toán, case studies và cạm bẫy phòng thi.
- 💻 **Mã nguồn mẫu (`template.cpp`)**: Code C++20 tối ưu, sạch, đã kiểm thử chính xác, sẵn sàng copy dự thi.
- 🎯 **Tuyển tập bài tập (`problems.md`)**: Phân cấp (Cơ bản $\to$ Trung cấp $\to$ Nâng cao) kèm gợi ý giải (hints).

---

### ⭐ One-star: Nền tảng & Khởi động ICPC

| STT | Chuyên đề | Bài giảng & Lý thuyết | Template C++20 | Tuyển tập bài tập | Trạng thái |
| :---: | :--- | :---: | :---: | :---: | :---: |
| 1 | [**Số học Modulo & Nghịch đảo**](./One-star/modulo/) | [theory.md](./One-star/modulo/theory.md) | [template.cpp](./One-star/modulo/template.cpp) | [18 bài](./One-star/modulo/problems.md) | ✅ Hoàn thành |
| 2 | [**Đệ quy & Thuật toán Quay lui**](./One-star/recursion-backtracking/) | [theory.md](./One-star/recursion-backtracking/theory.md) | [template.cpp](./One-star/recursion-backtracking/template.cpp) | [14 bài](./One-star/recursion-backtracking/problems.md) | ✅ Hoàn thành |
| 3 | [**Sàng số nguyên tố & Thừa số nguyên tố**](./One-star/sieve-primes/) | [theory.md](./One-star/sieve-primes/theory.md) | [template.cpp](./One-star/sieve-primes/template.cpp) | [16 bài](./One-star/sieve-primes/problems.md) | ✅ Hoàn thành |
| 4 | [**Mảng tiền tố, Hậu tố & Mảng hiệu**](./One-star/prefix-suffix-sum/) | [theory.md](./One-star/prefix-suffix-sum/theory.md) | [template.cpp](./One-star/prefix-suffix-sum/template.cpp) | [15 bài](./One-star/prefix-suffix-sum/problems.md) | ✅ Hoàn thành |
| 5 | [**Nhận dạng Quy luật & Công thức đóng**](./One-star/pattern-analysis/) | [theory.md](./One-star/pattern-analysis/theory.md) | [template.cpp](./One-star/pattern-analysis/template.cpp) | [14 bài](./One-star/pattern-analysis/problems.md) | ✅ Hoàn thành |
| 6 | [**Kỹ thuật Hai con trỏ & Cửa sổ trượt**](./One-star/two-pointers/) | [theory.md](./One-star/two-pointers/theory.md) | [template.cpp](./One-star/two-pointers/template.cpp) | [16 bài](./One-star/two-pointers/problems.md) | ✅ Hoàn thành |
| 7 | [**Ước chung lớn nhất (GCD) & Bội chung nhỏ nhất (LCM)**](./One-star/gcd-lcm/) | [theory.md](./One-star/gcd-lcm/theory.md) | [template.cpp](./One-star/gcd-lcm/template.cpp) | [16 bài](./One-star/gcd-lcm/problems.md) | ✅ Hoàn thành |
| 8 | [**Số chính phương & Square-Free Numbers**](./One-star/square-numbers/) | [theory.md](./One-star/square-numbers/theory.md) | [template.cpp](./One-star/square-numbers/template.cpp) | [16 bài](./One-star/square-numbers/problems.md) | ✅ Hoàn thành |
| 9 | [**Toán học Nền tảng & Đại số Rời rạc**](./One-star/basic-math/) | [theory.md](./One-star/basic-math/theory.md) | [template.cpp](./One-star/basic-math/template.cpp) | [16 bài](./One-star/basic-math/problems.md) | ✅ Hoàn thành |
| 10 | **Tìm kiếm nhị phân & Chặt nhị phân kết quả** | — | — | — | ⏳ Sắp có |
| 11 | **Cấu trúc dữ liệu nền tảng (DSU, Monotonic)** | — | — | — | ⏳ Sắp có |
| 12 | **Đồ thị cơ bản (BFS/DFS, Dijkstra, Kruskal)** | — | — | — | ⏳ Sắp có |
| 13 | **Quy hoạch động cơ bản (Knapsack, LIS, Grid)** | — | — | — | ⏳ Sắp có |
| 14 | **Xử lý xâu cơ bản & Đếm phân phối** | — | — | — | ⏳ Sắp có |

---

### ⭐⭐ Two-star: Trung cấp (ICPC National & Regional Mid)

| STT | Chuyên đề | Bài giảng & Lý thuyết | Template C++20 | Tuyển tập bài tập | Trạng thái |
| :---: | :--- | :---: | :---: | :---: | :---: |
| 1 | **Cây phân đoạn (Segment Tree & Lazy)** | — | — | — | ⏳ Sắp có |
| 2 | **Cây chỉ số nhị phân (Fenwick Tree / BIT)** | — | — | — | ⏳ Sắp có |
| 3 | **Bảng thưa (Sparse Table & RMQ)** | — | — | — | ⏳ Sắp có |
| 4 | **Quy hoạch động trên cây (Tree DP)** | — | — | — | ⏳ Sắp có |
| 5 | **Quy hoạch động trạng thái (Bitmask DP)** | — | — | — | ⏳ Sắp có |
| 6 | **Quy hoạch động chữ số (Digit DP)** | — | — | — | ⏳ Sắp có |
| 7 | **Thành phần liên thông mạnh (Tarjan / 2-SAT)** | — | — | — | ⏳ Sắp có |
| 8 | **Khớp & Cầu (Bridges & Articulations)** | — | — | — | ⏳ Sắp có |
| 9 | **Thuật toán chuỗi (KMP & Z-Algorithm)** | — | — | — | ⏳ Sắp có |
| 10 | **Bao lồi & Hình học cơ bản** | — | — | — | ⏳ Sắp có |

---

### ⭐⭐⭐ Three-star: Nâng cao (ICPC Regional Hard & World Finals)

| STT | Chuyên đề | Bài giảng & Lý thuyết | Template C++20 | Tuyển tập bài tập | Trạng thái |
| :---: | :--- | :---: | :---: | :---: | :---: |
| 1 | **Persistent Segment Tree & Treap** | — | — | — | ⏳ Sắp có |
| 2 | **Heavy-Light Decomposition (HLD)** | — | — | — | ⏳ Sắp có |
| 3 | **Centroid Decomposition** | — | — | — | ⏳ Sắp có |
| 4 | **Luồng cực đại (Dinic & Push-Relabel)** | — | — | — | ⏳ Sắp có |
| 5 | **Luồng chi phí cực tiểu (Min-Cost Max-Flow)** | — | — | — | ⏳ Sắp có |
| 6 | **Cặp ghép cực đại (Hopcroft-Karp & Blossom)** | — | — | — | ⏳ Sắp có |
| 7 | **Biến đổi Fourier nhanh (FFT & NTT)** | — | — | — | ⏳ Sắp có |
| 8 | **Aho-Corasick & Suffix Automaton** | — | — | — | ⏳ Sắp có |

---

## 🛠️ Hướng dẫn phát triển & Đóng góp nội dung mới

Kho lưu trữ được tích hợp hệ thống **Antigravity AI Agent Skills**:
- [**`skills.md` - Cẩm nang Skills & Quy chuẩn CP**](./skills.md): Hướng dẫn chi tiết phong cách code, chuẩn mực nội dung và danh sách lệnh AI.
- [**`cp-topic-manager`**](./.agents/skills/cp-topic-manager/SKILL.md): Tự động tạo trọn bộ chuyên đề mới đúng chuẩn Bộ 3 Tác chiến (`README.md`, `template.cpp`, `problems.md`).
- [**`cp-editorial-generator`**](./.agents/skills/cp-editorial-generator/SKILL.md): Tự động phân tích và viết lời giải chi tiết cho bài tập bất kỳ.

### Mẫu câu lệnh nhanh để AI tạo chuyên đề mới:
```text
"Hãy dùng skill cp-topic-manager để tạo trọn bộ chuyên đề One-star/sieve-primes (bài giảng lý thuyết toàn diện, template C++20, danh sách bài tập CSES/VNOJ và references)."
```

---

<div align="center">

<a href="https://www.neu.edu.vn" target="_blank"><img src="./assets/logos/NEU_Logo.png" alt="NEU Logo" height="50" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.fda.neu.edu.vn" target="_blank"><img src="./assets/logos/FDA_Logo.png" alt="FDA Logo" height="50" /></a>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<a href="https://www.facebook.com/slscm.lab" target="_blank"><img src="./assets/logos/SLSCM_logo.jpg" alt="SLSCM Logo" height="50" /></a>

<br/><br/>

**Competitive Programming Handbook**  
*SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA) — National Economics University (NEU)*  
*Tài liệu được soạn thảo và tối ưu bởi Agentic AI tool*  
© 2026 Duc-Minh Vu. Toàn bộ bản quyền được bảo lưu.

</div>

