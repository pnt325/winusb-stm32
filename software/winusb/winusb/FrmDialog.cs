using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winusb
{
    public partial class FrmDialog : Form
    {
        public FrmDialog()
        {
            InitializeComponent();
        }

        public void ShowMsg(string msg, string title)
        {
            this.Text = title;
            lblMsg.Text = msg;
            this.ShowDialog();
        }
    }
}
