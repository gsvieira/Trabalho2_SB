#include "transpiler.h"



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
    if (!jmpTable.contains(stoi(token)))
    {
        std::string label = "label" + std::to_string(label_num++);
        jmpTable.insert(std::pair<int, std::string>(stoi(token), label));
    }
}
void Transpiler::insert2labelTable(std::string token)
{
    if (!labelTable.contains(stoi(token)))
    {
        std::string label = "label" + std::to_string(label_num++);
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
    std::string outfilename = m_filename.replace(m_filename.find('.'), 2, ".s");
    std::ofstream outfile(outfilename); // open file

    // Prepare section data

    outfile << "section .data" << std::endl;
    outfile << "acc dd 0" << std::endl;
    for (auto &&tableline : labelTable)
    {
        outfile << tableline.second << " dd " << m_code[tableline.first] << std::endl;
    }
    outfile << std::endl;
    outfile << "section .text" << std::endl;
    outfile << std::endl;
    outfile << "global _start" << std::endl;
    outfile << "_start:" << std::endl;

    int addr = 0;
    while (addr >= 0)
    {
        // TODO: append jump label if addr is found on jmptable
        switch (stoi(m_code[addr]))
        {
        // ADD
        case 1:
            outfile << "mov eax, [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "add [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // SUB
        case 2:
            outfile << "mov eax, [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "sub [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // MUL
        case 3: // TODO:treatment of overflow
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "imul dword [" << labelTable.find(addr + 1)->second << "]" << std::endl; // multiply eax with memory location
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // DIV
        case 4:
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "cdq" << std::endl;
            outfile << "idiv dword [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMP
        case 5:
            outfile << "jmp " << labelTable.find(addr + 1)->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPN
        case 6:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "jl " << labelTable.find(addr + 1)->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPP
        case 7:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "jg " << labelTable.find(addr + 1)->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPZ
        case 8:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "je " << labelTable.find(addr + 1)->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // COPY
        case 9:
            outfile << "mov eax, [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "mov [" << labelTable.find(addr + 2)->second << "], eax" << std::endl;
            outfile << std::endl;
            addr += 3;
            break;

        // LOAD
        case 10:
            outfile << "mov eax, [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // STORE
        case 11:
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "mov [" << labelTable.find(addr + 1)->second << "], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // INPUT
        case 12: // TODO: pass arguments
            outfile << "" ;
            outfile << "call input_function" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // OUTPUT
        case 13: // TODO: pass arguments
            outfile << "call output_function" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // STOP
        case 14:
            outfile << "mov eax, 1" << std::endl;
            outfile << "mov ebx, 0" << std::endl;
            outfile << "int 80h" << std::endl;
            outfile << std::endl;
            addr = -1;
            break;

        // S_INPUT
        case 15:
            outfile << "push dword [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "push dword " << labelTable.find(addr + 2)->second << std::endl;
            outfile << "call s_input_function" << std::endl;
            outfile << "add esp, 8" << std::endl;
            outfile << std::endl;
            addr += 3;
            break;

        // S_OUTPUT
        case 16:
            outfile << "push dword [" << labelTable.find(addr + 1)->second << "]" << std::endl;
            outfile << "push dword " << labelTable.find(addr + 2)->second << std::endl;
            outfile << "call s_output_function" << std::endl;
            outfile << "add esp, 8" << std::endl;
            outfile << std::endl;
            addr += 3;
            break;

        default:
            std::cout << "Erro: operação não identificada" << std::endl;
            exit(1);
            break;
        }
    }

    outfile << "input_function:" << std::endl;
    outfile << "enter 0,0" << std::endl;
    outfile << "mov eax, 3" <<std::endl;
    outfile << "mov ebx, 0" <<std::endl;
    outfile << "mov ecx," <<std::endl;
    outfile << "mov edx," <<std::endl;
    outfile << "int 80h" <<std::endl;
    outfile << "leave" <<std::endl;
    outfile << "ret" <<std::endl;
    outfile << std::endl;

    outfile << "s_input_function:" << std::endl;
    outfile << "enter 0,0" << std::endl;
    outfile << "mov eax, 3" << std::endl;
    outfile << "mov ebx, 0" << std::endl;
    outfile << "mov ecx, [ebp+12]" << std::endl;
    outfile << "mov edx, [ebp+8]" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << "leave" <<std::endl;
    outfile << "ret" <<std::endl;
    outfile << std::endl;

    outfile << "s_output_function:" << std::endl;
    outfile << "enter 0,0" << std::endl;
    outfile << "mov eax, 4" << std::endl;
    outfile << "mov ebx, 1" << std::endl;
    outfile << "mov ecx, [ebp+12]" << std::endl;
    outfile << "mov edx, [ebp+8]" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << "leave" <<std::endl;
    outfile << "ret" <<std::endl;
    outfile << std::endl;

    







    return *this;
}