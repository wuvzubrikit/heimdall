#pragma once
#include "stdafx.h"

NTSTATUS    HeimdallDispatchGeneral(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS    HeimdallDispatchControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS    HeimdallDispatchPnp(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS    HeimdallStartDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS    HeimdallRemoveDevice(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, HeimdallDispatchGeneral)
#pragma alloc_text (PAGE, HeimdallDispatchControl)
#pragma alloc_text (PAGE, HeimdallDispatchPnp)
#pragma alloc_text (PAGE, HeimdallStartDevice)
#pragma alloc_text (PAGE, HeimdallRemoveDevice)
#endif