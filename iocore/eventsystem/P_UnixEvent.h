#ifndef _P_UnixEvent_h_
#define _P_UnixEvent_h_

TS_INLINE Event *
Event::init(Continuation * c, ink_hrtime atimeout_at, ink_hrtime aperiod) {
  continuation = c;
  timeout_at = atimeout_at;
  period = aperiod;
  immediate = !period && !atimeout_at;
  cancelled = false;
  return this;
}

TS_INLINE void
Event::free() {
  mutex = NULL;
  eventAllocator.free(this);
}

TS_INLINE
Event::Event(): ethread(0),
                in_the_prot_queue(false),
                in_the_priority_queue(false),
                immediate(false),
                globally_allocated(true),
                in_heap(false),
                timeout_at(0),
                period(0) {
}

#endif /*_UnixEvent_h_*/
