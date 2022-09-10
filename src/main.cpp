#include "transpiler.h"

int main(int argc, char *argv[])
{
    const std::map<std::string, InstructionsTable> ti = {{"1", {"ADD", 2}},
                                           {"2", {"SUB", 2}},
                                           {"3", {"MUL", 2}},
                                           {"4", {"DIV", 2}},
                                           {"5", {"JMP", 2}},
                                           {"6", {"JMPN", 2}},
                                           {"7", {"JMPP", 2}},
                                           {"8", {"JMPZ", 2}},
                                           {"9", {"COPY", 3}},
                                           {"10", {"LOAD", 2}},
                                           {"11", {"STORE", 2}},
                                           {"12", {"INPUT", 2}},
                                           {"13", {"OUTPUT", 2}},
                                           {"14", {"STOP", 1}},
                                           {"15", {"S_INPUT", 3}},
                                           {"16", {"S_OUTPUT", 3}}};

    std::fstream file(argv[1]);
    readfile(file);
}

std::vector<std::string> readfile(std::fstream &file)
{
    std::vector<std::string> vec;
    std::string line;

    while (!file.eof())
    {
        getline(file, line, ' ');
        // std::cout << line << std::endl;
        vec.push_back(line);
    }
}