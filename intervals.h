#ifndef _INTERVALS_H
#define _INTERVALS_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "expr.h"
#include "xsb2cpp.h"

typedef enum { START = INT8_MIN } SymEvent;
#define SYME(E) { #E, E }


class Interval
{
    ofstream& _ceplog;
    Event _start, _end;
    BoolExpr *_condexpr;
    vector<VoidExpr*> _actions;
    vector<ExprBase*> _aggrHandlers;
    // Common handlers for all interval types, in turn call subtype's handler
    void handleStart(Event e) { handleStart_(e); }
    void handleEnd(Event e)
    {
        doActions();
        handleEnd_(e);
    }
protected:
    EventHandler _startHandler, _endHandler;
    virtual void init_() { _startHandler.start(); }
    virtual void handleStart_(Event e)
    {
        _startHandler.stop();
        startInterval();
    }
    virtual void handleEnd_(Event) = 0;
    string intervalstr()
    { return "(" + (_start == START ? "START" : to_string(_start)) + "," + to_string(_end) + ")"; }
    void startAggrs() { for(auto ah:_aggrHandlers) ah->start(); }
    void stopAggrs() { for(auto ah:_aggrHandlers) ah->stop(); }
    void resetAggrs() { for(auto ah:_aggrHandlers) ah->reset(); }
    void startInterval()
    {
        _endHandler.start();
        startAggrs();
        resetAggrs();
    }
    void stopInterval()
    {
        _endHandler.stop();
        stopAggrs();
    }
    void doActions()
    {
        bool condeval = _condexpr->eval();
        if ( condeval ) return;
        _ceplog << "Endof " << intervalstr() << " " << _condexpr->str() << " = " << condeval << endl;
        for(auto aggr:_aggrHandlers)
            _ceplog << "\t" << aggr->str() << " = " << aggr->eval2str() << endl;
    }
public:
    void init() { init_(); }
    Interval(ofstream& ceplog, Event start, Event end, BoolExpr* condexpr, EventRouter& router) :
        _ceplog(ceplog), _start(start), _end(end), _condexpr(condexpr),
        _startHandler(router, start, [this](Event e){this->handleStart(e);}),
        _endHandler(router, end, [this](Event e){this->handleEnd(e);})
    {
        _condexpr->aggregators( _aggrHandlers );
    }
};

// Guidelines for writing Interval subclasses
// - 3 handlers are needed: init_, handleStart_, handleEnd_
// - Common actions are in the base class, code only the remaining actions
// - In each one of them check the following:
//   - Any change in state of _startHandler
//   - Any change in state of _endHandler
//   - Any change in state of _aggrHandlers
//   - Any change in data state of aggregators (reset needed?)
// Now tidy it up all:
//   - If there is similar handler for several classes lift it into base
//   - If some action sequence repeats across handlers, make it a support function in base
class AutostartInterval : public Interval
{
using Interval::Interval;
protected:
    void init_() { startInterval(); };
    void handleEnd_(Event e) { stopInterval(); }
};

class SelfInterval : public Interval
{
using Interval::Interval;
protected:
    void handleEnd_(Event e) { resetAggrs(); }
};

class OtherInterval : public Interval
{
using Interval::Interval;
protected:
    void handleEnd_(Event e) { _startHandler.start(); stopInterval(); }
};

#define NEWINTERVAL(TYP) (Interval*) new TYP(_ceplog, estrt, eend, condexpr, _router)

class IntervalManager
{
    ofstream _ceplog;
    EventRouter _router;
    ExprFactory _efactory = {_router};
    vector<Interval*> _interv;
    const map<string,SymEvent> _symevents =
        {
            SYME(START),
        };
    int term2event(PTerm *pterm)
    {
        if ( pterm->isInt() ) return AS(int, pterm);
        else if ( pterm->isString() )
        {
            auto estr = AS(string, pterm);
            auto it = _symevents.find(estr);
            if ( it == _symevents.end() )
            {
                cout << "Invalid symbolic event string " << estr << endl;
                exit(1);
            }
            else
            {
                return it->second;
            }
        }
        else
        {
            cout << "Event can be string or int " << pterm->tostr() << endl;
            exit(1);
        }
    }

    void processSpec(PTerm* spec)
    {
        auto topargs = spec->args();
        auto istrt = topargs[0]->args()[0];
        auto iend = topargs[0]->args()[1];
        auto exprterm = topargs[1];
        auto condexpr = (BoolExpr*) _efactory.pterm2expr(exprterm);

        int estrt = term2event(istrt);
        int eend = term2event(iend);
        if ( condexpr->etyp() != bool__ )
        {
            cout << "Interval condition must be a boolean expression" << endl;
            exit(1);
        }
        auto i = estrt == START ? NEWINTERVAL(AutostartInterval) :
            estrt == eend ? NEWINTERVAL(SelfInterval) : NEWINTERVAL(OtherInterval);
        i->init();
        _interv.push_back(i);
    }
public:
    void route(Event e) { _router.route(e); }
    IntervalManager(string cepdatflnm, string ceplogflnm = "cep.log")
    {
        _ceplog.open(ceplogflnm);
        PDb pdb;
        t_predspec cep2 = {"cep",2};
        pdb.load(cepdatflnm, {cep2} );
        auto specs = pdb.get(cep2);
        for(auto spec:specs) processSpec(spec);
    }
    ~IntervalManager() { for(auto i:_interv) delete i; }
};

#endif
