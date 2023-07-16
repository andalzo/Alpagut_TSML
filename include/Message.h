#pragma once
#include <vector>
#include <iostream>
#include <cstring>


namespace TSML
{
    template<typename ThreadMessageEnum>
    class Message;

    //Message header is sent at start of all messages. The template allows us
    //to use "enum class" to ensure that the messages are valid at compile time
    template<typename ThreadMessageEnum>
    class TSMessageHeader
    {
        friend class Message<ThreadMessageEnum>;

        template <typename DataType>
        friend Message<ThreadMessageEnum>& operator<<(Message<ThreadMessageEnum>& msg, const DataType& data);

        template <typename DataType>
        friend Message<ThreadMessageEnum>& operator>>(Message<ThreadMessageEnum>& msg, DataType& data)  ;

    public:
        TSMessageHeader() = default;
        ThreadMessageEnum id{};
    private:
        uint32_t size = 0;
    };

    template<typename ThreadMessageEnum>
    class Message
    {
    private:
        std::vector<uint8_t> body;
        bool is_body_keep_raw_data = false;
    public:
        TSMessageHeader<ThreadMessageEnum> header{};

        Message() = default;

        //returns size of entire message packet in bytes
        [[nodiscard]] size_t size() const
        {
            return body.size();
        }

        friend std::ostream& operator<<(std::ostream& os, const Message<ThreadMessageEnum>& msg)
        {
            os << "ID:" << int(msg.header.id) << "Size:" << msg.header.size;
            return os;
        }

        //Pushes any POD-like data into the message buffer
        template <typename DataType>
        friend Message<ThreadMessageEnum>& operator<<(Message<ThreadMessageEnum>& msg, const DataType& data)
        {
            if(msg.is_body_keep_raw_data)
            {
                throw std::logic_error("The raw buffer data which has dynamic size, cannot be written over it.");
            }
            //Check that the type of the data being pushed is trivially copyable
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to serialize for adding");

            //Cache current size of vector, as this will be point we insert the data
            size_t i = msg.body.size();

            //Resize the vector by size of the data being pushed
            msg.body.resize(msg.body.size() + sizeof(DataType));

            //Physically copy the data into newly allocated vector space
            std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

            //Recalculate the message size
            msg.header.size = msg.size();

            //Return target msg, so it can be chained
            return msg;
        }


        //Reading data via msg
        template <typename DataType>
        friend Message<ThreadMessageEnum>& operator>>(Message<ThreadMessageEnum>& msg, DataType& data)
        {
            if(msg.is_body_keep_raw_data)
            {
                throw std::logic_error("The raw buffer data which has dynamic size, cannot be read within normal fixed size data.");
            }

            //Check the type of data being pushed is trivially copyable
            static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to serialize for reading");

            //Cache the location towards the end of the vector where the pulled data starts
            size_t i = msg.body.size() - sizeof(DataType);

            // Physically copy the data from the vector into user variable
            std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

            //Shrink the vector to remove read bytes, and reset end position
            msg.body.resize(i);

            //Recalculate the msg size
            msg.header.size = msg.size();

            //Return the target message so it can be chained
            return msg;
        }


        void set_body_as_raw_buffer(const std::vector<uint8_t>& raw_buffer)
        {
            //We need to set this internal bool as true to protect adding fixed size data to that message
            is_body_keep_raw_data = true;
            body = raw_buffer;
            header.size = body.size();
        }

        void get_body_raw_buffer(std::vector<uint8_t>& buffer_to_fill)
        {
            buffer_to_fill = body;
            body.resize(0);
            header.size = body.size();
            is_body_keep_raw_data = false;
        }

        [[nodiscard]] bool is_message_body_raw_data() const
        {
            return is_body_keep_raw_data;
        }

    };

}
