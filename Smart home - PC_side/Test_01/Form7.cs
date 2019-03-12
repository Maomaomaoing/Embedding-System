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
    public partial class Form7 : Form
    {
        public Form7()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("Interface Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("f");

                switch (comboBox1.Text)
                {
                    case "RS485":
                        Form1.GetInstance().SerialPort1.Write("0");
                        break;
                    case "RS232":
                        Form1.GetInstance().SerialPort1.Write("1");
                        break;
                }

                MessageBox.Show("Interface Saved!");
                this.Close();
            }
        }
    }
}
