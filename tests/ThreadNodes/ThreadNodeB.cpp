#include "ThreadNodeB.h"

namespace TSML::Test
{
    void ThreadNodeB::OnInitialize()
    {
        if(test_init_exception)
        {
            throw InitException();
        }
        std::cout << "Initializing ThreadNodeB" << std::endl;
        test_result_init = true;
    }

    void ThreadNodeB::OnRun()
    {
        if(test_runtime_exception)
        {
            throw RunTimeException();
        }
        //Doing some fake work
        if(test_messaging)
        {
            Message<TestThreadMessages> msg;
            msg.header.id = TestThreadMessages::TestMessage1;
            Send("NodeA", msg);
            std::cout << "[ThreadNodeB]: Message 1 Sent." << std::endl;
            test_messaging = false;
        }

        counter++;
        if(counter == 1000)
        {
            test_result_runtime = true;
            FinishLoop();
        }
    }

    void ThreadNodeB::OnMessage(Message<TestThreadMessages> msg)
    {
        switch(msg.header.id)
        {
            case TestThreadMessages::TestMessage2:
            {
                std::cout << "[ThreadNodeB]: Message 2 Received." << std::endl;
                Message<TestThreadMessages> msg3;
                msg3.header.id = TestThreadMessages::TestMessage3;
                Send("NodeA",msg3);
                std::cout << "[ThreadNodeB]: Message 3 Sent." << std::endl;
                test_result_message_received = true;
                break;
            }
        }
    }

    void ThreadNodeB::OnFinish()
    {
        std::cout << "Finishing ThreadNodeB" << std::endl;
        test_result_finish = true;
    }

    void ThreadNodeB::OnException(const TSMLException<TestExceptionType> &e)
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
