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

        private GraphPane myPane;
        private RollingPointPairList dataLists_real_line;
        private RollingPointPairList dataLists_sp_line;
        private LineItem real_line;
        private LineItem sp_line;
        
        private string[] baud = { "9600", "14400", "19200", "38400", "56000", "57600", "115200" };

        private string data_sp = "0";
        private double value_sp;
        private string data_real;
        private double value_real;

        private DateTime startTime;
        private double time_XAxis;

        private bool isDrawing; // Biến kiểm tra trạng thái vẽ
        public Form1()
        {
            InitializeComponent();
            // Init USART
            string[] myport = SerialPort.GetPortNames();
            portname.Items.AddRange(myport);
            baudrate.Items.AddRange(baud);
            serial_Port.DataReceived += serial_Port_DataReceived;

            textBox_setpoint.Text = "0";

            // Lấy tham chiếu đến GraphPane
            myPane = zedGraph.GraphPane;

            // Đặt tiêu đề cho đồ thị
            myPane.Title.Text = "Đồ thị Vận Tốc";

            // Đặt tên cho trục x và trục y
            myPane.XAxis.Title.Text = "Giá trị vận tốc (rad/s)";
            myPane.YAxis.Title.Text = "Thời gian (s)";

            // Tạo một list điểm để lưu trữ dữ liệu của đồ thị
            dataLists_real_line = new RollingPointPairList(6000);
            dataLists_sp_line = new RollingPointPairList(6000);


            // Vẽ đường đồ thị
            real_line = myPane.AddCurve("Real line", dataLists_real_line, Color.Blue, SymbolType.None);
            sp_line = myPane.AddCurve("Set Point", dataLists_sp_line, Color.Red, SymbolType.None);
        }

        private void serial_Port_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            data_real = serial_Port.ReadLine(); // nhận dữ liệu gửi về từ USART

            try
            {
                // Thử chuyển đổi chuỗi thành giá trị double
                value_real = double.Parse(data_real);
                value_sp = double.Parse(data_sp);

                // Chuyển đổi thành công Thành giá trị double
                // Thực hiện cập nhật giao diện trên luồng UI
                if (isDrawing)
                {
                    BeginInvoke((MethodInvoker)delegate
                    {
                        textBox_real_value.Text = data_real;
                        UpdateGraph(value_real, value_sp);
                    });
                }
            }
            catch (FormatException)
            {
                // Xử lý nếu không thể chuyển đổi thành double
                Console.WriteLine("Không thể chuyển đổi chuỗi thành giá trị double: " + data_real);
            }
        }

        private void UpdateGraph(double real, double set_point)
        {
            dataLists_real_line.Add(time_XAxis, real);
            dataLists_sp_line.Add(time_XAxis, set_point);

            // Xóa hết dữ liệu cũ trên đồ thị
            myPane.CurveList.Clear();

            // Thêm dữ liệu mới vào đồ thị
            real_line = myPane.AddCurve("Real line", dataLists_real_line, Color.Blue, SymbolType.None);
            sp_line = myPane.AddCurve("Set Point", dataLists_sp_line, Color.Red, SymbolType.None);

            real_line.Line.Width = 2;
            sp_line.Line.Width = 2;

            // Cập nhật đồ thị
            zedGraph.AxisChange();
            zedGraph.Invalidate();

            double delta_time = (DateTime.Now - startTime).TotalSeconds;
            startTime = DateTime.Now;

            time_XAxis += delta_time;
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
            isDrawing = true;
            startTime = DateTime.Now;

            // Lấy dữ liệu từ TextBox
            data_sp = textBox_setpoint.Text;

            // Truyền dữ liệu đi bằng serial Port
            serial_Port.WriteLine(data_sp);

            // Enable and disable button
            button_stop.Enabled = true;
            button_start.Enabled = false;
            // Color Button
            button_start.BackColor = Color.Green;
            button_stop.BackColor = Color.AliceBlue;
        }

        private void button_stop_Click(object sender, EventArgs e)
        {
            button_stop.Enabled = false;
            button_start.Enabled = true;
            isDrawing = false;
            button_stop.BackColor = Color.Red;
            button_start.BackColor = Color.AliceBlue;
        }

        private void button_send_Click(object sender, EventArgs e)
        {
            // Lấy dữ liệu từ TextBox
            data_sp = textBox_setpoint.Text;

            // Truyền dữ liệu đi bằng serial Port
            serial_Port.WriteLine(data_sp);
        }
    }
}
