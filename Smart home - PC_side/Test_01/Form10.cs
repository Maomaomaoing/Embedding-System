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
    public partial class Form10 : Form
    {
        public Form10()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("UART Data Bit Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("u");

                switch (comboBox1.Text)
                {
                    case "7-bit":
                        Form1.GetInstance().SerialPort1.Write("0");
                        break;
                    case "8-bit":
                        Form1.GetInstance().SerialPort1.Write("1");
                        break;
                }

                MessageBox.Show("UART Data Bit Saved!");
                this.Close();
            }
        }
    }
}
