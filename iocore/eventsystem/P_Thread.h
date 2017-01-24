/**
  Basic Threads
 */
#ifndef _P_Thread_h_
#define _P_Thread_h_

#include "I_Thread.h"

// Common Interface impl
TS_INLINE
Thread::~ Thread() {
}

TS_INLINE void
Thread::set_specific() {
  ink_thread_setspecific(Thread::thread_data_key, this);
}

TS_INLINE Thread *
this_thread() {
  return (Thread *) ink_thread_getspecific(Thread::thread_data_key);
}

TS_INLINE ink_hrtime
ink_get_hrtime() {
  return Thread::cur_time;
}

TS_INLINE ink_hrtime
ink_get_based_hrtime() {
  return Thread::cur_time;
}

#endif //_P_Thread_h_
