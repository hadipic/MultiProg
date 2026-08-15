#include <QtGui>
#include <QDebug>
#include "HexPlainTextEdit.h"


HexPlainTextEdit::HexPlainTextEdit(QWidget *parent):QPlainTextEdit(parent)
{

    lastHexPosition=0;
    _isInSelection=false;

    setBitLength(16);

    lineNumberArea = new AddressArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(textChanged()), this, SLOT(myTextChanged()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();



}

quint8 HexPlainTextEdit::bitLength()
{
    return _bitLength;
}

quint8 HexPlainTextEdit::byteLength()
{
    return _byteLength;
}



void HexPlainTextEdit::keyPressEvent ( QKeyEvent * event )
{
    //qDebug()<<event->key()<<event->modifiers();
    //event->modifiers().Int
    //0x2000000
    int pos=textCursor().position();
    int len=toPlainText().length();
    //qDebug()<<pos<<len;
    if(pos>=0 && pos<len && len>0)
        if(toPlainText().at(pos).unicode()==10)
        {
            QTextCursor c = textCursor();
            c.setPosition(pos-1);
            setTextCursor(c);
        }
    //qDebug()<<"keyPressEvent"<<pos<<toPlainText().at(pos);
    int key=event->key();
    QChar ch=key;
    ch=ch.toUpper();
    switch (event->key())
    {
        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Right:
        case Qt::Key_Left:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageDown:
        case Qt::Key_PageUp:
            QPlainTextEdit::keyPressEvent ( event );
    }

    if(key==Qt::Key_Backspace)
    {
        QKeyEvent * myKeyEvent = new QKeyEvent ( QEvent::KeyPress , Qt::Key_Left , Qt::NoModifier, 0);
        QPlainTextEdit::keyPressEvent ( myKeyEvent );
    }

    if(ch.isDigit() || (key>='A' && key<='F') )
    {
        key =ch.toLatin1();
        //if(key>='a' && key<='f')
        {
            QKeyEvent * myKeyEvent = new QKeyEvent ( QEvent::KeyPress , key , Qt::ShiftModifier, ch);
            QPlainTextEdit::keyPressEvent ( myKeyEvent );
        }
    }
    //qDebug()<<"hex keyPressEvent finish";
}

bool HexPlainTextEdit::isInSelection()
{
    return _isInSelection;
}


void HexPlainTextEdit::setBitLength(int length)
{
    if(length>=4  && length<=64)
    {
        _bitLength=length;
        _byteLength = length/8+ (length%8>0);
        _maxData=qPow(2, _bitLength)-1;
        qDebug()<<"maxData:"<<_maxData<<"bitLength:"<<_bitLength;
    }

}

quint64 HexPlainTextEdit::maxData()
{
    return _maxData;
}




QString HexPlainTextEdit::prepareAddressToShow(int number)
{
    int len=4;//QString::number(blockCount()*8, 16).length();
    return QString("%1:").arg(number*8, len, 16, QChar('0')).toUpper();
}

int HexPlainTextEdit::lineNumberAreaWidth()
{
    int digits = prepareAddressToShow(blockCount()).length();
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    //qDebug()<<"Blockcount Changed:"<<space<<blockCount();
    return space;
}



void HexPlainTextEdit::updateLineNumberAreaWidth(int newBlockCount )
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void HexPlainTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    //qDebug()<<"updateRequest:"<<rect.x()<<rect.y()<<rect.width()<<rect.height()<<dy;
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void HexPlainTextEdit::myTextChanged()
{
    int i=0;
    QString text= toPlainText();
    for (;i<text.length();i++)
    {
        if(text.at(i)==10 || text.at(i)==13)
            break;
    }
    i+=10;
    //setMinimumWidth(fontMetrics().width(QLatin1Char('A')) * i);
}



void HexPlainTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void HexPlainTextEdit::highlightCurrentLine()
{

    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void HexPlainTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(238,227,236));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = prepareAddressToShow(blockNumber);
            painter.setPen(Qt::blue);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        //qDebug()<<"blockNumber"<<blockNumber;
        ++blockNumber;

    }
}
