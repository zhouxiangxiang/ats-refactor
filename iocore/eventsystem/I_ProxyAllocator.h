/**
  ProxyAllocator.h
 */
#ifndef _I_ProxyAllocator_h_
#define _I_ProxyAllocator_h_

#include "libts.h"

class EThread;

extern int thread_freelist_size;

struct ProxyAllocator {
  int allocated;
  void *freelist;

  ProxyAllocator():allocated(0), freelist(0) { }
};

template<class C>
inline C * thread_alloc(ClassAllocator<C> &a, ProxyAllocator & l) {
#if TS_USE_FREELIST && !TS_USE_RECLAIMABLE_FREELIST
  if (l.freelist) {
    C *v = (C *) l.freelist;
    l.freelist = *(C **) l.freelist;
    l.allocated--;
    *(void **) v = *(void **) &a.proto.typeObject;
    return v;
  }
#else
  (void)l;
#endif
  return a.alloc();
}

template<class C>
inline C * thread_alloc_init(ClassAllocator<C> &a, ProxyAllocator & l) {
#if TS_USE_FREELIST && !TS_USE_RECLAIMABLE_FREELIST
  if (l.freelist) {
    C *v = (C *) l.freelist;
    l.freelist = *(C **) l.freelist;
    l.allocated--;
    memcpy((void *) v, (void *) &a.proto.typeObject, sizeof(C));
    return v;
  }
#else
  (void)l;
#endif
  return a.alloc();
}

template<class C>
inline void thread_free(ClassAllocator<C> &a, C *p) {
  a.free(p);
}

static inline void
thread_free(Allocator &a, void *p) {
  a.free_void(p);
}

template<class C>
inline void thread_freeup(ClassAllocator<C> &a, ProxyAllocator & l) {
  while (l.freelist) {
    C *v = (C *) l.freelist;
    l.freelist = *(C **) l.freelist;
    l.allocated--;
    a.free(v);                  // we could use a bulk free here
  }
  ink_assert(!l.allocated);
}

void* thread_alloc(Allocator &a, ProxyAllocator &l);
void thread_freeup(Allocator &a, ProxyAllocator &l);

#define THREAD_ALLOC(_a, _t) thread_alloc(::_a, _t->_a)
#define THREAD_ALLOC_INIT(_a, _t) thread_alloc_init(::_a, _t->_a)
#if TS_USE_FREELIST && !TS_USE_RECLAIMABLE_FREELIST
#define THREAD_FREE(_p, _a, _t) do {            \
  *(char **)_p = (char*)_t->_a.freelist;        \
  _t->_a.freelist = _p;                         \
  _t->_a.allocated++;                           \
  if (_t->_a.allocated > thread_freelist_size)  \
    thread_freeup(::_a, _t->_a);                \
} while (0)
#else /* !TS_USE_FREELIST || TS_USE_RECLAIMABLE_FREELIST */
#define THREAD_FREE(_p, _a, _t) do {  \
  (void)_t;                           \
  thread_free(::_a, _p);              \
} while (0)
#endif

#endif /* _ProxyAllocator_h_ */
