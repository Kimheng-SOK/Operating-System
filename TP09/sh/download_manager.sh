#!/bin/bash
set -euo pipefail

for file in *; do
  [[ -f "$file" ]] || continue

  ext="${file##*.}"
  if [[ "$file" == "$ext" ]]; then
    ext="no_extension"
  fi

  mkdir -p "$ext"
  mv -- "$file" "$ext/"
done

echo "Downloads organized by file extension."
