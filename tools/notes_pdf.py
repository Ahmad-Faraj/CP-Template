#!/usr/bin/env python3
"""Combine notes/*.md into one notes/Notes.md, then render it to notes/Notes.pdf.

The PDF is the MARKDOWN rendered - headings, tables, fenced code and LaTeX - not
plain text. Math is protected from the markdown converter and typeset by MathJax,
then Edge prints the page headlessly.

    py tools/notes_pdf.py            # rebuild both Notes.md and Notes.pdf
    py tools/notes_pdf.py --md-only  # just the combined markdown
"""
import argparse, html, pathlib, re, subprocess, sys, tempfile

ROOT = pathlib.Path(__file__).resolve().parent.parent
NOTES = ROOT / 'notes'
COMBINED = NOTES / 'Notes.md'
PDF = NOTES / 'Notes.pdf'

# maths first, then the algorithm write-ups, then the language note
ORDER = ['Knowledge.md', 'Combinatorics.md', 'Stars_and_Bars_with_Upper_Bound.md',
         'Divisability_Rules.md', 'Bits.md', 'Geometry.md', 'FFT_Apps.md',
         'Fast_NTT.md', 'Subarray_Hashing.md', 'Lambda.md']

EDGE = [r'C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe',
        r'C:\Program Files\Microsoft\Edge\Application\msedge.exe']


def slug(text):
    return re.sub(r'[^a-z0-9]+', '-', text.lower()).strip('-')


def build_markdown():
    files = [f for f in ORDER if (NOTES / f).exists()]
    files += sorted(p.name for p in NOTES.glob('*.md')
                    if p.name not in files and p.name != COMBINED.name)

    titles, bodies = [], []
    for f in files:
        text = (NOTES / f).read_text(encoding='utf-8').replace('\r\n', '\n').strip()
        lines = text.split('\n')
        title = lines[0].lstrip('# ').strip() if lines[0].startswith('#') else f[:-3]
        body = '\n'.join(lines[1:]).strip() if lines[0].startswith('#') else text
        titles.append((title, f))
        bodies.append('# ' + title + '\n\n' + body)

    out = ['# Competitive Programming Notes', '',
           'Everything under `notes/`, collected into one document.', '',
           '## Contents', '']
    for i, (title, f) in enumerate(titles, 1):
        out.append('%d. [%s](#%s) — `notes/%s`' % (i, title, slug(title), f))
    out += ['', '---', '']
    out.append('\n\n---\n\n'.join(bodies))
    COMBINED.write_text('\n'.join(out) + '\n', encoding='utf-8', newline='\n')
    return len(files)


CSS = """
:root { --fg:#1f2328; --muted:#59636e; --line:#d1d9e0; --bg:#ffffff; --code-bg:#f6f8fa; }
* { box-sizing: border-box; }
body { font-family: -apple-system, "Segoe UI", Helvetica, Arial, sans-serif;
       font-size: 10.5pt; line-height: 1.6; color: var(--fg); background: var(--bg);
       margin: 0; padding: 0; }
h1, h2, h3, h4 { line-height: 1.25; margin: 1.4em 0 .6em; font-weight: 600; }
h1 { font-size: 20pt; padding-bottom: .3em; border-bottom: 1px solid var(--line);
     page-break-before: always; }
h1:first-of-type { page-break-before: avoid; }
h2 { font-size: 15pt; padding-bottom: .25em; border-bottom: 1px solid var(--line); }
h3 { font-size: 12.5pt; } h4 { font-size: 11pt; color: var(--muted); }
p, ul, ol, table, pre, blockquote { margin: 0 0 .85em; }
ul, ol { padding-left: 1.6em; }
li { margin: .2em 0; }
a { color: #0969da; text-decoration: none; }
code { font-family: "Cascadia Mono", Consolas, "Liberation Mono", monospace;
       font-size: .88em; background: var(--code-bg); padding: .15em .35em;
       border-radius: 4px; }
pre { background: var(--code-bg); border: 1px solid var(--line); border-radius: 6px;
      padding: .8em 1em; overflow-x: auto; page-break-inside: avoid; }
pre code { background: none; padding: 0; font-size: .85em; line-height: 1.45; }
table { border-collapse: collapse; width: 100%; page-break-inside: avoid; }
th, td { border: 1px solid var(--line); padding: .4em .7em; text-align: left;
         vertical-align: top; }
th { background: var(--code-bg); font-weight: 600; }
blockquote { border-left: 3px solid var(--line); padding-left: 1em; color: var(--muted); }
hr { border: none; border-top: 1px solid var(--line); margin: 2em 0; }
img { max-width: 100%; }
mjx-container { overflow-x: auto; overflow-y: hidden; }
.alert { display: inline-block; font-weight: 600; font-size: .85em; letter-spacing: .04em;
         text-transform: uppercase; margin-bottom: .3em; }
.alert-note { color: #0969da; } .alert-tip { color: #1a7f37; }
.alert-important { color: #8250df; } .alert-warning { color: #9a6700; }
.alert-caution { color: #cf222e; }
@page { size: A4; margin: 16mm 14mm; }
"""

