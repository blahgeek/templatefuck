#include <cstdio>
#include <type_traits>

#ifndef TEMPLATEFUCK_MEMORY_HPP
#define TEMPLATEFUCK_MEMORY_HPP value

struct EmptyMemory {
    inline static void print() {}
};

template <int V, typename TAIL=EmptyMemory>
struct Memory {
    static constexpr int value = V;
    using tail = TAIL;

    inline static void print() {
        putchar(value);
        tail::print();
    }
};


template <typename L, int N>
struct MemoryGetter {
    static constexpr int value = MemoryGetter<typename L::tail, N-1>::value;
};

template <typename L>
struct MemoryGetter<L, 0> {
    static constexpr int value = L::value;
};


template <typename L, int N, int V>
struct MemorySetter {
    using value = Memory<
        L::value,
        typename MemorySetter<typename L::tail, N-1, V>::value
    >;
};

template <typename L, int V>
struct MemorySetter<L, 0, V> {
    using value = Memory<V, typename L::tail>;
};


template <typename L, int V>
struct MemoryAppender {
    using value = Memory<L::value, typename MemoryAppender<typename L::tail, V>::value>;
};

template <int V>
struct MemoryAppender<EmptyMemory, V> {
    using value = Memory<V, EmptyMemory>;
};

template <int V, int N>
struct MemoryMaker {
    using value = Memory<V, typename MemoryMaker<V, N-1>::value>;
};

template <int V>
struct MemoryMaker<V, 0> {
    using value = EmptyMemory;
};


namespace {
    using ARR = typename MemoryMaker<0, 10>::value;
    static_assert(MemoryGetter<ARR, 1>::value == 0, "");

    using MOD_ARR = typename MemorySetter<ARR, 1, 100>::value;
    static_assert(MemoryGetter<MOD_ARR, 0>::value == 0, "");
    static_assert(MemoryGetter<MOD_ARR, 1>::value == 100, "");
}

#endif /* ifndef TEMPLATEFUCK_MEMORY_HPP */
