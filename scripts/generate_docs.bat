@echo off
REM ------------------------------------------------------------------
REM generate_docs.bat
REM Generates Doxygen HTML documentation locally
REM Run from the project root: scripts\generate_docs.bat
REM ------------------------------------------------------------------

echo [docs] Generating documentation...

REM clean previous output
if exist "docs\output" (
    echo [docs] Cleaning previous output...
    rmdir /s /q "docs\output"
)

REM run doxygen
doxygen docs\Doxyfile

echo [docs] Documentation generated at docs\output\html\index.html