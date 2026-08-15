#ifndef CONVERT_H
#define CONVERT_H

#include <QMainWindow>
#include <QObject>
#include <QAction>
#include "QString"

#include <cstring>
#include <unistd.h>
#include <QPlainTextEdit>
#include <getopt.h>
#include <QDebug>


namespace Ui {
class convert;
}

class convert : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit convert(QWidget *parent = 0);
    ~convert();
    
private slots:
    void on_convert_iconSizeChanged(const QSize &iconSize);

    void on_textBrowser_textChanged();

    void on_textEdit_textChanged();

private:
    Ui::convert *ui;
};

#endif // CONVERT_H
