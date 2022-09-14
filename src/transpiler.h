#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
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
    const std::map<std::string, InstructionsTable> instTable = {{"1", {"ADD", 2}},
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
    std::map<int, std::string> labelTable, jmpTable;
    std::vector<int> is_label;
    std::vector<std::string> m_code;
    std::string m_filename;
    int static label_num;

    void insert2labelTable(std::string);
    void insert2jmpTable(std::string);
    void printTable(std::map<int, std::string>, std::string);

public:
    Transpiler(std::vector<std::string>, std::string);
    Transpiler &getlabels();
    Transpiler &translate();
    void printTables();
};

std::vector<std::string> readfile(std::fstream &);
