
#include "stringoperations.h"


std::vector<std::string> nucmath::tokenize(const std::string& str, std::vector<char> delims)
{
    std::vector<std::string> tokens;
    std::string token = "";
    for(size_t i = 0; i < str.size(); i++)
    {
        bool delim_found = false;
        for(size_t j = 0; j < delims.size(); j++)
        {
            if(str[i] == delims[j])
            {
                if(token.size() > 0)
                    tokens.push_back(token);

                delim_found = true;
                token = "";
                break;
            }
        }

        if(delim_found == false)
            token += str[i];
    }

    if(token.size() > 0)
        tokens.push_back(token);

    return tokens;
}
