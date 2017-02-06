/**
   EventSystem.cc
*/

#include "P_EventSystem.h"

void
ink_event_system_init(ModuleVersion v)
{
  ink_release_assert(!checkModuleVersion(v, EVENT_SYSTEM_MODULE_VERSION));
  int config_max_iobuffer_size = DEFAULT_MAX_BUFFER_SIZE;

  REC_EstablishStaticConfigInt32(thread_freelist_size, "proxy.config.allocator.thread_freelist_size");
  REC_ReadConfigInteger(config_max_iobuffer_size, "proxy.config.io.max_buffer_size");

  max_iobuffer_size = buffer_size_to_index(config_max_iobuffer_size, DEFAULT_BUFFER_SIZES - 1);
  if (default_small_iobuffer_size > max_iobuffer_size)
    default_small_iobuffer_size = max_iobuffer_size;
  if (default_large_iobuffer_size > max_iobuffer_size)
    default_large_iobuffer_size = max_iobuffer_size;
  init_buffer_allocators();
}
