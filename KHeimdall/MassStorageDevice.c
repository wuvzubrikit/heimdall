#include "stdafx.h"
#include "MassStorageDevice.h"
#include "Globals.h"
#include "Utils.h"

PMSDEVICE MsDeviceConstruct()
{
    PMSDEVICE ThisDevice = (PMSDEVICE)HmdlAllocatePool(PagedPool, sizeof(MSDEVICE));
    if (!ThisDevice) return NULL;

    return ThisDevice;
}

void MsDeviceDestroy(PMSDEVICE ThisDevice)
{
    PAGED_CODE();

    if (!ThisDevice) return;
    if (ThisDevice->DeviceId != NULL) HmdlFreePool(ThisDevice->DeviceId);
    if (ThisDevice->InstanceId != NULL) HmdlFreePool(ThisDevice->InstanceId);
    HmdlFreePool(ThisDevice);
}

PMSDEVICE MsDeviceConstructInit(PWCHAR DeviceId, PWCHAR InstanceId)
{
    NTSTATUS Status = STATUS_SUCCESS;
    size_t DeviceIdLength;
    size_t InstanceIdLength;

    PMSDEVICE ThisDevice = NULL;

    PAGED_CODE();

    BREAKABLE_SECTION_START

    ThisDevice = MsDeviceConstruct();
    if (!ThisDevice)
    {
        KdPrint(("Heimdall!MsDeviceConstructInit: failed to allocate MSDEVICE\n"));
        break;
    }

    Status = RtlStringCbLengthW(DeviceId, DeviceIdMaximumSize, &DeviceIdLength);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!MsDeviceConstructInit: failed to get device Id length (0x%08x)\n", Status));
        break;
    }

    Status = RtlStringCbLengthW(InstanceId, DeviceIdMaximumSize, &InstanceIdLength);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!MsDeviceConstructInit: failed to get instance Id length (0x%08x)\n", Status));
        break;
    }

    DeviceIdLength += sizeof(UNICODE_NULL);
    ThisDevice->DeviceIdLength = DeviceIdLength;
    ThisDevice->DeviceId = (PWCHAR)HmdlAllocatePool(PagedPool, ThisDevice->DeviceIdLength);

    InstanceIdLength += sizeof(UNICODE_NULL);
    ThisDevice->InstanceIdLength = InstanceIdLength;
    ThisDevice->InstanceId = (PWCHAR)HmdlAllocatePool(PagedPool, ThisDevice->InstanceIdLength);

    Status = RtlStringCbCopyW(ThisDevice->DeviceId, ThisDevice->DeviceIdLength, DeviceId);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!MsDeviceConstructInit: failed to copy device Id (0x%08x)\n", Status));
        break;
    }

    Status = RtlStringCbCopyW(ThisDevice->InstanceId, ThisDevice->InstanceIdLength, InstanceId);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!MsDeviceConstructInit: failed to copy instance Id (0x%08x)\n", Status));
        break;
    }

    BREAKABLE_SECTION_END

    if (!NT_SUCCESS(Status)) MsDeviceDestroy(ThisDevice);

    return ThisDevice;
}

NTSTATUS MsDeviceEqual(PMSDEVICE DeviceToCompare, PMSDEVICE ThisDevice)
{
    PAGED_CODE();

    if (RtlCompareUnicodeStrings(
        ThisDevice->DeviceId,
        ThisDevice->DeviceIdLength / sizeof(WCHAR),
        DeviceToCompare->DeviceId,
        DeviceToCompare->DeviceIdLength / sizeof(WCHAR),
        TRUE) != 0) return STATUS_UNSUCCESSFUL;

    if (RtlCompareUnicodeStrings(
        ThisDevice->InstanceId,
        ThisDevice->InstanceIdLength / sizeof(WCHAR),
        DeviceToCompare->InstanceId,
        DeviceToCompare->InstanceIdLength / sizeof(WCHAR),
        TRUE) != 0) return STATUS_UNSUCCESSFUL;

    return STATUS_SUCCESS;
}

NTSTATUS MsDeviceQueryId(PDEVICE_OBJECT DeviceObject, BUS_QUERY_ID_TYPE IdType, PWCHAR* pId) 
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;

    PIRP Irp;
    PIO_STACK_LOCATION StackLocation;
    IO_STATUS_BLOCK iosb;
    KEVENT Event;

    PAGED_CODE();

    BREAKABLE_SECTION_START

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Irp = IoBuildSynchronousFsdRequest(
        IRP_MJ_PNP,
        DeviceObject,
        NULL,
        0,
        NULL,
        &Event,
        &iosb
    );

    if (!Irp)
    {
        KdPrint(("Heimdall!QueryId: failed to build Irp\n"));
        break;
    }

    StackLocation = IoGetNextIrpStackLocation(Irp);
    StackLocation->MinorFunction = IRP_MN_QUERY_ID;
    StackLocation->Parameters.QueryId.IdType = IdType;

    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

    Status = IoCallDriver(DeviceObject, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
        Status = iosb.Status;
        if (!NT_SUCCESS(Status))
        {
            KdPrint(("Heimdall!QueryId: failed to wait for Irp (0x%08x)\n", Status));
            break;
        }
    }
    else if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!QueryId: failed to pass Irp to next driver (0x%08x)\n", Status));
        break;
    }

    PWCHAR Info;
    size_t IdLength;

    if (IdType == BusQueryDeviceID)
    {
        Info = (PWCHAR)((PUCHAR)iosb.Information + sizeof(L"USBSTOR"));
        Status = RtlStringCbLengthW(Info, DeviceIdMaximumSize, &IdLength);
        if (!NT_SUCCESS(Status))
        {
            KdPrint(("Heimdall!QueryId: failed to get device Id length (0x%08x)\n", Status));
            break;
        }
    }
    else
    { 
        Info = (PWCHAR)(iosb.Information);
        Status = RtlStringCbLengthW(Info, DeviceIdMaximumSize, &IdLength);
        if (!NT_SUCCESS(Status))
        {
            KdPrint(("Heimdall!QueryId: failed to get instance Id length (0x%08x)\n", Status));
            break;
        }
    }
    
    IdLength += sizeof(UNICODE_NULL);
    *pId = (PWCHAR)HmdlAllocatePool(PagedPool, IdLength);

    Status = RtlStringCbCopyW(*pId, IdLength, Info);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!QueryId: failed to copy queued Id (0x%08x)\n", Status));
        break;
    }

    BREAKABLE_SECTION_END

    return Status;
}
