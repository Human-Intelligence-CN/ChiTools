#include "widget.h"
#include "./ui_widget.h"
#include <QFileDialog>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QString>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QWidget>
#include <QAudioOutput>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QInputDialog>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(800,500);
    this->createToolBar();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)//更新时把图片放进来
{
    QPainter p(this);
    p.drawPixmap(0,0,this->usingPix.width(),this->usingPix.height(),this->usingPix);
    QWidget::paintEvent(event);
}

void Widget::createToolBar()
{
    this->toolBar=new QToolBar(this);

    QAction* openAct=new QAction(QIcon(":/icons/open.png"),"打开图片");
    this->toolBar->addAction(openAct);
    connect(openAct,&QAction::triggered,this,&Widget::openPic);

    QAction* usingBuiltin=new QAction(QIcon(":/icons/pic.png"),"使用内置文件");
    this->toolBar->addAction(usingBuiltin);
    connect(usingBuiltin,&QAction::triggered,this,&Widget::openBuiltin);

    QAction* restore=new QAction(QIcon(":/icons/restore.png"),"还原");
    this->toolBar->addAction(restore);
    connect(restore,&QAction::triggered,this,&Widget::restorePic);

    QAction* greyScale=new QAction(QIcon(":/icons/grey.png"),"灰阶化");
    this->toolBar->addAction(greyScale);
    connect(greyScale,&QAction::triggered,this,&Widget::greyScalization);

    QAction* binarize=new QAction(QIcon(":/icons/bin.png"),"二值化");
    this->toolBar->addAction(binarize);
    connect(binarize,&QAction::triggered,this,&Widget::binarization);

    QAction* selfBinarize=new QAction(QIcon(":/icons/auto.png"),"自适应阈值二值化");
    this->toolBar->addAction(selfBinarize);
    connect(selfBinarize,&QAction::triggered,this,&Widget::selfBinarization);
}

void Widget::openPic()
{
    QString filePath=QFileDialog::getOpenFileName(this,"选择图片","./","图片(*.jpg *.bmp *.png *.webp *.jpeg)");
    this->originalPix=QPixmap(filePath);
    this->resize(this->originalPix.size());
    usingPix=originalPix;
    this->update();
}

void Widget::openBuiltin()
{
    this->originalPix=QPixmap(":/images/builtInImage.jpg");
    this->resize(this->originalPix.size());
    usingPix=originalPix;
    this->update();
}

void Widget::restorePic()
{
    usingPix=originalPix;
    this->update();
}

void Widget::greyScalization()
{
    usingPix=originalPix;
    QImage pic=this->usingPix.toImage();
    pic=pic.convertToFormat(QImage::Format_Grayscale16);
    this->usingPix=QPixmap::fromImage(pic);
    this->update();
}

void Widget::binarization()
{
    usingPix=originalPix;
    QImage pic=this->usingPix.toImage();
    pic=pic.convertToFormat(QImage::Format_Grayscale16);
    qint32 avgGrey=0;
    for(qint32 i=0;i<pic.width();i++)
        for(qint32 j=0;j<pic.height();j++)
        {
            QRgb pix=pic.pixel(i,j);
            avgGrey+=qGray(pix);
        }
    avgGrey=avgGrey/(pic.width())/(pic.height());
    for(qint32 i=0;i<pic.width();i++)
        for(qint32 j=0;j<pic.height();j++)
        {
            QRgb pix=pic.pixel(i,j);
            qint32 grey=qGray(pix);
            if (grey<=avgGrey)
                pic.setPixel(i,j,qRgb(0,0,0));
            else
                pic.setPixel(i,j,qRgb(255,255,255));
        }
    this->usingPix=QPixmap::fromImage(pic);
    this->update();
}

QPixmap blurPic(const QPixmap&sourcePix,qreal blurSize)
{
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurSize);
    QGraphicsScene scene;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(sourcePix);
    item->setGraphicsEffect(blur);
    scene.addItem(item);
    QPixmap blurPix = QPixmap(sourcePix.size());
    QPainter painter(&blurPix);
    scene.render(&painter);
    delete item;
    return blurPix;
}

void Widget::selfBinarization()
{
    qreal value=QInputDialog::getDouble(this,"输入窗口","请输入自适应二值化阈值",5,1.01,6000,2);
    usingPix=originalPix;
    QImage pic=usingPix.toImage();
    QPixmap *tempPix=new QPixmap(blurPic(usingPix,value));
    QImage tempPic=tempPix->toImage();
    pic=pic.convertToFormat(QImage::Format_ARGB32);
    tempPic=tempPic.convertToFormat(QImage::Format_ARGB32);
    for(qint32 i=0;i<pic.width();i++)
        for(qint32 j=0;j<pic.height();j++)
        {
            QRgb tempPixel=tempPic.pixel(i,j);
            QRgb realPixel=pic.pixel(i,j);
            qint32 grey=qGray(realPixel);
            qint32 grey2=qGray(tempPixel);
            if(grey<grey2)
                pic.setPixel(i,j,qRgb(0,0,0));
            else
                pic.setPixel(i,j,qRgb(255,255,255));
        }
    usingPix=QPixmap::fromImage(pic);
    this->update();
}

