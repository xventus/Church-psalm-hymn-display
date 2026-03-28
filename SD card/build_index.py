import os
import re

INPUT_DIR = "/Users/petrvanek/Documents/text-zalm/zalmy"
OUTPUT_ALPHA = "index_abecedne.txt"
OUTPUT_NUM = "index_cisla.txt"

entries = []

for fname in os.listdir(INPUT_DIR):
    if not fname.endswith(".txt"):
        continue
    fpath = os.path.join(INPUT_DIR, fname)
    try:
        with open(fpath, "r", encoding="cp1250") as f:
            raw = f.read()
    except Exception:
        with open(fpath, "r", encoding="utf-8") as f:
            raw = f.read()

    # Normalize whitespace: collapse all whitespace sequences to single space, strip edges
    text = re.sub(r"\s+", " ", raw).strip()

    name = fname[:-4]  # strip .txt
    entries.append((name, text))

# Sort by number (filename)
entries_by_num = sorted(entries, key=lambda x: x[0])

# Sort alphabetically by text content, case-insensitive, Czech-aware via locale would be ideal
# Use unicode casefold for reasonable Czech ordering
entries_by_alpha = sorted(entries, key=lambda x: x[1].casefold())

def write_output(path, rows):
    with open(path, "w", encoding="utf-8") as f:
        for name, text in rows:
            f.write(f"{name}     {text}\n")

write_output(OUTPUT_ALPHA, entries_by_alpha)
write_output(OUTPUT_NUM, entries_by_num)

print(f"Zpracovano {len(entries)} souboru.")
print(f"Abecedni poradi  -> {OUTPUT_ALPHA}")
print(f"Ciselne poradi   -> {OUTPUT_NUM}")
