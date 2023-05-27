#pragma once

#include "stdafx.h"

#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3

#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)
#define FILE_READ_ACCESS          ( 0x0001 )    
#define FILE_WRITE_ACCESS         ( 0x0002 )    

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define HEIMDALL_DEVICE 0x8008
#define IOCTL_HEIMDALL_ADD_DEVICE				CTL_CODE(HEIMDALL_DEVICE, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HEIMDALL_REMOVE_DEVICE			CTL_CODE(HEIMDALL_DEVICE, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HEIMDALL_START_PLUG_NOTIFICTAION	CTL_CODE(HEIMDALL_DEVICE, 0x802, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_HEIMDALL_STOP_PLUG_NOTIFICTAION	CTL_CODE(HEIMDALL_DEVICE, 0x803, METHOD_NEITHER, FILE_ANY_ACCESS)
#define IOCTL_HEIMDALL_QUERY_DEVICE				CTL_CODE(HEIMDALL_DEVICE, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define DeviceIdMaximumLength 64
#define InstanceIdMaximumLength	64

typedef struct _IOCTL_PACKET
{
	WCHAR DeviceId[DeviceIdMaximumLength];
	WCHAR InstanceId[InstanceIdMaximumLength];
} IOCTL_PACKET, * PIOCTL_PACKET;