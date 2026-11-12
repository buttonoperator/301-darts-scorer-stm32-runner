#!/usr/bin/env python3
"""Build index.html from src/template.html and the firmware sources.

    python build.py

Requires Python 3 and nothing else. The build inlines every file under
firmware/ into the page, runs its checks, and writes index.html.
"""
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
TEMPLATE = ROOT / "src" / "template.html"
FIRMWARE = ROOT / "firmware"
OUT = ROOT / "index.html"
MARK = "/*__CODE__*/{}"


def main() -> int:
    html = TEMPLATE.read_text(encoding="utf-8")
    if html.count(MARK) != 1:
        print("build: template must contain the code marker exactly once")
        return 1

    code = {}
    for path in sorted(FIRMWARE.rglob("*")):
        if path.suffix in (".c", ".h"):
            code[path.relative_to(FIRMWARE).as_posix()] = path.read_text(encoding="utf-8")

    errors = []

    # Every file a step refers to must exist.
    for ref in sorted(set(re.findall(r'"((?:steps/)?[a-z0-9_]+\.[ch])"', html))):
        if ref not in code:
            errors.append(f"step refers to missing firmware file: {ref}")

    # The step count in the masthead must match the step list.
    steps = len(re.findall(r'^\s*(?:/\* \d+ \*/ )?\{g:"', html, re.M))
    for claim in re.findall(r"(\d+) steps", html):
        if int(claim) != steps:
            errors.append(f"page says {claim} steps, step list has {steps}")

    # Pin table rows must point at a real step.
    table = re.search(r"var PTAB = \[(.*?)\n\];", html, re.S).group(1)
    for idx in re.findall(r'",(\d+)\]', table):
        if int(idx) >= steps:
            errors.append(f"pin table refers to step index {idx}, only {steps} steps")

    if errors:
        for e in errors:
            print("build:", e)
        return 1

    blob = json.dumps(code, ensure_ascii=False).replace("</", "<\\/")
    OUT.write_text(html.replace(MARK, blob), encoding="utf-8")
    print(f"build: wrote {OUT.name}, {steps} steps, {len(code)} firmware files")
    return 0


if __name__ == "__main__":
    sys.exit(main())
