using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


using Windows.Devices.Usb;
using Windows.Devices.Enumeration;
using winusb.USB;

namespace winusb
{
    public partial class FrmMain : Form
    {

        DeviceWatcher deviceWatcher;
        List<USB.DeviceEntry> deviceList;
        string usbSelector;

        FrmDialog frmDialog;

        public FrmMain()
        {
            deviceList = new List<DeviceEntry>();
            InitializeComponent();
            button1.Click += Button1_Click;
            button1.Enabled = false;
            lsvDevice.Items.Clear();
            lsvDevice.SelectedIndexChanged += LsvDevice_SelectedIndexChanged;

            this.Shown += FrmMain_Shown;
            this.FormClosing += FrmMain_FormClosing;
        }

        int selectedIndex = -1;
        private void LsvDevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            selectedIndex = lsvDevice.FocusedItem.Index;
            if(selectedIndex != -1)
            {
                button1.Enabled = true;
            }
        }

        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            USB.EventHandlerForDevice.Current.CloseDevice();
        }

        private void FrmMain_Shown(object sender, EventArgs e)
        {
            // start usb device scan
            usbSelector = UsbDevice.GetDeviceSelector(USB.Scan.DeviceVid, USB.Scan.DevicePid);
            if(string.IsNullOrEmpty(usbSelector))
            {
                MessageBox.Show($"Can not find USB device VID: {USB.Scan.DeviceVid}, PID: {USB.Scan.DevicePid}");
                this.Close();
            }

            deviceWatcher = DeviceInformation.CreateWatcher(usbSelector);
            deviceWatcher.Added += DeviceWatcher_Added;
            deviceWatcher.Removed += DeviceWatcher_Removed;
            deviceWatcher.EnumerationCompleted += DeviceWatcher_EnumerationCompleted;

            deviceWatcher.Start();
        }

        private void DeviceWatcher_EnumerationCompleted(DeviceWatcher sender, object args)
        {

        }

        private void DeviceWatcher_Removed(DeviceWatcher sender, DeviceInformationUpdate args)
        {
            RemoveDeviceList(args.Id);
        }

        private void DeviceWatcher_Added(DeviceWatcher sender, DeviceInformation args)
        {
            AddDeviceList(args, usbSelector);
        }

        private void AddDeviceList(DeviceInformation deviceInfomation, string deviceSelector)
        {
            var match = FindDevice(deviceInfomation.Id);
            if (match == null)
            {
                match = new USB.DeviceEntry(deviceInfomation, deviceSelector);
                deviceList.Add(match);
                UpdateDeviceListView();
            }
        }

        private void RemoveDeviceList(string id)
        {
            var match = FindDevice(id);
            if(match != null)
            {
                UpdateDeviceListView();
            }
            deviceList.Remove(match);
        }

        private void UpdateDeviceListView()
        {
            lsvDevice.BeginInvoke((MethodInvoker)delegate
            {
                lsvDevice.Items.Clear();
                foreach (var dev in deviceList)
                {
                    var item = new ListViewItem(dev.InstanceId);
                    lsvDevice.Items.Add(item);
                }
                selectedIndex = -1;
                button1.Enabled = false;
            });
        }

        private USB.DeviceEntry FindDevice(string id)
        {
            if (id != null)
            {
                foreach (var entry in deviceList)
                {
                    if (entry.DeviceInformation.Id == id)
                    {
                        return entry;
                    }
                }
            }

            return null;
        }

        bool isUserDisconnect = false;
        private async void Button1_Click(object sender, EventArgs e)
        {
            if (button1.Text == "Disconnect")
            {
                button1.Text = "Connect";
                isUserDisconnect = true;
                USB.EventHandlerForDevice.Current.CloseDevice();
            }
            else
            {
                if(selectedIndex == -1)
                {
                    return;
                }

                USB.EventHandlerForDevice.CreateNewEventHandlerForDevice();
                USB.EventHandlerForDevice.Current.OnDeviceConnected = UsbOnConnected;
                USB.EventHandlerForDevice.Current.OnDeviceClose = UsbOnClose;

                bool connected = await USB.EventHandlerForDevice.Current.OpenDeviceAsync(deviceList[selectedIndex].DeviceInformation, 
                    deviceList[selectedIndex].DeviceSelector);
                if (connected)
                {
                    button1.Text = "Disconnect";
                }
            }
        }

        private void UsbOnClose(EventHandlerForDevice sender, DeviceInformation args)
        {
            this.BeginInvoke((MethodInvoker)delegate
            {
                button1.Enabled = false;
                this.Text = "USB Closed";
            });

            if(isUserDisconnect)
            {
                isUserDisconnect = false;
                return;
            }

            if (USB.EventHandlerForDevice.Current.IsDeviceConnected)
            {
                foreach (Form frm in Application.OpenForms)
                {
                    if (frm is FrmDialog)
                    {
                        return;
                    }
                }

                this.BeginInvoke((MethodInvoker)delegate
                {
                    frmDialog = new FrmDialog();
                    frmDialog.ShowMsg($"USB {usbSelector} disconnected", "Notify");
                    frmDialog = null;
                });
            }
        }

        private void UsbOnConnected(EventHandlerForDevice sender, DeviceInformation args)
        {
            this.BeginInvoke((MethodInvoker)delegate
            {
                button1.Enabled = true;
               this.Text = "USB Connected";

                foreach(Form frm in Application.OpenForms)
                {
                    if(frm is FrmDialog)
                    {
                        frm.Close();
                        frmDialog = null;
                    }
                }
            });
        }

        private void btnScan_Click(object sender, EventArgs e)
        {
            // Do nothing
        }
    }
}
