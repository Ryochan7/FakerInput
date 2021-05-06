/*++

Module Name:

    device.c - Device handling events for example driver.

Abstract:

   This file contains the device entry points and callbacks.
    
Environment:

    User-mode Driver Framework 2

--*/

#include "driver.h"
#include "device.tmh"

HID_REPORT_DESCRIPTOR       G_DefaultReportDescriptor[] = {

    //
    // Keyboard report starts here
    //    
        0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
        0x09, 0x06,                         // USAGE (Keyboard)
        0xa1, 0x01,                         // COLLECTION (Application)
        0x85, REPORTID_KEYBOARD,            //   REPORT_ID (Keyboard)    
        0x05, 0x07,                         //   USAGE_PAGE (Keyboard)
        0x19, 0xe0,                         //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7,                         //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                         //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,                         //   REPORT_SIZE (1)
        0x95, 0x08,                         //   REPORT_COUNT (8)
        0x81, 0x02,                         //   INPUT (Data,Var,Abs)
        0x95, 0x01,                         //   REPORT_COUNT (1)
        0x75, 0x08,                         //   REPORT_SIZE (8)
        0x81, 0x03,                         //   INPUT (Cnst,Var,Abs)
        0x95, 0x05,                         //   REPORT_COUNT (5)
        0x75, 0x01,                         //   REPORT_SIZE (1)
        0x05, 0x08,                         //   USAGE_PAGE (LEDs)
        0x19, 0x01,                         //   USAGE_MINIMUM (Num Lock)
        0x29, 0x05,                         //   USAGE_MAXIMUM (Kana)
        0x91, 0x02,                         //   OUTPUT (Data,Var,Abs)
        0x95, 0x01,                         //   REPORT_COUNT (1)
        0x75, 0x03,                         //   REPORT_SIZE (3)
        0x91, 0x03,                         //   OUTPUT (Cnst,Var,Abs)
        0x95, 0x06,                         //   REPORT_COUNT (6)
        0x75, 0x08,                         //   REPORT_SIZE (8)
        0x15, 0x00,                         //   LOGICAL_MINIMUM (0)
        0x25, 0x65,                         //   LOGICAL_MAXIMUM (101)
        0x05, 0x07,                         //   USAGE_PAGE (Keyboard)
        0x19, 0x00,                         //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65,                         //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00,                         //   INPUT (Data,Ary,Abs)
        0xc0,                               // END_COLLECTION

        // Multimedia Keys here
        0x05, 0x0c,                         // Usage Page (Consumer Devices)
        0x09, 0x01,                         // Usage (Consumer Control)
        0xa1, 0x01,                         // Collection (Application)
        0x85, REPORTID_ENHANCED_KEY,        //   Report ID (Volume)
        0x05, 0x0c,                         //   Usage Page (Consumer Devices)
        0x15, 0x00,                         //   Logical Minimum (0)
        0x25, 0x01,                         //   Logical Maximum (1)
        0x75, 0x01,                         //   Report Size (1)
        0x95, 0x10,                         //   Report Count (16)
        0x09, 0xb5,                         //   Usage (Scan Next Track)
        0x09, 0xb6,                         //   Usage (Scan Previous Track)
        0x09, 0xb7,                         //   Usage (Stop)
        0x09, 0xcd,                         //   Usage (Play / Pause)
        0x09, 0xe2,                         //   Usage (Mute)
        0x09, 0xea,                         //   Usage (Volume Down)
        0x09, 0xe9,                         //   Usage (Volume Up)
        0x0a, 0x23, 0x02,                   //   Usage (WWW Home)
        0x0a, 0x94, 0x01,                   //   Usage (My Computer)
        0x0a, 0x92, 0x01,                   //   Usage (Calculator)
        0x0a, 0x2a, 0x02,                   //   Usage (WWW fav)
        0x0a, 0x21, 0x02,                   //   Usage (WWW search)
        0x0a, 0x26, 0x02,                   //   Usage (WWW stop)
        0x0a, 0x24, 0x02,                   //   Usage (WWW back)
        0x0a, 0x83, 0x01,                   //   Usage (Media sel)
        0x0a, 0x8a, 0x01,                   //   Usage (Mail)
        0x81, 0x02,                         //   Input (Data, Variable, Absolute)
        0xc0,                               //  End Collection

    //
    // Relative mouse report starts here
    //
        0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
        0x09, 0x02,                         // USAGE (Mouse)
        0xa1, 0x01,                         // COLLECTION (Application)
        0x85, REPORTID_RELATIVE_MOUSE,      //   REPORT_ID (Mouse)
        0x09, 0x01,                         //   USAGE (Pointer)
        0xa1, 0x00,                         //   COLLECTION (Physical)
        0x05, 0x09,                         //     USAGE_PAGE (Button)
        0x19, 0x01,                         //     USAGE_MINIMUM (Button 1)
        0x29, 0x05,                         //     USAGE_MAXIMUM (Button 5)
        0x15, 0x00,                         //     LOGICAL_MINIMUM (0)
        0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)
        0x75, 0x01,                         //     REPORT_SIZE (1)
        0x95, 0x05,                         //     REPORT_COUNT (5)
        0x81, 0x02,                         //     INPUT (Data,Var,Abs)
        0x75, 0x01,                         //     REPORT_SIZE (1)
        0x95, 0x03,                         //     REPORT_COUNT (3)
        0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)
        0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                         //     USAGE (X)
        0x09, 0x31,                         //     USAGE (Y)
        0x16, 0xFF, 0x81,                   //     Logical Minimum (-32767)
        0x26, 0xFF, 0x7F,                   //     Logical Maximum (32767)
        0x75, 0x10,                         //     REPORT_SIZE (16)
        0x95, 0x02,                         //     REPORT_COUNT (2)
        0x81, 0x06,                         //     INPUT (Data,Var,Rel)
        0x05, 0x01,                         //     Usage Page (Generic Desktop)
        0x09, 0x38,                         //     Usage (Wheel)
        0x15, 0x81,                         //     Logical Minimum (-127)
        0x25, 0x7F,                         //     Logical Maximum (127)
        0x75, 0x08,                         //     Report Size (8)
        0x95, 0x01,                         //     Report Count (1)
        0x81, 0x06,                         //     Input (Data, Variable, Relative)
        0x05, 0x0c,                         //     Usage Page (Consumer Devices)
        0x0a, 0x38, 0x02,                   //     Usage (AC Pan)
        0x15, 0x81,                         //     Logical Minimum (-127)
        0x25, 0x7f,                         //     Logical Maximum (127)
        0x75, 0x08,                         //     Report Size (8)
        0x81, 0x06,                         //     Input (Data, Variable, Relative)
        0xc0,                               //   END_COLLECTION
        0xc0,                               // END_COLLECTION

    //
    // Mouse report starts here
    //
    0x05, 0x01,                         // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                         // USAGE (Mouse)
    0xa1, 0x01,                         // COLLECTION (Application)
    0x85, REPORTID_MOUSE,               //   REPORT_ID (Mouse)
    0x09, 0x01,                         //   USAGE (Pointer)
    0xa1, 0x00,                         //   COLLECTION (Physical)
    0x05, 0x09,                         //     USAGE_PAGE (Button)
    0x19, 0x01,                         //     USAGE_MINIMUM (Button 1)
    0x29, 0x05,                         //     USAGE_MAXIMUM (Button 5)
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                         //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                         //     REPORT_SIZE (1)
    0x95, 0x05,                         //     REPORT_COUNT (5)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x95, 0x03,                         //     REPORT_COUNT (3)
    0x81, 0x03,                         //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                         //     USAGE_PAGE (Generic Desktop)
    0x15, 0x00,                         //     LOGICAL_MINIMUM (0)
    0x26, 0xFF, 0x7F,                   //     LOGICAL_MAXIMUM (32767)
    0x75, 0x10,                         //     REPORT_SIZE (16)
    0x95, 0x01,                         //     REPORT_COUNT (1)
    0x55, 0x0F,                         //     UNIT_EXPONENT (-1)
    0x65, 0x11,                         //     UNIT (cm,SI Linear)
    0x35, 0x00,                         //     PHYSICAL_MINIMUM (0)
    0x45, 0x00,                         //     PHYSICAL_MAXIMUM (0)
    0x09, 0x30,                         //     USAGE (X)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x09, 0x31,                         //     USAGE (Y)
    0x81, 0x02,                         //     INPUT (Data,Var,Abs)
    0x05, 0x01,                         //     Usage Page (Generic Desktop)
    0x09, 0x38,                         //     Usage (Wheel)
    0x15, 0x81,                         //     Logical Minimum (-127)
    0x25, 0x7F,                         //     Logical Maximum (127)
    0x75, 0x08,                         //     Report Size (8)
    0x95, 0x01,                         //     Report Count (1)
    0x81, 0x06,                         //     Input (Data, Variable, Relative)
    0xc0,                               //   END_COLLECTION
    0xc0,                               // END_COLLECTION

    //
    // Vendor defined control report starts here
    //
        0x06, 0x00, 0xff,                    // USAGE_PAGE (Vendor Defined Page 1)
        0x09, 0x01,                          // USAGE (Vendor Usage 1)
        0xa1, 0x01,                          // COLLECTION (Application)
        0x85, REPORTID_CONTROL,              //   REPORT_ID (1)  
        0x15, 0x00,                          //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x00,                    //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,                          //   REPORT_SIZE  (8)   - bits
        0x95, 0x40,                          //   REPORT_COUNT (64)  - Bytes
        0x09, 0x02,                          //   USAGE (Vendor Usage 1)
        0x81, 0x02,                          //   INPUT (Data,Var,Abs)
        0x95, 0x40,                          //   REPORT_COUNT (64)  - Bytes
        0x09, 0x02,                          //   USAGE (Vendor Usage 1)
        0x91, 0x02,                          //   OUTPUT (Data,Var,Abs)
        0xc0,                                // END_COLLECTION
};

