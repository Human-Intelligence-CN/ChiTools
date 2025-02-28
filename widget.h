#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QToolBar>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void createToolBar();
    void openPic();
    void openBuiltin();
    void greyScalization();
    void binarization();
    void videoPlay();
    void selfBinarization();
    void restorePic();
    Ui::Widget *ui;
    QPixmap originalPix;
    QPixmap usingPix;
    QToolBar* toolBar;
    QGraphicsBlurEffect *blur;
    QGraphicsScene *scene;

protected:
    void paintEvent(QPaintEvent *event)Q_DECL_OVERRIDE;
};
#endif // WIDGET_H
