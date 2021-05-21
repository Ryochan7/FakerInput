/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    driver and application

--*/

#ifndef PUBLIC_H
#define PUBLIC_H

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_FakerInput,
    0xab67b0fa,0xd0f5,0x4f60,0x81,0xf4,0x34,0x6e,0x18,0xfd,0x08,0x05);
// {ab67b0fa-d0f5-4f60-81f4-346e18fd0805}

#define MAXIMUM_STRING_LENGTH           (126 * sizeof(WCHAR))
#define FAKERINPUT_MANUFACTURER_STRING    L"Ryochan7"  
#define FAKERINPUT_PRODUCT_STRING         L"FakerInput Device"  
#define FAKERINPUT_SERIAL_NUMBER_STRING   L"0.0.1"  
#define FAKERINPUT_DEVICE_STRING          L"FakerInput Device"
#define FAKERINPUT_DEVICE_STRING_INDEX    5

#endif // PUBLIC_H