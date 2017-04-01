using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinSendFileToSerial
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            string[] ports = SerialPort.GetPortNames();
            cbSerialPortList.Items.AddRange(ports);
        }

        private void btSelect_Click(object sender, EventArgs e)
        {            
                      
            if (openFileDialog1.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                label2.Text = openFileDialog1.FileName;
                listView1.Items.Add("Вибраний файл " + openFileDialog1.FileName);
                listView1.Items.Add("\tРозмір файлу " + new FileInfo((openFileDialog1.FileName)).Length);                
            }            
        }

        private void btUpload_Click(object sender, EventArgs e)
        {
            byte[] file;            
            serialPort1.PortName = cbSerialPortList.SelectedItem.ToString();
            listView1.Items.Add("PortName " + serialPort1.PortName + "/" + serialPort1.BaudRate + "/" + serialPort1.DataBits + "/" + serialPort1.Parity + "/" + serialPort1.StopBits);
            serialPort1.Open();
            file = System.IO.File.ReadAllBytes(label2.Text);
            serialPort1.Write(file, 0, file.Length);
            serialPort1.Close();
        }

    }
}
