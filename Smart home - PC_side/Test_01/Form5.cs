using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Test_01
{
    public partial class Form5 : Form
    {
        public Form5()
        {
            InitializeComponent();
            this.tbxPanID.Focus();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (tbxPanID.Text.Length != 4)
            {
                MessageBox.Show("PAN ID Set Failed!");
                this.tbxPanID.Clear();
                this.tbxPanID.Focus();
            }
            else if (tbxPanID.Text.ToLower() == "ffff")
            {
                MessageBox.Show("PAN ID Set Failed!");
                this.tbxPanID.Clear();
                this.tbxPanID.Focus();
            }
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("p");

                Form1.GetInstance().SerialPort1.Write(tbxPanID.Text);

                MessageBox.Show("PAN ID Saved!");
                this.Close();
            }
        }

        private void tbxPanID_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\b')
            {
                return;
            }
            else if (e.KeyChar == '\r')
            {
                this.button1_Click(sender, e);
                e.Handled = true;
            }
            else if (tbxPanID.Text.Length >= 4)
            {
                e.Handled = true;
            }
            else if (e.KeyChar.CompareTo('0') < 0 || e.KeyChar.CompareTo('9') > 0)
            {
                if (e.KeyChar.CompareTo('a') < 0 || e.KeyChar.CompareTo('f') > 0)
                {
                    if (e.KeyChar.CompareTo('A') < 0 || e.KeyChar.CompareTo('F') > 0)
                    {
                        e.Handled = true;
                    }
                }
            }
        }
    }
}
