#ifndef HexTextBox_H
#define HexTextBox_H

#include <QPlainTextEdit>

class HexPlainTextEdit : public QPlainTextEdit
{
 Q_OBJECT
public:

    HexPlainTextEdit(QWidget *parent=0);
    quint8 bitLength();
    quint8 byteLength();
    void setBitLength(int length);
    quint64 maxData();

    void keyPressEvent(QKeyEvent *event);
    bool isInSelection();

    bool loadLine(QString line);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void setBase(quint16 base);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void myTextChanged();

private:
    int lastHexPosition;
    quint8 _bitLength;
    quint8 _byteLength;
    quint16 _base;
    bool    _isInSelection;
    QString prepareAddressToShow(int number);
    QWidget *lineNumberArea;
    quint64 _maxData;
};

class AddressArea : public QWidget
{
public:
    AddressArea(HexPlainTextEdit *hexEdit) : QWidget(hexEdit)
    {
        hexPlainTextEdit = hexEdit;
    }

    QSize sizeHint() const
    {
        return QSize(hexPlainTextEdit->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        hexPlainTextEdit->lineNumberAreaPaintEvent(event);
    }

private:
    HexPlainTextEdit *hexPlainTextEdit;
};

#endif // HEXEDITOR_H


