#pragma once

#ifndef PROTO_LIBRARY
    #define EXPORT __declspec("dllimport")
#else
    #define EXPORT __declspec("dllexport")
#endif
