add_library(
        TSML INTERFACE
        TSML.h
        TSMLContext.h
        Queue.h
        Message.h
        ThreadNode.h
        TSMLException.h)

target_include_directories(TSML INTERFACE ${CMAKE_CURRENT_LIST_DIR})