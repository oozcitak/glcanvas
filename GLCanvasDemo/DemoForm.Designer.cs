namespace GLCanvasDemo
{
    partial class DemoForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.glCanvas3D1 = new GLCanvas.GLCanvas3D();
            this.glCanvas2D1 = new GLCanvas.GLCanvas2D();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.glCanvas2D1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.glCanvas3D1);
            this.splitContainer1.Size = new System.Drawing.Size(929, 466);
            this.splitContainer1.SplitterDistance = 412;
            this.splitContainer1.TabIndex = 1;
            // 
            // glCanvas3D1
            // 
            this.glCanvas3D1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.glCanvas3D1.Location = new System.Drawing.Point(0, 0);
            this.glCanvas3D1.Name = "glCanvas3D1";
            this.glCanvas3D1.Size = new System.Drawing.Size(513, 466);
            this.glCanvas3D1.TabIndex = 0;
            this.glCanvas3D1.Text = "glCanvas3D1";
            // 
            // glCanvas2D1
            // 
            this.glCanvas2D1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.glCanvas2D1.Location = new System.Drawing.Point(0, 0);
            this.glCanvas2D1.Name = "glCanvas2D1";
            this.glCanvas2D1.Size = new System.Drawing.Size(412, 466);
            this.glCanvas2D1.TabIndex = 0;
            this.glCanvas2D1.Text = "glCanvas2D1";
            // 
            // DemoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(929, 466);
            this.Controls.Add(this.splitContainer1);
            this.Name = "DemoForm";
            this.Text = "GLCanvas Demo Form";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.SplitContainer splitContainer1;
        private GLCanvas.GLCanvas3D glCanvas3D1;
        private GLCanvas.GLCanvas2D glCanvas2D1;
    }
}

