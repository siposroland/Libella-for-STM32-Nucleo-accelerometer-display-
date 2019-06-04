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
    /// <summary>
    /// Egy UserControl, mely egy adott dokumentum tabfülének vezérlőelemeit tartalmazza.
    /// </summary>
    public partial class DocumentControl : UserControl
    {
        /// <summary>
        /// Az ABC karakterei.
        /// </summary>
        private string abc = "abcdefghijklmnopqrstuvwxyz";
        /// <summary>
        /// A felhasználó által felvett FontEditorView-k számát tárolja.
        /// Azért szükséges nyilvántartani, hogy tudjuk, a következő új FontEditorView-t mely pozícióba
        /// kell felvenni.
        /// </summary>
        private int editorCount = 0;

        /// <summary>
        /// Az vezérlőn levő SampleTextView-t adja vissza.
        /// </summary>
        /// <value>The sample text view.</value>
        public SampleTextView SampleTextView
        {
            get { return this.sampleTextControl1; }
        }

        public DocumentControl()
        {
            InitializeComponent();
            lblFontSelect.Text = abc;
            //tableLayout.ColumnStyles.Clear();
        }

        /// <summary>
        /// Úgy betűszerkesztő ablakot akkor hozunk létre, amikor a felhasználó duplán kattint
        /// az lblFontSelect adott betűjén. Az a művelet az ehhez tartozó egér eseménykezelés.
        /// </summary>
        private void lblFontSelect_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            // Megállapítjuk, mely karakteren kattintott a felhasználó.
            int charIndex = e.X/(lblFontSelect.Width/abc.Length);
            if (charIndex >= abc.Length)
                charIndex = abc.Length - 1;
            char c = abc[charIndex];

            // Létrehoz egy új FontEditorView-t az aktuális dokumentumhoz, és ezt be is regisztrálja a 
            /// dokumentumnál (hogy a jövőben étesüljön a válatozásairól).
            DemoView view = (DemoView)App.Instance.CreateFontEditorView(c);

            // A felületre is feltesszük az új ablakot.
            view.Height = 150;
            panelEditors.Controls.Add(view);
            view.Left = (view.Width + 2) * editorCount;
            editorCount++;
        }

        /// <summary>
        /// Eseménykezelő: akkor hívódik, ha a felhasználó megváltoztatja a mintaszöveget.
        /// </summary>
        private void tbSampleText_TextChanged(object sender, EventArgs e)
        {
            SampleTextView.SetSampleText(tbSampleText.Text);
        }

    }
}