PAGE = """<!doctype html>
<html><head><meta charset="utf-8"><title>Competitive Programming Notes</title>
<style>%s</style>
<script>
window.MathJax = {
  tex: { inlineMath: [['$','$']], displayMath: [['$$','$$']] },
  options: { skipHtmlTags: ['script','noscript','style','textarea','pre','code'] }
};
</script>
<script id="MathJax-script" async
  src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js"></script>
</head><body>%s</body></html>
"""


def render_html(md_text):
    import markdown
    # keep the markdown converter's hands off the maths: $...$ and $$...$$ are
    # full of _ and \ and * , which it would otherwise turn into emphasis
    store = []

    def stash(m):
        store.append(m.group(0))
        return '\x00MATH%dZ\x00' % (len(store) - 1)

    # GitHub alert blocks: > [!NOTE] ... -> a styled callout, not the literal text
    md_text = re.sub(r'^(\s*)>\s*\[!(NOTE|TIP|IMPORTANT|WARNING|CAUTION)\]\s*$',
                     lambda m: '%s> <span class="alert alert-%s">%s</span><br>'
                               % (m.group(1), m.group(2).lower(), m.group(2).title()),
                     md_text, flags=re.M)
    protected = re.sub(r'\$\$.+?\$\$', stash, md_text, flags=re.S)
    protected = re.sub(r'(?<!\\)\$(?!\s)(?:[^$\n]|\\\$)+?(?<!\s)\$', stash, protected)

    body = markdown.markdown(protected, extensions=['extra', 'tables', 'fenced_code',
                                                    'sane_lists', 'toc'])
    for i, raw in enumerate(store):
        body = body.replace('\x00MATH%dZ\x00' % i, html.escape(raw, quote=False))
    return PAGE % (CSS, body)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--md-only', action='store_true')
    a = ap.parse_args()

    n = build_markdown()
    print('notes/Notes.md  <- %d files, %d KB' % (n, len(COMBINED.read_bytes()) // 1024))
    if a.md_only:
        return 0

    page = render_html(COMBINED.read_text(encoding='utf-8'))
    tmp = pathlib.Path(tempfile.gettempdir()) / 'cp_notes.html'
    tmp.write_text(page, encoding='utf-8')

    edge = next((e for e in EDGE if pathlib.Path(e).exists()), None)
    if not edge:
        print('Edge not found; wrote %s - print it to PDF manually' % tmp)
        return 1
    r = subprocess.run([edge, '--headless', '--disable-gpu', '--no-pdf-header-footer',
                        '--virtual-time-budget=30000',
                        '--print-to-pdf=' + str(PDF), tmp.as_uri()],
                       capture_output=True, text=True, timeout=300)
    if not PDF.exists():
        sys.stderr.write(r.stdout[-1500:] + r.stderr[-1500:])
        return 1
    print('notes/Notes.pdf <- %d KB' % (len(PDF.read_bytes()) // 1024))
    return 0


if __name__ == '__main__':
    sys.exit(main())
