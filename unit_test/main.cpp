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

struct TruthTableTest : testing::Test
{
    TruthTable truth_table;

    TruthTableTest() : truth_table{TruthTable()}
    {
    }

    ~TruthTableTest()
    {
    }
};

TEST_F(TruthTableTest, or_test)
{
    std::string exp = "a+b";
    std::stringstream ss;
    truth_table.generate(exp, ss);

    std::string expected = "000\n101\n011\n111\n";
    EXPECT_STREQ(expected.c_str(), ss.str().c_str());
}

TEST_F(TruthTableTest, and_test)
{
    std::string exp = "a*b";
    std::stringstream ss;
    truth_table.generate(exp, ss);

    std::string expected = "000\n100\n010\n111\n";
    EXPECT_STREQ(expected.c_str(), ss.str().c_str());
}

TEST_F(TruthTableTest, not_test)
{
    std::string exp = "~a";
    std::stringstream ss;
    truth_table.generate(exp, ss);

    std::string expected = "01\n10\n";
    EXPECT_STREQ(expected.c_str(), ss.str().c_str());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
