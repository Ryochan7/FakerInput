/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    User-mode Driver Framework 2

--*/

#include "driver.h"
#include "queue.tmh"

NTSTATUS
FakerInputQueueInitialize(
    _In_ WDFDEVICE Device
    )
/*++

Routine Description:

     The I/O dispatch callbacks for the frameworks device object
     are configured in this function.

     A single default I/O Queue is configured for parallel request
     processing, and a driver context memory allocation is created
     to hold our structure QUEUE_CONTEXT.

Arguments:

    Device - Handle to a framework device object.

Return Value:

    VOID

--*/
{
    NTSTATUS status;
    WDFQUEUE queue;
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDFQUEUE manualQueue;
    WDF_IO_QUEUE_CONFIG manualQueueConfig;
    PDEVICE_CONTEXT deviceContext = DeviceGetContext(Device);

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );

    queueConfig.EvtIoDeviceControl = FakerInputEvtIoDeviceControl;
    //queueConfig.EvtIoStop = FakerInputEvtIoStop;

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if(!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    deviceContext->DefaultQueue = queue;

    // Establish manual queue
    WDF_IO_QUEUE_CONFIG_INIT(
        &manualQueueConfig,
        WdfIoQueueDispatchManual);

    status = WdfIoQueueCreate(
        Device,
        &manualQueueConfig,
        WDF_NO_OBJECT_ATTRIBUTES,
        &manualQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    deviceContext->ManualQueue = manualQueue;

    return status;
}

VOID
FakerInputEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
/*++

Routine Description:

    This event is invoked when the framework receives IRP_MJ_DEVICE_CONTROL request.

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.

    Request - Handle to a framework request object.

    OutputBufferLength - Size of the output buffer in bytes

    InputBufferLength - Size of the input buffer in bytes

    IoControlCode - I/O control code.

Return Value:

    VOID

--*/
{
    NTSTATUS status = STATUS_NOT_IMPLEMENTED;
    BOOLEAN completeRequest = TRUE;
    WDFDEVICE Device;
    PDEVICE_CONTEXT deviceContext = NULL;

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    Device = WdfIoQueueGetDevice(Queue);
    deviceContext = DeviceGetContext(Device);

    /*TraceEvents(TRACE_LEVEL_INFORMATION, 
                TRACE_QUEUE, 
                "%!FUNC! Queue 0x%p, Request 0x%p OutputBufferLength %d InputBufferLength %d IoControlCode %d", 
                Queue, Request, (int) OutputBufferLength, (int) InputBufferLength, IoControlCode);
    */

    switch (IoControlCode)
    {
    case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
        _Analysis_assume_(deviceContext->HidDescriptor.bLength != 0);
        status = RequestCopyFromBuffer(Request,
            &deviceContext->HidDescriptor,
            deviceContext->HidDescriptor.bLength);
        break;

    case IOCTL_HID_GET_DEVICE_ATTRIBUTES:   // METHOD_NEITHER
       //
       //Retrieves a device's attributes in a HID_DEVICE_ATTRIBUTES structure.
       //
        status = RequestCopyFromBuffer(Request,
            &deviceContext->HidDeviceAttributes,
            sizeof(HID_DEVICE_ATTRIBUTES));
        break;

    case IOCTL_HID_GET_REPORT_DESCRIPTOR:   // METHOD_NEITHER
        //
        //Obtains the report descriptor for the HID device.
        //
        status = RequestCopyFromBuffer(Request,
            deviceContext->ReportDescriptor,
            deviceContext->HidDescriptor.DescriptorList[0].wReportLength);
        break;

    case IOCTL_HID_READ_REPORT:             // METHOD_NEITHER
    case IOCTL_UMDF_HID_GET_INPUT_REPORT:
        //
        // Returns a report from the device into a class driver-supplied
        // buffer.
        //
        status = ReadReport(deviceContext, Request, &completeRequest);
        break;

    case IOCTL_HID_WRITE_REPORT:            // METHOD_NEITHER
    case IOCTL_UMDF_HID_SET_OUTPUT_REPORT:
        //
        // Transmits a class driver-supplied report to the device.
        //
        status = WriteReport(deviceContext, Request);
        break;

    case IOCTL_UMDF_HID_GET_FEATURE:
        status = GetFeatureReport(deviceContext, Request);
        break;

    case IOCTL_HID_GET_STRING:                      // METHOD_NEITHER
        status = GetString(Request);
        break;

    case IOCTL_HID_GET_INDEXED_STRING:              // METHOD_OUT_DIRECT
        status = GetIndexedString(Request);
        break;

    default:
        status = STATUS_NOT_IMPLEMENTED;
        break;

    }

    //
    // Complete the request. Information value has already been set by request
    // handlers.
    //
    if (completeRequest)
    {
        WdfRequestComplete(Request, status);
    }

    return;
}

//VOID
//FakerInputEvtIoStop(
//    _In_ WDFQUEUE Queue,
//    _In_ WDFREQUEST Request,
//    _In_ ULONG ActionFlags
//)
///*++
//
//Routine Description:
//
//    This event is invoked for a power-managed queue before the device leaves the working state (D0).
//
//Arguments:
//
//    Queue -  Handle to the framework queue object that is associated with the
//             I/O request.
//
//    Request - Handle to a framework request object.
//
//    ActionFlags - A bitwise OR of one or more WDF_REQUEST_STOP_ACTION_FLAGS-typed flags
//                  that identify the reason that the callback function is being called
//                  and whether the request is cancelable.
//
//Return Value:
//
//    VOID
//
//--*/
//{
//    TraceEvents(TRACE_LEVEL_INFORMATION, 
//                TRACE_QUEUE, 
//                "%!FUNC! Queue 0x%p, Request 0x%p ActionFlags %d", 
//                Queue, Request, ActionFlags);
//
//    //
//    // In most cases, the EvtIoStop callback function completes, cancels, or postpones
//    // further processing of the I/O request.
//    //
//    // Typically, the driver uses the following rules:
//    //
//    // - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
//    //   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
//    //   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
//    //   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
//    //
//    //   Before it can call these methods safely, the driver must make sure that
//    //   its implementation of EvtIoStop has exclusive access to the request.
//    //
//    //   In order to do that, the driver must synchronize access to the request
//    //   to prevent other threads from manipulating the request concurrently.
//    //   The synchronization method you choose will depend on your driver's design.
//    //
//    //   For example, if the request is held in a shared context, the EvtIoStop callback
//    //   might acquire an internal driver lock, take the request from the shared context,
//    //   and then release the lock. At this point, the EvtIoStop callback owns the request
//    //   and can safely complete or requeue the request.
//    //
//    // - If the driver has forwarded the I/O request to an I/O target, it either calls
//    //   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
//    //   further processing of the request and calls WdfRequestStopAcknowledge with
//    //   a Requeue value of FALSE.
//    //
//    // A driver might choose to take no action in EvtIoStop for requests that are
//    // guaranteed to complete in a small amount of time.
//    //
//    // In this case, the framework waits until the specified request is complete
//    // before moving the device (or system) to a lower power state or removing the device.
//    // Potentially, this inaction can prevent a system from entering its hibernation state
//    // or another low system power state. In extreme cases, it can cause the system
//    // to crash with bugcheck code 9F.
//    //
//
//    return;
//}

NTSTATUS
ReadReport(
    _In_  PDEVICE_CONTEXT DeviceContext,
    _In_  WDFREQUEST Request,
    _Always_(_Out_)
    BOOLEAN* CompleteRequest
)
/*++

Routine Description:

    Handles IOCTL_HID_READ_REPORT for the HID collection. Normally the request
    will be forwarded to a manual queue for further process. In that case, the
    caller should not try to complete the request at this time, as the request
    will later be retrieved back from the manually queue and completed there.
    However, if for some reason the forwarding fails, the caller still need
    to complete the request with proper error code immediately.

Arguments:

    QueueContext - The object context associated with the queue

    Request - Pointer to  Request Packet.

    CompleteRequest - A boolean output value, indicating whether the caller
            should complete the request or not

Return Value:

    NT status code.

--*/
{
    NTSTATUS                status;

    KdPrint(("ReadReport\n"));

    //
    // forward the request to manual queue
    //
    status = WdfRequestForwardToIoQueue(
        Request,
        DeviceContext->ManualQueue);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfRequestForwardToIoQueue failed with 0x%x\n", status));
        *CompleteRequest = TRUE;
    }
    else {
        *CompleteRequest = FALSE;
    }

    return status;
}

