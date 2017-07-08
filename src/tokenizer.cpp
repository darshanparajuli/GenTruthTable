#include "includes/tokenizer.h"
#include <set>

Tokenizer::Tokenizer(const std::string &input) : m_input(input), m_current_index(0)
{
}

Tokenizer::~Tokenizer()
{
}

bool Tokenizer::tokenize()
{
    std::string token;
    State state = State::START_STATE;
    char c = next_char();
    std::set<char> valid_chars;

    // TODO: don't have these hardcoded
    valid_chars.insert('+');
    valid_chars.insert('*');
    valid_chars.insert('~');
    valid_chars.insert(' ');
    valid_chars.insert('(');
    valid_chars.insert(')');
    valid_chars.insert('<');
    valid_chars.insert('=');
    valid_chars.insert('0');
    valid_chars.insert('1');
    valid_chars.insert(0);

    bool running = true;
    while (running)
    {
        switch (state)
        {
            case State::START_STATE:
            {
                switch (c)
                {
                    case ' ':
                    {
                        do
                        {
                            c = next_char();
                        } while (c == ' ');
                    }
                    break;
                    case '(':
                    case ')':
                    case '*':
                    case '+':
                    case '0':
                    case '1':
                    case '~':
                    {
                        token += c;
                        state = State::TOKEN_STATE;
                        break;
                    }
                    case '=':
                    {
                        token += c;
                        state = State::IMPLICATION_STATE;
                        break;
                    }
                    case '<':
                    {
                        token += c;
                        if (peek_next_char() == '>')
                        {
                            c = next_char();
                            token += c;
                            state = State::TOKEN_STATE;
                        }
                        else if (peek_next_char() == '+')
                        {
                            c = next_char();
                            token += c;
                            state = State::XOR_STATE;
                        }
                        else
                        {
                            state = State::INVALID_STATE;
                        }
                    }
                    break;
                    default:
                    {
                        if (is_char_valid_non_keyword(c))
                        {
                            token += c;
                            while (true)
                            {
                                char peeked = peek_next_char();
                                if (is_char_valid_non_keyword(peeked))
                                {
                                    c = next_char();
                                    token += c;
                                }
                                else
                                {
                                    if (valid_chars.find(peeked) == valid_chars.end())
                                    {
                                        state = State::INVALID_STATE;
                                        break;
                                    }
                                    else
                                    {
                                        state = State::TOKEN_STATE;
                                        break;
                                    }
                                }
                            }
                        }
                        else if (valid_chars.find(c) == valid_chars.end())
                        {
                            state = State::INVALID_STATE;
                        }
                    }
                    break;
                }
            }
            break;
            case State::IMPLICATION_STATE:
            {
                c = next_char();
                if (c == '>')
                {
                    token += c;
                    state = State::TOKEN_STATE;
                }
                else
                {
                    state = State::INVALID_STATE;
                }
            }
            break;
            case State::XOR_STATE:
            {
                c = next_char();
                if (c == '>')
                {
                    token += c;
                    state = State::TOKEN_STATE;
                }
                else
                {
                    state = State::INVALID_STATE;
                }
            }
            break;
            case State::TOKEN_STATE:
            {
                m_tokens.push_back(token);
                token.clear();

                if (m_current_index < m_input.size())
                {
                    c = next_char();
                    state = State::START_STATE;
                }
                else
                {
                    state = State::STOP_STATE;
                }
            }
            break;
            case State::STOP_STATE:
            case State::INVALID_STATE:
            {
                running = false;
            }
            break;
        }
    }

    return state != State::INVALID_STATE;
}

char Tokenizer::next_char()
{
    if (m_current_index < m_input.size())
    {
        return m_input[m_current_index++];
    }
    else
    {
        return 0;
    }
}

char Tokenizer::peek_next_char()
{
    return m_input[m_current_index];
}
