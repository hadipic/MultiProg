#define SHOW_DEBUG 0

#include "hexEditor.h"
#include "ui_hexEditor.h"
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include "QMessageBox"

HexEditor::HexEditor(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::HexEditor)
{
   ui->setupUi(this);
   //loadFile("D:\\aplication\\ds1820_pic12f675.hex");
   lastHexPosition=lastAsciiPosition=0;
   otherPosChangeSignal=true;
   buffer=NULL;
   busyToLoad=false;
   _columnCount=16;
   textChangedCount=0;
   ui->asciiTextEdit->setParent(ui->hexTextEdit);
   connect(ui->asciiTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->hexTextEdit->verticalScrollBar(), SLOT(setValue(int)));
   connect(ui->hexTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->asciiTextEdit->verticalScrollBar(), SLOT(setValue(int)));
}

HexEditor::~HexEditor()
{
   delete ui;
}

void HexEditor::setColumnCount(int length)
{
   if(length>=4  && length<=64)
   {
      _columnCount=length;
   }
}

bool HexEditor::eventFilter(QObject *obj, QEvent *event)
{
   return true;
}

int HexEditor::hexPosToAsciiPos(int pos)
{
   int a=ui->hexTextEdit->byteLength()*2+1;
   int b=ui->hexTextEdit->byteLength();
   int newPos=((pos+1)*b/a);
   newPos += newPos/_columnCount;
   return newPos;
}

int HexEditor::asciiPosToDataPos(int pos, int byteLength)
{
   if(byteLength==0)
      byteLength=this->byteLength();
   return ((pos-(pos/(_columnCount+1)))/byteLength);
}


int HexEditor::asciiPosToHexPos(int pos, int byteLength)
{
   if(byteLength==0)
      byteLength=this->byteLength();
   int a=(pos-(pos/(_columnCount+1)))*2;
   int b=a/(ui->hexTextEdit->byteLength()*2);
   int newPos=a + b;
   return newPos;
}

void HexEditor::makeEmptyLines()
{
   emptyHexLine=emptyAsciiLine="";
   quint8 max=16/byteLength();
   quint8 k=_columnCount/byteLength();
   for(int i=0;i<k; i++)
   {
      if(i>0)
         emptyHexLine+=" ";
      for(int j=0;j<byteLength(); j++)
      {
         emptyHexLine+="FF";
         emptyAsciiLine+=".";
      }
   }
   qDebug()<<emptyAsciiLine;
}

void HexEditor::resizeEvent(QResizeEvent *event)
{
   QWidget::resizeEvent(event);
   ui->asciiTextEdit->resize(ui->asciiTextEdit->width(), ui->hexTextEdit->height());
   ui->asciiTextEdit->move(width()-ui->asciiTextEdit->width()-18,0);
}

void HexEditor::loadFile(QString address)
{
   busyToLoad=true;
   otherPosChangeSignal=false;
   QFile *file = new QFile(address);
   if(!file->open(QFile::ReadOnly | QFile::Text))
   {
      QMessageBox::warning(this, tr("Log Reader"),
                           tr("Cannot read file %1:\n%2")
                           .arg(address)
                           .arg(file->errorString()));
      return;
   }
   QTextStream in(file);
   QString fileData=in.readAll();
   QStringList lines=fileData.trimmed().split("\n");
   bool fileIsOk=true;
   ui->hexTextEdit->clear();
   ui->asciiTextEdit->clear();
   //ui->txtAddress->clear();

   int bufSize=32*1024;// 32kb
   if(buffer)
      free(buffer);
   buffer=(unsigned char*)malloc(bufSize);
   memset(buffer,0xFF,bufSize);

   makeEmptyLines();
   index=0;
   int lineNumber=0;
   for(int i=0; i<lines.length();i++)
      if(!loadLine(lines[i].trimmed(), lineNumber))
      {

      }

   int len=bufSize/_columnCount;
   qDebug()<<"lineNumber"<<lineNumber;
   if(lineNumber<len)
   {

      QString hexStr="";
      QString asciiStr="";
      for(int k=0;lineNumber<len;lineNumber++, k++)
      {
         if(k>0)
         {
            hexStr+=10;
            asciiStr+=10;
         }
         hexStr+=emptyHexLine;
         asciiStr+=emptyAsciiLine;
      }
      ui->hexTextEdit->appendPlainText(hexStr);
      ui->asciiTextEdit->appendPlainText(asciiStr);
   }
   QTextCursor c = ui->asciiTextEdit->textCursor();
   c.setPosition(0);
   ui->asciiTextEdit->setTextCursor(c);
   c = ui->hexTextEdit->textCursor();
   c.setPosition(0);
   ui->hexTextEdit->setTextCursor(c);
   busyToLoad=false;
   otherPosChangeSignal=true;
}