NTSTATUS
WriteReport(
    _In_  PDEVICE_CONTEXT DeviceContext,
    _In_  WDFREQUEST Request
)
/*++

Routine Description:

    Handles IOCTL_HID_WRITE_REPORT all the collection.

Arguments:

    QueueContext - The object context associated with the queue

    Request - Pointer to  Request Packet.

Return Value:

    NT status code.

--*/

{
    NTSTATUS                status;
    HID_XFER_PACKET         packet;
    ULONG                   reportSize;
    WDFREQUEST reqRead;
    size_t bytesReturned = 0;
    PBYTE outputReport;

    //KdPrint(("WriteReport\n"));

    status = RequestGetHidXferPacket_ToWriteToDevice(
        Request,
        &packet);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "WHY ME %d", packet.reportId);

    switch (packet.reportId)
    {
    case REPORTID_CONTROL:
        //
        // before touching buffer make sure buffer is big enough.
        //
        reportSize = packet.reportBufferLen - sizeof(FakerInputControlReportHeader);
        FakerInputControlReportHeader* pReport = (FakerInputControlReportHeader*)packet.reportBuffer;

        if (pReport->ReportLength > reportSize)
        {
            status = STATUS_INVALID_BUFFER_SIZE;
            KdPrint(("WriteReport: invalid input buffer. size %d, expect %d\n",
                packet.reportBufferLen, reportSize));
            return status;
        }

        //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "REPORT %d INNER LEN %d FULL LEN %d 0 %d",
        //    packet.reportId, pReport->ReportLength, packet.reportBufferLen, pReport->ReportID);

        outputReport = (BYTE*)packet.reportBuffer + sizeof(FakerInputControlReportHeader);

        PVOID pReadReport = NULL;

        // Obtain a WDFREQUEST object from manual queue
        status = WdfIoQueueRetrieveNextRequest(DeviceContext->ManualQueue,
            &reqRead);

        if (!NT_SUCCESS(status)) {
            return status;
        }

        // Copy current report data to request in manual queue
        status = WdfRequestRetrieveOutputBuffer(reqRead,
            reportSize,
            &pReadReport,
            &bytesReturned);

        RtlCopyMemory(pReadReport,
            outputReport,
            reportSize);

        // Mark from manual queue as complete
        WdfRequestCompleteWithInformation(reqRead,
            status,
            bytesReturned);

        // set status and information for initial request
        WdfRequestSetInformation(Request, reportSize);

        break;
    case REPORTID_CHECK_API_VERSION:
    {
        FakerInputAPIVersionReport* tempReport =
            (FakerInputAPIVersionReport*)(packet.reportBuffer);
        if (tempReport->ApiVersion < FAKERINPUT_MIN_API_VERSION)
        {
            status = STATUS_UNSUCCESSFUL;
            KdPrint(("WriteReport: invalid API Version. passed %d, expect %d\n",
                tempReport->ApiVersion, FAKERINPUT_MIN_API_VERSION));
            return status;
        }

        status = STATUS_SUCCESS;
        break;
    }

    default:
        //
        // Return error for unknown collection
        //
        status = STATUS_INVALID_PARAMETER;
        KdPrint(("WriteReport: unkown report id %d\n", packet.reportId));
    }

    return status;
}

