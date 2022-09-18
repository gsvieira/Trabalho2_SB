#include "transpiler.h"

int Transpiler::label_num{0};
int main(int argc, char *argv[])
{
    std::fstream file(argv[1]);
    std::vector<std::string> code = readfile(file);
    Transpiler tradutor(code, argv[1]);
    tradutor.getlabels();
    tradutor.printTables();
    tradutor.translate();
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
    return vec;
}