#ifndef LOGICLONG_H
#define LOGICLONG_H

class Bytestream;

class LogicLong
{
public:
    int high, low;
    LogicLong(int high, int low)
    {
        this->high = high;
        this->low = low;
    }
    LogicLong(){};
    void decode(Bytestream *stream);
    void encode(Bytestream *stream);
    static int getLowerInt(long long longLong);
    static int getHigherInt(long long longLong);
};
#endif
