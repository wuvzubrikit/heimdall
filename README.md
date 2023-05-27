# Heimdall
Heimdall is a Windows application that provides security control over plugged USB mass storage devices.

### Project is divided in 3 parts:
- KHeimdall - WDM upper-filter driver for Usbstor.sys that filters incoming PnP IRPs and blocks mass storage devices that are not in white list;
- Heimdall - .NET Windows Forms desktop client;   
- IHeimdall - DLL for IOCTL communication betweeen driver and client.
