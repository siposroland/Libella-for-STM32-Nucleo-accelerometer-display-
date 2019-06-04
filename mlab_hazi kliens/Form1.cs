using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Windows.Forms;



namespace Signals
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            cmdClose.Enabled = false;
            foreach (String s in System.IO.Ports.SerialPort.GetPortNames()) 
            {
                txtPort.Items.Add(s);
            }
        }

        DegreeDocument portDoc = new DegreeDocument("serialport", false);
        private enum dataSetter { xDegree, yDegree, other, error };

        public System.IO.Ports.SerialPort sport;
        private double ActualX = 0;
        private double ActualY = 0;
        private int cntr = 0;
        private bool saveEnable = false;

        public void serialport_connect(String port, int baudrate , Parity parity, int databits, StopBits stopbits) 
        {
            DateTime dt = DateTime.Now;
            String dtn = dt.ToShortTimeString();

            sport = new System.IO.Ports.SerialPort(
            port, baudrate, parity, databits, stopbits);
            try
            {
                sport.Open();
                cmdClose.Enabled = true;
                cmdConnect.Enabled = false;
                txtReceive.AppendText("[" + dtn + "] " + "Connected\n");
                sport.DataReceived += new SerialDataReceivedEventHandler(sport_DataReceived);
            }
            catch (Exception ex) { MessageBox.Show(ex.ToString(), "Error"); }
        }

        private void sport_DataReceived(object sender, SerialDataReceivedEventArgs e) 
        {
            DateTime dt = DateTime.Now;
            String dtn = dt.ToShortTimeString();
            
            String temzp = sport.ReadExisting();
            dataSetter selector = DataAssigner();
            if (selector == dataSetter.xDegree)
            {
                ActualX = Convert.ToDouble(sport.ReadExisting());
                txtReceive.AppendText("[" + dtn + "] " + "X_Degree: " + ActualX.ToString() + "\n");
            }
            if (selector == dataSetter.yDegree)
            {
                ActualY = Convert.ToDouble(sport.ReadExisting());
                txtReceive.AppendText("[" + dtn + "] " + "Y_Degree: " + ActualX.ToString() + "\n");
            }
            if (cntr == 2)
            {
                DegreeValue temp = new DegreeValue(ActualX,ActualY,dt); 
                portDoc.DegreeValues.Add(temp);
            }
            if (selector == dataSetter.other || selector == dataSetter.error)
            {
                txtReceive.AppendText("[" + dtn + "] " + "Received: " + sport.ReadExisting() + "\n");
            }

        }

        private dataSetter DataAssigner()
        {
            if (sport.ReadExisting() == "x")
            {
                cntr = 1;
                return dataSetter.xDegree;
            }
            if (sport.ReadExisting() == "y")
            {
                if (cntr == 1)
                {
                    cntr = 2;
                }
                return dataSetter.yDegree;
            }
            if (sport.ReadExisting() == "x" || sport.ReadExisting() == "y")
            {
                return dataSetter.other;
            }
            return dataSetter.error;

        }

        private void cmdConnect_Click(object sender, EventArgs e)
        {
            String port = txtPort.Text;
            int baudrate = Convert.ToInt32(cmbbaudrate.Text);
            Parity parity = (Parity)Enum.Parse(typeof(Parity), cmbparity.Text);
            int databits = Convert.ToInt32(cmbdatabits.Text);
            StopBits stopbits = (StopBits)Enum.Parse(typeof(StopBits), cmbstopbits.Text);
            
            serialport_connect(port, baudrate, parity, databits, stopbits);
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DateTime dt = DateTime.Now;
            String dtn = dt.ToShortTimeString();
            String data = txtDatatoSend.Text;
            sport.Write(data);
            txtReceive.AppendText("[" + dtn + "] " + "Sent: " + data + "\n");
        }

        private void cmdClose_Click_1(object sender, EventArgs e)
        {
            DateTime dt = DateTime.Now;
            String dtn = dt.ToShortTimeString();

            if (sport.IsOpen) 
            {
                sport.Close();
                cmdClose.Enabled = false;
                cmdConnect.Enabled = true;
                txtReceive.AppendText("[" + dtn + "] " + "Disconnected\n");
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            saveEnable = true;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            saveEnable = false;
            // Útvonal bekérése a felhasználótól a SaveFileDialog segítségével.
            // http://msdn.microsoft.com/en-us/library/system.windows.forms.savefiledialog.aspx
            string path = "";
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            saveFileDialog1.FilterIndex = 1;
            saveFileDialog1.DefaultExt = ".txt";
            saveFileDialog1.RestoreDirectory = false;


            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // Útvonal mentése
                path = saveFileDialog1.FileName;
                // A dokumentum adatainak elmentése.
                portDoc.SaveDocument(path);
            }
        }

    }
}