NTSTATUS
GetFeatureReport(
    _In_  PDEVICE_CONTEXT DeviceContext,
    _In_  WDFREQUEST Request
)
{
    NTSTATUS status;
    HID_XFER_PACKET packet;
    ULONG reportSize;

    UNREFERENCED_PARAMETER(DeviceContext);

    status = RequestGetHidXferPacket_ToReadFromDevice(
        Request,
        &packet);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    /*TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "FEATURE ID: %d %d",
        packet.reportId,
        packet.reportBufferLen);
     */

    switch (packet.reportId)
    {
    case REPORTID_API_VERSION_FEATURE_ID:
        //TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "IN FEATURE REPORT METHOD");
        reportSize = sizeof(FakerInputAPIVersionFeature);
        FakerInputAPIVersionFeature* tempFeatureReport = (FakerInputAPIVersionFeature*)(packet.reportBuffer);
        packet.reportBuffer[0] = REPORTID_API_VERSION_FEATURE_ID;
        tempFeatureReport->ApiVersion = FAKERINPUT_API_VERSION;

        //
        // Report how many bytes were copied
        //
        WdfRequestSetInformation(Request, reportSize);
        break;
    default:
        status = STATUS_INVALID_PARAMETER;
        break;
    }

    return status;
}

NTSTATUS
GetStringId(
    _In_  WDFREQUEST        Request,
    _Out_ ULONG* StringId,
    _Out_ ULONG* LanguageId
)
/*++

Routine Description:

    Helper routine to decode IOCTL_HID_GET_INDEXED_STRING and IOCTL_HID_GET_STRING.

Arguments:

    Request - Pointer to Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS                status;
    ULONG                   inputValue;
    WDFMEMORY               inputMemory;
    size_t                  inputBufferLength;
    PVOID                   inputBuffer;

    //
    // mshidumdf.sys updates the IRP and passes the string id (or index) through
    // the input buffer correctly based on the IOCTL buffer type
    //

    status = WdfRequestRetrieveInputMemory(Request, &inputMemory);
    if (!NT_SUCCESS(status)) {
        KdPrint(("WdfRequestRetrieveInputMemory failed 0x%x\n", status));
        return status;
    }
    inputBuffer = WdfMemoryGetBuffer(inputMemory, &inputBufferLength);

    //
    // make sure buffer is big enough.
    //
    if (inputBufferLength < sizeof(ULONG))
    {
        status = STATUS_INVALID_BUFFER_SIZE;
        KdPrint(("GetStringId: invalid input buffer. size %d, expect %d\n",
            (int)inputBufferLength, (int)sizeof(ULONG)));
        return status;
    }

    inputValue = (*(PULONG)inputBuffer);

    //
    // The least significant two bytes of the INT value contain the string id.
    //
    *StringId = (inputValue & 0x0ffff);

    //
    // The most significant two bytes of the INT value contain the language
    // ID (for example, a value of 1033 indicates English).
    //
    *LanguageId = (inputValue >> 16);

    return status;
}


NTSTATUS
GetIndexedString(
    _In_  WDFREQUEST        Request
)
/*++

Routine Description:

    Handles IOCTL_HID_GET_INDEXED_STRING

Arguments:

    Request - Pointer to Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS                status;
    ULONG                   languageId, stringIndex;

    status = GetStringId(Request, &stringIndex, &languageId);

    // While we don't use the language id, some minidrivers might.
    //
    UNREFERENCED_PARAMETER(languageId);

    if (NT_SUCCESS(status)) {

        if (stringIndex != FAKERINPUT_DEVICE_STRING_INDEX)
        {
            status = STATUS_INVALID_PARAMETER;
            KdPrint(("GetString: unkown string index %d\n", stringIndex));
            return status;
        }

        status = RequestCopyFromBuffer(Request, FAKERINPUT_DEVICE_STRING, sizeof(FAKERINPUT_DEVICE_STRING));
    }
    return status;
}


NTSTATUS
GetString(
    _In_  WDFREQUEST        Request
)
/*++

Routine Description:

    Handles IOCTL_HID_GET_STRING.

Arguments:

    Request - Pointer to Request Packet.

Return Value:

    NT status code.

--*/
{
    NTSTATUS                status;
    ULONG                   languageId, stringId;
    size_t                  stringSizeCb;
    PWSTR                   string;

    status = GetStringId(Request, &stringId, &languageId);

    // While we don't use the language id, some minidrivers might.
    //
    UNREFERENCED_PARAMETER(languageId);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    switch (stringId)
    {
    case HID_STRING_ID_IMANUFACTURER:
        stringSizeCb = sizeof(FAKERINPUT_MANUFACTURER_STRING);
        string = FAKERINPUT_MANUFACTURER_STRING;
        break;
    case HID_STRING_ID_IPRODUCT:
        stringSizeCb = sizeof(FAKERINPUT_PRODUCT_STRING);
        string = FAKERINPUT_PRODUCT_STRING;
        break;
    case HID_STRING_ID_ISERIALNUMBER:
        stringSizeCb = sizeof(FAKERINPUT_SERIAL_NUMBER_STRING);
        string = FAKERINPUT_SERIAL_NUMBER_STRING;
        break;
    default:
        status = STATUS_INVALID_PARAMETER;
        KdPrint(("GetString: unkown string id %d\n", stringId));
        return status;
    }

    status = RequestCopyFromBuffer(Request, string, stringSizeCb);
    return status;
}