# Competitive Programming (CP) Skills & Curriculum Guide

> **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
> **Soạn thảo:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
> **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu.


> **Mục tiêu**: Kho lưu trữ tri thức, bài giảng, mã nguồn chuẩn (C++ templates) và bài tập phân cấp dành cho **Lập trình thi đấu cấp Đại học** (ICPC Vietnam, ICPC Asia Regional, Olympic Tin học Sinh viên, VNOI, Codeforces, AtCoder, CSES).  
> 📖 Hướng dẫn phương pháp luyện tập: [**Cẩm nang Luyện tập CP (`guidelines.md`)**](./guidelines.md) | Tra cứu bài giảng: [**`index.md`**](./index.md).

Repository này tích hợp hệ thống **AI Agent Skills** (`.agents/skills/`), giúp tự động hóa việc cập nhật tài liệu, biên soạn bài giảng trực quan, chuẩn hóa code template và phân tích lời giải (editorial).

---

## 1. Hệ thống Skills tích hợp trong Repo

Các Antigravity Skills đã được cấu hình sẵn trong `.agents/skills/` để hỗ trợ phát triển nội dung:

| Skill | Vị trí | Mô tả chức năng | Quy tắc kích hoạt |
| :--- | :--- | :--- | :---: |
| **`cp-topic-manager`** | [`.agents/skills/cp-topic-manager/SKILL.md`](./.agents/skills/cp-topic-manager/SKILL.md) | Tự động sinh trọn bộ chuyên đề mới theo Bộ 3 Tác chiến: bài giảng lý thuyết toàn diện (`theory.md`), mã nguồn mẫu chuẩn (`template.cpp`), và tuyển tập bài tập (`problems.md`). | Khi soạn chuyên đề |
| **`cp-editorial-generator`** | [`.agents/skills/cp-editorial-generator/SKILL.md`](./.agents/skills/cp-editorial-generator/SKILL.md) | Phân tích bài toán, thiết kế giải thuật từ trâu đến tối ưu, viết editorial chi tiết, code C++20 chuẩn và script stress-test tự động. | Khi giải/phân tích bài |
| **`cp-pdf-exporter`** | [`.agents/skills/cp-pdf-exporter/SKILL.md`](./.agents/skills/cp-pdf-exporter/SKILL.md) | Tự động biên dịch và xuất bản các tệp Markdown (`theory.md`, `problems.md`, `guidelines.md`) sang file PDF chất lượng cao (KaTeX vector math, Mermaid diagram, chuẩn A4 và bộ 3 logo NEU/FDA/SLSCM). | 🔒 **Chỉ khi người dùng ra lệnh trực tiếp** |

### Cách ra lệnh nhanh cho Agent:
- *"Hãy dùng skill `cp-topic-manager` để hoàn thiện nội dung chuyên đề `One-star/modulo` (lý thuyết, bài giảng, template C++, bài tập CSES/VNOJ)."*
- *"Hãy dùng skill `cp-editorial-generator` để phân tích và viết lời giải chi tiết cho bài toán [Tên bài/Link]."*
- *"Hãy dùng skill `cp-pdf-exporter` để xuất file PDF cho chuyên đề `One-star/modulo`."* *(Lưu ý: Skill xuất PDF chỉ hoạt động khi có câu lệnh yêu cầu này).*

---

## 2. Chuẩn mực mã nguồn khi AI sinh giải thuật (C++ CP Standard)

Mọi code template và lời giải trong repo đều tuân thủ các quy tắc:
1. **Tiêu chuẩn ngôn ngữ**: C++17 hoặc C++20.
2. **Fast I/O**:
   ```cpp
   ios_base::sync_with_stdio(false);
   cin.tie(NULL);
   ```
3. **An toàn kiểu dữ liệu**:
   - Tránh tràn số: Sử dụng `long long` cho các phép tính có nguy cơ tràn $32\text{-bit}$ ($> 2 \times 10^9$).
   - Với các bài toán modulo, viết hàm chuẩn:
     ```cpp
     inline long long modAdd(long long a, long long b, long long m) { return (a + b) % m; }
     inline long long modMul(long long a, long long b, long long m) { return (__int128)a * b % m; }
     ```
4. **Đóng gói**: Ưu tiên đóng gói các CTDL phức tạp vào `struct` độc lập (ví dụ `struct SegmentTree`, `struct DSU`) để dễ kiểm thử và nhúng vào bài thi.

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

