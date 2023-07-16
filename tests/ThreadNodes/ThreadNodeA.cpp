#include "ThreadNodeA.h"

namespace TSML::Test
{
    void ThreadNodeA::OnInitialize()
    {
        if(test_init_exception)
        {
            throw InitException();
        }
        std::cout << "Initializing ThreadNodeA" << std::endl;
        test_result_init = true;
    }

    void ThreadNodeA::OnRun()
    {
        if(test_runtime_exception)
        {
            throw RunTimeException();
        }
        //Doing some fake work
        counter++;
        if(counter == 120)
        {
            test_result_runtime = true;
            FinishLoop();
        }
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    void ThreadNodeA::OnMessage(Message<TestThreadMessages> msg)
    {
        switch(msg.header.id)
        {
            case TestThreadMessages::TestMessage1:
            {
                std::cout << "[ThreadNodeA]: Message 1 Taken" << std::endl;
                Message<TestThreadMessages> msg2;
                msg2.header.id = TestThreadMessages::TestMessage2;
                Send("NodeB",msg2);
                std::cout << "[ThreadNodeA]: Message 2 Sent" << std::endl;
                break;
            }
            case TestThreadMessages::TestMessage3:
            {
                std::cout << "[ThreadNodeA]: Message 3 Taken" << std::endl;
                test_result_message_received = true;
                break;
            }
        }
    }
#pragma clang diagnostic pop

    void ThreadNodeA::OnFinish()
    {
        std::cout << "Finishing ThreadNodeA" << std::endl;
        test_result_finish = true;
    }

    void ThreadNodeA::OnException(const TSMLException<TestExceptionType> &e)
    {
        switch(e.Type())
        {
            case TestExceptionType::InitException:
                test_result_init_exception = true;
                FinishLoop();
                break;
            case TestExceptionType::RunException:
                test_result_runtime_exception = true;
                FinishLoop();
                break;
        }
    }
}