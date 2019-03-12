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
    public partial class Form4 : Form
    {
        public Form4()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedIndex == -1)
                MessageBox.Show("Channel Selected Failed!");
            else
            {
                Form1.GetInstance().textBox1.Clear();
                Form1.GetInstance().SerialPort1.Write("c");

                if (Convert.ToInt16(comboBox1.Text) <= 20)
                {
                    Form1.GetInstance().SerialPort1.Write(Convert.ToString(Convert.ToInt16(comboBox1.Text) - 11));
                }
                else
                {
                    switch (Convert.ToInt16(comboBox1.Text))
                    {
                        case 21:
                            Form1.GetInstance().SerialPort1.Write("a");
                            break;
                        case 22: 
                            Form1.GetInstance().SerialPort1.Write("b");
                            break;
                        case 23: 
                            Form1.GetInstance().SerialPort1.Write("c");
                            break;
                        case 24:
                            Form1.GetInstance().SerialPort1.Write("d");
                            break;
                        case 25:
                            Form1.GetInstance().SerialPort1.Write("e");
                            break;
                        case 26:
                            Form1.GetInstance().SerialPort1.Write("f");
                            break;
                    }
                }

                MessageBox.Show("Channel Saved!");
                
            }
            this.Close();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
