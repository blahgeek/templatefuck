#include "./engine.hpp"

#ifndef MEMORYSIZE
#define MEMORYSIZE 256
#endif

using InitialState = State<
    typename MemoryMaker<0, MEMORYSIZE>::value,
    0,
    0,
    IOState<EmptyMemory, 0>
>;


template <typename STATE=InitialState>
struct Computer {
    static constexpr char current_inst = STATE::io::get_inst(STATE::pc);
    using state_next = typename StateNext<current_inst, STATE>::value;

    using value = typename std::conditional<
        current_inst == '\0',
        STATE,
        typename Computer<state_next>::value
    >::type;
};

template <>
struct Computer <InvalidState> {
    using value = InvalidState;
};


int main(int argc, char *argv[]) {
    Computer<>::value::io::output::print();
    return 0;
}
