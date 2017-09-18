#include "./memory.hpp"

#ifndef TEMPLATEFUCK_IO_HPP
#define TEMPLATEFUCK_IO_HPP value

#ifndef SOURCECODE
#define SOURCECODE "++++++[>++++++++++<-]>+++++."
#endif

#ifndef INPUTSTR
#define INPUTSTR ""
#endif


static constexpr const char _CODE[] = SOURCECODE;
static constexpr const char _INPUT[] = INPUTSTR;

template <typename OUTPUT, int INPUT_POS>
struct IOState {
    using output = OUTPUT;
    static constexpr int input_pos = INPUT_POS;

    static constexpr char get_inst(int n) {
        return _CODE[n];
    }

    static constexpr int find_end_bracket(int p, int depth=0) {
        return _CODE[p] == '[' ? find_end_bracket(p+1, depth+1) :
            _CODE[p] != ']' ? find_end_bracket(p+1, depth) :
            depth == 1 ? p : find_end_bracket(p+1, depth-1);
    }

    static constexpr int find_begin_bracket(int p, int depth=0) {
        return _CODE[p] == ']' ? find_begin_bracket(p-1, depth+1) :
            _CODE[p] != '[' ? find_begin_bracket(p-1, depth) :
            depth == 1 ? p : find_begin_bracket(p-1, depth-1);
    }

    static constexpr int find_pair(int n) {
        return _CODE[n] == '[' ? find_end_bracket(n) : find_begin_bracket(n);
    }

    static constexpr int find_pc(int start, int step, char ch) {
        return get_inst(start) == ch ? start : find_pc(start+step, step, ch);
    }
};


template <typename STATE, int CH>
struct IOPrint {
    using value = IOState<typename MemoryAppender<typename STATE::output, CH>::value, STATE::input_pos>;
};

template <typename STATE>
struct IOGetc {
    static constexpr int ch = _INPUT[STATE::input_pos];
    using value = IOState<typename STATE::output, STATE::input_pos + 1>;
};

#endif /* ifndef TEMPLATEFUCK_IO_HPP */
