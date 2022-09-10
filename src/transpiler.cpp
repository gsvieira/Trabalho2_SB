#include "transpiler.h"

Transpiler::Transpiler(std::vector<std::string> code) : code{code}
{
    is_label.reserve(code.size());
    std::fill(is_label.begin(), is_label.end(), -1); // TODO: test if its working
}

Transpiler &Transpiler::getlabels()
{
    for (int i = 0; i < code.size(); i++)
    {
        
    }
}
