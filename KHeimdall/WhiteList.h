#pragma once
#include "stdafx.h"
#include "MassStorageDevice.h"

typedef struct _WHITE_LIST_ENTRY
{
	PMSDEVICE Device;
	SINGLE_LIST_ENTRY Link;
} WHITE_LIST_ENTRY, * PWHITE_LIST_ENTRY;

NTSTATUS WhiteListInit();
NTSTATUS WhiteListAdd(PMSDEVICE Device);
NTSTATUS WhiteListRemove(PMSDEVICE Device);
NTSTATUS WhiteListIsDeviceAllowed(PMSDEVICE Device);