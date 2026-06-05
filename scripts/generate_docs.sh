#!/usr/bin/env bash
# ------------------------------------------------------------------
# generate_docs.sh
# Generates Doxygen HTML documentation locally
# Run from the project root: ./scripts/generate_docs.sh
# ------------------------------------------------------------------

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "[docs] Generating documentation..."

cd "${PROJECT_ROOT}"

# clean previous output
if [ -d "docs/output" ]; then
    echo "[docs] Cleaning previous output..."
    rm -rf docs/output
fi

# run doxygen
doxygen docs/Doxyfile

echo "[docs] Documentation generated at docs/output/html/index.html"