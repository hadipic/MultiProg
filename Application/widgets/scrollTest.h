#ifndef SCROLLTEST_H
#define SCROLLTEST_H

#include <QWidget>
#include <QScrollBar>

class ScrollTest : public QWidget
{
    Q_OBJECT
public:
    explicit ScrollTest(QWidget *parent = 0);
    
signals:
    
public slots:
    void updateRange(int min, int max);
private:
    QScrollBar *scrollbar;
    
};

#endif // SCROLLTEST_H
