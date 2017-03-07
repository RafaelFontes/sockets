#pragma once

#undef EXPORT

#ifndef SERVER_LIBRARY
    #define EXPORT __declspec("dllimport")
#else
    #define EXPORT __declspec("dllexport")
#endif
