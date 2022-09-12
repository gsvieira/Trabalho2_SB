#include "transpiler.h"

Transpiler::Transpiler(std::vector<std::string> code) : code{code}
{
    is_label.reserve(code.size());
    std::fill(is_label.begin(), is_label.end(), -1); // TODO: test if its working
}

Transpiler &Transpiler::getlabels()
{
    int i = 0;
    while (true)
    {
        auto instruction_found = instTable.find(code[i]);
        switch (instruction_found->second.size)
        {
        case 1:
            return *this;
            break;

        case 2:
            if (code[i] >= "5" && code[i] <= "8") // if its a jump
            {
                insert2jmpTable(code[i + 1]);
            }
            else
            {
                insert2labelTable(code[i + 1]);
            }
            i += 2;
            break;
        case 3:
            if (code[i] == "9") // if its a copy
            {
                insert2labelTable(code[i + 1]);
                insert2labelTable(code[i + 2]);
            }
            else
            {
                insert2labelTable(code[i + 1]);
            }
            i += 3;
            break;
        default:
            std::cout << "erro para identificar instrução: " << code[i] << std::endl;
            break;
        }
    }
}

void Transpiler::insert2jmpTable(std::string token)
{
    int label_num = jmpTable.size() + labelTable.size();
    std::string label = "label" + std::to_string(label_num);
    if (!jmpTable.contains(stoi(token)))
    {
        jmpTable.insert(std::pair<int, std::string>(stoi(token), label));
    }
}
void Transpiler::insert2labelTable(std::string token)
{
    int label_num = jmpTable.size() + labelTable.size();
    std::string label = "label" + std::to_string(label_num);
    if (!labelTable.contains(stoi(token)))
    {
        labelTable.insert(std::pair<int, std::string>(stoi(token), label));
    }
}