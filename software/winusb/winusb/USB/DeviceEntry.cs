using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Windows.Devices.Usb;
using Windows.Devices.Enumeration;

namespace winusb.USB
{
    public class DeviceEntry
    {
        DeviceInformation deviceInformation;
        string deviceSelector;

        public String InstanceId
        {
            get
            {
                return (String)deviceInformation.Properties[DeviceProperties.DeviceInstanceId];
            }
        }

        public DeviceInformation DeviceInformation
        {
            get
            {
                return deviceInformation;
            }
        }

        public String DeviceSelector
        {
            get
            {
                return deviceSelector;
            }
        }

        public DeviceEntry(DeviceInformation deviceInformation, string deviceSelector)
        {
            this.deviceInformation = deviceInformation;
            this.deviceSelector = deviceSelector;
        }
    }
}
