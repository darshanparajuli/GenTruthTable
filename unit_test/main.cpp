#include <iostream>
#include <gtest/gtest.h>
#include "../src/includes/truthtable.h"

struct Arg
{
    size_t argc;
    char **argv;

    Arg(std::initializer_list<std::string> list) : argc(list.size() + 1), argv(nullptr)
    {
        argv = new char *[argc];
        argv[0] = new char[16];
        strcpy(argv[0], "program");
        auto itr = std::begin(list);
        for (int i = 1; i < argc; ++i)
        {
            std::string option = *itr++;
            argv[i] = new char[option.length() + 1];
            strcpy(argv[i], option.c_str());
        }
    }

    ~Arg()
    {
        for (int i = 0; i < argc; ++i)
        {
            delete[] argv[i];
        }
        delete[] argv;
    }
};

struct TestInfo
{
    std::string input;
    std::string expected;

    TestInfo(const std::string &i, const std::string &e) : input{i}, expected{e}
    {
    }

    std::string get_expected() const
    {
        std::stringstream ss;
        for (int i = 0; i < expected.length(); ++i)
        {
            if (expected[i] == ':')
            {
                ss << '\n';
            }
            else
            {
                ss << expected[i];
            }
        }
        ss << "\n";
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const TestInfo &info)
    {
        return os << "Input: " << info.input << ", Expected: " << info.expected;
    }
};

struct TruthTableTest : testing::Test, testing::WithParamInterface<TestInfo>
{
    TruthTable truth_table;

    TruthTableTest() : truth_table{TruthTable()}
    {
    }

    ~TruthTableTest()
    {
    }
};

TEST_P(TruthTableTest, test)
{
    std::stringstream ss;
    truth_table.generate(GetParam().input, ss);

    EXPECT_STREQ(GetParam().get_expected().c_str(), ss.str().c_str());
}

INSTANTIATE_TEST_CASE_P(Default, TruthTableTest, testing::Values(
    TestInfo{"a+b", "000:101:011:111"},
    TestInfo{"a*b", "000:100:010:111"},
    TestInfo{"~a", "01:10"},
    TestInfo{"a<+>b", "000:101:011:110"},
    TestInfo{"a=>b", "001:100:011:111"},
    TestInfo{"a<>b", "001:100:010:111"}
));

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
