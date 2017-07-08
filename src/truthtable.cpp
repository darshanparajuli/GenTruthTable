#include "includes/truthtable.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stack>
#include "includes/symbols.h"
#include "includes/tokenizer.h"

TruthTable::TruthTable()
    : m_operator_map(std::map<std::string, Op>()),
      m_keywords_set(std::set<std::string>()),
      m_operator_precedence_map(std::map<std::string, int>()),
      m_operands(std::vector<std::string>()),
      m_expression_tree(nullptr),
      m_pretty_print(false)
{
    m_operator_map[symbol::AND] = Op::AND;
    m_operator_map[symbol::OR] = Op::OR;
    m_operator_map[symbol::NOT] = Op::NOT;
    m_operator_map[symbol::XOR] = Op::XOR;
    m_operator_map[symbol::IMPLICATION] = Op::IMPLICATION;
    m_operator_map[symbol::EQUIVALENCE] = Op::EQUIVALENCE;

    for (auto &&entry : m_operator_map)
    {
        m_keywords_set.insert(entry.first);
    }
    m_keywords_set.insert(symbol::LEFT_PAREN);
    m_keywords_set.insert(symbol::RIGHT_PAREN);

    m_operator_precedence_map[symbol::NOT] = 1;
    m_operator_precedence_map[symbol::AND] = 2;
    m_operator_precedence_map[symbol::OR] = 3;
    m_operator_precedence_map[symbol::IMPLICATION] = 4;
    m_operator_precedence_map[symbol::EQUIVALENCE] = 5;
}

TruthTable::~TruthTable()
{
    delete_expression_tree(m_expression_tree);
}

bool TruthTable::generate(const std::string &input)
{
    m_operands.clear();

    Tokenizer tokenizer(input);
    if (!tokenizer.tokenize())
    {
        return false;
    }

    std::vector<std::string> tokens = tokenizer.get();
    std::set<std::string> operands;
    for (const auto &t : tokens)
    {
        if (is_operand(t))
        {
            operands.insert(t);
        }
    }

    std::for_each(std::begin(operands), std::end(operands), [this](std::string s) { m_operands.push_back(s); });
    std::sort(m_operands.begin(), m_operands.end());

    std::vector<std::string> postfix = to_post_fix(tokens);
    if (postfix.empty())
    {
        return false;
    }

    //    print_container(postfix);

    m_expression_tree = to_expression_tree(postfix);
    if (!m_expression_tree)
    {
        return false;
    }

    solve(tokens);

    return true;
}

std::vector<std::string> TruthTable::to_post_fix(const std::vector<std::string> &tokens)
{
    std::stack<std::string> op_stack;
    std::vector<std::string> postfix;
    std::string last_symbol;

    bool error_possibility = false;
    unsigned int i = 0;
    while (i < tokens.size())
    {
        auto symbol = tokens[i++];
        if (is_operand(symbol))
        {
            error_possibility = false;
            postfix.push_back(symbol);
        }
        else
        {
            if (error_possibility && symbol != symbol::NOT)
            {
                postfix.clear();
                return postfix;
            }

            if (last_symbol == symbol::NOT)
            {
                error_possibility = true;
            }

            if (op_stack.empty() || op_stack.top() == symbol::LEFT_PAREN)
            {
                op_stack.push(symbol);
            }
            else if (symbol == symbol::LEFT_PAREN)
            {
                op_stack.push(symbol);
            }
            else if (symbol == symbol::RIGHT_PAREN)
            {
                for (std::string &top = op_stack.top(); top != symbol::LEFT_PAREN; top = op_stack.top())
                {
                    postfix.push_back(top);
                    op_stack.pop();
                }
                op_stack.pop();  // discard left parenthesis
            }
            else if (has_higher_precendence(symbol, op_stack.top()) || has_equal_precendence(symbol, op_stack.top()))
            {
                op_stack.push(symbol);
            }
            else
            {
                std::string &top = op_stack.top();
                postfix.push_back(top);
                op_stack.pop();
                --i;
            }
        }
        last_symbol = symbol;
    }

    if (last_symbol == symbol::NOT)
    {
        postfix.clear();
        return postfix;
    }

    while (op_stack.size() > 0)
    {
        std::string &top = op_stack.top();
        if (top == symbol::LEFT_PAREN || top == symbol::RIGHT_PAREN)
        {
            postfix.clear();
            break;
        }

        postfix.push_back(top);
        op_stack.pop();
    }

    return postfix;
}

TruthTable::Node *TruthTable::to_expression_tree(std::vector<std::string> postfix)
{
    std::stack<Node *> expression_stack;
    for (const auto &symbol : postfix)
    {
        if (is_operand(symbol))
        {
            expression_stack.push(new Node(symbol));
        }
        else
        {
            if (expression_stack.empty())
            {
                return nullptr;
            }

            Node *node = new Node(symbol);
            Node *left = expression_stack.top();
            expression_stack.pop();
            node->left = left;

            if (symbol != symbol::NOT)  // special case
            {
                if (expression_stack.empty())
                {
                    return nullptr;
                }

                Node *right = expression_stack.top();
                expression_stack.pop();
                node->right = right;
            }

            expression_stack.push(node);
        }
    }
    if (expression_stack.size() > 1)
    {
        Node *node = expression_stack.top();
        expression_stack.pop();
        while (!expression_stack.empty())
        {
            delete_expression_tree(node);
            node = expression_stack.top();
            expression_stack.pop();
        }
        return nullptr;
    }
    return expression_stack.top();
}

