#if !defined (I_Tasks_h)
#define I_Tasks_h

#include "I_EventSystem.h"

extern EventType ET_TASK;

class TasksProcessor: public Processor {
 public:
  int start(int task_threads, size_t stacksize=DEFAULT_STACKSIZE);
};

extern TasksProcessor tasksProcessor;

#endif
