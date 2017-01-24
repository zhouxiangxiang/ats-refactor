#ifndef _I_Continuation_h_
#define _I_Continuation_h_

#include <mutex>
#include <list>

class Continuation;

// Constants and Type Definitions
#define CONTINUATION_EVENT_NONE      0
#define CONTINUATION_DONE            0
#define CONTINUATION_CONT            1

typedef int (Continuation::*ContinuationHandler) (int event, void *data);

class force_VFPT_to_top {
public:
  virtual ~force_VFPT_to_top() { }
};

/**
  Base class for all state machines to receive notification of events.

  The Continuation class represents the main abstraction mechanism
  used throughout the IO Core Event System to communicate its users
  the occurrence of an event. A Continuation is a lightweight data
  structure that implements a single method with which the user is
  called back.

  Continuations are typically subclassed in order to implement
  event-driven state machines. By including additional state and
  methods, continuations can combine state with control flow, and
  they are generally used to support split-phase, event-driven
  control flow.

  Given the multithreaded nature of the Event System, every
  continuation carries a reference to a ProxyMutex object to protect
  its state and ensure atomic operations. This ProxyMutex object
  must be allocated by continuation-derived classes or by clients
  of the IO Core Event System and it is required as a parameter to
  the Continuation's class constructor.

*/

class Continuation: private force_VFPT_to_top {
public:

  /**
    The current continuation handler function.

    The current handler should not be set directly. In order to
    change it, first aquire the Continuation's lock and then use
    the SET_HANDLER macro which takes care of the type casting
    issues.
  */
  ContinuationHandler handler;

  /**
    The Contination's lock.

    A reference counted pointer to the Continuation's lock. This
    lock is initialized in the constructor and should not be set
    directly.

  */
  std::mutex* mutex;

  /**
    Link to other continuations.

    A doubly-linked element to allow Lists of Continuations to be
    assembled.

  */
  std::list<Continuation> link;

#ifdef DEBUG
  const char *handler_name;
#endif

  /**
    Receives the event code and data for an Event.

    This function receives the event code and data for an event and
    forwards them to the current continuation handler. The processor
    calling back the continuation is responsible for acquiring its
    lock.

    @param event Event code to be passed at callback (Processor specific).
    @param data General purpose data related to the event code (Processor specific).
    @return State machine and processor specific return code.

  */
  int handleEvent(int event = CONTINUATION_EVENT_NONE, void *data = 0) {
    return (this->*handler) (event, data);
  }

  /**
    Contructor of the Continuation object. It should not be used
    directly. Instead create an object of a derived type.

    @param amutex Lock to be set for this Continuation.

  */
  Continuation(std::mutex* amutex = NULL);
};

/**
  Sets the Continuation's handler. The preferred mechanism for
  setting the Continuation's handler.

  @param _h Pointer to the function used to callback with events.

*/
#ifdef DEBUG
#  define SET_HANDLER(_h) \
     (handler = ((ContinuationHandler)_h),handler_name = #_h)
#else
#  define SET_HANDLER(_h) (handler = ((ContinuationHandler)_h))
#endif

/**
  Sets a Continuation's handler.

  The preferred mechanism for setting the Continuation's handler.

  @param _c Pointer to a Continuation whose handler is being set.
  @param _h Pointer to the function used to callback with events.

*/
#ifdef DEBUG
#  define SET_CONTINUATION_HANDLER(_c,_h) \
     (_c->handler = ((ContinuationHandler) _h),_c->handler_name = #_h)
#else
#  define SET_CONTINUATION_HANDLER(_c,_h) \
     (_c->handler = ((ContinuationHandler) _h))
#endif

inline
Continuation::Continuation(std::mutex* amutex): handler(NULL), mutex(amutex) {
#ifdef DEBUG
    handler_name(NULL),
#endif
}

#endif /*_Continuation_h_*/
