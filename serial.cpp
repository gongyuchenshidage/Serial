#include "serial.h"
#include "ui_serial.h"

#include <QTextCodec>

Serial::Serial(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Serial)
{
    ui->setupUi(this);
    system_init();
}

Serial::~Serial()
{
    delete ui;
}

/*--------functions---------*/
void Serial::system_init(){
    //port config
    global_port.setParity(QSerialPort::NoParity);//奇偶校验位
    global_port.setDataBits(QSerialPort::Data8);//数据位
    global_port.setStopBits(QSerialPort::OneStop);//停止位

    //connect
    connect(ui->btn_open,&QPushButton::clicked,this,&Serial::btn_open_port);
    connect(ui->btn_close,&QPushButton::clicked,this,&Serial::btn_close_port);
    connect(ui->btn_send,&QPushButton::clicked,this,&Serial::btn_send_data);
    connect(&global_port,&QSerialPort::readyRead,this,&Serial::receive_data);
}
/*--------slots---------*/
/*------------------------打开串口---------------------------*/
void Serial::btn_open_port(bool){
    /*----------------------port_name--------------------*/
   //qDebug()<<ui->com_port_name->currentIndex();
    //qDebug("10");
   switch (ui->com_port_name->currentIndex()) {
   case 0:
       global_port.setPortName("COM1");
       break;
   case 1:
       global_port.setPortName("COM2");
       break;
   case 2:
       global_port.setPortName("COM3");
       break;
   case 3:
       global_port.setPortName("COM4");
       break;
   case 4:
       global_port.setPortName("COM5");
       break;
   case 5:
       global_port.setPortName("COM6");
       break;
   case 6:
       global_port.setPortName("COM7");
       break;
   default:
       global_port.setPortName("COM8");
       break;

   }
   /*------------------------baud_rate---------------------------*/
   switch (ui->com_baud_rate->currentIndex()) {
   case 0:
       global_port.setBaudRate(QSerialPort::Baud115200);
       break;
   case 1:
       global_port.setBaudRate(QSerialPort::Baud57600);
       break;
   case 2:
       global_port.setBaudRate(QSerialPort::Baud38400);
       break;
   case 3:
       global_port.setBaudRate(QSerialPort::Baud19200);
       break;
   case 4:
       global_port.setBaudRate(QSerialPort::Baud9600);
       break;
   case 5:
       global_port.setBaudRate(QSerialPort::Baud4800);
       break;
   case 6:
       global_port.setBaudRate(QSerialPort::Baud2400);
       break;
   default:
       global_port.setBaudRate(QSerialPort::Baud1200);
       break;
   }
   //打开串口
   global_port.open(QIODevice::ReadWrite);
   ui->lab_status->setText("已连接");
   //测试
   global_port.write("1");
}

/*------------------------关闭串口---------------------------*/
void Serial::btn_close_port(bool){
    global_port.close();
    ui->lab_status->setText("未连接");

}

/*------------------------转成中文,感觉不太好用---------------------------*/
QString Serial::GetCorrectUnicode(const QByteArray &ba){
    QTextCodec::ConverterState state;
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QString text = codec->toUnicode(ba.constData(),ba.size(),&state);
    if(state.invalidChars > 0){
        text = QTextCodec::codecForName("GBK")->toUnicode(ba);
    }else{
        text = ba;
    }
    return text;
}

/*------------------------发送数据---------------------------*/
void Serial::btn_send_data(bool){
   QString data = ui->line_send_data->text();
   //QByteArray array = data.toLatin1();//QString--->QByteArray
   QByteArray array = data.toLocal8Bit();//QString--->QByteArray，可发送中文
   global_port.write(array);
}

/*------------------------接收数据---------------------------*/
void Serial::receive_data(){
    QByteArray array = global_port.readAll();
    if(ui->checkBox->checkState() == Qt::Checked){
        QString str = QString::fromLocal8Bit(array.toHex(' ').toUpper().append("\n"));
        ui->plainTextEdit->insertPlainText(str);
    }
    else{
        QString str = QString::fromLocal8Bit(array.append("\n"));//可接收中文
        ui->plainTextEdit->insertPlainText(str);
    }

}