void HexEditor::loadFromDevice(unsigned char *data, quint32 len, quint32 maxLen)
{
   textChangedCount=0;
   data16=NULL;
   data8=data;
   if(!data)
      return;
   busyToLoad=true;
   otherPosChangeSignal=false;
   QString hexStr="",asciiStr="";
   qDebug()<<"byteLength"<<byteLength();
   int k=0;
   quint8 byte=0;
   for(int i=0;i<maxLen; i++)
   {
      if(i<len*2)
      {
         if(byteLength()==1)
            byte=(quint8)data[i];
         else
         {
            if(i%2==0)
               byte=(quint8)data[i+1];
            else
               byte=(quint8)data[i-1];
         }
      }
      else
         byte = (quint8)ui->hexTextEdit->maxData();

      hexStr+=QString("%1").arg(byte, 2,16, QChar('0')).toUpper();
      if(byte>=32 && byte<127)
         asciiStr+=byte;
      else
         asciiStr+=".";

      if((i+1)%_columnCount==0)
      {
         if(k==0)
         {
            int a=ui->hexTextEdit->fontMetrics().width(hexStr+"0000: ");
            int b=ui->asciiTextEdit->fontMetrics().width(asciiStr)+30;
            ui->asciiTextEdit->setMinimumWidth(b);
            ui->asciiTextEdit->setMaximumWidth(b);
            setMinimumWidth(b+a+20);
            ui->asciiTextEdit->move(width()-ui->asciiTextEdit->width()-18,0);
            qDebug()<<"fontMetrics:"<<a<<b;
            k=1;
         }
         hexStr+="\n";
         asciiStr+="\n";
      }
      else
         if((i+1)%byteLength()==0)
            hexStr+=" ";
   }
   ui->hexTextEdit->clear();
   ui->asciiTextEdit->clear();
   hexStr.remove(hexStr.length()-1, 1);
   asciiStr.remove(asciiStr.length()-1, 1);
   ui->hexTextEdit->setPlainText(hexStr+" ");
   ui->asciiTextEdit->setPlainText(asciiStr+" ");
   busyToLoad=false;
   otherPosChangeSignal=true;
}

void HexEditor::loadFromDevice(quint16 *data, quint32 len, quint32 maxLen, int byteLen)
{
   textChangedCount=0;
   data8=NULL;
   data16=data;
   if(!data)
      return;
   busyToLoad=true;
   otherPosChangeSignal=false;
   QString hexStr="",asciiStr="";
   int a=3-byteLen;
   int k=0;
   unsigned char byte=0;
   for(int i=2-byteLen, j=0;i<maxLen*2; i+=a, j++)
   {
      if(i<len*2 && data[i/2]<=ui->hexTextEdit->maxData())
      {
         if(i%2==1)
            byte= data[i/2];
         else
            byte = data[i/2]>>8;
      }
      else
      {
         if(i%2==1)
            byte = (quint8)ui->hexTextEdit->maxData();
         else
            byte = (quint8)(ui->hexTextEdit->maxData()>>8);
      }

      hexStr+=QString("%1").arg((quint8)byte, 2, 16, QChar('0')).toUpper();
      if(byte>=32 && byte<127)
         asciiStr+=byte;
      else
         asciiStr+=".";
      if(i<maxLen*2-1)
      {
         if((j+1)%_columnCount*byteLength()==0)
         {
            if(k==0)
            {
               int a=ui->hexTextEdit->fontMetrics().width(hexStr+"0000: ");
               int b=ui->asciiTextEdit->fontMetrics().width(asciiStr)+35;
               ui->asciiTextEdit->setMinimumWidth(b);
               ui->asciiTextEdit->setMaximumWidth(b);
               setMinimumWidth(b+a+25);
               ui->asciiTextEdit->move(width()-ui->asciiTextEdit->width()-18,0);
               qDebug()<<"fontMetrics:"<<a<<b;
               k=1;
            }
            hexStr+="\n";
            asciiStr+="\n";
         }
         else
            if((i+1)%byteLength()==0)
               hexStr+=" ";
      }
   }
   ui->hexTextEdit->clear();
   ui->asciiTextEdit->clear();
   qDebug()<<"hexStr"<<hexStr[hexStr.length()-1];
   //hexStr.remove(hexStr.length()-1, 1);
   //asciiStr.remove(asciiStr.length()-1, 1);
   ui->hexTextEdit->setPlainText(hexStr+" ");
   ui->asciiTextEdit->setPlainText(asciiStr+" ");
   busyToLoad=false;
   otherPosChangeSignal=true;
}

