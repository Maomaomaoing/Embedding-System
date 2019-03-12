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
    public partial class Form8 : Form
    {
        public Form8()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("UART Parity Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("r");

                switch (comboBox1.Text)
                {
                    case "none":
                        Form1.GetInstance().SerialPort1.Write("0");
                        break;
                    case "odd":
                        Form1.GetInstance().SerialPort1.Write("1");
                        break;
                    case "even":
                        Form1.GetInstance().SerialPort1.Write("2");
                        break;
                }

                MessageBox.Show("UART Parity Saved!");
                this.Close();
            }
        }
    }
}
