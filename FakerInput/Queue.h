/*++

Module Name:

    queue.h

Abstract:

    This file contains the queue definitions.

Environment:

    User-mode Driver Framework 2

--*/

#ifndef QUEUE_H
#define QUEUE_H

EXTERN_C_START

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    ULONG PrivateDeviceData;  // just a placeholder

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
FakerInputQueueInitialize(
    _In_ WDFDEVICE Device
    );

//
// Events from the IoQueue object
//
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL FakerInputEvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP FakerInputEvtIoStop;


NTSTATUS
ReadReport(
    _In_  PDEVICE_CONTEXT DeviceContext,
    _In_  WDFREQUEST Request,
    _Always_(_Out_)
    BOOLEAN* CompleteRequest
);

NTSTATUS
WriteReport(
    _In_  PDEVICE_CONTEXT DeviceContext,
    _In_  WDFREQUEST Request
);

NTSTATUS
GetString(
    _In_  WDFREQUEST        Request
);

NTSTATUS
GetIndexedString(
    _In_  WDFREQUEST        Request
);

NTSTATUS
GetStringId(
    _In_  WDFREQUEST        Request,
    _Out_ ULONG* StringId,
    _Out_ ULONG* LanguageId
);

EXTERN_C_END

#endif // QUEUE_H