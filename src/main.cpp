#include <option_parser/option_parser.h>
#include <iostream>
#include "includes/truthtable.h"

void print_title()
{
    std::cout << "***************************************************" << std::endl;
    std::cout << "*                                                 *" << std::endl;
    std::cout << "*  Truth Table Generator by Darshan Parajuli      *" << std::endl;
    std::cout << "*                                                 *" << std::endl;
    std::cout << "*  Syntax:                                        *" << std::endl;
    std::cout << "*  NOT           => '~'                           *" << std::endl;
    std::cout << "*  AND           => '*'                           *" << std::endl;
    std::cout << "*  Or            => '+'                           *" << std::endl;
    std::cout << "*  XOR           => '<+>'                         *" << std::endl;
    std::cout << "*  IMPLICATION   => '=>'                          *" << std::endl;
    std::cout << "*  EQUIVALENCE   => '<>'                          *" << std::endl;
    std::cout << "*  Tautology     => '1'                           *" << std::endl;
    std::cout << "*  Contradiction => '0'                           *" << std::endl;
    std::cout << "*                                                 *" << std::endl;
    std::cout << "*  Operand names can be [a-zA-Z]+ only.           *" << std::endl;
    std::cout << "*  Enter 'quit' or 'exit' to exit.                *" << std::endl;
    std::cout << "*                                                 *" << std::endl;
    std::cout << "*  Example: (a + b)<+>(~c*d)                      *" << std::endl;
    std::cout << "*                                                 *" << std::endl;
    std::cout << "***************************************************" << std::endl;
    std::cout << std::endl;
}

void process_input(TruthTable &truth_table, const std::string &arg)
{
    if (truth_table.generate(arg))
    {
        // do nothing
    }
    else
    {
        std::cout << "Error: invalid expression!" << std::endl;
    }
}

void interactive_mode(TruthTable &truth_table)
{
    print_title();

    for (;;)
    {
        std::cout << "Logical expression >> ";
        std::string line;
        getline(std::cin, line);
        if (line.empty())
        {
            continue;
        }
        if (line == "quit" || line == "exit")
        {
            break;
        }

        process_input(truth_table, line);
        std::cout << std::endl;
    }

    std::cout << "Bye!" << std::endl;
}

void process_stdin(TruthTable &truth_table)
{
    std::string input;
    while (getline(std::cin, input))
    {
        process_input(truth_table, input);
    }
}

int main(int argc, char **argv)
{
    OptionParser parser;
    parser.new_option('h')
        .long_name("help")
        .description("print descriptions for all options")
        .valid_by_itself_only();
    parser.new_option('r')
        .long_name("read-stdin")
        .description("read input from stdin");
    parser.new_option('p')
        .long_name("pretty-print")
        .description("prints the output in a nicer form");

    if (!parser.parse(argc, argv))
    {
        std::cerr << parser.get_error_message() << std::endl;
        return EXIT_FAILURE;
    }

    TruthTable truth_table;

    if (!parser.has_options() && parser.has_args())
    {
    }

    bool read_stdin = false;

    Option option;
    while (parser.get_next_option(option))
    {
        switch (option.name)
        {
            case 'h':
            {
                std::cout << parser.get_usage({"GenTruthTable [OPTIONS] [ARGUMENTS]"});
                std::cout << "Runs in interactive mode by default." << std::endl;
                return EXIT_SUCCESS;
            }
            case 'r':
            {
                read_stdin = true;
            }
            break;
            case 'p':
            {
                truth_table.set_pretty_print(true);
            }
            break;
        }
    }

    if (parser.has_args())
    {
        if (read_stdin)
        {
            std::cerr << "Error: option -r does not take any arguments" << std::endl;
            return EXIT_FAILURE;
        }

        std::vector<std::string> args = parser.get_args();
        if (args.size() > 1)
        {
            std::cerr << "Error: only 1 expression allowed" << std::endl;
            return EXIT_FAILURE;
        }

        process_input(truth_table, args[0]);
        return EXIT_SUCCESS;
    }

    if (read_stdin)
    {
        process_stdin(truth_table);
    }
    else
    {
        truth_table.set_pretty_print(true);
        interactive_mode(truth_table);
    }

    return EXIT_SUCCESS;
}
