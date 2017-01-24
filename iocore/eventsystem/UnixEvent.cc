/**
  Event.cc
 */
#include <cassert>

void
Event::schedule_imm(int acallback_event) {
  callback_event = acallback_event;
  assert(ethread == this_ethread());
  if (in_the_priority_queue)
    ethread->EventQueue.remove(this);
  timeout_at = 0;
  period = 0;
  immediate = true;
  mutex = continuation->mutex;
  if (!in_the_prot_queue)
    ethread->EventQueueExternal.enqueue_local(this);
}

void
Event::schedule_at(ink_hrtime atimeout_at, int acallback_event) {
  callback_event = acallback_event;
  assert(ethread == this_ethread());
  assert(atimeout_at > 0);
  if (in_the_priority_queue)
    ethread->EventQueue.remove(this);
  timeout_at = atimeout_at;
  period = 0;
  immediate = false;
  mutex = continuation->mutex;
  if (!in_the_prot_queue)
    ethread->EventQueueExternal.enqueue_local(this);
}

void
Event::schedule_in(ink_hrtime atimeout_in, int acallback_event) {
  callback_event = acallback_event;
  assert(ethread == this_ethread());
  if (in_the_priority_queue)
    ethread->EventQueue.remove(this);
  timeout_at = ink_get_based_hrtime() + atimeout_in;
  period = 0;
  immediate = false;
  mutex = continuation->mutex;
  if (!in_the_prot_queue)
    ethread->EventQueueExternal.enqueue_local(this);
}

void
Event::schedule_every(ink_hrtime aperiod, int acallback_event) {
  callback_event = acallback_event;
  assert(ethread == this_ethread());
  assert(aperiod != 0);
  if (in_the_priority_queue)
    ethread->EventQueue.remove(this);
  if (aperiod < 0) {
    timeout_at = aperiod;
  } else {
    timeout_at = ink_get_based_hrtime() + aperiod;
  }
  period = aperiod;
  immediate = false;
  mutex = continuation->mutex;
  if (!in_the_prot_queue)
    ethread->EventQueueExternal.enqueue_local(this);
}
