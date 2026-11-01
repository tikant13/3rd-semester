#pragma once
#include <string>
using namespace std;

// Хеш-функция методом свёртки: ключ режется на части по 3 цифры, части складываются.
// 523456795 -> 523 + 456 + 795 = 1774.
// В expr записывается само выражение, чтобы его можно было вывести
long long foldHash(const string& key, string& expr);

// То же самое, но без выражения (для хеш-таблиц)
long long foldHash(const string& key);