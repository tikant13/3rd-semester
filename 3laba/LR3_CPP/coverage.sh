#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

# Без аргументов проверяем все три набора, либо выбираем один.
case "${1:-all}" in
    all) frameworks=(boost google catch) ;;
    boost|google|catch) frameworks=("$1") ;;
    *) echo 'Использование: bash coverage.sh [all|boost|google|catch]'; exit 1 ;;
esac
sources=(array.cpp linkedList.cpp doublyList.cpp stack.cpp Queue.cpp Rbtree.cpp Chaintable.cpp Cuckootable.cpp)
python3 -m gcovr --version >/dev/null

for name in "${frameworks[@]}"; do
    folder="build/$name"
    mkdir -p "$folder"
    # Старые счётчики не должны влиять на новый результат.
    rm -f "$folder"/*.gcda "$folder"/*.gcno "$folder"/*.o
    objects=()
    for source in "${sources[@]}"; do
        object="$folder/${source%.cpp}.o"
        g++ -std=c++17 -O0 -g --coverage -Wall -Wextra -Wpedantic -c "$source" -o "$object"
        objects+=("$object")
    done
    libs=(-pthread)
    if [ "$name" = google ]; then libs+=(-lgtest_main -lgtest); fi
    # Catch2 v2 создаёт main в файле тестов, v3 подключает отдельную библиотеку.
    if [ "$name" = catch ] && printf '#include <catch2/catch_all.hpp>\n' | g++ -x c++ -E - >/dev/null 2>&1; then
        libs+=(-lCatch2Main -lCatch2)
    fi
    # Измеряем классы, а не внутренний код тестового фреймворка.
    g++ -std=c++17 -I. -O0 -g -Wall -Wextra -Wpedantic -c "tests/tests_$name.cpp" -o "$folder/tests.o"
    g++ --coverage "$folder/tests.o" "${objects[@]}" "${libs[@]}" -o "$folder/test_$name"
    "$folder/test_$name" 2>&1 | tee "$folder/tests.txt"
    python3 -m gcovr --root . "$folder" --filter '^[^/]+\.(cpp|h)$' \
        --html-details "$folder/coverage.html" --txt "$folder/coverage.txt" \
        --json-summary "$folder/summary.json" --print-summary --fail-under-line 85
    echo "Отчёт: $folder/coverage.html"
done
