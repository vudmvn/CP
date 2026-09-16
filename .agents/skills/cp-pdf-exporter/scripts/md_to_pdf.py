#!/usr/bin/env python3
"""
md_to_pdf.py - Professional Markdown to PDF Converter for CP Handbook
Uses Python Markdown + KaTeX + Mermaid.js + Headless Microsoft Edge.
Handles math equations ($...$, $$...$$), Mermaid diagrams, GitHub callouts, and local logo assets.
"""

import os
import sys
import re
import argparse
import subprocess
import shutil

# Locate Edge or Chrome on Windows
DEFAULT_EDGE_PATHS = [
    r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe",
    r"C:\Program Files\Microsoft\Edge\Application\msedge.exe",
    shutil.which("msedge.exe") or "",
    shutil.which("chrome.exe") or "",
]

def find_browser():
    for p in DEFAULT_EDGE_PATHS:
        if p and os.path.exists(p):
            return p
    raise FileNotFoundError("Could not find Microsoft Edge or Google Chrome executable.")

def preprocess_markdown(md_text, base_dir, repo_root):
    """
    1. Transform ```mermaid into <pre class="mermaid">
    2. Transform GitHub callouts (> [!NOTE], > [!TIP], etc.)
    3. Transform relative image src paths into absolute file:/// URIs
    """
    # 1. Mermaid code blocks
    md_text = re.sub(
        r'```mermaid\s*\n(.*?)\n```',
        r'<pre class="mermaid">\1</pre>',
        md_text,
        flags=re.DOTALL
    )

    # 2. GitHub Alerts / Callouts
    def replace_alert(match):
        alert_type = match.group(1).upper()
        content = match.group(2).strip()
        color_map = {
            'NOTE': ('#0969da', '#ddf4ff', 'ℹ️ Ghi chú'),
            'TIP': ('#1a7f37', '#dafbe1', '💡 Mẹo / Tối ưu'),
            'IMPORTANT': ('#8250df', '#fbefff', '📌 Quan trọng'),
            'WARNING': ('#9a6700', '#fff8c5', '⚠️ Cảnh báo / Bẫy'),
            'CAUTION': ('#cf222e', '#ffebe9', '🚨 Lưu ý đặc biệt')
        }
        border_col, bg_col, title = color_map.get(alert_type, ('#0969da', '#ddf4ff', alert_type))
        # format lines
        formatted_content = "<br/>".join(line.lstrip("> ").strip() for line in content.split("\n"))
        return f'<div style="border-left: 4px solid {border_col}; background-color: {bg_col}; padding: 10px 14px; margin: 14px 0; border-radius: 4px;"><strong>{title}:</strong><br/>{formatted_content}</div>'

    md_text = re.sub(
        r'>\s*\[!(NOTE|TIP|IMPORTANT|WARNING|CAUTION)\]\s*\n((?:>.*(?:\n|$))*)',
        replace_alert,
        md_text,
        flags=re.IGNORECASE
    )

    # 3. Fix relative image links: <img src="..." /> or ![alt](...)
    def fix_img_src(match):
        orig_src = match.group(1)
        if orig_src.startswith(('http://', 'https://', 'file://', 'data:')):
            return match.group(0)
        # resolve against base_dir
        abs_path = os.path.abspath(os.path.join(base_dir, orig_src))
        abs_uri = "file:///" + abs_path.replace("\\", "/")
        return match.group(0).replace(orig_src, abs_uri)

    md_text = re.sub(r'<img\s+[^>]*src=["\']([^"\']+)["\']', fix_img_src, md_text)
    md_text = re.sub(r'!\[([^\]]*)\]\(([^)]+)\)', lambda m: f'![{m.group(1)}]({fix_img_src_md(m.group(2), base_dir)})', md_text)

    # 4. Format and style unparsed Markdown inside <div align="center"> blocks
    def fix_div_center(match):
        inner = match.group(1)
        # Convert bold: **text** -> <strong>text</strong>
        inner = re.sub(r'\*\*([^*]+)\*\*', r'<strong style="color: #0b3c68; font-size: 11pt;">\1</strong>', inner)
        # Convert italic: *text* -> <em>text</em>
        inner = re.sub(r'(?<!\*)\*([^*]+)\*(?!\*)', r'<em style="color: #59636e; font-size: 9.5pt;">\1</em>', inner)
        # Convert headings and dividers
        inner = re.sub(r'^###\s+(.+)$', r'<h3 style="margin: 6px 0; color: #0b3c68;">\1</h3>', inner, flags=re.MULTILINE)
        inner = re.sub(r'^#\s+(.+)$', r'<h1 style="margin: 10px 0; color: #0b3c68;">\1</h1>', inner, flags=re.MULTILINE)
        inner = re.sub(r'^---$', r'<hr style="border: none; border-top: 1px solid #d0d7de; margin: 12px 0;"/>', inner, flags=re.MULTILINE)

        lines = [l.strip() for l in inner.strip().split('\n') if l.strip()]
        out_lines = []
        current_img_line = []

        for l in lines:
            if '<img' in l or '<a' in l or l.startswith('&nbsp;'):
                current_img_line.append(l)
            else:
                if current_img_line:
                    out_lines.append('<div style="margin-bottom: 12px;">' + " ".join(current_img_line) + '</div>')
                    current_img_line = []
                if l in ('<br/>', '<br/><br/>'):
                    continue
                elif l.startswith(('<h1', '<h2', '<h3', '<hr', '<p', '<div')):
                    out_lines.append(l)
                else:
                    out_lines.append(f'<div style="margin: 3px 0;">{l}</div>')

        if current_img_line:
            out_lines.append('<div style="margin-bottom: 12px;">' + " ".join(current_img_line) + '</div>')

        return '<div align="center" style="text-align: center; margin: 25px 0; padding-top: 15px; border-top: 1px solid #e1e4e8; page-break-inside: avoid;">\n' + "\n".join(out_lines) + '\n</div>'

    md_text = re.sub(r'<div\s+align=["\']center["\']>(.*?)</div>', fix_div_center, md_text, flags=re.DOTALL | re.IGNORECASE)

    return md_text