//
// This is the default HID descriptor returned by the mini driver
// in response to IOCTL_HID_GET_DEVICE_DESCRIPTOR. The size
// of report descriptor is currently the size of G_DefaultReportDescriptor.
//

HID_DESCRIPTOR              G_DefaultHidDescriptor = {
    0x09,   // length of HID descriptor
    0x21,   // descriptor type == HID  0x21
    0x0100, // hid spec release
    0x00,   // country code == Not Specified
    0x01,   // number of HID class descriptors
    {                                       //DescriptorList[0]
        0x22,                               //report descriptor type 0x22
        sizeof(G_DefaultReportDescriptor)   //total length of report descriptor
    }
};

NTSTATUS
FakerInputCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    )
/*++

Routine Description:

    Worker routine called to create a device and its software resources.

Arguments:

    DeviceInit - Pointer to an opaque init structure. Memory for this
                    structure will be freed by the framework when the WdfDeviceCreate
                    succeeds. So don't access the structure after that point.

Return Value:

    NTSTATUS

--*/
{
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    PDEVICE_CONTEXT deviceContext;
    WDFDEVICE device;
    PHID_DEVICE_ATTRIBUTES  hidAttributes;
    NTSTATUS status;

    //
    // Mark ourselves as a filter, which also relinquishes power policy ownership
    //
    WdfFdoInitSetFilter(DeviceInit);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);

    if (NT_SUCCESS(status)) {
        //
        // Get a pointer to the device context structure that we just associated
        // with the device object. We define this structure in the device.h
        // header file. DeviceGetContext is an inline function generated by
        // using the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro in device.h.
        // This function will do the type checking and return the device context.
        // If you pass a wrong object handle it will return NULL and assert if
        // run under framework verifier mode.
        //
        deviceContext = DeviceGetContext(device);

        //
        // Initialize the context.
        //
        deviceContext->Device = device;
        hidAttributes = &deviceContext->HidDeviceAttributes;
        RtlZeroMemory(hidAttributes, sizeof(HID_DEVICE_ATTRIBUTES));
        hidAttributes->Size = sizeof(HID_DEVICE_ATTRIBUTES);
        hidAttributes->VendorID = FAKERINPUT_VID;
        hidAttributes->ProductID = FAKERINPUT_PID;
        hidAttributes->VersionNumber = FAKERINPUT_VERSION;

        //
        // Create a device interface so that applications can find and talk
        // to us.
        //
        status = WdfDeviceCreateDeviceInterface(
            device,
            &GUID_DEVINTERFACE_FakerInput,
            NULL // ReferenceString
            );

        if (NT_SUCCESS(status)) {
            //
            // Initialize the I/O Package and any Queues
            //
            status = FakerInputQueueInitialize(device);
        }

        //
        // Use default "HID Descriptor" (hardcoded). We will set the
        // wReportLength memeber of HID descriptor when we read the
        // the report descriptor either from registry or the hard-coded
        // one.
        //
        deviceContext->HidDescriptor = G_DefaultHidDescriptor;

        // Use hardcoded HID Report Descriptor for defining
        // virtual devices exposed by driver
        deviceContext->ReportDescriptor = G_DefaultReportDescriptor;
    }

    return status;
}

