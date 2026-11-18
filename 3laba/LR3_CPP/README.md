# ЛР3 — C++: структуры данных, тесты и сериализация

## Сериализация (задание 3)

Сделано по принципу репозитория одногруппника: отдельные `serialization.h` и
`serialization.cpp` больше не нужны. В каждом из восьми классов объявлены методы:

- `saveToFile(fileName)` — записать структуру в текстовый файл;
- `loadFromFile(fileName)` — восстановить структуру из текстового файла;
- `saveBinary(fileName)` — записать структуру в бинарный файл;
- `loadBinary(fileName)` — восстановить структуру из бинарного файла.

Методы возвращают `true` при успехе и `false` при ошибке. Чтение выполняется
сначала во временный объект, поэтому повреждённый файл не меняет старые данные.
Данные из файлов сохраняют порядок элементов, в том числе для стека и очереди;
в красно-чёрном дереве сохраняются ключи и цвета узлов. Бинарный формат хранит
длины строк, поэтому поддерживает строки с пробелами.

Для упрощения общие операции чтения/записи реализованы в `Array`, а остальные
классы собирают свои элементы во временный `Array` с меткой типа. Это
внутренняя деталь реализации — новых вспомогательных файлов не добавлено.

**Важно:** новый бинарный формат `ARR1` отличается от прежнего `LR3B1`.
Старые бинарные файлы с `LR3B1` не читаются этой версией. Если они нужны,
сохрани исходную лабораторную с `serialization.cpp` отдельно. Старый текстовый
формат базы продолжает работать через `main.cpp`.

## Сборка программы

Выполнять из папки `3laba`:

```bash
g++ -std=c++17 main.cpp array.cpp linkedList.cpp doublyList.cpp stack.cpp Queue.cpp Rbtree.cpp Chaintable.cpp Cuckootable.cpp -o dbms
```

Примеры текстового файла:

```bash
./dbms --file data.txt --query 'SPUSH mystack first'
./dbms --file data.txt --query 'SPUSH mystack second'
./dbms --file data.txt --query 'SPOP mystack'
```

Примеры бинарного файла:

```bash
./dbms --file data.bin --format binary --query 'SPUSH mystack first'
./dbms --file data.bin --format binary --query 'SPUSH mystack second'
./dbms --file data.bin --format binary --query 'SPEEK mystack'
```

В `main.cpp` база по-прежнему поддерживает несколько именованных структур в
одном файле. Запись производится во временный `.tmp` с последующей заменой
основного файла. В CLI значение `--format binary` надо указывать каждый раз.

## Тестирование и покрытие

В каждом файле `tests/tests_boost.cpp`, `tests/tests_google.cpp`,
`tests/tests_catch.cpp` теперь 60 тестов (включая проверки сериализации).

```bash
bash coverage.sh boost
bash coverage.sh google
bash coverage.sh catch
# или сразу все три: bash coverage.sh
```

Для работы требуются g++, библиотеки Boost.Test, GoogleTest, Catch2 и модуль
Python `gcovr`. Отчёты будут в `build/boost/coverage.html`,
`build/google/coverage.html`, `build/catch/coverage.html`.

**Проверено здесь:** компиляция обычной программы, 60 тестов Boost.Test,
сохранение/загрузка каждой из восьми структур в обоих форматах, ручные прогоны
CLI. По `gcov` покрытие строк каждого `.cpp` класса выше 95%; полноценный
`gcovr`-отчёт и тесты GoogleTest/Catch2 в этой среде не запускались.

## Другие файлы

`benchmark.cpp`, `Makefile`, исходные алгоритмы контейнеров и команды CLI
оставлены. Для замеров производительности можно использовать отдельную
команду сборки с Google Benchmark, как в исходной ЛР3.
