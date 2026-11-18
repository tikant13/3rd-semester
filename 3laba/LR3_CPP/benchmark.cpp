#include <benchmark/benchmark.h>
#include "array.h"
#include "linkedList.h"
#include "doublyList.h"
#include "stack.h"
#include "Queue.h"
#include "Rbtree.h"
#include "Chaintable.h"
#include "Cuckootable.h"
#include <vector>
using namespace std;

// Заполнение происходит до цикла: измеряем только чтение/поиск.
static void ArrayRead(benchmark::State& state){
    Array arr;
    int n = state.range(0);
    for (int i = 0; i < n; ++i) arr.PushBack("value");
    for (auto _ : state) benchmark::DoNotOptimize(arr.At(n / 2));
}

static void LinkedRead(benchmark::State& state){
    linkedList list;
    int n = state.range(0);
    for (int i = 0; i < n; ++i) list.pushBack("value");
    for (auto _ : state) benchmark::DoNotOptimize(list.At(n / 2));
}

static void DoublyRead(benchmark::State& state){
    DoublyList list;
    int n = state.range(0);
    for (int i = 0; i < n; ++i) list.pushBack("value");
    for (auto _ : state) benchmark::DoNotOptimize(list.At(n / 2));
}

// Измеряем пару push/pop, чтобы размер оставался постоянным.
static void StackPushPop(benchmark::State& state){
    Stack stack;
    string value = "value";
    for (int i = 0; i < state.range(0); ++i) stack.push(value);
    for (auto _ : state){
        stack.push(value);
        stack.pop();
        benchmark::ClobberMemory();
    }
}

static void QueuePushPop(benchmark::State& state){
    Queue queue;
    string value = "value";
    for (int i = 0; i < state.range(0); ++i) queue.push(value);
    for (auto _ : state){
        queue.push(value);
        queue.pop();
        benchmark::ClobberMemory();
    }
}

static void TreeFind(benchmark::State& state){
    RBTree tree;
    int n = state.range(0);
    for (int i = 0; i < n; ++i) tree.Insert(i);
    int key = 0;
    for (auto _ : state){
        benchmark::DoNotOptimize(tree.Contains(key));
        key = (key + 1) % n;
    }
}

static void ChainFind(benchmark::State& state){
    ChainTable table;
    vector<string> keys;
    int n = state.range(0);
    for (int i = 0; i < n; ++i){
        keys.push_back("key" + to_string(i));
        table.Insert(keys.back(), "value");
    }
    string value;
    int indx = 0;
    for (auto _ : state){
        benchmark::DoNotOptimize(table.Get(keys[indx], value));
        benchmark::DoNotOptimize(value);
        indx = (indx + 1) % n;
    }
}

static void CuckooFind(benchmark::State& state){
    CuckooTable table;
    vector<string> keys;
    int n = state.range(0);
    for (int i = 0; i < n; ++i){
        keys.push_back("key" + to_string(i));
        table.Insert(keys.back(), "value");
    }
    string value;
    int indx = 0;
    for (auto _ : state){
        benchmark::DoNotOptimize(table.Get(keys[indx], value));
        benchmark::DoNotOptimize(value);
        indx = (indx + 1) % n;
    }
}

BENCHMARK(ArrayRead)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(LinkedRead)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(DoublyRead)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(StackPushPop)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(QueuePushPop)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(TreeFind)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(ChainFind)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK(CuckooFind)->Arg(100)->Arg(1000)->Arg(10000);
BENCHMARK_MAIN();
