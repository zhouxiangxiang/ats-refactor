/**
  Basic Threads
 */
#include <time.h>

#include <cstring>
#include <cassert>
#include <mutex>
#include <iostream>

#include "./headers.h"
#include "./thread.h"

// Common Interface impl
static Thread* init_thread_key();

std::mutex* global_mutex = NULL;
ink_hrtime Thread::cur_time = 0;
thread_local Thread* Thread::s_thread_data_key = init_thread_key();

Thread::Thread() {
  p_mutex = new std::mutex;
  mutex_ptr = std::shared_ptr<std::mutex>(p_mutex);
  // MUTEX_TAKE_LOCK(mutex, (EThread *) this);
  //mutex->nthread_holding = THREAD_MUTEX_THREAD_HOLDING;
}

Thread::~ Thread() {
}

static void
key_destructor(void *value) {
  (void) value;
}

Thread*
init_thread_key() {
  //pthread_key_create(&Thread::s_thread_data_key, key_destructor);
  return Thread::s_thread_data_key;
}

// Unix & non-NT Interface impl
struct thread_data_internal {
  ThreadFunction f;
  void *a;
  Thread *me;
  char name[MAX_THREAD_NAME_LENGTH];
};

static void *
spawn_thread_internal(void *a) {
  thread_data_internal *p = (thread_data_internal *) a;

  p->me->set_specific();
  pthread_setname_np(pthread_self(), p->name);
  if (p->f)
    p->f(p->a);
  else
    p->me->execute();
  // ats_free(a);
  return NULL;
}

static inline ink_thread
ink_thread_create(void *(*f) (void *),
                  void *a,
                  int detached=0,
                  size_t stacksize=0) {

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  if (stacksize) {
    pthread_attr_setstacksize(&attr, stacksize);
  }
  if (detached) {
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  }

  ink_thread t;
  int ret = pthread_create(&t,    // thread
                           &attr, // attributes
                           f,     // start routine
                           a);    // arguments

  pthread_join(t, NULL);
  pthread_attr_destroy(&attr);

  return ret ? (ink_thread) 0 : t;
}


ink_thread
Thread::start(const char* name,
              size_t stacksize,
              ThreadFunction f,
              void *a) { /* thread arguments*/

  unsigned t_data_size = sizeof(thread_data_internal);
  thread_data_internal *p = (thread_data_internal *)malloc(t_data_size);
  p->f = f;     // function
  p->a = a;     // arguments
  p->me = this; // thread

  // set thread name
  memset(p->name, 0, MAX_THREAD_NAME_LENGTH);
  strncpy(p->name, name, MAX_THREAD_NAME_LENGTH);

  // create thread
  tid = ink_thread_create(spawn_thread_internal, // start routine
                          (void *) p,            // arguments
                          0,                     // detach or not
                          stacksize);            // thread stacksize
  return tid;
}

void
Thread::set_specific() {
  s_thread_data_key = this;
}

Thread*
Thread::this_thread() const {
  return s_thread_data_key;
}
