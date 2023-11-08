#include "ThreadNodes/ThreadNodeA.h"
#include "ThreadNodes/ThreadNodeB.h"
#include "ThreadNodes/MainNodeTest.h"
#include "gtest/gtest.h"
using namespace TSML::Test;
using namespace TSML;

TEST(TSML_SingleNodeTests, TestMainWithNoExceptions)
{
    TSMLContext<TestThreadMessages> context;
    MainNodeTest node(&context, "MainNode");

    node.InitializeLoop();
    ASSERT_EQ(node.test_result_init, true) << "Initialization of single node of main with no exception failed.";

    node.StartLoop();

    ASSERT_EQ(node.test_result_runtime,true) << "Running of single node of main with no exception failed.";

    ASSERT_EQ(node.test_result_finish,true) << "Finishing of single nodeof main  with no exception failed.";
}

TEST(TSML_SingleNodeTests, TestMainWithInitException)
{
    TSMLContext<TestThreadMessages> context;
    MainNodeTest node(&context, "MainNode");
    node.test_init_exception = true;

    node.InitializeLoop();
    ASSERT_EQ(node.test_result_init_exception,true) << "The OnExecute() method did not handle init exception in single node of main test.";
    ASSERT_EQ(node.test_result_finish, true) << "After the caught of init exception the node did not finish himself correctly  in single node of main test.";
}


TEST(TSML_SingleNodeTests, TestMainWithRunTimeException) {
    TSMLContext<TestThreadMessages> context;
    MainNodeTest node(&context, "MainNode");
    node.test_runtime_exception = true;

    node.InitializeLoop();
    ASSERT_EQ(node.test_result_init, true) << "Initialization of single node of main with runtime exception failed.";

    node.StartLoop();
    ASSERT_EQ(node.test_result_runtime_exception, true) << "The OnExecute() method did not handle runtime exception in single node of main test.";
    ASSERT_EQ(node.test_result_finish, true) << "After the caught of run time exception the node did not finish himself correctly in single node of main test.";
}

TEST(TSML_SingleNodeTests, TestWithNoExceptions)
{
    TSMLContext<TestThreadMessages> context;
    ThreadNodeA node_a(&context, "NodeA");

    node_a.InitializeLoop();
    ASSERT_EQ(node_a.test_result_init, true)
    << "Initialization of single node with no exception failed.";

    node_a.StartLoop();
    if(node_a.IsJoinable())
    {
        node_a.Join();
    }

    ASSERT_EQ(node_a.test_result_runtime,true)
    << "Running of single node with no exception failed.";

    ASSERT_EQ(node_a.test_result_finish,true)
    << "Finishing of single node with no exception failed.";
}

TEST(TSML_SingleNodeTests, TestWithInitException)
{
    TSMLContext<TestThreadMessages> context;
    ThreadNodeA node_a(&context, "NodeA");
    node_a.test_init_exception = true;

    node_a.InitializeLoop();
    ASSERT_EQ(node_a.test_result_init_exception,true)
    << "The OnExecute() method did not handle init exception in single node test.";
    ASSERT_EQ(node_a.test_result_finish, true)
    << "After the caught of init exception the node did not finish himself correctly"
       " in single node test.";
}


TEST(TSML_SingleNodeTests, TestWithRunTimeException)
{
    TSMLContext<TestThreadMessages> context;
    ThreadNodeA node_a(&context, "NodeA");
    node_a.test_runtime_exception = true;

    node_a.InitializeLoop();
    ASSERT_EQ(node_a.test_result_init, true)
    << "Initialization of single node with runtime exception failed.";

    node_a.StartLoop();
    if(node_a.IsJoinable())
    {
        node_a.Join();
    }
    ASSERT_EQ(node_a.test_result_runtime_exception,true)
    << "The OnExecute() method did not handle runtime exception in single node test.";
    ASSERT_EQ(node_a.test_result_finish, true)
    << "After the caught of run time exception the node did not finish himself correctly"
       " in single node test.";

}