quint8 HexEditor::bitLength()
{
   return ui->hexTextEdit->bitLength();
}

quint8 HexEditor::byteLength()
{
   return ui->hexTextEdit->byteLength();
}

bool HexEditor::loadLine(QString line, int &lineNumber)
{
   try
   {
      if(line.startsWith(":10"))
      {
         if(line.length()>=7)
         {
            bool ok=false;
            int address =line.mid(3, 4).toInt(&ok, 16);
            //ui->txtAddress->appendPlainText(line.mid(3, 4));
            //qDebug()<<line.mid(3, 4)<<address;
            if(ok)
            {
               line=line.mid(9, line.length()-2-9);
               int rowSize=line.length()/2;
               address/=rowSize;
               QString hexStr="", asciiStr="";
               lineNumber++;
               for( int k=0; lineNumber<address; lineNumber++)
               {
                  for(int j=0;j<rowSize/_columnCount;j++,k++)
                  {
                     if(k>0)
                     {
                        hexStr+=10;
                        asciiStr+=10;
                     }
                     hexStr+=emptyHexLine;
                     asciiStr+=emptyAsciiLine;
                  }
               }
               if(hexStr.length()>0)
               {
                  qDebug()<<"hexStr"<<hexStr<<hexStr.length();
                  ui->hexTextEdit->appendPlainText(hexStr);
                  ui->asciiTextEdit->appendPlainText(asciiStr);
               }
               lineNumber=address;
               QString asciiPartStr="",hexPartStr="", byteStr="";
               hexStr=""; asciiStr="";
               int k=0;
               QChar ch;
               for(int i=0, j=0;i<line.length(); i+=2, j++)
               {
                  if(i-2>0 && (i-2)%(_columnCount*2)==0 && i<line.length()-2)
                  {
                     hexStr+=10;
                     asciiStr+=10;
                     k=0;
                  }
                  byteStr= line.mid(i, 2);
                  if(hexPartStr.length()>0)
                  {
                     if(j%byteLength()==0)
                     {
                        if(k>0)
                           hexStr+=" ";
                        hexStr+=hexPartStr;
                        hexPartStr="";
                        asciiStr+=asciiPartStr;
                        asciiPartStr="";
                        k++;
                     }
                  }
                  hexPartStr=byteStr+hexPartStr;
                  int byte=byteStr.toInt(&ok, 16);
                  buffer[index++]=byte;
                  if(byte>=32 && byte<127)
                     ch=byte;
                  else
                     ch='.';
                  asciiPartStr=ch+asciiPartStr;
               }
               if(k>0)
                  hexStr+=" ";
               hexStr+=hexPartStr;
               asciiStr+=asciiPartStr;
               ui->hexTextEdit->appendPlainText(hexStr);
               ui->asciiTextEdit->appendPlainText(asciiStr);
               //ui->asciiTextEdit->appendHtml("<p  >"+asciiStr+"</p>");
               //ui->asciiTextEdit->to
               return true;
            }
         }
      }
   }
   catch (...)
   {
      qDebug()<<"error to read data from file";
   }
   return false;
}



