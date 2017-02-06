/** @file
  A brief file description
 */

/**

  Processor.cc

  Processor objects process requests which are placed in the Processor's
  input queue.  A Processor can contain multiple threads to process
  requests in the queue.  Requests in the queue are Continuations, which
  describe functions to run, and what to do when the function is complete
  (if anything).

  Basically, Processors should be viewed as multi-threaded schedulers which
  process request Continuations from their queue.  Requests can be made of
  a Processor either by directly adding a request Continuation to the queue,
  or more conveniently, by calling a method service call which synthesizes
  the appropriate request Continuation and places it in the queue.
 */

#include "P_EventSystem.h"
//      Processor::Processor()
//
//      Constructor for a Processor.
Processor::Processor() {
}


//      Processor::~Processor()
//
//      Destructor for a Processor.
Processor::~Processor() {
}

//  Processor::create_thread()
Thread *
Processor::create_thread(int /* thread_index */) {
  ink_release_assert(!"Processor::create_thread -- no default implementation");
  return ((Thread *) 0);
}

//  Processor::get_thread_count()
int
Processor::get_thread_count() {
  return (0);
}
