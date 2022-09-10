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
    const std::map<std::string, InstructionsTable> ti;
    std::map<int, std::string> tl, tj;
    std::vector<int> is_label;
    std::vector<std::string> code;

public:
    void getlabels(std::vector<std::string>);

    Transpiler(const std::map<std::string, InstructionsTable> ti, std::vector<std::string> code) : ti{ti}, code{code}
    {
    }
};

std::vector<std::string> readfile(std::fstream &);
