#ifndef ASCIIPLAINTEXTEDIT_H
#define ASCIIPLAINTEXTEDIT_H

#include <QPlainTextEdit>

class AsciiPlainTextEdit : public QPlainTextEdit
{
public:
    AsciiPlainTextEdit(QWidget *parent);
    void keyPressEvent(QKeyEvent *event);
};

#endif // ASCIIPLAINTEXTEDIT_H
