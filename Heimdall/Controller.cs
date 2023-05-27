using Microsoft.Win32.SafeHandles;
using System.Runtime.InteropServices;
using System.Text;

namespace UHeimdall
{

    class Controller
    {
        #region HeimdallDllImport

        static internal SafeWaitHandle hEvent;

        [DllImport("DHeimdall.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool AddDevice(
            [MarshalAs(UnmanagedType.LPWStr)] string DeviceId,
            [MarshalAs(UnmanagedType.LPWStr)] string InstanceId
        );

        [DllImport("DHeimdall.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool RemoveDevice(
            [MarshalAs(UnmanagedType.LPWStr)] string DeviceId,
            [MarshalAs(UnmanagedType.LPWStr)] string InstanceId
        );

        [DllImport("DHeimdall.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool StartPlugNotification(SafeWaitHandle hEvent);

        [DllImport("DHeimdall.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool StopPlugNotification();

        [DllImport("DHeimdall.dll", CallingConvention = CallingConvention.Cdecl)]
        internal static extern bool QueryDevice(
            [MarshalAs(UnmanagedType.LPWStr)] StringBuilder DeviceId,
            [MarshalAs(UnmanagedType.LPWStr)] StringBuilder InstanceId
        );

        #endregion
       
    }
}
