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
    public partial class Form11 : Form
    {
        public Form11()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Length != 16)
            {
                MessageBox.Show("Destination Address Set Failed!");
                this.textBox1.Clear();
                this.textBox1.Focus();
            }
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("a");

                Form1.GetInstance().SerialPort1.Write(textBox1.Text);

                MessageBox.Show("Destination Address Saved!");
                this.Close();
            }
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
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
            else if (this.textBox1.Text.Length >= 16)
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