def fix_img_src_md(src, base_dir):
    if src.startswith(('http://', 'https://', 'file://', 'data:')):
        return src
    abs_path = os.path.abspath(os.path.join(base_dir, src))
    return "file:///" + abs_path.replace("\\", "/")

def protect_math_blocks(md_text):
    math_store = {}

    # 1. Protect multi-line / display math $$ ... $$
    def save_display_math(match):
        idx = len(math_store)
        token = f"KATEXDISPLAYMATHBLOCK{idx}XYZ"
        math_store[token] = match.group(0)
        return token

    md_text = re.sub(r'\$\$.*?\$\$', save_display_math, md_text, flags=re.DOTALL)

    # 2. Protect inline math $ ... $
    def save_inline_math(match):
        idx = len(math_store)
        token = f"KATEXINLINEMATHBLOCK{idx}XYZ"
        math_store[token] = match.group(0)
        return token

    md_text = re.sub(r'(?<!\\)\$(?:\\\$|[^\$\n])+?(?<!\\)\$', save_inline_math, md_text)

    return md_text, math_store

def restore_math_blocks(html_text, math_store):
    for token, raw_math in math_store.items():
        html_text = html_text.replace(token, raw_math)
    return html_text

def build_html_document(html_content, title="Competitive Programming Document"):
    return f"""<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>{title}</title>

<!-- KaTeX for High-Speed Vector Math -->
<link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.css">
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/katex.min.js"></script>
<script defer src="https://cdn.jsdelivr.net/npm/katex@0.16.9/dist/contrib/auto-render.min.js"></script>

<!-- Mermaid.js for Diagrams -->
<script src="https://cdn.jsdelivr.net/npm/mermaid@10/dist/mermaid.min.js"></script>

<style>
@page {{
    size: A4;
    margin: 18mm 14mm 18mm 14mm;
    @bottom-center {{
        content: counter(page);
    }}
}}

body {{
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 10.5pt;
    line-height: 1.6;
    color: #1f2328;
    background-color: #ffffff;
    max-width: 100%;
    margin: 0 auto;
    padding: 0;
}}

/* Typography */
h1 {{
    font-size: 20pt;
    color: #0b3c68;
    border-bottom: 2px solid #0969da;
    padding-bottom: 6px;
    margin-top: 15px;
    margin-bottom: 12px;
}}

h2 {{
    font-size: 14.5pt;
    color: #0969da;
    border-bottom: 1px solid #d0d7de;
    padding-bottom: 4px;
    margin-top: 20px;
    margin-bottom: 10px;
    page-break-after: avoid;
}}

h3 {{
    font-size: 12pt;
    color: #1f2328;
    margin-top: 16px;
    margin-bottom: 6px;
    page-break-after: avoid;
}}

p {{
    margin-top: 0;
    margin-bottom: 10px;
}}

/* Tables */
table {{
    border-collapse: collapse;
    width: 100%;
    margin: 14px 0;
    font-size: 9.5pt;
    page-break-inside: auto;
}}

tr {{
    page-break-inside: avoid;
    page-break-after: auto;
}}

th, td {{
    border: 1px solid #d0d7de;
    padding: 7px 10px;
    text-align: left;
    vertical-align: top;
}}

th {{
    background-color: #f6f8fa;
    font-weight: 600;
    color: #1f2328;
}}

tr:nth-child(even) td {{
    background-color: #fcfcfc;
}}

/* Code Blocks */
pre {{
    background-color: #f6f8fa;
    border: 1px solid #d0d7de;
    border-radius: 6px;
    padding: 10px 14px;
    font-size: 9pt;
    font-family: Consolas, "Courier New", monospace;
    line-height: 1.45;
    overflow-x: auto;
    page-break-inside: avoid;
}}

code {{
    font-family: Consolas, "Courier New", monospace;
    font-size: 9.2pt;
    background-color: rgba(175, 184, 193, 0.2);
    padding: 0.15em 0.35em;
    border-radius: 4px;
}}

pre code {{
    background-color: transparent;
    padding: 0;
}}

/* Blockquotes */
blockquote {{
    margin: 12px 0;
    padding: 8px 14px;
    color: #59636e;
    border-left: 4px solid #d0d7de;
    background-color: #f6f8fa;
}}

/* Mermaid Diagrams */
.mermaid {{
    text-align: center;
    background-color: #ffffff;
    margin: 16px 0;
    page-break-inside: avoid;
}}

/* Header & Footer Branding */
div[align="center"] img {{
    vertical-align: middle;
    max-height: 60px;
    margin: 0 8px;
}}

hr {{
    border: none;
    border-top: 1px solid #d0d7de;
    margin: 18px 0;
}}
</style>
</head>
<body>

{html_content}

<script>
document.addEventListener("DOMContentLoaded", function() {{
    // Render Mermaid diagrams
    if (typeof mermaid !== "undefined") {{
        mermaid.initialize({{
            startOnLoad: true,
            theme: "neutral",
            flowchart: {{ htmlLabels: true, curve: "basis" }}
        }});
    }}

    // Render LaTeX Math with KaTeX
    if (typeof renderMathInElement !== "undefined") {{
        renderMathInElement(document.body, {{
            delimiters: [
                {{left: "$$", right: "$$", display: true}},
                {{left: "$", right: "$", display: false}},
                {{left: "\\\\[", right: "\\\\]", display: true}},
                {{left: "\\\\(", right: "\\\\)", display: false}}
            ],
            throwOnError: false
        }});
    }}
}});
</script>
</body>
</html>"""

