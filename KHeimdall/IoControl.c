#include "stdafx.h"
#include "IoControl.h"
#include "Globals.h"
#include "Utils.h"

NTSTATUS ControlInit(PDRIVER_OBJECT DriverObject)
{
    NTSTATUS Status;
    UNICODE_STRING DeviceName;
    RtlInitUnicodeString(&DeviceName, HMDL_DEV);

    PDEVICE_OBJECT DeviceObject;
    Status = IoCreateDevice(
        DriverObject,
        0,
        &DeviceName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &DeviceObject
    );

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!InitControl: failed to create device object (0x%08x)\n", Status));
        return Status;
    }

    UNICODE_STRING DeviceSymLink;
    RtlInitUnicodeString(&DeviceSymLink, HMDL_LNK);
    Status = IoCreateSymbolicLink(&DeviceSymLink, &DeviceName);

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!InitControl: failed to create symbolic link (0x%08x)\n", Status));
        IoDeleteDevice(DeviceObject);
        return Status;
    }

    gHeimdall.DeviceObject = DeviceObject;

    return Status;
}
