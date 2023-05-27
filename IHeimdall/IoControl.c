#include "stdafx.h"
#include "IoControl.h"

EXPORTED_FUNCTION BOOL AddDevice(PWCHAR DeviceId, PWCHAR InstanceId)
{
	HANDLE hDevice;
	BOOL Result;
	DWORD BytesReturned;

	size_t DeviceIdLength;
	size_t InstanceIdLength;

	IOCTL_PACKET IoCtlPacket;
	
	hDevice = CreateFile(L"\\\\.\\Heimdall", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DeviceIdLength = wcsnlen_s(DeviceId, DeviceIdMaximumLength);
	if (DeviceIdLength == 0 || DeviceIdLength >= DeviceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	memcpy(IoCtlPacket.DeviceId, DeviceId, DeviceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	InstanceIdLength = wcsnlen_s(InstanceId, InstanceIdMaximumLength);
	if (InstanceIdLength == 0 || InstanceIdLength >= InstanceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	memcpy(IoCtlPacket.InstanceId, InstanceId, InstanceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	if (DeviceIoControl(hDevice, IOCTL_HEIMDALL_ADD_DEVICE, &IoCtlPacket, sizeof(IOCTL_PACKET), NULL, 0, &BytesReturned, NULL) != TRUE)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	return TRUE;
}

EXPORTED_FUNCTION BOOL RemoveDevice(PWCHAR DeviceId, PWCHAR InstanceId)
{
	HANDLE hDevice;
	BOOL Result;
	DWORD BytesReturned;

	size_t DeviceIdLength;
	size_t InstanceIdLength;

	IOCTL_PACKET IoCtlPacket;

	hDevice = CreateFile(L"\\\\.\\Heimdall", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	DeviceIdLength = wcsnlen_s(DeviceId, DeviceIdMaximumLength);
	if (DeviceIdLength == 0 || DeviceIdLength >= DeviceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	memcpy(IoCtlPacket.DeviceId, DeviceId, DeviceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	InstanceIdLength = wcsnlen_s(InstanceId, InstanceIdMaximumLength);
	if (InstanceIdLength == 0 || InstanceIdLength >= InstanceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	memcpy(IoCtlPacket.InstanceId, InstanceId, InstanceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	if (DeviceIoControl(hDevice, IOCTL_HEIMDALL_REMOVE_DEVICE, &IoCtlPacket, sizeof(IOCTL_PACKET), NULL, 0, &BytesReturned, NULL) != TRUE)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	CloseHandle(hDevice);

	return TRUE;
}

EXPORTED_FUNCTION BOOL StartPlugNotification(HANDLE hEvent)
{
	HANDLE hDevice;
	BOOL Result;
	DWORD BytesReturned;

	hDevice = CreateFile(L"\\\\.\\Heimdall", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (DeviceIoControl(hDevice, IOCTL_HEIMDALL_START_PLUG_NOTIFICTAION, (LPVOID)hEvent, 0, NULL, 0, &BytesReturned, NULL) != TRUE)
	{
		CloseHandle(hDevice);
		CloseHandle(hEvent);
		return FALSE;
	}

	CloseHandle(hDevice);

	return TRUE;
}

EXPORTED_FUNCTION BOOL StopPlugNotification()
{
	HANDLE hDevice;
	BOOL Result;
	DWORD BytesReturned;

	hDevice = CreateFile(L"\\\\.\\Heimdall", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (DeviceIoControl(hDevice, IOCTL_HEIMDALL_STOP_PLUG_NOTIFICTAION, NULL, 0, NULL, 0, &BytesReturned, NULL) != TRUE)
	{
		return FALSE;
	}

	CloseHandle(hDevice);

	return TRUE;
}

EXPORTED_FUNCTION BOOL QueryDevice(PWCHAR DeviceId, PWCHAR InstanceId)
{
	HANDLE hDevice;
	BOOL Result;
	DWORD BytesReturned;

	size_t DeviceIdLength;
	size_t InstanceIdLength;

	IOCTL_PACKET IoCtlPacket;

	hDevice = CreateFile(L"\\\\.\\Heimdall", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	Result = DeviceIoControl(hDevice,
		IOCTL_HEIMDALL_QUERY_DEVICE,
		NULL, 0,
		&IoCtlPacket, sizeof(IOCTL_PACKET),
		&BytesReturned, NULL);

	if (!Result)
	{
		return FALSE;
	}

	DeviceIdLength = wcsnlen_s(IoCtlPacket.DeviceId, DeviceIdMaximumLength);
	if (DeviceIdLength == 0 || DeviceIdLength >= DeviceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}


	memcpy(DeviceId, IoCtlPacket.DeviceId, DeviceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	InstanceIdLength = wcsnlen_s(IoCtlPacket.InstanceId, InstanceIdMaximumLength);
	if (InstanceIdLength == 0 || InstanceIdLength >= InstanceIdMaximumLength)
	{
		CloseHandle(hDevice);
		return FALSE;
	}

	memcpy(InstanceId, IoCtlPacket.InstanceId, InstanceIdLength * sizeof(WCHAR) + sizeof(UNICODE_NULL));

	return TRUE;
}