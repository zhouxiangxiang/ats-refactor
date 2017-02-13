#include "I_Tasks.h"

// Globals
EventType ET_TASK = ET_CALL;
TasksProcessor tasksProcessor;

int
TasksProcessor::start(int task_threads, size_t stacksize) {
  if (task_threads > 0) {
    ET_TASK = eventProcessor.spawn_event_threads(task_threads, "ET_TASK", stacksize);
  }
  return 0;
}
