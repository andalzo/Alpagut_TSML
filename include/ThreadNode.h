#pragma once
#include <utility>
#include <thread>
#include <atomic>
#include "TSMLContext.h"
#include "TSMLException.h"

namespace TSML
{
    template <typename ThreadMessageEnum, typename ExceptionVariationEnum>
    class ThreadNode
    {
    public:
        explicit ThreadNode(TSMLContext<ThreadMessageEnum>* context, std::string id) : node_id(std::move(id))
        {
            this->context = context;
        }

        void InitializeLoop()
        {
            try
            {
                OnInitialize();
            }
            catch(const TSMLException<ExceptionVariationEnum>& e)
            {
                OnException(e);
            }
            context->emplace_ts_queue(node_id);
        }

        void StartLoop()
        {
            runner_thread = std::thread(ThreadNode<ThreadMessageEnum, ExceptionVariationEnum>::Loop,this);
            is_in_loop_stage = true;
        }
        void FinishLoop()
        {
            if(!is_in_loop_stage)
            {
                OnFinish();
                return;
            }

            thread_finisher = true;
            is_in_loop_stage = false;
        }
        void Join()
        {
            runner_thread.join();
            context->erase_ts_queue(node_id);
        }

        bool IsJoinable()
        {
            return runner_thread.joinable();
        }

        void Send(const std::string& target_node_id, Message<ThreadMessageEnum> msg)
        {
            context->get_ts_queue(target_node_id).push(msg);
        }

        bool CheckIncomingMessage(Message<ThreadMessageEnum>& msg)
        {
            return context->get_ts_queue(node_id).try_pop(msg);
        }

    protected:

        virtual void OnInitialize() = 0;
        virtual void OnRun() = 0;
        virtual void OnMessage(Message<ThreadMessageEnum> msg) = 0;
        virtual void OnFinish() = 0;
        virtual void OnException(const TSMLException<ExceptionVariationEnum>& e) = 0;


    private:
        static void Loop(ThreadNode<ThreadMessageEnum,ExceptionVariationEnum>* node)
        {
            do
            {
                try
                {
                    Message<ThreadMessageEnum> possible_incoming_msg;
                    while(node->CheckIncomingMessage(possible_incoming_msg))
                    {
                        node->OnMessage(possible_incoming_msg);
                    }
                    node->OnRun();
                }
                catch(const TSMLException<ExceptionVariationEnum>& e)
                {
                    node->OnException(e);
                }
            }
            while(!node->thread_finisher);
            node->OnFinish();
        }

        TSMLContext<ThreadMessageEnum>* context = nullptr;
        std::atomic<bool> thread_finisher = false;
        std::atomic<bool> is_in_loop_stage = false;
        const std::string node_id;
        std::thread runner_thread;
    };

}
