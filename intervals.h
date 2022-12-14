#ifndef _INTERVALS_H
#define _INTERVALS_H

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "expr.h"
#include "xsb2cpp.h"

class Interval
{
    ofstream& _ceplog;
    EventRouter& _router;
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
protected:
    PTerm *_ispec;
    EventHandler *_startHandler = NULL;
    EventHandler *_endHandler = NULL;
    vector<PTerm*>& iargs() { return _ispec->args()[0]->args(); }
    void setStartHandler(Event e)
    {
        _startHandler = new EventHandler(_router, e, [this](Event e, unsigned long eseqno){this->handleStart(e,eseqno);});
    }
    void setEndHandler(Event e)
    {
        _endHandler = new EventHandler(_router, e, [this](Event e, unsigned long eseqno){this->handleEnd(e, eseqno);});
    }
    virtual void init_() { _startHandler->start(); }
    virtual void handleStart_(Event e)
    {
        _startHandler->stop();
        startInterval();
    }
    virtual void handleEnd_(Event) = 0;
    string seqnostr()
    {
        return "(" + to_string(_seqstart) + "," + to_string(_seqend) + ")";
    }
    virtual string intervalstr()=0;
    void startAggrs() { for(auto ah:_aggrHandlers) ah->start(); }
    void stopAggrs() { for(auto ah:_aggrHandlers) ah->stop(); }
    void resetAggrs() { for(auto ah:_aggrHandlers) ah->reset(); }
    void startInterval()
    {
        _endHandler->start();
        startAggrs();
        resetAggrs();
    }
    void stopInterval()
    {
        _endHandler->stop();
        stopAggrs();
    }
    virtual void setIntervalDetls()=0;
public:
    void init()
    {
        setIntervalDetls();
        init_();
    }
    Interval(PTerm* ispec, ofstream& ceplog, EventRouter& router, ExprFactory& efactory) :
        _ispec(ispec), _ceplog(ceplog), _router(router)
    {
        auto topargs = _ispec->args();

        auto condterm = topargs[1];
        auto _condexpr = (BoolExpr*) efactory.pterm2expr(condterm);
        if ( _condexpr->etyp() != bool__ )
        {
            cout << "Interval condition must be a boolean expression" << endl;
            exit(1);
        }

        _condexpr->aggregators( _aggrHandlers );
        _condexpr->statevars( _statevars );

        auto tactslist = topargs[2];
        for(auto t:tactslist->args()) _tacts.push_back( efactory.pterm2action(t) );

        auto factslist = topargs[3];
        for(auto t:factslist->args()) _facts.push_back( efactory.pterm2action(t) );
    }
    virtual ~Interval()
    {
        if ( _startHandler ) delete _startHandler;
        if ( _endHandler ) delete _endHandler;
    }
};

// Guidelines for writing Interval subclasses
// - 4 handlers are needed: setIntervalDetls, init_, handleStart_, handleEnd_
// setIntervalDetls mainly sets event handlers, but it may do more custom
// things as more interval types get added.
// - Common actions are in the base class, code only the remaining actions
// - In each one of them check the following:
//   - Any change in state of _startHandler
//   - Any change in state of _endHandler
//   - Any change in state of _aggrHandlers
//   - Any change in data state of aggregators (reset needed?)
// Now tidy it up all:
//   - If there is similar handler for several classes lift it into base
//   - If some action sequence repeats across handlers, make it a support function in base
class Interval_itill : public Interval
{
using Interval::Interval;
    Event _e;
protected:
    string intervalstr() { return "itill(" + to_string(_e) + ")"; }
    void setIntervalDetls()
    {
        auto eterm = iargs()[0];
        _e = eterm->asInt();
        setEndHandler(_e);
    }
    void init_() { startInterval(); };
    void handleEnd_(Event e) { stopInterval(); }
};

class Interval_iself : public Interval
{
using Interval::Interval;
    Event _e;
protected:
    string intervalstr() { return "iself(" + to_string(_e) + ")"; }
    void setIntervalDetls()
    {
        auto eterm = iargs()[0];
        _e = eterm->asInt();
        setStartHandler(_e);
        setEndHandler(_e);
    }
    void handleEnd_(Event e) { resetAggrs(); }
};

class Interval_iab : public Interval
{
using Interval::Interval;
    Event _a, _b;
protected:
    string intervalstr() { return "iab(" + to_string(_a) + "," + to_string(_b) + ")"; }
    void setIntervalDetls()
    {
        auto eargs = iargs();
        _a = eargs[0]->asInt();
        _b = eargs[1]->asInt();
        setStartHandler(_a);
        setEndHandler(_b);
    }
    void handleEnd_(Event e) { _startHandler->start(); stopInterval(); }
};

#define INTERVALMAP(TYP) { #TYP, [this] ( PTerm* ispec ) { return new Interval_##TYP(ispec, _ceplog, _router, _efactory); } }
class IntervalManager
{
    CEPStateIf *_stateif;
    ofstream _ceplog;
    EventRouter _router;
    ExprFactory _efactory = {_router, _stateif, _ceplog};
    vector<Interval*> _interv;
    map<string,function<Interval*(PTerm*)>> _intervalf =
    {
        INTERVALMAP(itill),
        INTERVALMAP(iself),
        INTERVALMAP(iab),
    };

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
        for(auto spec:specs)
        {
            auto ityp = spec->args()[0]->functor();
            auto it = _intervalf.find( ityp );
            if ( it == _intervalf.end() )
            {
                cout << "Unknown interval type " << ityp << endl;
                exit(1);
            }
            auto i = (Interval*) it->second(spec);
            i->init();
            _interv.push_back(i);
        }
    }
    ~IntervalManager() { for(auto i:_interv) delete i; }
};

#endif
