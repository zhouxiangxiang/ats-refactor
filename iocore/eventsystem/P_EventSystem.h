/** @file
  A brief file description
 */

/****************************************************************************

  Event Subsystem



**************************************************************************/
#ifndef _P_EventSystem_h
#define _P_EventSystem_h

#include "libts.h"

#include "I_EventSystem.h"

#include "P_Thread.h"
#include "P_VIO.h"
#include "P_IOBuffer.h"
#include "P_VConnection.h"
#include "P_Freer.h"
#include "P_UnixEvent.h"
#include "P_UnixEThread.h"
#include "P_ProtectedQueue.h"
#include "P_UnixEventProcessor.h"
#include "P_UnixSocketManager.h"
#undef  EVENT_SYSTEM_MODULE_VERSION
#define EVENT_SYSTEM_MODULE_VERSION makeModuleVersion(                    \
                                       EVENT_SYSTEM_MODULE_MAJOR_VERSION, \
                                       EVENT_SYSTEM_MODULE_MINOR_VERSION, \
                                       PRIVATE_MODULE_HEADER)

#endif
