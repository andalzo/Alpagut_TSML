#pragma once
#include <utility>

#include "TSML.h"
#include "TestThreadMessages.h"
#include "CustomException.h"

namespace TSML::Test
{
    class MainNodeTest : public MainNode<TestThreadMessages, TestExceptionType>
    {
    public:
        explicit MainNodeTest(TSMLContext<TestThreadMessages> *context ,std::string node_id) :
                MainNode<TestThreadMessages, TestExceptionType>(context, std::move(node_id))
        {
        }

        //Test Booleans
        std::atomic<bool> test_init_exception = false;
        std::atomic<bool> test_runtime_exception = false;

        //Result Booleans
        std::atomic<bool> test_result_init = false;
        std::atomic<bool> test_result_runtime = false;
        std::atomic<bool> test_result_finish = false;
        std::atomic<bool> test_result_init_exception = false;
        std::atomic<bool> test_result_runtime_exception = false;
        std::atomic<bool> test_result_message_received = false;

    protected:
        void OnInitialize() override;
        void OnRun() override;
        void OnMessage(Message<TestThreadMessages> msg) override;
        void OnFinish() override;
        void OnException(const TSMLException<TestExceptionType>& e) override;
    private:
        int counter = 0;
    };

}

