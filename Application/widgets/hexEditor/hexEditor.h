#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <QWidget>

#include <QObject>
#include <QPlainTextEdit>
#include <QEvent>
#include <QString>
#include <QScrollBar>
#include "asciiPlainTextEdit.h"

namespace Ui {
class HexEditor;
}

class HexEditor : public QWidget
{
    Q_OBJECT
    
public:
    explicit HexEditor(QWidget *parent = 0);
    ~HexEditor();
    unsigned char *buffer;
    bool eventFilter(QObject *obj, QEvent *event);
    void loadFile(QString address);
    void loadFromDevice(unsigned char * data, quint32 len, quint32 maxLen);
    void loadFromDevice(quint16 *data, quint32 len, quint32 maxLen, int byteLen=2);
    quint8 bitLength();
    quint8 byteLength();

    void setBitLength(int length);

    QChar hexToAscii(QString hexStr);
    QString asciiToHex(QChar ch);
    quint32 size();
    void setSize(quint32 size);
    void setColumnCount(int length);
private slots:


    void on_asciiTextEdit_textChanged();
    void on_asciiTextEdit_selectionChanged();
    void on_asciiTextEdit_cursorPositionChanged();

    void on_hexTextEdit_cursorPositionChanged();
    void on_hexTextEdit_selectionChanged();
    void on_hexTextEdit_textChanged();
    void on_hexTextEdit_copyAvailable(bool b);
    void on_hexTextEdit_blockCountChanged(int newBlockCount);

    void on_asciiTextEdit_modificationChanged(bool arg1);

private:
    Ui::HexEditor *ui;


    int lastHexPosition;
    int lastAsciiPosition;
    bool loadLine(QString line, int &lineNumber);
    bool otherPosChangeSignal;
    bool busyToLoad;
    quint32 index;
    quint32 _size;
    int hexPosToAsciiPos(int pos);

    int asciiPosToHexPos(int pos, int byteLength=0);

    QString emptyHexLine;
    QString emptyAsciiLine;
    void makeEmptyLines();
    quint8 _columnCount;
    QScrollBar *scrollbar;
	unsigned char *data8;
	quint16 * data16;
    quint8 textChangedCount;
    int asciiPosToDataPos(int pos, int byteLength=0);
protected:
    void resizeEvent(QResizeEvent * event);

};

#endif