def convert_md_to_pdf(input_md_path, output_pdf_path=None, repo_root=None):
    if not os.path.exists(input_md_path):
        print(f"[Error] File not found: {input_md_path}")
        return False

    input_md_path = os.path.abspath(input_md_path)
    base_dir = os.path.dirname(input_md_path)
    if not repo_root:
        repo_root = base_dir

    if not output_pdf_path:
        base_name = os.path.splitext(os.path.basename(input_md_path))[0]
        output_pdf_path = os.path.join(base_dir, f"{base_name}.pdf")
    else:
        output_pdf_path = os.path.abspath(output_pdf_path)

    print(f"[1/4] Reading Markdown: {input_md_path}")
    with open(input_md_path, "r", encoding="utf-8") as f:
        raw_md = f.read()

    # Preprocess
    print("[2/4] Preprocessing Math, Diagrams & Branding assets...")
    processed_md = preprocess_markdown(raw_md, base_dir, repo_root)

    # Convert to HTML via python-markdown
    try:
        import markdown
    except ImportError:
        print("[Error] Python 'markdown' package is missing. Run: pip install markdown")
        return False

    # Protect math formulas from markdown parser (italics, underscores, nl2br)
    protected_md, math_store = protect_math_blocks(processed_md)

    html_body = markdown.markdown(
        protected_md,
        extensions=['tables', 'fenced_code', 'toc', 'nl2br']
    )

    # Restore raw math formulas for KaTeX
    html_body = restore_math_blocks(html_body, math_store)

    doc_title = os.path.splitext(os.path.basename(input_md_path))[0].replace("-", " ").title()
    full_html = build_html_document(html_body, title=doc_title)

    temp_html_path = os.path.join(base_dir, f"__temp_{os.path.basename(input_md_path)}.html")
    with open(temp_html_path, "w", encoding="utf-8") as f:
        f.write(full_html)

    # Locate headless browser
    browser_path = find_browser()
    print(f"[3/4] Rendering via Headless Browser: {browser_path}")

    cmd = [
        browser_path,
        "--headless",
        "--disable-gpu",
        "--run-all-compositor-stages-before-draw",
        "--virtual-time-budget=4500",
        "--no-pdf-header-footer",
        f"--print-to-pdf={output_pdf_path}",
        f"file:///{temp_html_path.replace(chr(92), '/')}"
    ]

    try:
        res = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
        if res.returncode == 0 and os.path.exists(output_pdf_path):
            file_size_kb = os.path.getsize(output_pdf_path) / 1024
            print(f"[4/4] SUCCESS: Exported PDF ({file_size_kb:.1f} KB) -> {output_pdf_path}")
            return True
        else:
            print(f"[Error] Browser process failed (Code {res.returncode}): {res.stderr}")
            return False
    finally:
        # Cleanup temp HTML
        if os.path.exists(temp_html_path):
            os.remove(temp_html_path)

