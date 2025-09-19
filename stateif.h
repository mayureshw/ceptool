#ifndef _STATEIF_H
#define _STATEIF_H

#include <cstdint>
#include <vector>
#include "exprf.h"
using namespace std;

// Macro recommended for the implementor of stateif interface
#define TYPES2ETYP \
        TYP2ETYP(string); \
        TYP2ETYP(int); \
        TYP2ETYP(float); \
        TYP2ETYP(double); \
        TYP2ETYP(bool); \
        TYP2ETYP(uint8_t); \
        TYP2ETYP(uint16_t); \
        TYP2ETYP(uint32_t); \
        TYP2ETYP(uint64_t); \
        cout << "Unknown expression type for: " << str() << endl; \
        exit(1); \


// Design note about void*: It mainly stems from the restriction in C++ that
// we cannot define virtual methods as templates. There are other ways to deal
// with it such as providing the implementor class as a template argument. But
// that doesn't look worth the pain as compared to simply taking a pointer to
// state variable.
class CEPStateIf
{
public:

// Application should provide an interface to the state variables
// State variable's identity is in the form of a vector of integers
// The pointer is internally typecast to respective register type
// The application has to assure that the address of the register remains the same.
//
    virtual void* getStatePtr(vector<int>&)=0;
    virtual Etyp getStateTyp(vector<int>&)=0;
    virtual void quit()=0;
};

#endif