void HexEditor::on_asciiTextEdit_textChanged()
{
   //int pos=ui->asciiTextEdit->textCursor().position();

   if(SHOW_DEBUG)
      qDebug()<<"on_asciiTextEdit_textChanged";
   if(!otherPosChangeSignal || busyToLoad || ui->asciiTextEdit->toPlainText().length()==0)
      return;

   QString asciiText=ui->asciiTextEdit->toPlainText();

   int pos=ui->asciiTextEdit->textCursor().position();
   int len=asciiText.length();
   if(pos<len && pos>=0)
   {
      otherPosChangeSignal=false;
      int start,end;
      start=end=pos;
      int hexPos=0;
      for(int i=0;start>0 && i<2;start--, i++);
      for(int i=0;end<len && i<2;end++, i++);

      QTextCursor c=ui->hexTextEdit->textCursor();
      int j=0;
      quint16 dt=0;
      for(int i=start;i<end ;i++)
      {
         QChar ch=asciiText.at(i);
         qDebug()<<"on_asciiTextEdit_textChanged:"<<ch;
         if(ch!=10)
         {
            QString hexCh="";
            quint8 byte=0;
            int dataPos=0;
            if(data8 && byteLength()>1)
               dataPos=asciiPosToDataPos(i,1);
            else
               dataPos=asciiPosToDataPos(i);
            /*if(data8 && byteLength()>1 && dataPos!=i)
                    dataPos=i;*/
            j=(i-(i/(_columnCount+1)))%2;
            if(ch!='.')
            {
               hexCh=asciiToHex(ch);
               byte=hexCh.toInt(0, 16);
            }
            else
            {
               if(data8)
               {
                  if(byteLength()==1)
                     byte=data8[dataPos];
                  else
                  {
                     if(dataPos%2==0)
                        byte=data8[dataPos+1];
                     else
                        byte=data8[dataPos-1];
                  }
               }
               else  if(data16)
               {
                  if(j==1 || byteLength()==1)
                     byte=data16[dataPos];
                  else
                     byte=data16[dataPos]>>8;
               }
               hexCh=asciiToHex(QChar(byte));
            }

            if(data16)
            {
               if(byteLength()==1)
                  data16[dataPos]=byte;
               else
               {
                  quint16 word=byte;
                  if(j==0)//high byte
                  {
                     data16[dataPos]=((data16[dataPos]&0x00FF) | (word<<8));
                  }
                  else
                     if(j==1)
                     {
                        data16[dataPos]=((data16[dataPos]&0xFF00) | word);
                     }
               }
               //j=0;
               //dt=byte+dt;
               //data16[dataPos]=dt;
            }
            else
               dt=byte;
            if(data8)
            {
               if(byteLength()==1)
                  data8[dataPos]=dt;
               if(byteLength()>1)
               {
                  if(dataPos%2==0)
                     data8[dataPos+1]=dt;
                  else
                     data8[dataPos-1]=dt;
               }

            }
            if(ch!='.')
            {

               //qDebug()<<i<<hexCh<<ui->asciiTextEdit->toPlainText().at(i);
               hexPos=asciiPosToHexPos(i);
               c.clearSelection();
               c.setPosition(hexPos);
               c.setPosition(hexPos+2, QTextCursor::KeepAnchor);
               c.deleteChar();
               c.insertText(hexCh);
               qDebug()<<"hexPos"<<hexPos<<"pos"<<c.position()<<c.selectionStart()<<c.selectionEnd();
            }
         }
      }
      ui->hexTextEdit->setTextCursor(c);
      otherPosChangeSignal=true;
   }
}


void HexEditor::on_hexTextEdit_cursorPositionChanged()
{
   if(!otherPosChangeSignal)
      return;
   if(SHOW_DEBUG)
      qDebug()<<"on_hexTextEdit_cursorPositionChanged";
   int pos=ui->hexTextEdit->textCursor().position();
   int len=ui->hexTextEdit->toPlainText().length();
   //if(pos<len)
   //    qDebug()<<"Pos:"<<pos<<ui->hexTextEdit->toPlainText().at(pos)<<(int)ui->hexTextEdit->toPlainText().at(pos).toLatin1();

   QTextCursor c = ui->asciiTextEdit->textCursor();
   int newPos = hexPosToAsciiPos(pos);
   c.setPosition(newPos);
   c.setPosition(newPos+1, QTextCursor::KeepAnchor);
   otherPosChangeSignal=false;
   ui->asciiTextEdit->setTextCursor(c);
   otherPosChangeSignal=true;

   if(ui->hexTextEdit->isInSelection())
      return;

   pos=ui->hexTextEdit->textCursor().position();
   //qDebug()<<pos<<ui->hexTextEdit->toPlainText().length();
   if(pos>=len-1)
   {
      QTextCursor c = ui->hexTextEdit->textCursor();
      c.setPosition(len-2);//, QTextCursor::KeepAnchor, 2);
      ui->hexTextEdit->setTextCursor(c);

   }
   else
   {
      QChar ch=ui->hexTextEdit->toPlainText().at(ui->hexTextEdit->textCursor().position());
      if ((ch==' ' ||  ch==10))
      {
         QTextCursor c = ui->hexTextEdit->textCursor();
         if(ui->hexTextEdit->textCursor().position()-lastHexPosition>0)
            c.movePosition(QTextCursor::Right);
         else
            c.movePosition(QTextCursor::Left);
         ui->hexTextEdit->setTextCursor(c);
      }
   }
   lastHexPosition = ui->hexTextEdit->textCursor().position();

}

