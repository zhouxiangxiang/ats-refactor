/** @file
  Event subsystem
 */

#ifndef _I_EventSystem_h
#define _I_EventSystem_h

#include "libts.h"

#include "I_IOBuffer.h"
#include "I_Action.h"
#include "I_Continuation.h"
#include "I_EThread.h"
#include "I_Event.h"
#include "I_EventProcessor.h"

#include "I_Lock.h"
#include "I_PriorityEventQueue.h"
#include "I_Processor.h"
#include "I_ProtectedQueue.h"
#include "I_Thread.h"
#include "I_VIO.h"
#include "I_VConnection.h"
#include "I_RecProcess.h"
#include "I_SocketManager.h"

#define EVENT_SYSTEM_MODULE_MAJOR_VERSION 1
#define EVENT_SYSTEM_MODULE_MINOR_VERSION 0
#define EVENT_SYSTEM_MODULE_VERSION makeModuleVersion(                 \
                                    EVENT_SYSTEM_MODULE_MAJOR_VERSION, \
                                    EVENT_SYSTEM_MODULE_MINOR_VERSION, \
                                    PUBLIC_MODULE_HEADER)

void ink_event_system_init(ModuleVersion version);

#endif
