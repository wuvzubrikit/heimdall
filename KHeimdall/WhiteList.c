#include "stdafx.h"
#include "WhiteList.h"
#include "Globals.h"
#include "Utils.h"

// // // // // // // //
//  Private members  //
// // // // // // // //

BOOLEAN bIsInitializing;
HANDLE hWhiteListRegistryKey;

// // // // // // // //
// Private functions //
// // // // // // // //

NTSTATUS WhiteListAddToRegistry(PMSDEVICE Device)
{
    NTSTATUS Status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES Attributies;

    HANDLE AllowedDeviceKey = 0;
    HANDLE AllowedDeviceSubKey = 0;

    UNICODE_STRING AllowedDeviceKeyName;
    UNICODE_STRING AllowedDeviceSubKeyName;

    BREAKABLE_SECTION_START

    RtlInitUnicodeString(&AllowedDeviceKeyName, Device->DeviceId);
    InitializeObjectAttributes(
        &Attributies,
        &AllowedDeviceKeyName,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        hWhiteListRegistryKey,
        NULL
    );

    Status = ZwCreateKey(
        &AllowedDeviceKey,
        KEY_WRITE,
        &Attributies,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        NULL
    );

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListAddToRegistry: failed to create device Id subkey (0x%08x)\n", Status));
        break;
    }

    RtlInitUnicodeString(&AllowedDeviceSubKeyName, Device->InstanceId);
    InitializeObjectAttributes(
        &Attributies,
        &AllowedDeviceSubKeyName,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        AllowedDeviceKey,
        NULL
    );

    Status = ZwCreateKey(
        &AllowedDeviceSubKey,
        KEY_WRITE,
        &Attributies,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        NULL
    );

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListAddToRegistry: failed to create instance Id value (0x%08x)\n", Status));
        Status = ZwDeleteKey(AllowedDeviceKey);
        break;
    }

    BREAKABLE_SECTION_END

    ZwClose(AllowedDeviceSubKey);
    ZwClose(AllowedDeviceKey);

    return Status;
}

NTSTATUS WhiteListRemoveFromRegistry(PMSDEVICE Device)
{
    NTSTATUS Status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES Attributies;

    HANDLE AllowedDeviceKey = 0;
    HANDLE AllowedDeviceSubKey = 0;

    UNICODE_STRING AllowedDeviceKeyName;
    UNICODE_STRING AllowedDeviceSubKeyName;

    BREAKABLE_SECTION_START

    RtlInitUnicodeString(&AllowedDeviceKeyName, Device->DeviceId);
    InitializeObjectAttributes(
        &Attributies,
        &AllowedDeviceKeyName,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        hWhiteListRegistryKey,
        NULL
    );

    Status = ZwCreateKey(
        &AllowedDeviceKey,
        KEY_WRITE,
        &Attributies,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        NULL);

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListRemoveFromRegistry: failed to open deice Id key (0x%08x)\n", Status));
        break;
    }

    RtlInitUnicodeString(&AllowedDeviceSubKeyName, Device->InstanceId);
    InitializeObjectAttributes(
        &Attributies,
        &AllowedDeviceSubKeyName,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        AllowedDeviceKey,
        NULL
    );

    Status = ZwCreateKey(
        &AllowedDeviceSubKey,
        KEY_WRITE,
        &Attributies,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        NULL
    );

    Status = ZwDeleteKey(AllowedDeviceSubKey);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListRemoveFromRegistry: failed to delete InstanceId value (0x%08x)\n", Status));
        break;
    }

    BREAKABLE_SECTION_END

    ZwClose(AllowedDeviceSubKey);
    ZwClose(AllowedDeviceKey);

    return Status;
}

// // // // // // // //
// Public functions  //
// // // // // // // //