void HexEditor::on_hexTextEdit_selectionChanged()
{
   if(SHOW_DEBUG)
      qDebug()<<"on_hexTextEdit_selectionChanged";
   if(!otherPosChangeSignal)
      return;
   QTextCursor c = ui->asciiTextEdit->textCursor();
   int start=ui->hexTextEdit->textCursor().selectionStart();
   int end=ui->hexTextEdit->textCursor().selectionEnd();

   int newStart = hexPosToAsciiPos(start);
   int newEnd = hexPosToAsciiPos(end);

   c.setPosition(newStart);
   c.setPosition(newEnd, QTextCursor::KeepAnchor);
   otherPosChangeSignal=false;
   ui->asciiTextEdit->setTextCursor(c);
   otherPosChangeSignal=true;
}

void HexEditor::on_asciiTextEdit_selectionChanged()
{
   if(SHOW_DEBUG)
      qDebug()<<"on_asciiTextEdit_selectionChanged";
   if(!otherPosChangeSignal)
      return;
   QTextCursor c = ui->hexTextEdit->textCursor();
   int start=ui->asciiTextEdit->textCursor().selectionStart();
   int end=ui->asciiTextEdit->textCursor().selectionEnd();

   int newStart = asciiPosToHexPos(start);
   int newEnd = asciiPosToHexPos(end);

   c.setPosition(newStart);
   c.setPosition(newEnd, QTextCursor::KeepAnchor);
   otherPosChangeSignal=false;
   ui->hexTextEdit->setTextCursor(c);
   otherPosChangeSignal=true;
}

void HexEditor::on_asciiTextEdit_cursorPositionChanged()
{
   if(SHOW_DEBUG)
      qDebug()<<"on_asciiTextEdit_cursorPositionChanged";

   if(!otherPosChangeSignal)
      return;
   QTextCursor c = ui->hexTextEdit->textCursor();
   int pos=ui->asciiTextEdit->textCursor().position();

   quint16 len=ui->asciiTextEdit->toPlainText().length();
   if(pos>=len-1)
   {
      QTextCursor c = ui->asciiTextEdit->textCursor();
      c.setPosition(len-1);//, QTextCursor::KeepAnchor, 2);
      ui->asciiTextEdit->setTextCursor(c);

   }
   else
   {
      QChar ch=ui->asciiTextEdit->toPlainText().at(ui->asciiTextEdit->textCursor().position());
      if (ch==10)
      {
         QTextCursor c = ui->asciiTextEdit->textCursor();
         if(ui->asciiTextEdit->textCursor().position()-lastAsciiPosition>0)
            c.movePosition(QTextCursor::Right);
         else
            c.movePosition(QTextCursor::Left);
         ui->asciiTextEdit->setTextCursor(c);
      }
      else
      {
         int newPos = asciiPosToHexPos(pos);
         c.setPosition(newPos);
         c.setPosition(newPos+2, QTextCursor::KeepAnchor);
         otherPosChangeSignal=false;
         ui->hexTextEdit->setTextCursor(c);
         otherPosChangeSignal=true;
      }
   }
   lastAsciiPosition = ui->asciiTextEdit->textCursor().position();
   //qDebug()<<"lastAsciiPosition"<<lastAsciiPosition ;
}
void HexEditor::on_hexTextEdit_copyAvailable(bool b)
{
   //qDebug()<<"copyAvailable";
}

void HexEditor::on_hexTextEdit_blockCountChanged(int newBlockCount)
{
   //qDebug()<<"blockCountChanged"<<newBlockCount;
   //ui->asciiTextEdit->setb
}


