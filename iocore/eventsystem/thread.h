/** @file

  Thread

  @section details Details

  Thread class provides the basic functionality for threads. Typically,
  there will be additional derived classes. Having a common base class
  for all threads is useful in many cases. I discuss below the use of
  Threads in the context of Event Subsystem. Hopefully this would be
  typical of other situations.

  EventProcessor needs to create a bunch of threads. It declares a
  class called EThread, derived from Thread. It is the responsibility of
  the EventProcessor to create and manage all the threads needed in the
  Event Subsystem (Note: we have removed the original ThreadManager class
  which used to create and manage *all* the threads in the system). By
  monitoring, we mean checking the heartbeat of each thread and the
  number of threads in the system etc.

  A derived class should either provide the function (and arguments)
  needed by the Thread class (see start()), or should define the virtual
  function execute().

  The Thread class maintains a thread_key which registers *all*
  the threads in the system (that have been created using Thread or
  a derived class), using thread specific data calls.  Whenever, you
  call this_thread() you get a pointer to the Thread that is currently
  executing you.  Additionally, the EThread class (derived from Thread)
  maintains its own independent key. All (and only) the threads created
  in the Event Subsystem are registered with this key. Thus, whenever you
  call this_ethread() you get a pointer to EThread. If you happen to call
  this_ethread() from inside a thread which is not an EThread, you will
  get a NULL value (since that thread will not be  registered with the
  EThread key). This will hopefully make the use of this_ethread() safer.
  Note that an event created with EThread can also call this_thread(),
  in which case, it will get a pointer to Thread (rather than to EThread).
 */

#ifndef _I_Thread_h
#define _I_Thread_h

#include <memory>
#include <mutex>
#include <thread>

#include "./headers.h"

class Thread;

#define THREADAPI
#define THREADAPI_RETURN_TYPE void*
typedef THREADAPI_RETURN_TYPE(THREADAPI * ThreadFunction) (void *arg);

extern std::mutex* global_mutex;

static const int MAX_THREAD_NAME_LENGTH  = 16;
static const int DEFAULT_STACKSIZE = 1048576; // 1MB


/**
  Base class for the threads in the Event System. Thread is the base
  class for all the thread classes in the Event System. Objects of the
  Thread class represent spawned or running threads and provide minimal
  information for its derived classes. Thread objects have a reference
  to a ProxyMutex, that is used for atomic operations internally, and
  an ink_thread member that is used to identify the thread in the system.

  You should not create an object of the Thread class, they are typically
  instantiated after some thread startup mechanism exposed by a processor,
  but even then you would probably deal with processor functions and
  not the Thread object itself.
*/
class Thread : std::thread {
public:
  Thread();
  virtual ~Thread();

  ink_thread start(const char* name,
                   size_t stacksize=DEFAULT_STACKSIZE,
                   ThreadFunction f=NULL,
                   void *a=NULL);

  virtual void execute() {
  }

  static ink_hrtime get_based_hrtime() {
      return Thread::cur_time;
  }

  static ink_hrtime get_hrtime() {
      return Thread::cur_time;
  }

  void set_specific();
  Thread* this_thread() const;

  // prevent unauthorized copies
  Thread(const Thread &)=delete;
  Thread& operator=(const Thread &)=delete;

public:
  /**
    System-wide thread identifier. The thread identifier is represented
    by the platform independent type ink_thread and it is the system-wide
    value assigned to each thread. It is exposed as a convenience for
    processors and you should not modify it directly.
  */
  ink_thread tid;

  /**
    Thread lock to ensure atomic operations. The thread lock available
    to derived classes to ensure atomic operations and protect critical
    regions. Do not modify this member directly.
  */
  std::mutex* p_mutex;

  static ink_hrtime cur_time;
  static thread_local Thread* s_thread_data_key;
private:
  std::shared_ptr<std::mutex> mutex_ptr;
};

#endif /*_I_Thread_h*/
