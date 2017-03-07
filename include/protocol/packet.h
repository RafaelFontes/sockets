#pragma once

class buffer;

class packet
{
    buffer * data;

public:

    packet();
    ~packet();

    void setBuffer( buffer * );

    bool multibyte;
    bool containsToken;
    bool requiresResponse;
    bool keepAlive;

    unsigned int id;
    unsigned int lenght;
    unsigned int fullSize;
    unsigned int tokenSize;

    unsigned char * token;
    unsigned char * body;

private:
    void defaults();
    void extractFlags();
    void extractNumbers();
    void swapBytes(unsigned int &destination);
};
