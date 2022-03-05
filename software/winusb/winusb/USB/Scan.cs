using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Windows.Devices.Enumeration;
using Windows.Devices.Usb;
using Windows.Foundation;
using Windows.ApplicationModel;

namespace winusb.USB
{
    public class Scan
    {
        UsbDevice device;
        DeviceWatcher usbWatcher;
        string usbSelector;

        public const UInt16 DeviceVid = 0x0483;
        public const UInt16 DevicePid = 0x3030;

        public event TypedEventHandler<DeviceInformation, string> DeviceAdded;
        bool isDeviceOpen = false;

        public Scan()
        {
            usbSelector = UsbDevice.GetDeviceSelector(DeviceVid, DevicePid);
            if(usbSelector == null)
            {
                Debug.WriteLine("USB selector is NULL");
                return;
            }

            usbWatcher = DeviceInformation.CreateWatcher(usbSelector);
            usbWatcher.Added += UsbWatcher_Added;
            usbWatcher.Removed += UsbWatcher_Removed;
            usbWatcher.EnumerationCompleted += UsbWatcher_EnumerationCompleted;
        }

        public void Start()
        {
            if(usbWatcher == null)
            {
                throw new Exception("USB watcher is NULL");
            }

            if(usbWatcher.Status != DeviceWatcherStatus.Started || 
                usbWatcher.Status != DeviceWatcherStatus.EnumerationCompleted)
            {
                usbWatcher.Start();
                Debug.WriteLine("USB watcher started");
            }
        }


        public void Stop()
        {
            if(usbWatcher == null)
            {
                throw new Exception("USB watcher is NULL");
            }

            if(usbWatcher.Status == DeviceWatcherStatus.Started || 
                usbWatcher.Status == DeviceWatcherStatus.EnumerationCompleted)
            {
                Debug.WriteLine("USB watcher stoped");
                usbWatcher.Stop();
            }
        }

        private void UsbWatcher_EnumerationCompleted(DeviceWatcher sender, object args)
        {

        }

        private void UsbWatcher_Removed(DeviceWatcher sender, DeviceInformationUpdate args)
        {
            Debug.WriteLine($"USB removed {args.Id}");
            CloseDevice();
        }

        private void UsbWatcher_Added(DeviceWatcher sender, DeviceInformation args)
        {
            Debug.WriteLine("USB added");
            Debug.WriteLine($"Product Name: {args.Name}");
            Debug.WriteLine($"Product ID: {args.Id}");

            DeviceAdded?.Invoke(args, usbSelector);
        }

        public async Task<bool> Open(string id)
        {
            device = await UsbDevice.FromIdAsync(id);
            if (device == null)
            {
                return false;
            }

            isDeviceOpen = true;

            var deviceAccessInfo = DeviceAccessInformation.CreateFromId(id);
            deviceAccessInfo.AccessChanged += DeviceAccessInfo_AccessChanged;

            return true;
        }

        private void DeviceAccessInfo_AccessChanged(DeviceAccessInformation sender, DeviceAccessChangedEventArgs args)
        {
            if(args.Status == DeviceAccessStatus.DeniedByUser ||
                args.Status == DeviceAccessStatus.DeniedBySystem)
            {
                CloseDevice();
            }
        }


        private void CloseDevice()
        {
            if (device != null)
            {
                device.Dispose();
                device = null;
            }
        }
    }
}
