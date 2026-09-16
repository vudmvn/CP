---
name: cp-pdf-exporter
description: >-
  Use this skill ONLY when the user explicitly requests generating, exporting, or converting competitive programming
  markdown files (theory.md, problems.md, guidelines.md, etc.) into professional PDF documents (e.g., 'xuất pdf',
  'tạo pdf', 'convert to pdf', 'export pdf'). DO NOT invoke or run this skill automatically or proactively during normal tasks.
---

# Competitive Programming PDF Exporter Skill

Skill này cung cấp công cụ tự động hóa xuất bản toàn bộ tài liệu lý thuyết (`theory.md`), bài tập (`problems.md`), và cẩm nang (`guidelines.md`) từ định dạng Markdown sang file **PDF chất lượng cao (Vector Print-Ready)**.

> [!IMPORTANT]
> **Quy tắc kích hoạt (Trigger Rule)**:
> - **CHỈ sử dụng skill này khi người dùng trực tiếp ra lệnh** (ví dụ: *"xuất PDF chuyên đề modulo"*, *"tạo PDF bài tập hai con trỏ"*, *"convert all to PDF"*, *"export PDF"*).
> - **TUYỆT ĐỐI KHÔNG tự ý kích hoạt skill này** trong các tác vụ tạo mới hoặc cập nhật tài liệu thông thường nếu người dùng không yêu cầu.

---

## 1. Tính năng cốt lõi của công cụ chuyển đổi

Script chuyển đổi nằm tại: [`scripts/md_to_pdf.py`](./scripts/md_to_pdf.py)

1. **Toán học Vector chất lượng cao & Cơ chế bảo vệ công thức chuyên sâu**:
   - Tự động cô lập và bảo vệ toàn bộ khối toán học inline (`$...$`) và block multi-line (`$$...$$`) trước khi bộ phân tích Markdown xử lý.
   - Triệt tiêu 100% các lỗi kinh điển: Markdown hiểu nhầm dấu gạch dưới `_` trong chỉ số thành thẻ in nghiêng `<em>`, lỗi ngắt dòng `nl2br` chèn thẻ `<br>` vào giữa phương trình đa dòng, hoặc xung đột dấu `|` trong bảng.
   - Sử dụng thư viện **KaTeX** render vector toán học cực nét, chuẩn typography học thuật.
2. **Sơ đồ giải thuật trực quan**: Tự động render toàn bộ sơ đồ Mermaid (`flowchart`, `sequenceDiagram`, `stateDiagram`) thành vector đồ họa trước khi xuất bản.
3. **Đồng bộ nhận diện thương hiệu**: Nhận diện và tải chính xác bộ 3 logo tại header và footer ([`NEU_Logo.png`](../../assets/logos/NEU_Logo.png), [`FDA_Logo.png`](../../assets/logos/FDA_Logo.png), [`SLSCM_logo.jpg`](../../assets/logos/SLSCM_logo.jpg)).
4. **Hộp cảnh báo chuẩn GitHub (Callouts)**: Định dạng trang trọng các khối `[!NOTE]`, `[!TIP]`, `[!IMPORTANT]`, `[!WARNING]`, `[!CAUTION]`.
5. **Định dạng in ấn A4**: Tối ưu ngắt trang (`page-break-inside: avoid` cho bảng và mã nguồn C++), lề chuẩn A4, kích thước chữ tối ưu cho đọc trên iPad/máy tính hoặc in ra giấy.

---

## 2. Hướng dẫn sử dụng chi tiết

### 2.1. Xuất PDF cho một file lý thuyết hoặc bài tập cụ thể
```powershell
python ".agents/skills/cp-pdf-exporter/scripts/md_to_pdf.py" -i "One-star/modulo/theory.md"
```
*Kết quả:* Tạo ra file `One-star/modulo/theory.pdf` cùng thư mục.

### 2.2. Xuất PDF cho toàn bộ một chuyên đề (cả theory.md và problems.md)
```powershell
python ".agents/skills/cp-pdf-exporter/scripts/md_to_pdf.py" -i "One-star/two-pointers"
```
*Kết quả:* Tạo ra cả `One-star/two-pointers/theory.pdf` và `One-star/two-pointers/problems.pdf`.

### 2.3. Xuất PDF hàng loạt cho toàn bộ các chuyên đề đã hoàn thành
```powershell
python ".agents/skills/cp-pdf-exporter/scripts/md_to_pdf.py" --all
```
*Kết quả:* Quét toàn bộ thư mục `One-star/` và tự động biên dịch toàn bộ các file `theory.md` và `problems.md` sang PDF.

---

## 3. Checklist nghiệm thu chất lượng bản in PDF

Trước khi bàn giao file PDF cho người dùng:
1. Kiểm tra kích thước file: File PDF phải có dung lượng từ vài chục KB đến vài trăm KB (không phải file rỗng 0 bytes).
2. Kiểm tra công thức toán: Không còn ký hiệu `$ ... $` dạng text thô, toàn bộ phải được dựng thành ký hiệu toán đẹp mắt.
3. Kiểm tra khối code: Không bị cắt ngang dòng một cách kỳ lạ qua trang mới.
4. Kiểm tra logo: Header/Footer hiển thị đầy đủ bộ 3 logo rõ nét.
