namespace terminal_velocity
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.zedGraph = new ZedGraph.ZedGraphControl();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button_disconnect = new System.Windows.Forms.Button();
            this.button_connect = new System.Windows.Forms.Button();
            this.progressBar_percent = new System.Windows.Forms.ProgressBar();
            this.label2 = new System.Windows.Forms.Label();
            this.baudrate = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.portname = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.button_send = new System.Windows.Forms.Button();
            this.textBox_real_value = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_setpoint = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.button_stop = new System.Windows.Forms.Button();
            this.button_start = new System.Windows.Forms.Button();
            this.serial_Port = new System.IO.Ports.SerialPort(this.components);
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // zedGraph
            // 
            this.zedGraph.Location = new System.Drawing.Point(266, 12);
            this.zedGraph.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.zedGraph.Name = "zedGraph";
            this.zedGraph.ScrollGrace = 0D;
            this.zedGraph.ScrollMaxX = 0D;
            this.zedGraph.ScrollMaxY = 0D;
            this.zedGraph.ScrollMaxY2 = 0D;
            this.zedGraph.ScrollMinX = 0D;
            this.zedGraph.ScrollMinY = 0D;
            this.zedGraph.ScrollMinY2 = 0D;
            this.zedGraph.Size = new System.Drawing.Size(884, 378);
            this.zedGraph.TabIndex = 1;
            this.zedGraph.UseExtendedPrintDialog = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button_disconnect);
            this.groupBox1.Controls.Add(this.button_connect);
            this.groupBox1.Controls.Add(this.progressBar_percent);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.baudrate);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.portname);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(247, 163);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Serial Port";
            // 
            // button_disconnect
            // 
            this.button_disconnect.Location = new System.Drawing.Point(135, 126);
            this.button_disconnect.Name = "button_disconnect";
            this.button_disconnect.Size = new System.Drawing.Size(97, 31);
            this.button_disconnect.TabIndex = 5;
            this.button_disconnect.Text = "Disconnect";
            this.button_disconnect.UseVisualStyleBackColor = true;
            this.button_disconnect.Click += new System.EventHandler(this.button_disconnect_Click);
            // 
            // button_connect
            // 
            this.button_connect.Location = new System.Drawing.Point(6, 126);
            this.button_connect.Name = "button_connect";
            this.button_connect.Size = new System.Drawing.Size(97, 31);
            this.button_connect.TabIndex = 3;
            this.button_connect.Text = "Connect";
            this.button_connect.UseVisualStyleBackColor = true;
            this.button_connect.Click += new System.EventHandler(this.button_connect_Click);
            // 
            // progressBar_percent
            // 
            this.progressBar_percent.Location = new System.Drawing.Point(6, 97);
            this.progressBar_percent.Name = "progressBar_percent";
            this.progressBar_percent.Size = new System.Drawing.Size(226, 23);
            this.progressBar_percent.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Baudrate";
            // 
            // baudrate
            // 
            this.baudrate.FormattingEnabled = true;
            this.baudrate.Location = new System.Drawing.Point(111, 58);
            this.baudrate.Name = "baudrate";
            this.baudrate.Size = new System.Drawing.Size(121, 24);
            this.baudrate.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 16);
            this.label1.TabIndex = 2;
            this.label1.Text = "COM";
            // 
            // portname
            // 
            this.portname.FormattingEnabled = true;
            this.portname.Location = new System.Drawing.Point(111, 28);
            this.portname.Name = "portname";
            this.portname.Size = new System.Drawing.Size(121, 24);
            this.portname.TabIndex = 2;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.button_send);
            this.groupBox2.Controls.Add(this.textBox_real_value);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.textBox_setpoint);
            this.groupBox2.Location = new System.Drawing.Point(12, 181);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(247, 137);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Value Velocity";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(9, 66);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(72, 16);
            this.label7.TabIndex = 7;
            this.label7.Text = "Real value";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(9, 35);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(59, 16);
            this.label6.TabIndex = 6;
            this.label6.Text = "Set point";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(197, 64);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 18);
            this.label4.TabIndex = 6;
            this.label4.Text = "rad/s";
            // 
            // button_send
            // 
            this.button_send.Location = new System.Drawing.Point(6, 91);
            this.button_send.Name = "button_send";
            this.button_send.Size = new System.Drawing.Size(226, 34);
            this.button_send.TabIndex = 4;
            this.button_send.Text = "Send";
            this.button_send.UseVisualStyleBackColor = true;
            this.button_send.Click += new System.EventHandler(this.button_send_Click);
            // 
            // textBox_real_value
            // 
            this.textBox_real_value.Location = new System.Drawing.Point(87, 63);
            this.textBox_real_value.Name = "textBox_real_value";
            this.textBox_real_value.Size = new System.Drawing.Size(103, 22);
            this.textBox_real_value.TabIndex = 5;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(197, 36);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 18);
            this.label3.TabIndex = 4;
            this.label3.Text = "rad/s";
            // 
            // textBox_setpoint
            // 
            this.textBox_setpoint.Location = new System.Drawing.Point(88, 35);
            this.textBox_setpoint.Name = "textBox_setpoint";
            this.textBox_setpoint.Size = new System.Drawing.Size(102, 22);
            this.textBox_setpoint.TabIndex = 2;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.button_stop);
            this.groupBox3.Controls.Add(this.button_start);
            this.groupBox3.Location = new System.Drawing.Point(12, 324);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(247, 67);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Start - Stop";
            // 
            // button_stop
            // 
            this.button_stop.Location = new System.Drawing.Point(135, 21);
            this.button_stop.Name = "button_stop";
            this.button_stop.Size = new System.Drawing.Size(97, 33);
            this.button_stop.TabIndex = 6;
            this.button_stop.Text = "Stop";
            this.button_stop.UseVisualStyleBackColor = true;
            this.button_stop.Click += new System.EventHandler(this.button_stop_Click);
            // 
            // button_start
            // 
            this.button_start.Location = new System.Drawing.Point(6, 21);
            this.button_start.Name = "button_start";
            this.button_start.Size = new System.Drawing.Size(97, 33);
            this.button_start.TabIndex = 5;
            this.button_start.Text = "Start";
            this.button_start.UseVisualStyleBackColor = true;
            this.button_start.Click += new System.EventHandler(this.button_start_Click);
            // 
            // serial_Port
            // 
            this.serial_Port.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serial_Port_DataReceived);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1169, 409);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.zedGraph);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private ZedGraph.ZedGraphControl zedGraph;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button_disconnect;
        private System.Windows.Forms.Button button_connect;
        private System.Windows.Forms.ProgressBar progressBar_percent;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox baudrate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox portname;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button button_send;
        private System.Windows.Forms.TextBox textBox_real_value;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_setpoint;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button button_stop;
        private System.Windows.Forms.Button button_start;
        private System.IO.Ports.SerialPort serial_Port;
    }
}