NTSTATUS WhiteListInit()
{
    NTSTATUS Status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES Attributies;

    HANDLE Root = 0;
    HANDLE AllowedDeviceKey = 0;

    PWCHAR DeviceId;
    PWCHAR InstanceId;

    PKEY_BASIC_INFORMATION KeyInfo;
    PKEY_BASIC_INFORMATION SubKeyInfo;

    ULONG ReturnedLength;

    PMSDEVICE Device;

    if (gWhiteListHead.Next != NULL)
        return STATUS_UNSUCCESSFUL;

    bIsInitializing = TRUE;

    BREAKABLE_SECTION_START

    InitializeObjectAttributes(
        &Attributies,
        &gHeimdall.RegistryPath,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );

    Status = ZwOpenKey(&Root, KEY_CREATE_SUB_KEY, &Attributies);
    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListInit: failed to open root key handle (0x%08x)\n", Status));
        break;
    }

    UNICODE_STRING AllowedDevicesKeyName;
    RtlInitUnicodeString(&AllowedDevicesKeyName, L"White list");
    InitializeObjectAttributes(
        &Attributies,
        &AllowedDevicesKeyName,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        Root,
        NULL
    );

    Status = ZwCreateKey(
        &hWhiteListRegistryKey,
        KEY_ALL_ACCESS,
        &Attributies,
        0,
        NULL,
        REG_OPTION_NON_VOLATILE,
        NULL
    );

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListInit: failed to open allowed devices key handle (0x%08x)\n", Status));
        break;
    }

    for (ULONG DeviceIdIndex = 0; ; DeviceIdIndex++)
    {
        ULONG KeyInfoLength = sizeof(KEY_BASIC_INFORMATION);
        KeyInfo = (PKEY_BASIC_INFORMATION)HmdlAllocatePool(PagedPool, KeyInfoLength);
        if (!KeyInfo)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        do
        {
            Status = ZwEnumerateKey(
                hWhiteListRegistryKey,
                DeviceIdIndex,
                KeyBasicInformation,
                KeyInfo,
                KeyInfoLength,
                &ReturnedLength
            );

            if (Status == STATUS_BUFFER_OVERFLOW)
            {
                HmdlFreePool(KeyInfo);
                KeyInfoLength = ReturnedLength;
                KeyInfo = (PKEY_BASIC_INFORMATION)HmdlAllocatePool(PagedPool, KeyInfoLength);
                if (!KeyInfo) Status = STATUS_INSUFFICIENT_RESOURCES;
            }

        } while (Status == STATUS_BUFFER_OVERFLOW);

        if (!NT_SUCCESS(Status))
        {
            if (Status != STATUS_NO_MORE_ENTRIES)
                KdPrint(("Heimdall!WhiteListInit: failed to enumerate devices on index %d (0x%0xx)\n", DeviceIdIndex, Status));
            break;
        }

        DeviceId = (PWCHAR)HmdlAllocatePool(PagedPool, KeyInfo->NameLength + sizeof(UNICODE_NULL));
        if (!DeviceId)
        {
            KdPrint(("Heimdall!WhiteListInit: failed to allocate buffer for device Id\n"));
            continue;
        }

        RtlCopyMemory(DeviceId, KeyInfo->Name, KeyInfo->NameLength);
        DeviceId[KeyInfo->NameLength / sizeof(WCHAR)] = UNICODE_NULL;

        UNICODE_STRING AllowedDeviceKeyName;
        RtlInitUnicodeString(&AllowedDeviceKeyName, DeviceId);
        InitializeObjectAttributes(
            &Attributies,
            &AllowedDeviceKeyName,
            OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
            hWhiteListRegistryKey,
            NULL
        );

        Status = ZwCreateKey(
            &AllowedDeviceKey,
            KEY_READ,
            &Attributies,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            NULL
        );

        if (!NT_SUCCESS(Status))
        {
            KdPrint(("Heimdall!WhiteListInit: failed to open allowed device key (0x%08x)\n", Status));
            break;
        }

        for (ULONG InstanceIdIndex = 0; ; InstanceIdIndex++)
        {
            ULONG SubKeyInfoLength = sizeof(KEY_BASIC_INFORMATION);
            SubKeyInfo = (PKEY_BASIC_INFORMATION)HmdlAllocatePool(PagedPool, SubKeyInfoLength);
            if (!SubKeyInfo)
            {
                KdPrint(("Heimdall!WhiteListInit: failed to allocate subkey info structure (0x%08x)\n", Status));
                Status = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }

            do
            {
                Status = ZwEnumerateKey(
                    AllowedDeviceKey,
                    InstanceIdIndex,
                    KeyBasicInformation,
                    SubKeyInfo,
                    SubKeyInfoLength,
                    &ReturnedLength
                );

                if (Status == STATUS_BUFFER_OVERFLOW)
                {
                    HmdlFreePool(SubKeyInfo);
                    SubKeyInfoLength = ReturnedLength;
                    SubKeyInfo = (PKEY_BASIC_INFORMATION)HmdlAllocatePool(PagedPool, SubKeyInfoLength);
                    if (!SubKeyInfo) Status = STATUS_INSUFFICIENT_RESOURCES;
                }

            } while (Status == STATUS_BUFFER_OVERFLOW);

            if (!NT_SUCCESS(Status))
            {
                KdPrint(("Heimdall!WhiteListInit: failed to enumerate instances on index %d (0x%08x)\n", InstanceIdIndex, Status));
                break;
            }

            InstanceId = (PWCHAR)HmdlAllocatePool(PagedPool, SubKeyInfo->NameLength + sizeof(UNICODE_NULL));
            if (!InstanceId)
            {
                KdPrint(("Heimdall!WhiteListInit: failed to allocate buffer for instance Id\n"));
                continue;
            }

            RtlCopyMemory(InstanceId, SubKeyInfo->Name, SubKeyInfo->NameLength);
            InstanceId[SubKeyInfo->NameLength / sizeof(WCHAR)] = UNICODE_NULL;

            Device = MsDeviceConstructInit(DeviceId, InstanceId);
            HmdlFreePool(DeviceId);
            HmdlFreePool(InstanceId);

            if (Device != NULL) Status = WhiteListAdd(Device);
            else Status = STATUS_UNSUCCESSFUL;

            HmdlFreePool(SubKeyInfo);
        }

        if (!NT_SUCCESS(Status)) HmdlFreePool(SubKeyInfo);

        HmdlFreePool(KeyInfo);
        ZwClose(AllowedDeviceKey);
    }

    if (!NT_SUCCESS(Status)) HmdlFreePool(KeyInfo);

    if (Status == STATUS_NO_MORE_ENTRIES)
        Status = STATUS_SUCCESS;

    BREAKABLE_SECTION_END

    bIsInitializing = FALSE;

    return Status;
}

