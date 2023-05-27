#pragma once
#include "stdafx.h"
#include "Heimdall.h"

#define DeviceIdMaximumLength 64
#define InstanceIdMaximumLength	64

#define DeviceIdMaximumSize DeviceIdMaximumLength * sizeof(WCHAR)
#define InstanceIdMaximumSize InstanceIdMaximumLength * sizeof(WCHAR)


typedef struct _MSDEVICE
{
	PWCHAR DeviceId;
	size_t DeviceIdLength;		// Length in bytes, including terminating null

	PWCHAR InstanceId;
	size_t InstanceIdLength;	// Length in bytes, including terminating null

	HANDLE hRegistryKey;		// Resereved
} MSDEVICE, * PMSDEVICE;

PMSDEVICE MsDeviceConstruct();
void MsDeviceDestroy(PMSDEVICE ThisDevice);
PMSDEVICE MsDeviceConstructInit(PWCHAR DeviceId, PWCHAR InstanceId);
NTSTATUS MsDeviceEqual(PMSDEVICE DeviceToCompare, PMSDEVICE ThisDevice);
NTSTATUS MsDeviceQueryId(PDEVICE_OBJECT DeviceObject, BUS_QUERY_ID_TYPE IdType, PWCHAR* pId);

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, MsDeviceConstruct)
#pragma alloc_text (PAGE, MsDeviceDestroy)
#pragma alloc_text (PAGE, MsDeviceConstructInit)
#pragma alloc_text (PAGE, MsDeviceEqual)
#pragma alloc_text (PAGE, MsDeviceQueryId)
#endif
