#pragma once
#include <exception>
#include <string>

namespace TSML
{
    template <typename ExceptionVariationEnum>
    class TSMLException : std::exception
    {
    public:
        explicit TSMLException(const ExceptionVariationEnum& type) : type(type) {}
        ExceptionVariationEnum Type() const
        {
            return type;
        }
        [[nodiscard]] virtual std::string What() const = 0;
    private:
        const ExceptionVariationEnum type;
    };
}