NTSTATUS WhiteListAdd(PMSDEVICE Device)
{
    NTSTATUS Status = STATUS_SUCCESS;

    PWHITE_LIST_ENTRY AllowedDeviceNewEntry;
    PWHITE_LIST_ENTRY AllowedDeviceEntry;
    PSINGLE_LIST_ENTRY WhiteListEntryLink = &gWhiteListHead;

    while (WhiteListEntryLink->Next != NULL)
    {
        WhiteListEntryLink = WhiteListEntryLink->Next;
        AllowedDeviceEntry = CONTAINING_RECORD(WhiteListEntryLink, WHITE_LIST_ENTRY, Link);

        Status = MsDeviceEqual(Device, AllowedDeviceEntry->Device);
        if (NT_SUCCESS(Status))
        {
            KdPrint(("Heimdall!WhiteListAdd: device is already in white list\n"));
            return STATUS_REQUEST_ABORTED;
        }
    }

    Status = bIsInitializing 
        ? STATUS_SUCCESS
        : WhiteListAddToRegistry(Device);

    if (!NT_SUCCESS(Status))
    {
        KdPrint(("Heimdall!WhiteListAdd: failed to add device to registry\n"));
        return Status;
    }
    
    AllowedDeviceNewEntry = (PWHITE_LIST_ENTRY)HmdlAllocatePool(PagedPool, sizeof(WHITE_LIST_ENTRY));
    if (!AllowedDeviceNewEntry)
    {
        KdPrint(("Heimdall!WhiteListAdd: failed to allocate WHITE_LIST_ENTRY\n"));
        return STATUS_MEMORY_NOT_ALLOCATED;
    }

    AllowedDeviceNewEntry->Device = Device;
    AllowedDeviceNewEntry->Link.Next = NULL;
    WhiteListEntryLink->Next = &AllowedDeviceNewEntry->Link;

    return Status;
}

NTSTATUS WhiteListRemove(PMSDEVICE Device)
{
    NTSTATUS Status = STATUS_SUCCESS;

    PWHITE_LIST_ENTRY AllowedDeviceEntry;
    PSINGLE_LIST_ENTRY WhiteListEntryLink = gWhiteListHead.Next;
    PSINGLE_LIST_ENTRY WhiteListEntryBlink = &gWhiteListHead;


    if (!WhiteListEntryLink)
    {
        KdPrint(("Heimdall!WhiteListRemove: white list is empty\n"));
        return STATUS_REQUEST_ABORTED;
    }

    do
    {
        AllowedDeviceEntry = CONTAINING_RECORD(WhiteListEntryLink, WHITE_LIST_ENTRY, Link);

        Status = MsDeviceEqual(Device, AllowedDeviceEntry->Device);
        if (NT_SUCCESS(Status))
        {
            Status = WhiteListRemoveFromRegistry(AllowedDeviceEntry->Device);
            if (!NT_SUCCESS(Status))
            {
                KdPrint(("Heimdall!WhiteListRemove: failed to removed device from registry\n"));
                return Status;
            }

            WhiteListEntryBlink->Next = WhiteListEntryLink->Next;
            MsDeviceDestroy(AllowedDeviceEntry->Device);
            HmdlFreePool(AllowedDeviceEntry);

            return STATUS_SUCCESS;
        }

        WhiteListEntryBlink = WhiteListEntryLink;
        WhiteListEntryLink = WhiteListEntryLink->Next;

    } while (WhiteListEntryLink != NULL);

    KdPrint(("Heimdall!WhiteListRemove: requested device is not in white list\n"));

    return STATUS_NOT_FOUND;
}

NTSTATUS WhiteListIsDeviceAllowed(PMSDEVICE Device)
{
    NTSTATUS Status;

    PWHITE_LIST_ENTRY AllowedDeviceEntry;
    PSINGLE_LIST_ENTRY WhiteListEntryLink = gWhiteListHead.Next;

    while (WhiteListEntryLink != NULL)
    {
        AllowedDeviceEntry = CONTAINING_RECORD(WhiteListEntryLink, WHITE_LIST_ENTRY, Link);

        Status = MsDeviceEqual(Device, AllowedDeviceEntry->Device);
        if (NT_SUCCESS(Status)) return Status;

        WhiteListEntryLink = WhiteListEntryLink->Next;
    }

    KdPrint(("Heimdall!WhiteListIsDeviceAllowed: access denied for \"%ws\\%ws\"\n", 
        Device->DeviceId, 
        Device->InstanceId
        ));

    return STATUS_ACCESS_DENIED;
}
