#ifndef _EVENTHANDLER_H
#define _EVENTHANDLER_H

#include <functional>
#include <map>
#include <set>

typedef int Event; // TODO make it unsigned long?

class EventHandlerBase
{
public:
    virtual void handleEvent(Event, unsigned long)=0;
};

class EventRouter
{
    map<Event,set<EventHandlerBase*>> _route;
public:
    void route(Event e, unsigned long eseqno)
    {
        auto it = _route.find(e);
        if ( it == _route.end() ) return;

        // Important to work on a copy, as the handlers may mutate the set
        set<EventHandlerBase*> sendto = it->second;
        for( auto h:sendto ) h->handleEvent(e, eseqno);
    }
    void registerH(Event e, EventHandlerBase* h) { _route[e].insert(h); }
    void unregisterH(Event e, EventHandlerBase* h) { _route[e].erase(h); }
};

// To avoid interpreting event again, it is better if the users create separate
// handlers for each event. handleEvent's parameter should be preferably
// unused.
// NOTE: Event handler can be used in 2 ways. 1. Containment. Supply a handler
// function, which is an optional argument of the constructor OR 2.
// Inheritance: implement the handleEvent method.
class EventHandler : public EventHandlerBase
{
    EventRouter& _router;
    Event _e;
    function<void(Event,unsigned long)> _handler;
public:
    void start() { _router.registerH(_e, this); }
    void stop() { _router.unregisterH(_e, this); }
    virtual void handleEvent(Event e, unsigned long eseqno) { _handler(e, eseqno); }
    EventHandler(EventRouter& router, Event e, function<void(Event, unsigned long)> handler = NULL) :
        _router(router), _e(e), _handler(handler) {}
};

#endif
