using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace Test_01
{
    public partial class Form1 : Form
    {
        static Form1 Instance = null;
        public SerialPort SerialPort1 = new SerialPort();
        Thread workerThread;

        public Form1()
        {
            InitializeComponent();
        }

        public static Form1 GetInstance()
        {
            return Instance;
        }
                
        private void connect_Click(object sender, EventArgs e)
        {           
            if (comboBox1.SelectedIndex==-1)
                MessageBox.Show("Com Port Failed");
            else if (comboBox2.SelectedIndex == -1)
                MessageBox.Show("Baudrate Failed!!");
            else
            {

                SerialPort1.BaudRate = int.Parse(comboBox2.SelectedItem.ToString());
                SerialPort1.PortName = comboBox1.SelectedItem.ToString();

                try
                {
                    SerialPort1.Open();
                    SerialPort1.WriteLine("m");
                    //string data = SerialPort1.ReadExisting();
                    int bytes = SerialPort1.BytesToRead;
                    char[] test = new char[bytes];
                    SerialPort1.Read(test, 0, bytes);
                    MessageBox.Show("Connect Successful!");
                    Size = new Size(750, 420);
                    this.AutoScroll = true;
                    
                    //this.button2.Visible = true;
                    //this.button3.Visible = true;
                    //this.button4.Visible = true;
                    //this.button5.Visible = true;
                    this.button6.Visible = true;
                    this.menuStrip1.Visible = true;
                    this.groupBox1.Enabled = false;
                    this.groupBox2.Enabled = true;
                    this.groupBox3.Enabled = true;
                    this.button8.Visible = true;
                    //this.button9.Visible = true;
                    this.button10.Visible = true;
                    //SerialPort1.Close();

                    Instance = this;
                    workerThread = new Thread(RunSample01);
                    workerThread.Start();

                    while (!workerThread.IsAlive) ; 
                }
                catch (Exception ex)
                {

                    MessageBox.Show(ex.Message);
                }

            }           

        }

        private void button8_Click(object sender, EventArgs e)
        {
            //SerialPort
            new Form2().Show();
        }
        private void button3_Click(object sender, EventArgs e)
        {
            new Form3().Show();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            new Form7().Show();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            //send
            this.textBox1.Clear();
            SerialPort1.Write("d");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            new Form4().Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            new Form5().Show();
            //MessageBox.Show("            Saved");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            workerThread.Abort();
            SerialPort1.Close();
            this.textBox1.Clear();
            this.button6.Visible = false;
            Size = new Size(200, 420);
            this.AutoScroll = false;
            this.menuStrip1.Visible = false;
            this.groupBox1.Enabled = true;
            this.groupBox2.Enabled = false;
            this.groupBox3.Enabled = false;
            comboBox1.Text = null;
            comboBox2.Text = null;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Size = new Size(200, 420);
            this.AutoScroll = false;
            //this.button8.Enabled = false;
            //this.button9.Enabled = false;
            this.button8.Visible = false;
            this.button10.Visible = false;
            string[] ports = SerialPort.GetPortNames();
            List<string> listPorts = new List<string>(ports);

            Comparison<string> comparer = delegate (string name1, string name2)
            {
                int port1 = Convert.ToInt32(name1.Remove(0,3));
                int port2 = Convert.ToInt32(name2.Remove(0,3));
                return port1 - port2;
            };

            listPorts.Sort(comparer);
            comboBox1.Items.AddRange(listPorts.ToArray());

            //foreach (string port in ports)
            //    comboBox1.Items.Add(port);
        }

        private void 關於ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form6().Show();
        }

        private void setPANIDToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form5().Show();
        }

        private void 結束程式ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public void RunSample01()
        {
            Form.CheckForIllegalCrossThreadCalls = false;
            while (!_shouldStop)
            {
                SerialPort SerialPort1 = Form1.GetInstance().SerialPort1;

                int bytes = SerialPort1.BytesToRead;
                char[] test = new char[bytes];
                if (bytes > 0)
                {
                    SerialPort1.Read(test, 0, bytes);
                    for (int i = 0; i < bytes; i++)
                    {
                        if (test[i] != 10)
                            this.textBox1.Text += test[i].ToString();
                        else
                            this.textBox1.Text += "\r\n";
                    }
                    //this.textBox1.Select(textBox1.Text.Length, 0);
                    //this.textBox1.ScrollToCaret();
                }
            }

            Console.WriteLine("worker thread: terminating gracefully.");
        }

        public void RequestStop()
        {
            _shouldStop = true;
        }

        //============================================================================
        //============================================================================
        // Volatile is used as hint to the compiler that this data
        // member will be accessed by multiple threads.
        private volatile bool _shouldStop;

        private void changeBaudrateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form3().Show();
        }

        private void changeChannelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form4().Show();
        }

        private void displayCurrentStatusToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //send
            this.textBox1.Clear();
            SerialPort1.WriteLine("d");
        }

        private void setNodeTypeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form2().Show();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                workerThread.Abort();
                SerialPort1.Close();
            }
            catch (Exception ex) { }
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            textBox1.Clear();
        }

        private void setNodeTypeToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            new Form2().Show();
        }

        private void setPANIDToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            new Form5().Show();
        }

        private void comboBox1_DropDown(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames();
            List<string> listPorts = new List<string>(ports);

            Comparison<string> comparer = delegate(string name1, string name2)
            {
                int port1 = Convert.ToInt32(name1.Remove(0, 3));
                int port2 = Convert.ToInt32(name2.Remove(0, 3));
                return port1 - port2;
            };

            listPorts.Sort(comparer);
            comboBox1.Items.AddRange(listPorts.ToArray());
            
            //foreach (string port in ports)
            //    comboBox1.Items.Add(port);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.textBox1.Clear();
            SerialPort1.WriteLine("s");
            MessageBox.Show("Save successfully !");
        }

        private void button11_Click(object sender, EventArgs e)
        {
            new Form8().Show();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            new Form9().Show();
        }

        private void button13_Click(object sender, EventArgs e)
        {
            new Form10().Show();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            new Form11().Show();
        }

        private void setNodeTypeToolStripMenuItem_Click_2(object sender, EventArgs e)
        {
            new Form2().Show();
        }

        private void setInterfaceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form7().Show();
        }

        private void parityToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form8().Show();
        }

        private void stopBitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form9().Show();
        }

        private void dataBitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form10().Show();
        }

        private void setDestinationAddressToolStripMenuItem_Click(object sender, EventArgs e)
        {
            new Form11().Show();
        }

        private void saveParemeterToFlashToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.textBox1.Clear();
            SerialPort1.WriteLine("s");
            MessageBox.Show("Save successfully !");
        }               
    }
}
