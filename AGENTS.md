# Workspace Rules & Agent Guidelines for CP Repository

## 1. Quy định về Git và Quản lý phiên bản (Git & GitHub Policy)
- ⛔ **TUYỆT ĐỐI KHÔNG TỰ ĐỘNG COMMIT HOẶC PUSH LÊN GITHUB**:
  - Không tự ý chạy `git add`, `git commit`, `git push` sau khi hoàn thành biên soạn bài giảng, code template, bài tập hay xuất bản file PDF.
  - Mọi thao tác liên quan đến Git commit & push **CHỈ ĐƯỢC PHÉP THỰC HIỆN** khi người dùng có yêu cầu rõ ràng, tường minh (ví dụ: *"hãy commit và push"*, *"push lên github"*, *"commit thay đổi"*).
  - Khi hoàn thành tác vụ biên soạn hoặc chỉnh sửa, chỉ làm việc trên không gian làm việc cục bộ (local workspace) và báo cáo kết quả cho người dùng.

## 2. Quy chuẩn biên soạn tài liệu & bài giảng
- Không dùng từ "Trường" trong "Đại học Kinh tế Quốc dân (NEU)".
- Luôn ghi rõ: *"Soạn thảo: Được hỗ trợ và soạn thảo bởi Agentic AI tool"* tại Header và Footer.
- Logo tài liệu (NEU, FDA, SLSCM Lab) ở Header và Footer luôn kèm liên kết redirect:
  - NEU: `https://www.neu.edu.vn`
  - FDA: `https://www.fda.neu.edu.vn`
  - SLSCM Lab: `https://www.facebook.com/slscm.lab`
- Không đưa phần giới thiệu cấu trúc 3 files/học liệu vào trong bài giảng lý thuyết `theory.md`.
- Tránh dùng ký tự `|` trần trong bảng Markdown (sử dụng `\lvert ... \rvert`).
- Xuất bản PDF (`cp-pdf-exporter`): Chỉ thực hiện khi người dùng trực tiếp yêu cầu tạo hoặc xuất PDF.
