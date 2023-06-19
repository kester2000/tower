#pragma once

extern "C"
{
    int init(const char* body);

    // If NULL is passed in, it is directly output in std::out, otherwise it is output in buffer.
    int spfa(char* buffer = 0);
}