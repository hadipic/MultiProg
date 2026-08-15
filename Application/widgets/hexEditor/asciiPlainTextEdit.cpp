#include "asciiPlainTextEdit.h"
#include <QDebug>

AsciiPlainTextEdit::AsciiPlainTextEdit(QWidget *parent):QPlainTextEdit(parent)
{
    qDebug()<<"AsciiPlainTextEdit";
}

void AsciiPlainTextEdit::keyPressEvent( QKeyEvent * event )
{
    qDebug()<<"Ascii keyPressEvent";
    int pos=textCursor().position();
    int len=toPlainText().length();

    if(pos>=0 && pos>=len-1 && len>0)
    {
       QTextCursor c = textCursor();
       c.setPosition(pos-1);
       setTextCursor(c);
    }
    int key=event->key();
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
    else if(key!=Qt::Key_Delete &&  key>=32 && key<127)
        QPlainTextEdit::keyPressEvent ( event );
    event->accept();

}
