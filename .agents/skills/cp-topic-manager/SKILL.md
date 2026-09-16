---
name: cp-topic-manager
description: >-
  Use this skill when creating, updating, or standardizing competitive programming (CP) topic modules,
  lectures, theoretical documentation, and standard algorithm templates for college-level CP (ICPC/OLP).
---

# Competitive Programming Topic Manager Skill

Skill này hướng dẫn quy trình chuẩn hóa và tạo mới các chuyên đề thuật toán phục vụ Lập trình thi đấu (Competitive Programming) cấp Đại học (ICPC, Olympic Tin học Sinh viên, VNOI, Codeforces, AtCoder).

## 1. Cấu trúc thư mục chuyên đề (Bộ 3 Tác chiến)

Mỗi chuyên đề nằm trong phân cấp tương ứng (ví dụ: `One-star/<topic>`, `Two-star/<topic>`, `Three-star/<topic>`) và tuân thủ cấu trúc **3 files phẳng** chuẩn mực:

```text
<Level>/<topic-name>/
├── theory.md          # Toàn bộ bài giảng lý thuyết chuyên sâu, trực quan hóa, trace thuật toán, case studies và cạm bẫy phòng thi
├── template.cpp       # Mã nguồn mẫu chuẩn C++20 (tối ưu, clean, an toàn tràn số, sẵn sàng copy dự thi)
└── problems.md        # Tuyển tập 15+ bài tập thực hành phân cấp kèm phân tích ý tưởng & gợi ý giải (hints)
```

## 2. Quy chuẩn nội dung & Nhận diện thương hiệu (Branding)

- **Tác giả & Chủ biên:** Duc-Minh Vu (Vũ Đức Minh)
- **Đơn vị công tác:** SLSCM Lab (Phòng nghiên cứu Smart Logistics & Supply Chain Management) — Khoa Khoa học Dữ liệu và Trí tuệ Nhân tạo (FDA — Faculty of Data Science and Artificial Intelligence), Đại học Kinh tế Quốc dân (NEU). *(Quy chuẩn: Không dùng từ "Trường", ghi đúng "Đại học Kinh tế Quốc dân")*.
- **Công cụ soạn thảo:** Soạn thảo và tối ưu bởi Agentic AI tool (Antigravity AI Agent). Luôn ghi rõ nguồn hỗ trợ này trong khối thông tin đầu bài và chân trang bản quyền.
- **Bản quyền:** `© 2026 Duc-Minh Vu. All rights reserved.`
- **Logo tài liệu:** Luôn gắn bộ 3 logo NEU, FDA & SLSCM (`../../assets/logos/NEU_Logo.png`, `../../assets/logos/FDA_Logo.png` và `../../assets/logos/SLSCM_logo.jpg`) tại chân trang các tài liệu `theory.md`, `problems.md`.

### 2.1. Tài liệu lý thuyết (`theory.md`)
- **Header bản quyền chuẩn**:
  ```markdown
  > **Tác giả:** Duc-Minh Vu | **Đơn vị:** SLSCM Lab — Faculty of Data Science and Artificial Intelligence (FDA), Đại học Kinh tế Quốc dân (NEU)  
  > **Biên soạn:** Được hỗ trợ và soạn thảo bởi Agentic AI tool  
  > **Bản quyền © 2026 Duc-Minh Vu.** Mọi quyền được bảo lưu. Tài liệu đào tạo và bồi dưỡng sinh viên Olympic Tin học / ICPC.
  ```
- **Tập trung chuyên môn**: Đi thẳng vào sơ đồ tư duy (Mindmap/Mermaid) và nội dung lý thuyết, **KHÔNG đưa phần giới thiệu cấu trúc học liệu/3 files vào trong file lý thuyết**.
- **Định nghĩa & Mô hình bài toán**: Nêu rõ bài toán gốc và ứng dụng trong CP.
- **Cơ sở toán học & Thuật toán**:
  - Phát biểu định lý, bổ đề chặt chẽ.
  - Chứng minh tính đúng đắn ngắn gọn.
- **Trace step-by-step & Trực quan hóa**:
  - Minh họa luồng thực thi bằng sơ đồ Mermaid hoặc bảng trace ASCII.
- **Cạm bẫy phòng thi & Tối ưu hóa**: Tràn số, edge case, memory footprint.
- **Footer bản quyền chuẩn**:
  ```html
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
  ```

### 2.2. Mã nguồn mẫu C++ (`template.cpp`)
- Sử dụng chuẩn C++20.
- Header Doxygen chuẩn chỉ rõ tác giả `Duc-Minh Vu`, đơn vị `SLSCM Lab - FDA - NEU`, và copyright notice.
- Fast I/O, đóng gói namespace gọn gàng, an toàn kiểu dữ liệu (`long long`, tránh UB).

### 2.3. Tuyển tập bài tập (`problems.md`)
- Tuyển chọn 15+ bài tập từ CSES, VNOJ, Codeforces, AtCoder, ICPC Regional.
- Bảng tổng hợp Markdown: Không sử dụng ký tự `|` trần trong công thức toán LaTeX (dùng `\lvert ... \rvert`).
- Gợi ý tư duy (hints) theo 3 bậc (Cơ bản $\to$ Trung cấp $\to$ Nâng cao).
- Footer bản quyền kèm bộ 3 logo NEU, FDA và SLSCM.
