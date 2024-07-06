#pragma once
#include <utility>
#include "TSMLContext.h"
#include "TSMLException.h"

namespace TSML
{
    template <typename ThreadMessageEnum, typename ExceptionVariationEnum>
    class MainNode
    {
    public:
        explicit MainNode(TSMLContext<ThreadMessageEnum>* context, std::string id) : node_id(std::move(id))
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
            is_in_loop_stage = true;
            MainNode::Loop(this);
        }

        void FinishLoop()
        {
            if(!is_in_loop_stage)
            {
                OnFinish();
                return;
            }

            is_in_loop_stage = false;
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
        static void Loop(MainNode<ThreadMessageEnum,ExceptionVariationEnum>* node)
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
            while(node->is_in_loop_stage);
            node->OnFinish();
        }

        TSMLContext<ThreadMessageEnum>* context = nullptr;
        bool is_in_loop_stage = false;
        const std::string node_id;
    };

}
