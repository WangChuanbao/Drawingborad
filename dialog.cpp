#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    resize(600,500);    //设置窗口大小
    _pix = QPixmap(200,200);
    _pix.fill(Qt::white);

    _scale = 1; //初始放大倍数
    _isDrawing = false; //是否正在绘制图形，用于处理绘制矩形时重影

    _button = new QPushButton(this);
    _button->setText(QString::fromLocal8Bit("zoomIn"));
    _button->move(500,450);
    connect(_button,SIGNAL(clicked()),this,SLOT(zoomIn()));
}

Dialog::~Dialog()
{
    delete ui;
}

/**
 * @brief Dialog::drawLine
 * 鼠标拖动，绘制直线
 */
void Dialog::drawLine()
{
    /*
        QPainter pp(&_pix);
        //第一种放大方式，放大窗口坐标系统
        //解决放大窗口坐标系统导致的图形和鼠标轨迹不重合问题。窗口放大多少倍，将获取的鼠标指针的坐标值缩小多少倍
        pp.drawLine(_lastPoint/_scale,_endPoint/_scale);
        _lastPoint = _endPoint;

        QPainter painter(this);
        //放大窗口坐标系统，这样会导致图形不能和鼠标的轨迹重合。
        //这是因为窗口的坐标的系统放大了，但是画布的坐标系统没有扩大，我们用的坐标值是鼠标指针的，而鼠标指针是获取的窗口坐标值。
        //所以窗口和画布的同一点的坐标并不相等。
        painter.scale(_scale,_scale);
        painter.drawPixmap(0,0,_pix);
    */


        //第二种放大方式，放大画布的坐标系统
    //    pp.scale(_scale,_scale);
    //    pp.drawLine(_lastPoint,_endPoint);
    //    _lastPoint = _endPoint;

    //    QPainter painter(this);
    //    painter.drawPixmap(0,0,_pix);

        //上面那样直接放大会有一些问题，我们需要像下面这样将画布和图形都进行放大
        if(_scale!=1)
        {
            //创建临时画布，大小变化了scale倍
            QPixmap cpPix(_pix.size()*_scale);
            QPainter pter(&cpPix);
            pter.scale(_scale,_scale);
            //将之前画布的内容绘制到现在画布上
            pter.drawPixmap(0,0,_pix);
            //将放大后的内容复制给原来的画布
            _pix = cpPix;
            _scale = 1;
        }
        QPainter pp(&_pix);
        pp.drawLine(_lastPoint,_endPoint);
        _lastPoint = _endPoint;
        QPainter painter(this);
        painter.drawPixmap(0,0,_pix);
}

/**
 * @brief Dialog::drawRect
 * 鼠标拖动，绘制矩形。使用双缓冲绘图
 * 直接在窗口上绘制矩形，不能保存之前的图形
 * 只有一个画布时，会产生重影
 */
void Dialog::drawRect()
{
    int x,y,w,h;
    x = _lastPoint.x();
    y = _lastPoint.y();
    w = _endPoint.x()-x;
    h = _endPoint.y()-y;

    QPainter painter(this);
    //painter.drawRect(x,y,w,h);//直接在窗口上绘制，不能保存之前的图形

    //使用一个画布，会产生重影
//    QPainter pp(&_pix);
//    pp.drawRect(x,y,w,h);
//    painter.drawPixmap(0,0,_pix);

    if (_isDrawing)
    {
        tempPix = _pix;
        QPainter pp(&tempPix);
        pp.drawRect(x,y,w,h);
        painter.drawPixmap(0,0,tempPix);
    }
    else {
        QPainter pp(&_pix);
        pp.drawRect(x,y,w,h);
        painter.drawPixmap(0,0,_pix);
    }
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    //drawLine();
    drawRect();
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)  //鼠标左键按下
    {
        _lastPoint = event->pos();
        _isDrawing = true;
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)  //鼠标左键按下的同时移动
    {
        _endPoint = event->pos();
        update();
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        _endPoint = event->pos();
        _isDrawing = false;
        update();
    }
}

/**
 * @brief Dialog::zoomIn
 * 放大功能有两种实现方式。
 * 一种是放大窗口坐标系统
 * 一种直接放大画布的坐标系统
 */
void Dialog::zoomIn()
{
    _scale*=2;
    update();
}
