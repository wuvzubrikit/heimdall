#include "stdafx.h"
#include "Dispatch.h"
#include "Globals.h"
#include "Utils.h"
#include "IoControl.h"
#include "MassStorageDevice.h"
#include "WhiteList.h"

// // // // // // // //
//  Private members  //
// // // // // // // //

PMSDEVICE PluggedDevice;
PRKEVENT PlugNotificationEvent;

// // // // // // // //
// Private functions //
// // // // // // // //

NTSTATUS IrpCompletion(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context)
{
    UNREFERENCED_PARAMETER(DeviceObject);
    UNREFERENCED_PARAMETER(Irp);

    PKEVENT Event = (PKEVENT)Context;

    if (Event != NULL)
        KeSetEvent(Event, IO_NO_INCREMENT, FALSE);

    return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS ForwardIrpSynchronous(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PDEV_EXT FilterDeviceExt;
    KEVENT Event;
    NTSTATUS Status;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);
    FilterDeviceExt = (PDEV_EXT)DeviceObject->DeviceExtension;

    IoCopyCurrentIrpStackLocationToNext(Irp);

    IoSetCompletionRoutine(Irp, IrpCompletion, &Event, TRUE, TRUE, TRUE);

    Status = IoCallDriver(FilterDeviceExt->TargetDeviceObject, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
        Status = Irp->IoStatus.Status;
    }

    return Status;
}

// // // // // // // //
// Public functions  //
// // // // // // // //

NTSTATUS HeimdallDispatchGeneral(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PAGED_CODE();

    if (DeviceObject == gHeimdall.DeviceObject)
    {
        switch (IoGetCurrentIrpStackLocation(Irp)->MajorFunction)
        {
        case IRP_MJ_CREATE:
        case IRP_MJ_CLOSE:
            Irp->IoStatus.Status = STATUS_SUCCESS;
            Irp->IoStatus.Information = 0;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            return Irp->IoStatus.Status;

        case IRP_MJ_DEVICE_CONTROL:
            return HeimdallDispatchControl(DeviceObject, Irp);
        }

        Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return Irp->IoStatus.Status;
    }

    IoSkipCurrentIrpStackLocation(Irp);
    PDEV_EXT FilterDeviceExt = (PDEV_EXT)DeviceObject->DeviceExtension;

    return IoCallDriver(FilterDeviceExt->TargetDeviceObject, Irp);
}

NTSTATUS HeimdallDispatchControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) 
{
    UNREFERENCED_PARAMETER(DeviceObject);

    NTSTATUS Status = STATUS_INVALID_DEVICE_REQUEST;

    PIOCTL_PACKET IoCtlPacket;
    ULONG IoCtlPacketLength;
    
    PMSDEVICE Device;
    size_t DeviceIdLength;
    size_t InstanceIdLength;

    HANDLE hPlugNotificationEvent;

    PAGED_CODE();

    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
    ULONG ControlCode = Stack->Parameters.DeviceIoControl.IoControlCode;

    switch (ControlCode)
    {
        case IOCTL_HEIMDALL_ADD_DEVICE:
        case IOCTL_HEIMDALL_REMOVE_DEVICE:
        {
            IoCtlPacket = (PIOCTL_PACKET)Irp->AssociatedIrp.SystemBuffer;
            IoCtlPacketLength = Stack->Parameters.DeviceIoControl.InputBufferLength;

            if (IoCtlPacket == NULL || IoCtlPacketLength > sizeof(IOCTL_PACKET))
            {
                Status = STATUS_INVALID_BUFFER_SIZE;
                KdPrint(("Heimdall!HeimdallDispatchControl: invalid input buffer (0x%08x)\n", Status));
                break;
            }

            Status = RtlStringCbLengthW(IoCtlPacket->DeviceId, DeviceIdMaximumSize, &DeviceIdLength);
            if (!NT_SUCCESS(Status))
            {
                KdPrint(("Heimdall!HeimdallDispatchControl: failed to get device Id length (0x%08x)\n", Status));
                break;
            }

            Status = RtlStringCbLengthW(IoCtlPacket->InstanceId, DeviceIdMaximumSize, &InstanceIdLength);
            if (!NT_SUCCESS(Status))
            {
                KdPrint(("Heimdall!HeimdallDispatchControl: failed to get instance Id length (0x%08x)\n", Status));
                break;
            }

            Device = MsDeviceConstructInit(IoCtlPacket->DeviceId, IoCtlPacket->InstanceId);
            if (!Device)
            {
                KdPrint(("Heimdall!HeimdallDispatchControl: failed to init device\n"));
                Status = STATUS_UNSUCCESSFUL;
                break;
            }
    
            Status = (ControlCode == IOCTL_HEIMDALL_ADD_DEVICE)
                ? WhiteListAdd(Device)
                : WhiteListRemove(Device);

            break;
        }
        case IOCTL_HEIMDALL_START_PLUG_NOTIFICTAION:
        {
            hPlugNotificationEvent = (HANDLE)Stack->Parameters.DeviceIoControl.Type3InputBuffer;
            Status = ObReferenceObjectByHandle(
                hPlugNotificationEvent,
                EVENT_ALL_ACCESS,
                *ExEventObjectType,
                UserMode, (PVOID*)&PlugNotificationEvent, 
                NULL
            );

            if (!NT_SUCCESS(Status))
                KdPrint(("Heimdall!HeimdallDispatchControl: failed to reference notification event (0x%08x)\n", Status));

            break;
        }
        case IOCTL_HEIMDALL_STOP_PLUG_NOTIFICTAION:
        {
            ObDereferenceObject(PlugNotificationEvent);
            PlugNotificationEvent = NULL;
            break;
        }
        case IOCTL_HEIMDALL_QUERY_DEVICE:
        {
            Status = STATUS_SUCCESS;
            IoCtlPacket = (PIOCTL_PACKET)Irp->AssociatedIrp.SystemBuffer;
            IoCtlPacketLength = Stack->Parameters.DeviceIoControl.OutputBufferLength;

            if (IoCtlPacket == NULL || IoCtlPacketLength > sizeof(IOCTL_PACKET))
            {
                Status = STATUS_INVALID_BUFFER_SIZE;
                KdPrint(("Heimdall!HeimdallDispatchControl: invalid output buffer (0x%08x)\n", Status));
                break;
            }

            if (PluggedDevice != NULL)
            {
                RtlCopyMemory(IoCtlPacket->DeviceId, PluggedDevice->DeviceId, PluggedDevice->DeviceIdLength);
                RtlCopyMemory(IoCtlPacket->InstanceId, PluggedDevice->InstanceId, PluggedDevice->InstanceIdLength);
            }
            else
                Status = STATUS_UNSUCCESSFUL;

            if (!NT_SUCCESS(Status))
            {
                KdPrint(("Heimdall!HeimdallDispatchControl: failed to build IOCTL packet (0x%08x)\n", Status));
                Irp->IoStatus.Information = 0;
            }
            else 
                Irp->IoStatus.Information = sizeof(IOCTL_PACKET);
        
            break;
        }
    }

    Irp->IoStatus.Status = Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}

