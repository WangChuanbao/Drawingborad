#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void drawLine();
    void drawRect();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void zoomIn();

private:
    Ui::Dialog *ui;
    QPixmap _pix;
    QPoint _lastPoint;
    QPoint _endPoint;
    double _scale;
    QPushButton *_button;

    bool _isDrawing;
    QPixmap tempPix;
};

#endif // DIALOG_H
