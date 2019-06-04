using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Signals
{
    class DegreeValue
    {
        private double xAxisDegree;
        private double yAxisDegree;
        private DateTime timeStamp;

        private const double maxDegree = 90;
        private const double minDegree = -90;

        // properties
        public double XAxisDegree
        {
            get { return this.xAxisDegree; }
            set
            {
                if ( value >= maxDegree || value <= minDegree )
                {
                    if (value < 0)
                    {
                        this.xAxisDegree = minDegree;
                    }
                    else
                    {
                        this.xAxisDegree = maxDegree;
                    }
                }
                else
                {
                    this.xAxisDegree = value;
                }
                
            }
        }

        public double YAxisDegree
        {
            get { return this.yAxisDegree; }
            set
            {
                if (value >= maxDegree || value <= minDegree)
                {
                    if (value < 0)
                    {
                        this.yAxisDegree = minDegree;
                    }
                    else
                    {
                        this.yAxisDegree = maxDegree;
                    }
                }
                else
                {
                    this.yAxisDegree = value;
                }

            }
        }

        public DateTime TimeStamp
        {
            get { return timeStamp; }
            set { this.timeStamp = value; }
        }

        // constructors
        public DegreeValue(double xVal, double yVal, DateTime tim)
        {
            XAxisDegree = xVal;
            YAxisDegree = yVal;
            TimeStamp = tim;
        }

        public DegreeValue()
        { }
    }
}
