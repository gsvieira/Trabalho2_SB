#include "transpiler.h"

#define GET_VARIABLE_NAME(Variable) (#Variable)

Transpiler::Transpiler(std::vector<std::string> code, std::string filename) : m_code{code}, m_filename{filename}
{
    is_label.reserve(code.size());
    std::fill(is_label.begin(), is_label.end(), -1); // TODO: test if its working
}

Transpiler &Transpiler::getlabels()
{
    int i = 0;
    while (true)
    {
        auto instruction_found = instTable.find(m_code[i]);
        switch (instruction_found->second.size)
        {
        case 1:
            return *this;
            break;

        case 2:
            if (m_code[i] >= "5" && m_code[i] <= "8") // if its a jump
            {
                insert2jmpTable(m_code[i + 1]);
            }
            else
            {
                insert2labelTable(m_code[i + 1]);
            }
            i += 2;
            break;
        case 3:
            if (m_code[i] == "9") // if its a copy
            {
                insert2labelTable(m_code[i + 1]);
                insert2labelTable(m_code[i + 2]);
            }
            else
            {
                insert2labelTable(m_code[i + 1]);
            }
            i += 3;
            break;
        default:
            std::cout << "erro para identificar instrução: " << m_code[i] << std::endl;
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

void Transpiler::printTable(std::map<int, std::string> table, std::string tablename)
{
    std::cout << tablename << std::endl;
    for (auto &&tableline : table)
    {
        std::cout << tableline.first << ": " << tableline.second << std::endl;
    }
}

void Transpiler::printTables()
{
    printTable(jmpTable, "Jump Table:");
    printTable(labelTable, "Label Table:");
}

Transpiler &Transpiler::translate()
{
    std::string outfilename = m_filename.replace(m_filename.find('.'),2,".s");
    std::ofstream outfile(outfilename); // open file 

    

    int addr = 0;
    while (addr >= 0)
    {
        // TODO: append jump label if addr is found on jmptable
        switch (stoi(m_code[addr]))
        {
        // ADD
        case 1:
            /* code */
            break;

        // SUB
        case 2:
            /* code */
            break;

        // MUL
        case 3:
            /* code */
            break;

        // DIV
        case 4:
            /* code */
            break;

        // JMP
        case 5:
            /* code */
            break;

        // JMPN
        case 6:
            /* code */
            break;

        // JMPP
        case 7:
            /* code */
            break;

        // JMPZ
        case 8:
            /* code */
            break;

        // COPY
        case 9:
            /* code */
            break;

        // LOAD
        case 10:
            /* code */
            break;

        // STORE
        case 11:
            /* code */
            break;

        // INPUT
        case 12:
            /* code */
            break;

        // OUTPUT
        case 13:
            /* code */
            break;

        // STOP
        case 14:
            /* code */
            break;

        // S_INPUT
        case 15:
            /* code */
            break;

        // S_OUTPUT
        case 16:
            /* code */
            break;

        default:
            std::cout << "Erro: operação não identificada" << std::endl;
            exit(1);
            break;
        }
    }
}