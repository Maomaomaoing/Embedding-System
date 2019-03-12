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
    public partial class Form9 : Form
    {
        public Form9()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("UART Stop Bit Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("t");

                switch (comboBox1.Text)
                {
                    case "1-bit":
                        Form1.GetInstance().SerialPort1.Write("0");
                        break;
                    case "2-bit":
                        Form1.GetInstance().SerialPort1.Write("1");
                        break;                
                }

                MessageBox.Show("UART Stop Bit Saved!");
                this.Close();
            }
        }
    }
}
