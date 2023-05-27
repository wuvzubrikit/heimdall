#pragma once

#include "stdafx.h"

NTSTATUS DriverEntry(
	PDRIVER_OBJECT DriverObject, 
	PUNICODE_STRING RegistryPath
);

void HeimdallUnload(
	PDRIVER_OBJECT DriverObject
);

NTSTATUS HeimdallAddDevice(
	PDRIVER_OBJECT DriverObject, 
	PDEVICE_OBJECT PhysicalDeviceObject
);

extern NTSYSAPI NTSTATUS NTAPI ObReferenceObjectByName(
	PUNICODE_STRING ObjectPath, 
	ULONG Attributes, 
	PACCESS_STATE PassedAccessState,
	ACCESS_MASK DesiredAccess,
	POBJECT_TYPE ObjectType,
	KPROCESSOR_MODE AccessMode,
	PVOID ParseContext,
	PVOID * Object
);

extern POBJECT_TYPE* IoDriverObjectType;


#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, HeimdallAddDevice)
#pragma alloc_text (PAGE, HeimdallUnload)
#endif