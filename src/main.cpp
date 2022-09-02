#include "aux.h"

int main(int argc, char *argv[])
{
    std::fstream file(argv[1]);
}

void readfile(std::fstream &file, std::vector<int> &vec)
{
    std::string line;

    while (!file.eof())
    {
        getline(file, line, ' ');
        // std::cout << line << std::endl;
        vec.push_back(std::stoi(line));
    }
}