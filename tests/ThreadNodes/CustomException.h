#pragma once
#include "TSMLException.h"
#include <string>

namespace TSML::Test
{
    enum class TestExceptionType : unsigned int
    {
        InitException = 0,
        RunException
    };

    class InitException : public TSMLException<TestExceptionType>
    {
    public:
        explicit InitException(const TestExceptionType& t = TestExceptionType::InitException)
        : TSMLException<TestExceptionType>(t) {}
        [[nodiscard]] std::string What() const override;
    };

    class RunTimeException : public TSMLException<TestExceptionType>
    {
    public:
        explicit RunTimeException(const TestExceptionType& t = TestExceptionType::RunException)
                : TSMLException<TestExceptionType>(t) {}
        [[nodiscard]] std::string What() const override;
    };




} // Exceptions


