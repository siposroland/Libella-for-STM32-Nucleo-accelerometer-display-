using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Signals
{
    public partial class GraphicsDegreeView : UserControl, IView
    {
        public GraphicsDegreeView()
        {
            InitializeComponent();
        }

        /// <summary>
        /// A dokumentum, melynek adatait a nézet megjeleníti.
        /// TODO: a típusa legyen a Document leszármazottunk.
        /// </summary>
        private DegreeDocument document;
        private double zoomFactor;
        private int horizontalAdd;
        private int verticalAdd;

        public GraphicsDegreeView(Document doc)
        {
            InitializeComponent();
            this.document = (DegreeDocument)doc;
            zoomFactor = 1;
            horizontalAdd = 0;
            verticalAdd = 0;
        }

        /// <summary>
        /// A View interfész Update műveletánek implementációja.
        /// </summary>
        public void Update()
        {
            Invalidate();
        }

        public Document GetDocument()
        {
            return document;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            int off = 6;

            double maxDouble = 90;
            double minDouble = -90;

            int elementNum = document.DegreeValues.Count;
            DateTime startDate = document.DegreeValues[0].TimeStamp;
            DateTime stopDate = document.DegreeValues[elementNum - 1].TimeStamp;
            TimeSpan maxTime = stopDate - startDate + new TimeSpan(0,1,0);


            // Középső tengely
            e.Graphics.DrawLine(Pens.Black,
                (int)(off * zoomFactor) + horizontalAdd,
                (int)(((ClientSize.Height)/2 - off) * zoomFactor) + verticalAdd,
                (int)((ClientSize.Width - off) * zoomFactor) + horizontalAdd,
                (int)(((ClientSize.Height) / 2 -  off ) * zoomFactor) + verticalAdd);

            // Jobb oldali nyíl
            Point[] p = new Point[3];
            p[0].X = (int)((ClientSize.Width - off) * zoomFactor) + horizontalAdd + off;
            p[0].Y = (int)(((ClientSize.Height) / 2 - off) * zoomFactor) + verticalAdd;
            p[1].X = (int)((ClientSize.Width - off) * zoomFactor) + horizontalAdd;
            p[1].Y = (int)(((ClientSize.Height) / 2 - off) * zoomFactor) + verticalAdd + off;
            p[2].X = (int)((ClientSize.Width - off) * zoomFactor) + horizontalAdd;
            p[2].Y = (int)(((ClientSize.Height) / 2 - off) * zoomFactor) + verticalAdd - off;
            e.Graphics.DrawPolygon(Pens.Black, p);

            // Szélső tengely
            /*e.Graphics.DrawLine(Pens.Black,
                (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd,
                (int)(off * zoomFactor) + verticalAdd,
                (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd,
                (int)((ClientSize.Height - off) * zoomFactor) + verticalAdd);*/

            // Szélső tengely
            e.Graphics.DrawLine(Pens.Black,
                (int)(off * zoomFactor) + horizontalAdd,
                (int)(off * zoomFactor) + verticalAdd,
                (int)(off * zoomFactor) + horizontalAdd,
                (int)((ClientSize.Height - off) * zoomFactor) + verticalAdd);

            // Felső nyíl
            /*p[0].X = (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd;
            p[0].Y = (int)(off * zoomFactor) + verticalAdd - off;
            p[1].X = (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd + off;
            p[1].Y = (int)(off * zoomFactor) + verticalAdd;
            p[2].X = (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd - off;
            p[2].Y = (int)(off * zoomFactor) + verticalAdd;*/

            // Felső nyíl
            p[0].X = (int)(off * zoomFactor) + horizontalAdd;
            p[0].Y = (int)(off * zoomFactor) + verticalAdd - off;
            p[1].X = (int)(off * zoomFactor) + horizontalAdd + off;
            p[1].Y = (int)(off * zoomFactor) + verticalAdd;
            p[2].X = (int)(off * zoomFactor) + horizontalAdd - off;
            p[2].Y = (int)(off * zoomFactor) + verticalAdd;
            
            e.Graphics.DrawPolygon(Pens.Black, p);


            



            int[] xArray = new int[document.DegreeValues.Count];
            int[] yXAxisArray = new int[document.DegreeValues.Count];
            int[] yYAxisArray = new int[document.DegreeValues.Count];
            int i = 0;
            foreach (var element in document.DegreeValues)
            {
                int yXAxis = ClientSize.Height - (int)(((element.XAxisDegree + maxDouble) / (maxDouble - minDouble)) * (ClientSize.Height - 2*off) + off);
                int yYAxis = ClientSize.Height - (int)(((element.YAxisDegree + maxDouble) / (maxDouble - minDouble)) * (ClientSize.Height - 2 * off) + off);
                TimeSpan temp = element.TimeStamp - startDate;
                int x = (int)(((float)temp.Minutes / (float)maxTime.Minutes) * ClientSize.Width + off);
                e.Graphics.FillEllipse(Brushes.Red,
                    (int)(x * zoomFactor) + horizontalAdd,
                    (int)(yXAxis * zoomFactor) + verticalAdd,
                    off,
                    off);
                e.Graphics.FillEllipse(Brushes.Blue,
                    (int)(x * zoomFactor) + horizontalAdd,
                    (int)(yYAxis * zoomFactor) + verticalAdd,
                    off,
                    off);
                xArray[i] = x;
                yXAxisArray[i] = yXAxis;
                yYAxisArray[i] = yYAxis;
                i++;
            }
            for (i = 0; i < xArray.Length - 1; i++)
            {
                e.Graphics.DrawLine(Pens.Red,
                    (int)((xArray[i] + off / 2) * zoomFactor) + horizontalAdd,
                    (int)((yXAxisArray[i] + off / 2) * zoomFactor) + verticalAdd,
                    (int)((xArray[i + 1] + off / 2) * zoomFactor) + horizontalAdd,
                    (int)((yXAxisArray[i + 1] + off / 2) * zoomFactor) + verticalAdd);
                e.Graphics.DrawLine(Pens.Blue,
                    (int)((xArray[i] + off / 2) * zoomFactor) + horizontalAdd,
                    (int)((yYAxisArray[i] + off / 2) * zoomFactor) + verticalAdd,
                    (int)((xArray[i + 1] + off / 2) * zoomFactor) + horizontalAdd,
                    (int)((yYAxisArray[i + 1] + off / 2) * zoomFactor) + verticalAdd);
            }

            e.Graphics.DrawString("Idő (p)", SystemFonts.DefaultFont, Brushes.Black,
                (int)((ClientSize.Width - off) * zoomFactor) + horizontalAdd - 5 * off,
                (int)((ClientSize.Height - off) * zoomFactor) + verticalAdd - 3 * off);

            e.Graphics.DrawString("Dőlésszög (°)", SystemFonts.DefaultFont, Brushes.Black,
                (int)((ClientSize.Width / 2) * zoomFactor) + horizontalAdd + 2 * off,
                (int)(off * zoomFactor) + verticalAdd);


            base.OnPaint(e);
        }

    }
}
