#ifndef TRUTHTABLE_TRUTHTABLE_H
#define TRUTHTABLE_TRUTHTABLE_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

enum class Op
{
    AND,
    OR,
    NOT,

    XOR,
    IMPLICATION,
    EQUIVALENCE,
};

class TruthTable
{
private:
    struct Node
    {
        std::string value;

        Node *left;
        Node *right;

        Node(std::string val) : value(val), left(nullptr), right(nullptr) {}
    };

    std::map<std::string, Op> m_operator_map;
    std::set<std::string> m_keywords_set;
    std::map<std::string, int> m_operator_precedence_map;
    std::vector<std::string> m_operands;
    Node *m_expression_tree;
    bool m_pretty_print;

public:
    TruthTable();

    ~TruthTable();

    bool generate(const std::string &input);

    inline void set_pretty_print(bool enabled) { m_pretty_print = enabled; }

private:
    void print_expression_tree(Node *node, std::string indent = "");

    void delete_expression_tree(Node *node);

    inline bool has_higher_precendence(const std::string &op_a, const std::string &op_b)
    {
        return m_operator_precedence_map[op_a] < m_operator_precedence_map[op_b];
    }

    inline bool has_equal_precendence(const std::string &op_a, const std::string &op_b)
    {
        return m_operator_precedence_map[op_a] == m_operator_precedence_map[op_b];
    }

    inline bool is_operand(const std::string &s) { return m_keywords_set.find(s) == m_keywords_set.end(); }

    std::vector<std::string> to_post_fix(const std::vector<std::string> &tokens);

    Node *to_expression_tree(std::vector<std::string> postfix);

    void generate(std::vector<std::string> &tokens);

    void expression_tree_to_string(Node *node, std::string &result, int depth = 0);

    bool solve_helper(std::map<std::string, bool> &value_map, Node *node);

    bool calculate(bool left, Op op, bool right = true);

    template <typename T>
    void print_container(const T &v);
};

#endif
