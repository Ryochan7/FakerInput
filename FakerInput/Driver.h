/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    User-mode Driver Framework 2

--*/

#ifndef DRIVER_H
#define DRIVER_H

#include <windows.h>
#include <wdf.h>
#include <initguid.h>

#include "device.h"
#include "fakerinputcommon.h"
#include "queue.h"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD FakerInputEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP FakerInputEvtDriverContextCleanup;

EXTERN_C_END

#endif // DRIVER_H