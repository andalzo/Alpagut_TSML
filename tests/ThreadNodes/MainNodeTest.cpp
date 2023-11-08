#include "MainNodeTest.h"

namespace TSML::Test
{
    void MainNodeTest::OnInitialize()
    {
        if(test_init_exception)
        {
            throw InitException();
        }
        std::cout << "Initializing MainNode" << std::endl;
        test_result_init = true;
    }

    void MainNodeTest::OnRun()
    {
        if(test_runtime_exception)
        {
            throw RunTimeException();
        }
        //Doing some fake work
        counter++;
        if(counter == 1200)
        {
            test_result_runtime = true;
            FinishLoop();
        }
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    void MainNodeTest::OnMessage(Message<TestThreadMessages> msg)
    {
        switch(msg.header.id)
        {
            case TestThreadMessages::TestMessage1:
            {
                std::cout << "[MainNode]: Message 1 Taken" << std::endl;
                Message<TestThreadMessages> msg2;
                msg2.header.id = TestThreadMessages::TestMessage2;
                Send("NodeB",msg2);
                std::cout << "[MainNode]: Message 2 Sent" << std::endl;
                break;
            }
            case TestThreadMessages::TestMessage3:
            {
                std::cout << "[MainNode]: Message 3 Taken" << std::endl;
                test_result_message_received = true;
                break;
            }
        }
    }
#pragma clang diagnostic pop

    void MainNodeTest::OnFinish()
    {
        std::cout << "Finishing ThreadNodeA" << std::endl;
        test_result_finish = true;
    }

    void MainNodeTest::OnException(const TSMLException<TestExceptionType> &e)
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