def main():
    parser = argparse.ArgumentParser(description="Convert CP Markdown files to professional PDF documents.")
    parser.add_argument("-i", "--input", help="Path to markdown file or topic directory", required=False)
    parser.add_argument("-o", "--output", help="Path to output PDF file", required=False)
    parser.add_argument("--all", action="store_true", help="Batch export all One-star topics")
    args = parser.parse_args()

    repo_dir = r"E:\MinhVD\Github\CP"

    if args.all:
        one_star_dir = os.path.join(repo_dir, "One-star")
        success_count = 0
        total_files = 0
        for topic in sorted(os.listdir(one_star_dir)):
            topic_path = os.path.join(one_star_dir, topic)
            if not os.path.isdir(topic_path):
                continue
            for target in ["theory.md", "problems.md"]:
                md_file = os.path.join(topic_path, target)
                if os.path.exists(md_file):
                    total_files += 1
                    pdf_file = os.path.join(topic_path, f"{os.path.splitext(target)[0]}.pdf")
                    if convert_md_to_pdf(md_file, pdf_file, repo_dir):
                        success_count += 1
        print(f"\n[Finished] Successfully converted {success_count}/{total_files} documents.")
        return

    if not args.input:
        print("Please provide --input <file_or_dir> or --all.")
        sys.exit(1)

    input_path = os.path.abspath(args.input)
    if os.path.isdir(input_path):
        for target in ["theory.md", "problems.md"]:
            md_file = os.path.join(input_path, target)
            if os.path.exists(md_file):
                pdf_file = os.path.join(input_path, f"{os.path.splitext(target)[0]}.pdf")
                convert_md_to_pdf(md_file, pdf_file, repo_dir)
    else:
        convert_md_to_pdf(input_path, args.output, repo_dir)

if __name__ == "__main__":
    main()
