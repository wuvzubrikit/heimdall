#include "stdafx.h"
#include "Main.h"
#include "Globals.h"
#include "Utils.h"
#include "Dispatch.h"
#include "IoControl.h"
#include "WhiteList.h"

// // // // // // // //
// Private functions //
// // // // // // // //

NTSTATUS CheckTargetPdo(PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS Status;

    UNICODE_STRING TargetDriverName;
    RtlInitUnicodeString(&TargetDriverName, L"\\Driver\\USBSTOR");

    PDRIVER_OBJECT TargetPdo;

    Status = ObReferenceObjectByName(
        &TargetDriverName,
        OBJ_CASE_INSENSITIVE,
        NULL,
        GENERIC_READ,
        *IoDriverObjectType,
        KernelMode,
        NULL,
        (PVOID*)&TargetPdo);

    if (!NT_SUCCESS(Status))
    {
        Status = STATUS_NOT_SUPPORTED;
        return Status;
    }

    if (PhysicalDeviceObject != TargetPdo->DeviceObject)
        Status = STATUS_NOT_SUPPORTED;

    ObDereferenceObject(TargetPdo);

    return Status;
}

// // // // // // // //
// Public functions  //
// // // // // // // //

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS Status = STATUS_SUCCESS;

    gHeimdall.RegistryPath.MaximumLength = RegistryPath->Length + sizeof(UNICODE_NULL);
    gHeimdall.RegistryPath.Length = RegistryPath->Length;
    gHeimdall.RegistryPath.Buffer = (PWCH)HmdlAllocatePool(PagedPool, gHeimdall.RegistryPath.MaximumLength);

    if (gHeimdall.RegistryPath.Buffer != NULL)
        RtlCopyUnicodeString(&gHeimdall.RegistryPath, RegistryPath);
    else
    {
        gHeimdall.RegistryPath.Length = 0;
        gHeimdall.RegistryPath.MaximumLength = 0;
    }

    for (ULONG i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
        DriverObject->MajorFunction[i] = HeimdallDispatchGeneral;
    DriverObject->MajorFunction[IRP_MJ_PNP] = HeimdallDispatchPnp;

    DriverObject->DriverExtension->AddDevice = HeimdallAddDevice;
    DriverObject->DriverUnload = HeimdallUnload;

    Status = ControlInit(DriverObject);
    if (!NT_SUCCESS(Status))
        KdPrint(("Heimdall!DriverEntry: failed to init control (0x%08x)\n", Status));

    gWhiteListHead.Next = NULL;
    Status = WhiteListInit();
    if (!NT_SUCCESS(Status))
        KdPrint(("Heimdall!DriverEntry: failed to init white list (0x%08x)\n", Status));

    return Status;
}

void HeimdallUnload(PDRIVER_OBJECT DriverObject)
{
    PAGED_CODE();

    UNICODE_STRING DeviceSymLink = RTL_CONSTANT_STRING(HMDL_LNK);
    IoDeleteSymbolicLink(&DeviceSymLink);
    IoDeleteDevice(DriverObject->DeviceObject);

    return;
}

NTSTATUS HeimdallAddDevice(PDRIVER_OBJECT DriverObject, PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS Status = STATUS_SUCCESS;
    PDEVICE_OBJECT FiDO;

    PAGED_CODE();

    Status = CheckTargetPdo(PhysicalDeviceObject);
    if (Status == STATUS_NOT_SUPPORTED)
        return STATUS_SUCCESS;

    Status = IoCreateDevice(DriverObject,
        sizeof(DEV_EXT),
        NULL,
        PhysicalDeviceObject->DeviceType,
        PhysicalDeviceObject->Characteristics,
        FALSE,
        &FiDO);

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!HeimdallAddDevice: Cannot create filter device object (0x%08x)\n", Status));
        return Status;
    }

    FiDO->Flags |= DO_BUFFERED_IO | DO_DIRECT_IO | DO_POWER_PAGABLE;

    PDEV_EXT FilterDeviceExt = (PDEV_EXT)FiDO->DeviceExtension;
    FilterDeviceExt->PhysicalDeviceObject = PhysicalDeviceObject;
    Status = IoAttachDeviceToDeviceStackSafe(FiDO, PhysicalDeviceObject, &FilterDeviceExt->TargetDeviceObject);

    if (!NT_SUCCESS(Status))
    {
        IoDeleteDevice(FiDO);
        KdPrint(("Heimdall!HeimdallAddDevice: failed to attach filter device to device stack (%x08x)\n", Status));
        return Status;
    }

    FilterDeviceExt->DeviceObject = FiDO;

    IoInitializeRemoveLock(&FilterDeviceExt->RemoveLock, HMDL_POOL, 1, 0);

    FiDO->Flags &= ~DO_DEVICE_INITIALIZING;

    return Status;
}
