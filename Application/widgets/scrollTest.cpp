#include "scrollTest.h"
#include "QGraphicsScene"
#include "QGraphicsView"
#include "QHBoxLayout"

ScrollTest::ScrollTest(QWidget *parent) :
    QWidget(parent)
{
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addEllipse(QRectF(-100, -100, 300, 200), QPen(Qt::blue), QBrush(Qt::cyan));

    QGraphicsView *view1 = new QGraphicsView(scene);
    view1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QGraphicsView *view2 = new QGraphicsView(scene);
    view2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scrollbar = new QScrollBar(Qt::Horizontal, this);
    QVBoxLayout *vertical = new QVBoxLayout(this);
    QWidget *subWidget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(subWidget);
    layout->addWidget(view1);
    layout->addWidget(view2);

    vertical->addWidget(subWidget);
    vertical->addWidget(scrollbar);
    scrollbar->setRange(view1->horizontalScrollBar()->minimum(), view1->horizontalScrollBar()->maximum());

    connect(scrollbar, SIGNAL(valueChanged(int)), view1->horizontalScrollBar(), SLOT(setValue(int)));
    connect(scrollbar, SIGNAL(valueChanged(int)), view2->horizontalScrollBar(), SLOT(setValue(int)));
    connect(view1->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(updateRange(int, int)));

}

void ScrollTest::updateRange(int min, int max)
{
    scrollbar->setRange(min, max);
}