void HexEditor::on_hexTextEdit_textChanged()
{
   if(SHOW_DEBUG)
      qDebug()<<"on_hexTextEdit_textChanged";


   if(!otherPosChangeSignal || busyToLoad || ui->hexTextEdit->toPlainText().length()==0)
      return;

   otherPosChangeSignal=false;
   int pos=ui->hexTextEdit->textCursor().position();

   QString text=ui->hexTextEdit->toPlainText();
   int len=text.length();
   if(pos>=len)
   {
      QTextCursor c = ui->hexTextEdit->textCursor();
      c.movePosition(QTextCursor::Left);//, QTextCursor::KeepAnchor, 2);
      ui->hexTextEdit->setTextCursor(c);
      pos=ui->hexTextEdit->textCursor().position();
   }
   textChangedCount++;
   if(textChangedCount<3)
   {
      otherPosChangeSignal=true;
      return;
   }
   textChangedCount=0;



   //QString asciiText=ui->asciiTextEdit->toPlainText();
   if(pos>=text.length())
      pos=text.length()-1;
   int start=pos, end=pos;


   int emptyFind=0;
   for(; emptyFind<2 && start>=0 ;start--)
   {
      if(text.at(start)==' ' || text.at(start)==13 || text.at(start)==10)
         emptyFind++;
   }
   emptyFind=0;
   for(;emptyFind<2 && end<text.length();end++)
   {
      if(text.at(end)==' '|| text.at(end)==13 || text.at(end)==10)
         emptyFind++;
   }
   //qDebug()<<start<<end;
   QString chStr;
   QTextCursor c=ui->asciiTextEdit->textCursor();
   int j=0;
   quint16 dt;
   for (int i = start+1; i < end; i++)
   {
      if(text.at(i)==' '|| text.at(i)==13 || text.at(i)==10)
      {
         chStr="";
      }
      else
         chStr+=text.at(i);
      if(chStr.length()==2)
      {
         quint8 byte=chStr.toInt(0, 16);
         QChar ch=hexToAscii(chStr);

         int asciiPos=hexPosToAsciiPos(i);
         int dataPos=0;
         if(data8 && byteLength()>1)
            dataPos=asciiPosToDataPos(asciiPos, 1);
         else
            dataPos=asciiPosToDataPos(asciiPos);
         j++;
         if(data16 && j==2 && byteLength()>1)
         {
            j=0;
            dt=(dt<<8)+byte;
         }
         else
            dt=byte;

         if(data8)
         {
            if(byteLength()==1)
               data8[dataPos]=byte;
            else
            {
               if(dataPos%2 == 0)
                  data8[dataPos+1]=byte;
               else
                  data8[dataPos-1]=byte;
            }
         }
         else if(data16)
            data16[dataPos]=dt;

         /*QString dataType="";
            if(data8)
            {
                data8[dataPos]=dt;
                dt=(quint8)data8[dataPos];
                dataType="data8";
            }
            if(data16)
            {
                data16[dataPos]=dt;
                dt=(quint16)data16[dataPos];
                dataType="data16";

            }

            qDebug()<<"hex:"<<asciiPos<<chStr<<":"<<ch<<QString(" "+dataType+"[%1]:%2").arg(dataPos).arg(dt);;
            */
         c.clearSelection();
         c.setPosition(asciiPos);
         c.setPosition(asciiPos+1, QTextCursor::KeepAnchor);
         c.deleteChar();
         c.insertText(ch);
         //qDebug()<<"HPos"<<i<<"APos"<<asciiPos;
         //asciiText[asciiPos]=ch;
         chStr="";
      }
   }
   ui->asciiTextEdit->setTextCursor(c);
   //ui->asciiTextEdit->setPlainText(asciiText);
   otherPosChangeSignal=true;
}


void HexEditor::setBitLength(int length)
{
   ui->hexTextEdit->setBitLength(length);
   makeEmptyLines();
}

QString HexEditor::asciiToHex(QChar ch)
{
   if(ch=='.')
      return "00";
   //return QString::number(ch.toLatin1(), 16).toUpper();
   return QString("%1").arg((quint8)ch.toLatin1(), 2,16, QChar('0')).toUpper();
}

quint32 HexEditor::size()
{
   return _size;
}

QChar HexEditor::hexToAscii(QString hexStr)
{
   bool ok;
   QChar ch='.';
   int byte=hexStr.toInt(&ok, 16);
   if(byte>=32 && byte<127)
      ch=byte;
   return ch;
}



void HexEditor::setSize(quint32 size)
{
   this->_size=size;
}



void HexEditor::on_asciiTextEdit_modificationChanged(bool arg1)
{
   qDebug ()<< ui->asciiTextEdit->horizontalScrollBar();
}