void TruthTable::delete_expression_tree(TruthTable::Node *node)
{
    if (node)
    {
        delete_expression_tree(node->left);
        delete_expression_tree(node->right);
        delete node;
    }
}

void TruthTable::print_expression_tree(TruthTable::Node *node, std::string indent)
{
    if (node)
    {
        print_expression_tree(node->left, indent += " ");
        std::cout << indent << node->value << std::endl;
        print_expression_tree(node->right, indent += " ");
    }
}

bool TruthTable::calculate(bool left, Op op, bool right)
{
    switch (op)
    {
        case Op::NOT:
        {
            return !left;
        }
        case Op::AND:
        {
            return left && right;
        }
        case Op::OR:
        {
            return left || right;
        }
        case Op::XOR:
        {
            return (left || right) && !(left && right);
        }
        case Op::IMPLICATION:
        {
            return !left || right;
        }
        case Op::EQUIVALENCE:
        {
            return !((left || right) && !(left && right));
        }
        default:
        {
            assert(!"Error: invalid code path");
            return false;
        }
    }
}

void TruthTable::solve(std::vector<std::string> &tokens)
{
    std::ostream &os = std::cout;

    unsigned int max_operand_text_width = 0;
    std::string input;

    if (m_pretty_print)
    {
        expression_tree_to_string(m_expression_tree, input);
        for (const auto &operand : m_operands)
        {
            if (operand.size() > max_operand_text_width)
            {
                max_operand_text_width = static_cast<unsigned int>(operand.size());
            }
        }
        max_operand_text_width += 1;
    }

    unsigned int input_text_width = static_cast<unsigned int>(input.size());

    unsigned long operand_count = m_operands.size();

    unsigned int max_chars = static_cast<unsigned int>(operand_count) * max_operand_text_width + input_text_width +
                             static_cast<unsigned int>(operand_count) + 2;

    auto print_row_separator = [&]() {
        for (unsigned int i = 0; i < max_chars; ++i)
        {
            if ((i < (max_chars - input_text_width) && i % (max_operand_text_width + 1) == 0) || i == max_chars - 1)
            {
                os << "+";
            }
            else
            {
                os << "-";
            }
        }
        os << std::endl;
    };

    if (m_pretty_print)
    {
        print_row_separator();

        for (auto &&s : m_operands)
        {
            os << "|";
            os << std::setw(max_operand_text_width) << s;
        }
        os << "|" << std::setw(input_text_width) << input << "|" << std::endl;

        print_row_separator();
    }

    unsigned long num_permutations = 1;
    unsigned long interval_count = 0;
    for (const auto &operand : m_operands)
    {
        if (operand != symbol::TRUE && operand != symbol::FALSE)
        {
            num_permutations *= 2;
            ++interval_count;
        }
    }

    bool *intervals = new bool[interval_count];
    for (unsigned long i = 0; i < interval_count; ++i)
    {
        intervals[i] = true;
    }

    unsigned long *counters = new unsigned long[interval_count];
    counters[0] = 1;
    for (unsigned long i = 1; i < interval_count; ++i)
    {
        counters[i] = counters[i - 1] * 2;
    }

    std::map<std::string, bool> map;
    for (unsigned long i = 0; i < num_permutations; ++i)
    {
        for (unsigned long j = 0; j < interval_count; ++j)
        {
            if (i % counters[j] == 0)
            {
                intervals[j] = !intervals[j];
            }
        }

        map.clear();

        int index = 0;
        for (const auto &operand : m_operands)
        {
            if (operand == symbol::FALSE)
            {
                map[operand] = false;
            }
            else if (operand == symbol::TRUE)
            {
                map[operand] = true;
            }
            else
            {
                map[operand] = intervals[index];
                ++index;
            }
        }

        bool result = solve_helper(map, m_expression_tree);

        if (m_pretty_print)
        {
            os << "|";

            for (unsigned long j = 0; j < operand_count; ++j)
            {
                os << std::setw(max_operand_text_width) << std::internal << map[m_operands[j]];
                os << "|";
            }

            os << std::setw(input_text_width) << result;

            os << "|";
        }
        else
        {
            for (unsigned long j = 0; j < operand_count; ++j)
            {
                os << map[m_operands[j]];
            }

            os << result;
        }
        os << std::endl;
    }

    if (m_pretty_print)
    {
        print_row_separator();
    }

    delete[] counters;
    delete[] intervals;
}

bool TruthTable::solve_helper(std::map<std::string, bool> &value_map, TruthTable::Node *node)
{
    if (is_operand(node->value))
    {
        return value_map[node->value];
    }
    bool result_left = solve_helper(value_map, node->left);
    bool result_right = false;
    if (node->right)
    {
        result_right = solve_helper(value_map, node->right);
    }
    return calculate(result_left, m_operator_map[node->value], result_right);
}

void TruthTable::expression_tree_to_string(TruthTable::Node *node, std::string &result, int depth)
{
    if (node)
    {
        bool can_print_paren = (node->left || node->right) && depth > 0;
        if (can_print_paren)
        {
            result += symbol::LEFT_PAREN;
        }
        expression_tree_to_string(node->right, result, depth + 1);
        result += node->value;
        expression_tree_to_string(node->left, result, depth + 1);
        if (can_print_paren)
        {
            result += symbol::RIGHT_PAREN;
        }
    }
}

template <typename T>
void TruthTable::print_container(const T &v)
{
    auto itr = std::begin(v);
    if (itr != std::end(v))
    {
        while (itr != std::end(v))
        {
            std::cout << *itr;
            if (itr + 1 != std::end(v))
            {
                std::cout << ", ";
            }
            ++itr;
        }
        std::cout << std::endl;
    }
}
