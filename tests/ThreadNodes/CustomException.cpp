#include "CustomException.h"

namespace TSML::Test
{
    std::string InitException::What() const
    {
        return {"Initialization exception occured"};
    }

    std::string RunTimeException::What() const
    {
        return {"Runtime exception occured"};;
    }
}

