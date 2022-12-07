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
    vector<Action*> _tacts;
    vector<Action*> _facts;
    vector<ExprBase*> _aggrHandlers;
    vector<ExprBase*> _statevars;
    unsigned long _seqstart = 0;
    unsigned long _seqend = 0;
    // Common handlers for all interval types, in turn call subtype's handler
    void handleStart(Event e, unsigned long eseqno)
    {
        _seqstart = eseqno;
        handleStart_(e);
    }
    void handleEnd(Event e, unsigned long eseqno)
    {
        _seqend = eseqno;
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
    string seqnostr()
    {
        return "(" + to_string(_seqstart) + "," + to_string(_seqend) + ")";
    }
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
        bool loginterval = condeval and _tacts.size() != 0 or not condeval and _facts.size() != 0;
        if ( condeval ) for(auto ta:_tacts) ta->act();
        else for(auto fa:_facts) fa->act();
        if ( loginterval )
        {
            _ceplog << "Endof " << intervalstr() << " Seqno " << seqnostr() << " " << _condexpr->str() << " = " << condeval << endl;
            for(auto aggr:_aggrHandlers)
                _ceplog << "\t" << aggr->str() << " = " << aggr->eval2str() << endl;
            for(auto sv:_statevars)
                _ceplog << "\t" << sv->str() << " = " << sv->eval2str() << endl;
        }
    }
public:
    void init() { init_(); }
    Interval(ofstream& ceplog, Event start, Event end, BoolExpr* condexpr, vector<Action*> tacts, vector<Action*> facts, EventRouter& router) :
        _ceplog(ceplog), _start(start), _end(end), _condexpr(condexpr), _tacts(tacts), _facts(facts),
        _startHandler(router, start, [this](Event e, unsigned long eseqno){this->handleStart(e,eseqno);}),
        _endHandler(router, end, [this](Event e, unsigned long eseqno){this->handleEnd(e, eseqno);})
    {
        _condexpr->aggregators( _aggrHandlers );
        _condexpr->statevars( _statevars );
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

#define NEWINTERVAL(TYP) (Interval*) new TYP(_ceplog, estrt, eend, condexpr, tacts, facts, _router)

class IntervalManager
{
    CEPStateIf *_stateif;
    ofstream _ceplog;
    EventRouter _router;
    ExprFactory _efactory = {_router, _stateif, _ceplog};
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
        auto tactslist = topargs[2];
        auto factslist = topargs[3];
        auto condexpr = (BoolExpr*) _efactory.pterm2expr(exprterm);

        vector<Action*> tacts, facts;
        for(auto t:tactslist->args()) tacts.push_back( _efactory.pterm2action(t) );
        for(auto t:factslist->args()) facts.push_back( _efactory.pterm2action(t) );

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
    void route(Event e, unsigned long eseqno) { _router.route(e, eseqno); }
    IntervalManager(string basename, CEPStateIf* stateif) : _stateif(stateif)
    {
        string ceplogflnm = basename + ".cep.log";
        string cepdatflnm = basename + ".cep";
        _ceplog.open(ceplogflnm);
        PDb pdb;
        t_predspec cep4 = {"cep",4};
        pdb.load(cepdatflnm, {cep4} );
        auto specs = pdb.get(cep4);
        for(auto spec:specs) processSpec(spec);
    }
    ~IntervalManager() { for(auto i:_interv) delete i; }
};

#endif