NTSTATUS HeimdallDispatchPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS Status = Irp->IoStatus.Status;
    BOOLEAN DeviceIsLocked = FALSE;

    PAGED_CODE();

    PDEV_EXT FilterDeviceExt = (PDEV_EXT)DeviceObject->DeviceExtension;
    Status = IoAcquireRemoveLock(&FilterDeviceExt->RemoveLock, Irp);

    if (!NT_SUCCESS(Status)) {

        KdPrint(("Heimdall!HeimdallDispatchPnp: failed acquiring remove lock (0x%08x)\n", Status));
        Irp->IoStatus.Status = Status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return Status;
    }

    DeviceIsLocked = TRUE;

    switch (IoGetCurrentIrpStackLocation(Irp)->MinorFunction)
    {

    case IRP_MN_START_DEVICE:
        Status = HeimdallStartDevice(DeviceObject, Irp);
        break;

    case IRP_MN_REMOVE_DEVICE:
        Status = HeimdallRemoveDevice(DeviceObject, Irp);
        DeviceIsLocked = FALSE;
        break;
    default:
        Status = HeimdallDispatchGeneral(DeviceObject, Irp);
    }

    if (DeviceIsLocked)
        IoReleaseRemoveLock(&FilterDeviceExt->RemoveLock, Irp);

    return Status;
}

NTSTATUS HeimdallStartDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS Status;
    PMSDEVICE Device    =   NULL;
    PWCHAR DeviceId     =   NULL;
    PWCHAR InstanceId   =   NULL;

    PAGED_CODE();

    PDEV_EXT FilterDeviceExt = (PDEV_EXT)DeviceObject->DeviceExtension;

    BREAKABLE_SECTION_START

    Status = MsDeviceQueryId(FilterDeviceExt->TargetDeviceObject, BusQueryDeviceID, &DeviceId);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!HeimdallStartDevice: failed to allocate buffer for device ID (0x%08x)\n", Status));
        break;
    }

    Status = MsDeviceQueryId(FilterDeviceExt->TargetDeviceObject, BusQueryInstanceID, &InstanceId);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!HeimdallStartDevice: failed to allocate buffer for instance ID (0x%08x)\n", Status));
        break;
    }

    Device = MsDeviceConstructInit(DeviceId, InstanceId);
    if (!Device)
    {
        KdPrint(("Heimdall!HeimdallStartDevice: failed to init device\n"));
        Status = STATUS_UNSUCCESSFUL;
        break;
    }

    Status = WhiteListIsDeviceAllowed(Device);
    if (Status == STATUS_ACCESS_DENIED)
    {
        if (!PlugNotificationEvent) break;
        
        if (PluggedDevice != NULL) MsDeviceDestroy(PluggedDevice);
        PluggedDevice = MsDeviceConstructInit(Device->DeviceId, Device->InstanceId);

        if (!PluggedDevice)
            KdPrint(("Heimdall!HeimdallStartDevice: failed to save plugged device\n"));
        else
        {
            KdPrint(("Heimdall!HeimdallStartDevice: device plug notification\n"));
            KeSetEvent(PlugNotificationEvent, 0, FALSE);
        }
        
        break;
    }

    Status = ForwardIrpSynchronous(DeviceObject, Irp);

    BREAKABLE_SECTION_END
    
    if (DeviceId != NULL) HmdlFreePool(DeviceId);
    if (InstanceId != NULL) HmdlFreePool(InstanceId);
    MsDeviceDestroy(Device);

    Irp->IoStatus.Status = Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}

NTSTATUS HeimdallRemoveDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    NTSTATUS Status = STATUS_SUCCESS;

    PAGED_CODE();

    PDEV_EXT FilterDeviceExt = (PDEV_EXT)DeviceObject->DeviceExtension;

    IoReleaseRemoveLockAndWait(&FilterDeviceExt->RemoveLock, Irp);

    Status = HeimdallDispatchGeneral(DeviceObject, Irp);

    IoDetachDevice(FilterDeviceExt->TargetDeviceObject);
    IoDeleteDevice(DeviceObject);

    return Status;
}
