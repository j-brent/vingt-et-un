# Handling Large PDFs

When attempting to read a PDF that is too large (file size error or content exceeds limits), automatically split it into smaller chunks.

## When to Use

- PDF read fails due to size
- User asks to read a large PDF and you anticipate it may fail

## Process

1. **Install pypdf** (if not already available):
   ```sh
   pip install pypdf --quiet
   ```

2. **Split the PDF** into 40-page chunks:
   ```python
   from pathlib import Path
   from pypdf import PdfReader, PdfWriter

   input_file = Path(r"path/to/large.pdf")
   pages_per_chunk = 40  # Safe default for text-heavy PDFs

   reader = PdfReader(input_file)
   total_pages = len(reader.pages)
   output_dir = input_file.parent
   stem = input_file.stem

   for start in range(0, total_pages, pages_per_chunk):
       end = min(start + pages_per_chunk, total_pages)
       writer = PdfWriter()
       for page_num in range(start, end):
           writer.add_page(reader.pages[page_num])
       chunk_num = (start // pages_per_chunk) + 1
       output_path = output_dir / f"{stem}_part{chunk_num:02d}.pdf"
       writer.write(output_path)
       print(f"Created: {output_path} (pages {start + 1}-{end})")
   ```

3. **Read the split files** individually as needed

## Chunk Size Guidelines

- **40 pages** - default, works for most text PDFs
- **20-30 pages** - use for image-heavy PDFs
- If 40-page chunks fail, offer to re-split with smaller chunks

## Output Location

Split files are created alongside the original:
- `document.pdf` → `document_part01.pdf`, `document_part02.pdf`, etc.

## Cleanup

After the user is done with the split files, offer to delete them (use PowerShell on Windows):
```sh
powershell -Command "Remove-Item 'path/to/document_part*.pdf'"
```
