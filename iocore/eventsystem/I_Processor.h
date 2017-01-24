#ifndef _I_Processor_h_
#define _I_Processor_h_

// #include "libts.h"
#include <cstddef>

#define PROCESSOR_RECONFIGURE            0x01
#define PROCESSOR_CHECK                  0x02
#define PROCESSOR_FIX                    0x04
#define PROCESSOR_IGNORE_ERRORS          0x08

#ifndef DEFAULT_STACKSIZE
#define DEFAULT_STACKSIZE               1024
#endif

class Processor;
class Thread;

/**
   Base class for all of the IO Core processors.

   The Processor class defines a common interface for all the processors in
   the IO Core. A processor is multithreaded subsystem specialized in some
   type of task or application. For example, the Event System module includes
   the EventProcessor which provides schedulling services, the Net module
   includes the NetProcessor which provides networking services, etc.

   You cannot create objects of the Processor class and its methods have no
   implementation. Therefore, you are expected to use objects of a derived type.

   Most of such derived classes, provide a singleton object and is common case
   to have a single instance in that application scope.
*/
class Processor
{
public:
  virtual ~Processor();

  /**
    Returns a Thread appropriate for the processor.

    Returns a new instance of a Thread or Thread derived class of a thread
    which is the thread class for the processor.

    @param thread_index reserved for future use.
  */
  virtual Thread *create_thread(int thread_index);

  /**
    Returns the number of threads required for this processor. If the number
    is not defined or not used, it is equal to 0.
  */
  virtual int get_thread_count();

  /**
    This function attemps to stop the processor. Please refer to the
    documentation on each processor to determine if it is supported.
  */
  virtual void shutdown()
  {
  }

  /**
    Starts execution of the processor.

    Attempts to start the number of threads specified for the
    processor, initializes their states and sets them running. On
    failure it returns a negative value.

    @param number_of_threads Positive value indicating the number of threads
           to spawn for the processor.
    @param stacksize The thread stack size to use for this processor.

  */
  virtual int start(int number_of_threads,
                    std::size_t stacksize=DEFAULT_STACKSIZE) {
    (void) number_of_threads;
    (void) stacksize;
    return 0;
  }

protected:
  Processor();

private:
  // prevent unauthorized copies (Not implemented)
  Processor(const Processor &);
  Processor & operator =(const Processor &);
};

#endif //_I_Processor_h_
