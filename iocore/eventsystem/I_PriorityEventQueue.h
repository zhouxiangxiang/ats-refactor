/** @file
  Queue of Events sorted by the "at_timeout" field
 */

#ifndef _I_PriorityEventQueue_h_
#define _I_PriorityEventQueue_h_

#include "libts.h"
#include "I_Event.h"


// <5ms, 10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120
#define N_PQ_LIST        10
#define PQ_BUCKET_TIME(_i) (HRTIME_MSECONDS(5) << (_i))

class EThread;

struct PriorityEventQueue
{

  Que(Event, link) after[N_PQ_LIST];
  ink_hrtime last_check_time;
  uint32_t last_check_buckets;

  void enqueue(Event * e, ink_hrtime now)
  {
    ink_hrtime t = e->timeout_at - now;
    int i = 0;
    // equivalent but faster
    if (t <= PQ_BUCKET_TIME(3))
    {
      if (t <= PQ_BUCKET_TIME(1)) {
        if (t <= PQ_BUCKET_TIME(0)) {
          i = 0;
        } else
        {
          i = 1;
        }
      } else {
        if (t <= PQ_BUCKET_TIME(2)) {
          i = 2;
        } else {
          i = 3;
        }
      }
    } else {
      if (t <= PQ_BUCKET_TIME(7)) {
        if (t <= PQ_BUCKET_TIME(5)) {
          if (t <= PQ_BUCKET_TIME(4)) {
            i = 4;
          } else {
            i = 5;
          }
        } else {
          if (t <= PQ_BUCKET_TIME(6)) {
            i = 6;
          } else {
            i = 7;
          }
        }
      } else {
        if (t <= PQ_BUCKET_TIME(8)) {
          i = 8;
        } else {
          i = 9;
        }
      }
    }
    e->in_the_priority_queue = 1;
    e->in_heap = i;
    after[i].enqueue(e);
  }

  void remove(Event * e)
  {
    ink_assert(e->in_the_priority_queue);
    e->in_the_priority_queue = 0;
    after[e->in_heap].remove(e);
  }

  Event *dequeue_ready(ink_hrtime t)
  {
    (void) t;
    Event *e = after[0].dequeue();
    if (e) {
      ink_assert(e->in_the_priority_queue);
      e->in_the_priority_queue = 0;
    }
    return e;
  }

  void check_ready(ink_hrtime now, EThread * t);

  ink_hrtime earliest_timeout()
  {
    for (int i = 0; i < N_PQ_LIST; i++) {
      if (after[i].head)
        return last_check_time + (PQ_BUCKET_TIME(i) / 2);
    }
    return last_check_time + HRTIME_FOREVER;
  }

  PriorityEventQueue();
};

#endif