TEST(TSML_MultipleNodeTests, TestWithNoExceptionsNoMessages)
{
    TSMLContext<TestThreadMessages> context;
    ThreadNodeA node_a(&context, "NodeA");
    ThreadNodeB node_b(&context, "NodeB");

    node_a.InitializeLoop();
    ASSERT_EQ(node_a.test_result_init, true)
    << "Initialization of NodeA with no exception failed.";
    node_b.InitializeLoop();
    ASSERT_EQ(node_b.test_result_init, true)
    << "Initialization of NodeB with no exception failed.";

    node_a.StartLoop();
    node_b.StartLoop();

    if(node_a.IsJoinable())
    {
        node_a.Join();
    }
    if(node_b.IsJoinable())
    {
        node_b.Join();
    }

    ASSERT_EQ(node_a.test_result_runtime,true)
    << "Running of NodeA with no exception failed.";
    ASSERT_EQ(node_b.test_result_runtime,true)
    << "Running of NodeB with no exception failed.";

    ASSERT_EQ(node_a.test_result_finish,true)
    << "Finishing of NodeA with no exception failed.";
    ASSERT_EQ(node_b.test_result_finish,true)
    << "Finishing of NodeB with no exception failed.";
}

TEST(TSML_MultipleNodeTests, TestWithNoExceptionsWithMessages)
{
    TSMLContext<TestThreadMessages> context;
    ThreadNodeA node_a(&context, "NodeA");
    ThreadNodeB node_b(&context, "NodeB");

    node_a.InitializeLoop();
    ASSERT_EQ(node_a.test_result_init, true)
    << "Initialization of NodeA with no exception failed.";
    node_b.InitializeLoop();
    ASSERT_EQ(node_b.test_result_init, true)
    << "Initialization of NodeB with no exception failed.";

    node_b.test_messaging = true;

    node_a.StartLoop();
    node_b.StartLoop();


    node_a.Join();
    node_b.Join();


    ASSERT_EQ(node_a.test_result_runtime,true)
    << "Running of NodeA with no exception failed.";
    ASSERT_EQ(node_b.test_result_runtime,true)
    << "Running of NodeB with no exception failed.";

    ASSERT_EQ(node_a.test_result_message_received,true)
    << "The messaging test at NodeA failed.";
    ASSERT_EQ(node_b.test_result_message_received,true)
    << "The messaging test at NodeB failed.";


    ASSERT_EQ(node_a.test_result_finish,true)
    << "Finishing of NodeA with no exception failed.";
    ASSERT_EQ(node_b.test_result_finish,true)
    << "Finishing of NodeB with no exception failed.";
}

TEST(TSML_MultipleNodeTests, TestWithNoExceptionsWithMessagesWithMain)
{
    TSMLContext<TestThreadMessages> context;
    MainNodeTest node(&context, "NodeA");
    ThreadNodeB node_b(&context, "NodeB");

    node.InitializeLoop();
    ASSERT_EQ(node.test_result_init, true) << "Initialization of main node with no exception failed.";
    node_b.InitializeLoop();
    ASSERT_EQ(node_b.test_result_init, true) << "Initialization of NodeB with no exception failed.";

    node_b.test_messaging = true;

    node_b.StartLoop();
    node.StartLoop();

    node_b.Join();

    ASSERT_EQ(node.test_result_runtime,true) << "Running of MainNode with no exception failed.";
    ASSERT_EQ(node_b.test_result_runtime,true) << "Running of NodeB with no exception failed.";

    ASSERT_EQ(node.test_result_message_received,true)<< "The messaging test at MainNode failed.";
    ASSERT_EQ(node_b.test_result_message_received,true)<< "The messaging test at NodeB failed.";


    ASSERT_EQ(node.test_result_finish,true) << "Finishing of NodeA with no exception failed.";
    ASSERT_EQ(node_b.test_result_finish,true)<< "Finishing of NodeB with no exception failed.";
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}