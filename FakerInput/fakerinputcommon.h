#ifndef FAKERINPUTCOMMON_H
#define FAKERINPUTCOMMON_H

#include <minwindef.h>

#define REPORTID_KEYBOARD 0x02
#define REPORTID_MOUSE          0x03
#define REPORTID_RELATIVE_MOUSE 0x04
#define REPORTID_ENHANCED_KEY   0x08
#define REPORTID_CONTROL  0x40

#define CONTROL_REPORT_SIZE 0x41

typedef struct _FAKERINPUT_CONTROL_REPORT_HEADER
{

    BYTE        ReportID;

    BYTE        ReportLength;

} FakerInputControlReportHeader;

//
// Keyboard specific report infomation
//

#define KBD_LCONTROL_BIT     1
#define KBD_LSHIFT_BIT       2
#define KBD_LALT_BIT         4
#define KBD_LGUI_BIT         8
#define KBD_RCONTROL_BIT     16
#define KBD_RSHIFT_BIT       32
#define KBD_RALT_BIT         64
#define KBD_RGUI_BIT         128

#define KBD_KEY_CODES        6

typedef struct _FAKERINPUT_KEYBOARD_REPORT
{

    BYTE      ReportID;

    // Left Control, Left Shift, Left Alt, Left GUI
    // Right Control, Right Shift, Right Alt, Right GUI
    BYTE      ShiftKeyFlags;

    BYTE      Reserved;

    // See http://www.usb.org/developers/devclass_docs/Hut1_11.pdf
    // for a list of key codes
    BYTE      KeyCodes[KBD_KEY_CODES];

} FakerInputKeyboardReport;

typedef struct _FAKERINPUT_MULTIMEDIA_REPORT
{
    BYTE        ReportID;

    BYTE        MultimediaKeys;

    BYTE        ExtraKeys;

} FakerInputMultimediaReport;

//
// Relative mouse specific report information
//

#define RELATIVE_MOUSE_MIN_COORDINATE   -32767
#define RELATIVE_MOUSE_MAX_COORDINATE   32767

typedef struct _FAKERINPUT_RELATIVE_MOUSE_REPORT
{

    BYTE        ReportID;

    BYTE        Button;

    SHORT      XValue;

    SHORT      YValue;

    BYTE        WheelPosition;

    BYTE        HWheelPosition;

} FakerInputRelativeMouseReport;

#endif // FAKERINPUTCOMMON_H