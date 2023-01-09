#ifndef _CEPBASE_H
#define _CEPBASE_H

class IntervalBase
{
    bool _extendRequested = false;
protected:
    bool extendRequested() { return _extendRequested; }
public:
    void extend() { _extendRequested = true; }
    void extended() { _extendRequested = false; }
};

#endif
