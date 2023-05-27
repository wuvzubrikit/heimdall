#pragma once
#include "stdafx.h"

#define	HMDL_POOL	'ldmh'
#define	HMDL_LNK	L"\\??\\Heimdall"
#define	HMDL_DEV	L"\\Device\\Heimdall"

typedef struct _HEIMDALL
{
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING RegistryPath;
} HEIMDALL, * PHEIMDALL;

typedef struct _DEV_EXT {
	PDEVICE_OBJECT DeviceObject;
	PDEVICE_OBJECT TargetDeviceObject;
	PDEVICE_OBJECT PhysicalDeviceObject;
	IO_REMOVE_LOCK RemoveLock;
} DEV_EXT, * PDEV_EXT;