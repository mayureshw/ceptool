#ifndef _EXPR_H
#define _EXPR_H

#include <vector>
#include <functional>
#include "xsb2cpp.h"
#include "eventhandler.h"
#include "exprf.h"
#include "stateif.h"

// Note: Avoid run-time interpretation of etyp. This is only for
// construction-time convenience
//
// Note: Expr classes whose template signature is derived from their arg types
// do not require arity validation. Rest of the classes should validate arity
// in their constructor.
#define TYP2ETYP(TYP) if constexpr ( is_same<T,TYP>::value ) return TYP##__

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


////////// Expression classes //////////
class ExprBase
{
protected:
    vector<ExprBase*> _args;
public:
    // An attempt to use PTerm::tostr was made, though it runs into strange
    // memory related issues, if XSB side is involved it will be hard to diagnose
    virtual string str() = 0;
    // Aggregator methods. They are here because aggregator is template
    // Giving a non-template aggregator base class results in complex MI pattern, not worth it
    virtual bool isAggregator() { return false; }
    virtual bool isStatevar() { return false; }
    virtual void reset() {}
    virtual void start() {}
    virtual void stop() {}

    virtual string eval2str() = 0;
    virtual Etyp etyp() = 0;
    void aggregators(vector<ExprBase*>& aggrv)
    {
        if ( isAggregator() ) aggrv.push_back(this);
        else for(auto arg:_args) arg->aggregators(aggrv);
    }
    void statevars(vector<ExprBase*>& statev)
    {
        if ( isStatevar() ) statev.push_back(this);
        else for(auto arg:_args) arg->statevars(statev);
    }
    ExprBase(vector<ExprBase*> args = {}) : _args(args) {}
};

// NOTE: Wherever output type has to be decided basis input argument types,
// consider using "using"
template <typename T> class Expr : public ExprBase
{
using ExprBase::ExprBase;
public:
    virtual T eval() = 0;
    string eval2str()
    {
        if constexpr ( is_same<T,string>::value ) return eval();
        else return to_string(eval());
    }
    Etyp etyp() { TYPES2ETYP }
};

template <typename T> class Aggregator : public Expr<T>
{
    EventHandler _handler;
public:
    bool isAggregator() { return true; }
    virtual void handleEvent(Event e, unsigned long) = 0;
    void start() { _handler.start(); }
    void stop() { _handler.stop(); }
    Aggregator(EventRouter& router, Event e) :
        _handler(router, e, [this](Event e, unsigned long eseqno) {this->handleEvent(e,eseqno);}) {}
};

#define ARG(TYP,INDX) ((Expr<TYP>*)_args[INDX])
#define EVALAS(TYP,INDX) ARG(TYP,INDX)->eval()
#define BINOPEXPR(T1,T2,OP) EVALAS(T1,0) OP EVALAS(T2,1)
#define BINOPSTR(T1,T2,OP) "( " + ARG(T1,0)->str() + OP + ARG(T2,1)->str() + " )"

template <typename T1, typename T2> class CEPXor : public Expr<bool>
{
using Expr<bool>::Expr;
public:
    string str() { return BINOPSTR(T1,T2," ^ "); }
    bool eval() { return BINOPEXPR(T1,T2,^); }
};

template <typename T1, typename T2> class CEPEq : public Expr<bool>
{
using Expr<bool>::Expr;
public:
    string str() { return BINOPSTR(T1,T2," == "); }
    bool eval() { return BINOPEXPR(T1,T2,==); }
};

template <typename T> class Const : public Expr<T>
{
    T _val;
public:
    string str()
    {
        if constexpr ( is_same<T, string> :: value ) return _val;
        else return to_string(_val);
    }
    T eval() { return _val; }
    Const(T val) : _val(val) {}
};

#define ARGEVENT (Event)((Const<int>*) argv[0])->eval()
class CEPCount : public Aggregator<int> // TODO: <unsigned long>
{
    //unsigned long _count = 0; // TODO
    int _count = 0;
    Event _e;
public:
    string str() { return "#" + to_string(_e); }
    void handleEvent(Event,unsigned long) { _count++; }
    void reset() { _count = 0; }
    //unsigned long eval() { return _count; } // TODO
    int eval() { return _count; }
    CEPCount(EventRouter& router, vector<ExprBase*> argv) : Aggregator<int>(router, ARGEVENT), _e(ARGEVENT) {}
};