NTSTATUS
RequestCopyFromBuffer(
    _In_  WDFREQUEST        Request,
    _In_  PVOID             SourceBuffer,
    _When_(NumBytesToCopyFrom == 0, __drv_reportError(NumBytesToCopyFrom cannot be zero))
    _In_  size_t            NumBytesToCopyFrom
)
/*++

Routine Description:

    A helper function to copy specified bytes to the request's output memory

Arguments:

    Request - A handle to a framework request object.

    SourceBuffer - The buffer to copy data from.

    NumBytesToCopyFrom - The length, in bytes, of data to be copied.

Return Value:

    NTSTATUS

--*/
{
    NTSTATUS                status;
    WDFMEMORY               memory;
    size_t                  outputBufferLength;

    status = WdfRequestRetrieveOutputMemory(Request, &memory);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfRequestRetrieveOutputMemory failed 0x%x\n", status));
        return status;
    }

    WdfMemoryGetBuffer(memory, &outputBufferLength);
    if (outputBufferLength < NumBytesToCopyFrom) {
        status = STATUS_INVALID_BUFFER_SIZE;
        KdPrint(("RequestCopyFromBuffer: buffer too small. Size %d, expect %d\n",
            (int)outputBufferLength, (int)NumBytesToCopyFrom));
        return status;
    }

    status = WdfMemoryCopyFromBuffer(memory,
        0,
        SourceBuffer,
        NumBytesToCopyFrom);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfMemoryCopyFromBuffer failed 0x%x\n", status));
        return status;
    }

    WdfRequestSetInformation(Request, NumBytesToCopyFrom);
    return status;
}
