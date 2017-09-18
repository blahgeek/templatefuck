#include "./memory.hpp"
#include "./io.hpp"
#include <type_traits>

#ifndef TEMPLATEFUCK_ENGINE_HPP
#define TEMPLATEFUCK_ENGINE_HPP value

struct InvalidState {};

template <typename MEM, int PC, int FP, typename IO>
struct State {
    using memory = MEM;
    using io = IO;
    static constexpr int pc = PC;
    static constexpr int fp = FP;

    static constexpr int fp_data = MemoryGetter<MEM, FP>::value;
};


template <char INST, typename STATE>
struct StateNext {
    using value = State <
        typename STATE::memory,
        STATE::pc + 1,
        STATE::fp,
        typename STATE::io
    >;
};

template <typename STATE>
struct StateNext<'\0', STATE> {
    using value = InvalidState;
};


template <typename STATE>
struct StateNext<'+', STATE> {
    using value = State<
        typename MemorySetter<typename STATE::memory, STATE::fp, STATE::fp_data + 1>::value,
        STATE::pc + 1,
        STATE::fp,
        typename STATE::io
    >;
};

template <typename STATE>
struct StateNext<'-', STATE> {
    using value = State<
        typename MemorySetter<typename STATE::memory, STATE::fp, STATE::fp_data - 1>::value,
        STATE::pc + 1,
        STATE::fp,
        typename STATE::io
    >;
};

template <typename STATE>
struct StateNext<'>', STATE> {
    using value = State<
        typename STATE::memory,
        STATE::pc + 1,
        STATE::fp + 1,
        typename STATE::io
    >;
};

template <typename STATE>
struct StateNext<'<', STATE> {
    using value = State<
        typename STATE::memory,
        STATE::pc + 1,
        STATE::fp - 1,
        typename STATE::io
    >;
};

template <typename STATE>
struct StateNext<'[', STATE> {
    static constexpr int pair_pc = STATE::io::find_pair(STATE::pc);

    using value = typename std::conditional<STATE::fp_data == 0,
          State<typename STATE::memory, pair_pc, STATE::fp, typename STATE::io>,
          State<typename STATE::memory, STATE::pc + 1, STATE::fp, typename STATE::io>
        >::type;
};

template <typename STATE>
struct StateNext<']', STATE> {
    static constexpr int pair_pc = STATE::io::find_pair(STATE::pc);

    using value = typename std::conditional<STATE::fp_data != 0,
          State<typename STATE::memory, pair_pc, STATE::fp, typename STATE::io>,
          State<typename STATE::memory, STATE::pc + 1, STATE::fp, typename STATE::io>
        >::type;
};

template <typename STATE>
struct StateNext<'.', STATE> {
    using value = State<
        typename STATE::memory,
        STATE::pc + 1,
        STATE::fp,
        typename IOPrint<typename STATE::io, STATE::fp_data>::value
    >;
};

template <typename STATE>
struct StateNext<',', STATE> {
    using iogetc = IOGetc<typename STATE::io>;
    using value = State<
        typename MemorySetter<typename STATE::memory, STATE::fp, iogetc::ch>::value,
        STATE::pc + 1,
        STATE::fp,
        typename iogetc::value
    >;
};

#endif /* ifndef TEMPLATEFUCK_ENGINE_HPP */
