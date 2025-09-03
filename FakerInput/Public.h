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

// New GUID for FakerInput
// {0E5AF65E-8134-425A-A5B7-8D97E60A3D60}
DEFINE_GUID(GUID_DEVINTERFACE_FakerInput,
    0xe5af65e, 0x8134, 0x425a, 0xa5, 0xb7, 0x8d, 0x97, 0xe6, 0xa, 0x3d, 0x60);

// OLD GUID from vmulti
//DEFINE_GUID (GUID_DEVINTERFACE_FakerInput,
//    0xab67b0fa,0xd0f5,0x4f60,0x81,0xf4,0x34,0x6e,0x18,0xfd,0x08,0x05);
//// {ab67b0fa-d0f5-4f60-81f4-346e18fd0805}

#define MAXIMUM_STRING_LENGTH           (126 * sizeof(WCHAR))
#define FAKERINPUT_MANUFACTURER_STRING    L"Ryochan7"  
#define FAKERINPUT_PRODUCT_STRING         L"FakerInput Device"  
#define FAKERINPUT_SERIAL_NUMBER_STRING   L"0.0.1"  
#define FAKERINPUT_DEVICE_STRING          L"FakerInput Device"
#define FAKERINPUT_DEVICE_STRING_INDEX    5

#endif // PUBLIC_H