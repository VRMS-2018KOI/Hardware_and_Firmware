using System;
using System.Security.Cryptography;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Web;
using System.Windows.Forms;

namespace User_Device_Seting_Win_Program
{
    public partial class Start : Form
    {
        public Start()
        {
            InitializeComponent();
        }

        private SerialPort _Port;
        private SerialPort Port
        {
            get
            {
                if (_Port == null)
                {
                    _Port = new SerialPort();
                    _Port.PortName = "COM1";
                    _Port.BaudRate = 9600;
                    _Port.DataBits = 8;
                    _Port.Parity = Parity.None;
                    _Port.Handshake = Handshake.None;
                    _Port.StopBits = StopBits.One;
                    _Port.Encoding = Encoding.UTF8;
                }
                return _Port;
            }
        }

        private Boolean IsOpen
        {
            get { return Port.IsOpen; }
            set
            {
                if (value)
                {
                    button1.Text = "Disconnect";
                    label1.Enabled = false;
                    comboBox1.Enabled = false;
                    groupBox2.Enabled = true;
                    MessageBox.Show("Your Device Connection Successful!");
                }
                else
                {
                    button1.Text = "Connect";
                    label1.Enabled = true;
                    comboBox1.Enabled = true;
                    groupBox2.Enabled = false;
                    textBox1.Clear();
                    textBox2.Clear();
                    textBox4.Clear();
                    textBox5.Clear();
                    textBox6.Clear();
                    textBox7.Clear();
                }
            }
        }

        private void SerialComm_Load(object sender, EventArgs e)
        {
            comboBox1.DataSource = SerialPort.GetPortNames();
        }

        private void btConnectControl_Click(object sender, EventArgs e)
        {
            if (!Port.IsOpen)
            {
                try
                {
                    Port.PortName = comboBox1.SelectedItem.ToString();
                    Port.BaudRate = 9600;
                    Port.DataBits = 8;
                    Port.Parity = Parity.None;
                    Port.Handshake = Handshake.None;
                    Port.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
            }
            else
            {
                Port.Close();
            }
            IsOpen = Port.IsOpen;
            groupBox3.Enabled = true;
            Port.Write("{Connect}");
            textBox7.Text = Port.ReadLine();
        }

        private void Login_Click(object sender, EventArgs e)
        {
            if (button2.Text == "Login")
            {
                if (textBox1.Text == "")
                {
                    MessageBox.Show("Error : Please write your ID");
                }
                else if (textBox2.Text == "")
                {
                    MessageBox.Show("Error : Please write your Password");
                }
                else
                {
                    try
                    {
                        String getParams;
                        getParams = "username=" + textBox1.Text + "&password=" + textBox2.Text + "&device=" + textBox7.Text;

                        HttpWebResponse myRes;

                        HttpWebRequest myReq = (HttpWebRequest)WebRequest.Create("http://45.119.146.229/profile/device_setting");
                        myReq.Method = "POST";
                        myReq.ContentType = @"application/x-www-form-urlencoded";
                        byte[] buffer = Encoding.Default.GetBytes(getParams);
                        myReq.ContentLength = buffer.Length;

                        Stream respGetStream = myReq.GetRequestStream();

                        respGetStream.Write(buffer, 0, buffer.Length);
                        respGetStream.Close();

                        using (myRes = (HttpWebResponse)myReq.GetResponse())
                        {
                            Stream respPostStream = myRes.GetResponseStream();
                            StreamReader readerPost = new StreamReader(respPostStream, Encoding.GetEncoding("EUC-KR"), true);

                            String resResult = readerPost.ReadToEnd();

                            char[] delimiterChars = { '{', ',', '"', '}'};

                            string[] words = resResult.Split(delimiterChars);

                            if (words[4] == "000")
                            {
                                MessageBox.Show("Login Successful!");
                                button2.Text = "Logout";
                                textBox4.Text = words[9];
                                textBox5.Text = words[14];
                                textBox6.Text = words[19];
                                String Login_imfor = "{" + textBox1.Text + "}, {" + textBox2.Text + "}";
                                Port.Write(Login_imfor);
                                textBox1.Enabled = false;
                                textBox2.Enabled = false;
                            }
                            else if (words[4] == "001")
                            {
                                MessageBox.Show("Error : Please check your ID again.");
                            }
                            else if (words[4] == "002")
                            {
                                MessageBox.Show("Error : Please check your PW again.");
                            }

                        }
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.ToString());
                    }
                }
            }
            if (button2.Text == "Logout")
            {
                textBox1.Enabled = true;
                textBox2.Enabled = true;
                button2.Text = "Login";
                textBox1.Text = "";
                textBox2.Text = "";
                textBox4.Text = "";
                textBox5.Text = "";
                textBox6.Text = "";
            }
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://placenconnect.tech/");
        }

        private void linkLabel2_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/Comgong2");
        }

        private void linkLabel3_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/P&C-2018KOI");
        }

        private void linkLabel4_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            System.Diagnostics.Process.Start("https://placenconnect.tech/");
        }
    }
}
