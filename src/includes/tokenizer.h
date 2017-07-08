#ifndef TRUTHTABLE_TOKENIZER_H
#define TRUTHTABLE_TOKENIZER_H

#include <iostream>
#include <vector>

enum class State
{
    START_STATE,

    XOR_STATE,
    IMPLICATION_STATE,
    TOKEN_STATE,

    STOP_STATE,
    INVALID_STATE,
};

class Tokenizer
{
private:
    std::string m_input;
    unsigned int m_current_index;
    std::vector<std::string> m_tokens;

public:
    Tokenizer(const std::string &input);

    ~Tokenizer();

    bool tokenize();

    inline std::vector<std::string> get() const { return m_tokens; }

private:
    char next_char();

    char peek_next_char();

    inline bool is_char_valid_non_keyword(char c) const { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
};

#endif
