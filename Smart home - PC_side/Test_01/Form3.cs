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
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("Baudrate Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("b");

                switch (Convert.ToInt32(comboBox1.Text))
                {
                    case 9600:
                        Form1.GetInstance().SerialPort1.Write("1");
                        break;
                    case 19200:
                        Form1.GetInstance().SerialPort1.Write("2");
                        break;
                    case 38400:
                        Form1.GetInstance().SerialPort1.Write("3");
                        break;
                    case 57600:
                        Form1.GetInstance().SerialPort1.Write("4");
                        break;
                    case 115200:
                        Form1.GetInstance().SerialPort1.Write("5");
                        break;
                }

                MessageBox.Show("New Baudrate Saved!");

                this.Close();
            }
        }

    }
}
