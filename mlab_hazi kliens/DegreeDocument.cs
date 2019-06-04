using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Signals
{
    class DegreeDocument : Document
    {
        public List<DegreeValue> DegreeValues = new List<DegreeValue>();

        public DegreeDocument(string name) : base(name)
        {
            //TestValuesSetter();
        }
        public DegreeDocument(string name, bool enableTest) : base(name)
        {
            DegreeValues.Add(new DegreeValue(0, 0, DateTime.Now + new TimeSpan(0, 0, 0)));
            if (enableTest)
            {
                TestValuesSetter();
            }
            
        }

        public void TestValuesSetter()
        {
            int i = 0;
            DegreeValues.Add(new DegreeValue(2, -20, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(31, 48, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(54, 44, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(76, 65, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(98, -10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(99, -50, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(120, -70, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-5, 10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-99, 49, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-92, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-165, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(2, -20, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(31, 48, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(54, 44, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(76, 65, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(98, -10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(99, -50, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(120, -70, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-5, 10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-99, 49, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-92, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-165, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(2, -20, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(31, 48, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(54, 44, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(76, 65, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(98, -10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(99, -50, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(120, -70, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-5, 10, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-99, 49, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-92, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
            DegreeValues.Add(new DegreeValue(-165, 120, DateTime.Now + new TimeSpan(0, i++, 0)));
        }

        public override void SaveDocument(string filePath)
        {

            try
            {
                using (StreamWriter sw = new StreamWriter(filePath))
                {
                    foreach (DegreeValue element in DegreeValues)
                    {
                        sw.WriteLine(string.Format("{0}\t{1}\t{2}", element.XAxisDegree, element.YAxisDegree, element.TimeStamp.ToUniversalTime().ToString("o")));
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Hiba, a fájl nem nyitható meg: " + e.ToString());
            }
        }
        public override void LoadDocument(string filePath)
        {
            try
            {
                string toEnd = "";
                using (StreamReader sr = new StreamReader(filePath))
                {
                    int select = 0;
                    toEnd = sr.ReadToEnd();
                    toEnd = toEnd.Trim();
                    string[] columns = toEnd.Split(new[] { Environment.NewLine }, StringSplitOptions.None);
                    for (int i = 0; i < columns.Length; i++)
                    {
                        string[] temp = columns[i].Split('\t');
                        double x = double.Parse(temp[select]);
                        select = 1;
                        double y = double.Parse(temp[select]);
                        select = 2;
                        DateTime dt = DateTime.Parse(temp[select]);
                        DateTime dtOk = dt.ToLocalTime();
                        DegreeValues.Add(new DegreeValue(x, y, dtOk));
                        select = 0;
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Hiba, a fájl nem nyitható meg: " + e.ToString());
            }
            finally
            {
                UpdateAllViews();
            }

            TraceValues();
        }

        void TraceValues()
        {
            foreach (var value in DegreeValues)
                Trace.WriteLine(value.XAxisDegree.ToString() + "° " + value.YAxisDegree.ToString()  + "° " + value.TimeStamp.ToString());
        }



    }
}
