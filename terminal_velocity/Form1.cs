using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using ZedGraph;
using System.IO.Ports;


namespace terminal_velocity
{
    public partial class Form1 : Form
    {
        // Init value Timer
        private Timer timer;

        private GraphPane myPane;
        private RollingPointPairList dataLists_real_line;
        private RollingPointPairList dataLists_sp_line;
        private LineItem real_line;
        private LineItem sp_line;
        
        private string[] baud = { "9600", "14400", "19200", "38400", "56000", "57600", "115200" };

        private string data_sp = "0";
        private string data_real = "0";

        private DateTime startTime;
        private double time_XAxis;

        private bool isDrawing; // Biến kiểm tra trạng thái vẽ
        public Form1()
        {
            InitializeComponent();
            InitializeGraph();

            // Init USART
            string[] myport = SerialPort.GetPortNames();
            portname.Items.AddRange(myport);
            baudrate.Items.AddRange(baud);
            serial_Port.DataReceived += serial_Port_DataReceived;

            // Khởi tạo và cấu hình Timer
            timer = new Timer();
            timer.Interval = 1; // 1ms
            timer.Tick += Timer_Tick;
        }
        /// <summary>
        /// function Init Graph
        /// </summary>
        private void InitializeGraph()
        {
            // Lấy tham chiếu đến GraphPane
            myPane = zedGraph.GraphPane;

            // Đặt tiêu đề cho đồ thị
            myPane.Title.Text = "Đồ thị Vận Tốc";

            // 2 textBox
            textBox_setpoint.Text = data_sp;
            textBox_real_value.Text = data_real;

            // Đặt tên cho trục x và trục y
            myPane.XAxis.Title.Text = "Thời gian (s)";
            myPane.YAxis.Title.Text = "Giá trị vận tốc (rad/s)";

            // Tạo một list điểm để lưu trữ dữ liệu của đồ thị
            dataLists_real_line = new RollingPointPairList(6000);
            dataLists_sp_line = new RollingPointPairList(6000);

            // Vẽ đường đồ thị
            real_line = myPane.AddCurve("Real line", dataLists_real_line, Color.Blue, SymbolType.None);
            sp_line = myPane.AddCurve("Set Point", dataLists_sp_line, Color.Red, SymbolType.None);
        }
        /// <summary>
        /// Timer internal clock 1ms
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Timer_Tick(object sender, EventArgs e)
        {
            // Thực hiện cập nhật giao diện trên luồng UI
            if (isDrawing)
            {
                BeginInvoke((MethodInvoker)delegate
                {
                    textBox_real_value.Text = data_real;
                    UpdateGraph(data_real, data_sp);
                });
            }
        }

        /// <summary>
        /// Function to comunication USART
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void serial_Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        { 
            try
            {
                // Received data by USART, Value received is real_velocity
                data_real = serial_Port.ReadLine();
            }
            catch (FormatException)
            {
                // Xử lý nếu không thể chuyển đổi thành double
                Console.WriteLine("Không thể chuyển đổi chuỗi thành giá trị double: " + data_real);
            }
        }
        /// <summary>
        /// Function to update Draw Graph Velocity
        /// </summary>
        /// <param name="real"></param>
        /// <param name="set_point"></param>
        private void UpdateGraph(string real, string set_point)
        {
            // sample Time
            double delta_time = (DateTime.Now - startTime).TotalSeconds;
            startTime = DateTime.Now;
            time_XAxis += delta_time;

            // Chuyển đổi chuỗi thành giá trị double
            double value_real = double.Parse(real);
            double value_sp = double.Parse(set_point);

            dataLists_real_line.Add(time_XAxis, value_real);
            dataLists_sp_line.Add(time_XAxis, value_sp);

            // Xóa hết dữ liệu cũ trên đồ thị
            myPane.CurveList.Clear();

            // Thêm dữ liệu mới vào đồ thị
            real_line = myPane.AddCurve("Real line", dataLists_real_line, Color.Blue, SymbolType.None);
            sp_line = myPane.AddCurve("Set Point", dataLists_sp_line, Color.Red, SymbolType.None);
            // Width
            real_line.Line.Width = 2;
            sp_line.Line.Width = 2;
            // Update
            zedGraph.AxisChange();
            zedGraph.Invalidate();
        }
        private void button_connect_Click(object sender, EventArgs e)
        {
            try
            {
                serial_Port.PortName = portname.Text;
                serial_Port.BaudRate = int.Parse(baudrate.Text);
                serial_Port.DataBits = 8;
                serial_Port.Parity = Parity.None;
                serial_Port.StopBits = StopBits.One;

                serial_Port.Open();
                progressBar_percent.Value = 100;
                button_connect.Enabled = false;
                button_disconnect.Enabled = true;

                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
        private void button_disconnect_Click(object sender, EventArgs e)
        {
            try
            {
                serial_Port.Close();
                progressBar_percent.Value = 0;
                button_connect.Enabled = true;
                button_disconnect.Enabled = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
        private void button_start_Click(object sender, EventArgs e)
        {
            data_sp = textBox_setpoint.Text;
            // Timer enable
            timer.Start();
            // Enable Draw save value time start
            isDrawing = true;
            startTime = DateTime.Now;
            

            if (serial_Port.IsOpen) // Check serial Is Open
            {
                // Truyền dữ liệu đi bằng serial Port
                serial_Port.WriteLine(data_sp);
            }

            // Update
            UpdateGraph(data_real, data_sp);

            // Enable and disable button
            button_stop.Enabled = true;
            button_start.Enabled = false;
            // Color Button
            button_start.BackColor = Color.Green;
            button_stop.BackColor = Color.AliceBlue;
        }
        private void button_stop_Click(object sender, EventArgs e)
        {
            // Disable timer
            timer.Stop();

            button_stop.Enabled = false;
            button_start.Enabled = true;
            isDrawing = false;
            button_stop.BackColor = Color.Red;
            button_start.BackColor = Color.AliceBlue;
        }
        /// <summary>
        /// Event send_button 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_send_Click(object sender, EventArgs e)
        {
            // Lấy dữ liệu từ TextBox
            data_sp = textBox_setpoint.Text;
            if (serial_Port.IsOpen) // Check serial Is Open
            {
                // Truyền dữ liệu đi bằng serial Port
                serial_Port.WriteLine(data_sp);
            }
            // Update
            UpdateGraph(data_real, data_sp);
        }
        /// <summary>
        /// Event close form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            
            timer.Stop(); // Dừng timer khi đóng form để tránh lỗi
            serial_Port.Close(); // Disconnect USART
            Application.Exit(); // Thoát chương trình
        }
    }
}
