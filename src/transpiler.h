#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

struct InstructionsTable
{
    std::string token;
    int size;
};


class Transpiler
{
private:
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
    std::map<int, std::string> tl, tj;
    std::vector<int> is_label;
    std::vector<std::string> code;

public:
    Transpiler& getlabels();

    Transpiler( std::vector<std::string> code );
};

std::vector<std::string> readfile(std::fstream &);
