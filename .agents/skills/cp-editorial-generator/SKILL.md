---
name: cp-editorial-generator
description: >-
  Use this skill when drafting editorials, problem analyses, solutions, or stress-testing scripts
  for competitive programming problems (ICPC, OLP, Codeforces, AtCoder, VNOI).
---

# Competitive Programming Editorial & Problem Solver Skill

Skill này hướng dẫn quy trình phân tích bài toán, biên soạn lời giải (editorial), viết mã nguồn tối ưu và tạo script đối chiếu (stress testing) cho các bài tập Competitive Programming.

## Quy trình biên soạn lời giải (Editorial)

Mỗi file lời giải (`editorial.md` hoặc `solution.md`) cần tuân thủ cấu trúc:

1. **Tóm tắt bài toán (Problem Restatement)**:
   - Phát biểu ngắn gọn mô hình cốt lõi dưới dạng toán học hoặc đồ thị.
   - Nêu rõ ràng các ràng buộc: $N, M \le 10^5$, thời gian $1.0\text{s}$, bộ nhớ $256\text{MB}$.

2. **Phân tích thuật toán**:
   - **Cách tiếp cận ngây thơ (Subtask 1 / Brute-force)**: Phân tích độ phức tạp thời gian/không gian và lý do bị TLE/MLE.
   - **Quan sát mấu chốt (Key Observations)**: Chỉ ra tính chất đặc biệt (bất biến, tính đơn điệu, cấu trúc tối ưu con, tính chất đồ thị...).
   - **Thuật toán tối ưu**: Mô tả chi tiết thuật toán tối ưu giải quyết bài toán đầy đủ.
   - **Đánh giá độ phức tạp**: Chứng minh thời gian $O(f(N))$ và bộ nhớ $O(g(N))$ đáp ứng trọn vẹn giới hạn.

3. **Cài đặt mẫu (Full Solution - C++20)**:
   - Viết code sạch, kèm giải thích tại các dòng quan trọng.
   - Luôn xử lý các trường hợp đặc biệt (corner cases).

4. **Kỹ thuật Stress-testing (Nếu bài toán phức tạp)**:
   - Cung cấp:
     - `naive.cpp`: Lời giải trâu chắc chắn đúng để so sánh.
     - `gen.cpp`: Script sinh test ngẫu nhiên và test biên.
     - Script bash / PowerShell để chạy so sánh kết quả tự động.
