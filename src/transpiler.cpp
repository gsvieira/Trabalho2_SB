#include "transpiler.h"

Transpiler::Transpiler(std::vector<std::string> code, std::string filename) : m_code{code}, m_filename{filename}
{
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
    std::string outfilename = m_filename;
    outfilename.erase(outfilename.find_last_of('.'));
    outfilename.append(".s");
    std::ofstream outfile(outfilename); // open file

    // Prepare section data

    outfile << "section .data" << std::endl;
    outfile << "acc dd 0" << std::endl;
    outfile << "ten dd 10" << std::endl;
    for (auto &&tableline : labelTable)
    {
        outfile << tableline.second << " dd " << m_code[tableline.first] << std::endl;
    }
    outfile << std::endl;
    outfile << "overflow_text db 'Overflow',0dH,0ah" << std::endl;
    outfile << "ov_size equ $ - msg" << std::endl;
    outfile << "section .bss" << std::endl;
    outfile << "aux resd 11" << std::endl; // -2,147,483,648 a 2,147,483,647
    outfile << "section .text" << std::endl;
    outfile << std::endl;
    outfile << "global _start" << std::endl;
    outfile << "_start:" << std::endl;

    int addr = 0;
    while (addr >= 0)
    {
        // append jump label if addr is found on jmptable
        auto jmptoken = jmpTable.find(stoi(m_code[addr]));
        if (jmptoken != jmpTable.end())
        {
            outfile << jmptoken->second << ": ";
        }

        std::map<int, std::string>::iterator token;
        if (m_code[addr] != "14")
        {
            token = labelTable.find(stoi(m_code[addr + 1]));
        }

        switch (stoi(m_code[addr]))
        {
        // ADD
        case 1:
            outfile << "mov eax, [" << token->second << "]" << std::endl;
            outfile << "add [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // SUB
        case 2:
            outfile << "mov eax, [" << token->second << "]" << std::endl;
            outfile << "sub [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // MUL
        case 3:
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "IMUL dword [" << token->second << "]" << std::endl; // multiply eax with memory location
            outfile << "JO overflow_call" << std::endl;
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // DIV
        case 4:
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "cdq" << std::endl;
            outfile << "idiv dword [" << token->second << "]" << std::endl;
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMP
        case 5:
            outfile << "jmp " << token->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPN
        case 6:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "jl " << token->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPP
        case 7:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "jg " << token->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // JMPZ
        case 8:
            outfile << "cmp dword [acc], 0" << std::endl;
            outfile << "je " << token->second << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // COPY
        case 9:
            outfile << "mov eax, [" << token->second << "]" << std::endl;
            token = labelTable.find(stoi(m_code[addr + 2]));
            outfile << "mov [" << token->second << "], eax" << std::endl;
            outfile << std::endl;
            addr += 3;
            break;

        // LOAD
        case 10:
            outfile << ";LOAD" << std::endl;
            outfile << "mov eax, [" << token->second << "]" << std::endl;
            outfile << "mov [acc], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // STORE
        case 11:
            outfile << "mov eax, [acc]" << std::endl;
            outfile << "mov [" << token->second << "], eax" << std::endl;
            outfile << std::endl;
            addr += 2;
            break;

        // INPUT
        case 12: // TODO: pass arguments
            outfile << "push dword 0";
            outfile << "call input_function" << std::endl;
            outfile << "pop dword [" << token->second << "]" << std::endl;
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
            outfile << "push dword [" << token->second << "]" << std::endl;
            outfile << "push dword " << token->second << std::endl;
            outfile << "call s_input_function" << std::endl;
            outfile << "add esp, 8" << std::endl;
            outfile << std::endl;
            addr += 3;
            break;

        // S_OUTPUT
        case 16:
            outfile << "push dword [" << token->second << "]" << std::endl;
            outfile << "push dword " << token->second << std::endl;
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
    outfile << "mov eax, 3" << std::endl;
    outfile << "mov ebx, 0" << std::endl;
    outfile << "mov ecx, aux" << std::endl;
    outfile << "mov edx, 12" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << "push eax" << std::endl;
    outfile  << std::endl;
    outfile << "mov eax, 0" << std::endl;
    outfile << "mov esi, 0" << std::endl;
    outfile << "mov ecx, 0" << std::endl;
    outfile  << std::endl;
    outfile << "input_loop:" << std::endl;
    outfile << "mov ebx, 0" << std::endl;
    outfile << "mov bl, [aux+esi]" << std::endl;
    outfile << "inc esi" << std::endl;
    outfile << "cmp bl, '-' ; if neg" << std::endl;
    outfile << "je input_neg" << std::endl;
    outfile << "cmp bl, '0'" << std::endl;
    outfile << "jb input_end" << std::endl;
    outfile << "cmp bl, '9'" << std::endl;
    outfile << "jg input_end" << std::endl;
    outfile << "sub bl, '0'" << std::endl;
    outfile << "mul dword [ten]" << std::endl;
    outfile << "add eax, ebx" << std::endl;
    outfile << "jmp input_loop" << std::endl;
    outfile << "input_neg:" << std::endl;
    outfile << "mov ecx,1" << std::endl;
    outfile << "jmp input_loop" << std::endl;
    outfile << std::endl;
    outfile << std::endl;
    outfile << "input_end:" << std::endl;
    outfile << "cmp ecx, 1" << std::endl;
    outfile << "jne input_cleanup" << std::endl;
    outfile << "neg eax" << std::endl;
    outfile << std::endl;
    outfile << "input_cleanup:" << std::endl;
    outfile << "mov [ebp+8], eax" << std::endl;
    outfile << "pop eax" << std::endl;
    outfile << "leave" << std::endl;
    outfile << "ret" << std::endl;
    outfile << std::endl;

    outfile << "s_input_function:" << std::endl;
    outfile << "enter 0,0" << std::endl;
    outfile << "mov eax, 3" << std::endl;
    outfile << "mov ebx, 0" << std::endl;
    outfile << "mov ecx, [ebp+12]" << std::endl;
    outfile << "mov edx, [ebp+8]" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << "leave" << std::endl;
    outfile << "ret" << std::endl;
    outfile << std::endl;

    outfile << "s_output_function:" << std::endl;
    outfile << "enter 0,0" << std::endl;
    outfile << "mov eax, 4" << std::endl;
    outfile << "mov ebx, 1" << std::endl;
    outfile << "mov ecx, [ebp+12]" << std::endl;
    outfile << "mov edx, [ebp+8]" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << "leave" << std::endl;
    outfile << "ret" << std::endl;
    outfile << std::endl;

    outfile << "overflow_call:" << std::endl;
    outfile << "mov eax, 4" << std::endl;
    outfile << "mov ebx, 1" << std::endl;
    outfile << "mov ecx, overflow_text" << std::endl;
    outfile << "mov edx, [ov_size]" << std::endl;
    outfile << "int 80h" << std::endl;
    outfile << std::endl;

    return *this;
}