template <typename T> class CEPReg : public Expr<T>
{
    T *_stateptr;
public:
    virtual bool isStatevar() { return true; }
    string str() { return "*(" + this->_args[0]->str() + "," + this->_args[1]->str() + ")"; }
    T eval() { return *_stateptr; }
    CEPReg(CEPStateIf *stateif, vector<ExprBase*> args) : Expr<T>(args)
    {
        if ( args.size() != 2 )
        {
            cout << "Operator * has arity 2, got:" << args.size() << endl;
            exit(1);
        }
        vector<int> stateidv;
        for(auto a:args) stateidv.push_back( ((Expr<int>*)a)->eval() );
        _stateptr = (T*) stateif->getStatePtr(stateidv);
    }
};

////////// Action classes //////////

class Action
{
protected:
    ofstream& _ceplog;
    EventRouter& _router;
public:
    virtual void operator () () = 0;
    Action(ofstream& ceplog, EventRouter& router) : _ceplog(ceplog), _router(router) {}
};

class LogAction : public Action
{
    string _msg;
public:
    void operator () ()
    {
    }
    LogAction(ofstream& ceplog, EventRouter& router, vector<PTerm*>& logargs) : Action(ceplog,router)
    {
        if ( logargs.size() != 1 )
        {
            cout << "log expects 1 argument, got " << logargs.size() << endl;
            exit(1);
        }
        _msg = logargs[0]->asString();
    }
};

class EventAction : public Action
{
    Event _e;
public:
    void operator () ()
    {
    }
    EventAction(ofstream& ceplog, EventRouter& router, vector<PTerm*>& eventargs) : Action(ceplog,router)
    {
    }
};

#define PTERM2ATOM(TYP) new Const<TYP>(AS(TYP,pterm))

#define ACTIONMAP(FUNCTOR,CLASS) { #FUNCTOR , [this](vector<PTerm*>& v) { return new CLASS(_ceplog,_router,v); } }

// TODO manager pointers, may be store and delete them in the end
class ExprFactory
{
    ofstream& _ceplog;
    CEPStateIf *_stateif;
    EventRouter& _router;
    vector<ExprBase*> _exprv;
    vector<Action*> _actionv;
    const map< pair<string,vector<Etyp>>, function< ExprBase*(vector<ExprBase*>&) > > _targmap = TARGMAP;
    void inferTemplateTyps(string functor, vector<ExprBase*>& argv, vector<Etyp>& argtpv)
    {
        // For CEPReg template argument depends on the type of state variable
        if ( functor == "*" )
        {
            vector<int> stateidv;
            for(auto a:argv) stateidv.push_back( ((Expr<int>*)a)->eval() );
            argtpv.push_back( _stateif->getStateTyp(stateidv) );
        }

        // In most cases template args are simply the types of arguments, hence the default
        else for(auto arg:argv) argtpv.push_back(arg->etyp());
    }
    ExprBase* _pterm2expr(PTerm* pterm)
    {
        if ( pterm->isAtom() )
        {
            if ( pterm->isInt() ) return PTERM2ATOM(int);
            else if ( pterm->isFloat() ) return PTERM2ATOM(float);
            else if ( pterm->isString() ) return PTERM2ATOM(string);
            else
            {
                cout << "Unknown atom type in pterm2expr" << endl;
                exit(1);
            }
        }
        else if ( pterm->isList() )
        {
            cout << "Lists currently unhandled" << endl;
            exit(1);
        }
        else
        {
            auto functor = pterm->functor();
            auto args = pterm->args();
            vector<ExprBase*> argv;
            for(auto arg:args) argv.push_back(pterm2expr(arg));
            vector<Etyp> argtpv;
            inferTemplateTyps(functor, argv, argtpv);
            auto it = _targmap.find( {functor, argtpv} );
            if ( it == _targmap.end() )
            {
                cout << "No method to convert functor: " << functor << " argtyps: ";
                for(auto typ:argtpv) cout << typ << " ";
                cout << endl;
                exit(1);
            }
            else return it->second(argv);

        }
    }
    map<string,function<Action*(vector<PTerm*>&)>> _actionf =
        {
            ACTIONMAP(log,   LogAction),
            ACTIONMAP(event, EventAction),
        };
public:
    ExprBase* pterm2expr(PTerm* pterm)
    {
        auto expr = _pterm2expr(pterm);
        _exprv.push_back(expr);
        return expr;
    }
    Action* pterm2action(PTerm* pterm)
    {
        auto functor = pterm->functor();
        auto it = _actionf.find(functor);
        if ( it == _actionf.end() )
        {
            cout << "Unknown action type: " << functor << endl;
            exit(1);
        }
        auto action = it->second(pterm->args());
        _actionv.push_back(action);
        return action;
    }
    ExprFactory(EventRouter& router, CEPStateIf *stateif, ofstream& ceplog) : _router(router), _stateif(stateif), _ceplog(ceplog) {}
    ~ExprFactory()
    {
        for(auto e:_exprv) delete e;
        for(auto a:_actionv) delete a;
    }
};

typedef Expr<bool> BoolExpr;


#endif
