/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    User-mode Driver Framework 2

--*/

#ifndef DEVICE_H
#define DEVICE_H

#include <hidport.h>  // located in $(DDK_INC_PATH)/wdm

#include "public.h"

typedef UCHAR HID_REPORT_DESCRIPTOR, *PHID_REPORT_DESCRIPTOR;

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    WDFDEVICE               Device;
    WDFQUEUE                DefaultQueue;
    WDFQUEUE                ManualQueue;
    HID_DEVICE_ATTRIBUTES   HidDeviceAttributes;
    HID_DESCRIPTOR          HidDescriptor;
    PHID_REPORT_DESCRIPTOR  ReportDescriptor;

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
FakerInputCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );


NTSTATUS
RequestCopyFromBuffer(
    _In_  WDFREQUEST        Request,
    _In_  PVOID             SourceBuffer,
    _When_(NumBytesToCopyFrom == 0, __drv_reportError(NumBytesToCopyFrom cannot be zero))
    _In_  size_t            NumBytesToCopyFrom
);

NTSTATUS
RequestGetHidXferPacket_ToReadFromDevice(
    _In_  WDFREQUEST        Request,
    _Out_ HID_XFER_PACKET* Packet
);

NTSTATUS
RequestGetHidXferPacket_ToWriteToDevice(
    _In_  WDFREQUEST        Request,
    _Out_ HID_XFER_PACKET* Packet
);

EXTERN_C_END

//
// These are the device attributes returned by the mini driver in response
// to IOCTL_HID_GET_DEVICE_ATTRIBUTES.
//
#define FAKERINPUT_VID             0xFE0F
#define FAKERINPUT_PID             0x00FF
#define FAKERINPUT_VERSION         0x0001
#define FAKERINPUT_MIN_API_VERSION 0x01

#endif // DEVICE_H