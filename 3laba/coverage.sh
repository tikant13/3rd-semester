#!/usr/bin/env bash
set -euo pipefail

# Помести этот файл в 3laba/task2 и запусти: bash task2/coverage.sh
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="$ROOT/task2/coverage_build"
REPORT="$ROOT/task2/coverage_report"

if ! command -v gcovr >/dev/null 2>&1; then
    echo "Не найден gcovr. Установи: sudo apt install gcovr"
    exit 1
fi

if ! command -v g++ >/dev/null 2>&1; then
    echo "Не найден g++. Установи: sudo apt install g++"
    exit 1
fi

cd "$ROOT"

# Исходники классов из первого задания.
SOURCES=(
    array.cpp linkedList.cpp doublyList.cpp stack.cpp
    Queue.cpp Rbtree.cpp Chaintable.cpp Cuckootable.cpp
)

for file in "${SOURCES[@]}" \
            task2/tests_boost.cpp task2/tests_google.cpp task2/tests_catch.cpp; do
    if [[ ! -f "$file" ]]; then
        echo "Файл не найден: $ROOT/$file"
        exit 1
    fi
done

# Чистим только результаты предыдущего измерения покрытия.
rm -rf "$BUILD" "$REPORT"
mkdir -p "$BUILD" "$REPORT"

FLAGS=(-std=c++17 -O0 -g --coverage -I"$ROOT")
OBJECTS=()

# Компилируем классы ОДИН раз: все три набора тестов будут
# записывать покрытие в одни и те же файлы .gcda.
for file in "${SOURCES[@]}"; do
    object="$BUILD/${file%.cpp}.o"
    g++ "${FLAGS[@]}" -c "$file" -o "$object"
    OBJECTS+=("$object")
done

# 1. Boost.Test
printf '\n=== Boost.Test ===\n'
g++ "${FLAGS[@]}" task2/tests_boost.cpp "${OBJECTS[@]}" -o "$BUILD/test_boost"
"$BUILD/test_boost" --log_level=error

# 2. GoogleTest
printf '\n=== GoogleTest ===\n'
g++ "${FLAGS[@]}" task2/tests_google.cpp "${OBJECTS[@]}" \
    -o "$BUILD/test_google" -lgtest_main -lgtest -pthread
"$BUILD/test_google"

# 3. Catch2 (v3)
printf '\n=== Catch2 ===\n'
g++ "${FLAGS[@]}" task2/tests_catch.cpp "${OBJECTS[@]}" \
    -o "$BUILD/test_catch" -lCatch2Main -lCatch2
"$BUILD/test_catch"

# Строим подробный HTML-отчёт: для каждого исходника будут
# показаны строки, которые выполнялись и не выполнялись тестами.
printf '\n=== HTML-отчёт ===\n'
gcovr --root "$ROOT" "$BUILD" \
    --exclude '.*/task2/.*' \
    --html-details -o "$REPORT/index.html" \
    --print-summary

printf '\nГотово! Открой файл:\n%s\n' "$REPORT/index.html"
