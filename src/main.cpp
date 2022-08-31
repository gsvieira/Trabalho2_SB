#include "aux.h"

int main(int argc, char *argv[])
{
    std::fstream file(argv[1]);
}


void readfile(std::fstream &file, int &dados, std::vector<int> &vec)
{
    std::string line;
    getline(file, line); // 12

    dados = std::stoi(line);

    while (!file.eof())
    {
        getline(file, line, ' ');
        if (line == "")
        {
            continue;
        }
        // std::cout << line << std::endl;
        vec.push_back(std::stoi(line));
    }
}