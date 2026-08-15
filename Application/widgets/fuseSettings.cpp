#include "widgets\fuseSettings.h"
#include "Libs/globalVariables.h"
#include <QSettings>
#include <qmath.h>

FuseSettings::FuseSettings(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FuseSettings)
{
	ui->setupUi(this);
	progAVR=GlobalVariables::progAVR;
	lowCheckBoxes.append(ui->chbAVRLow0);
	lowCheckBoxes.append(ui->chbAVRLow1);
	lowCheckBoxes.append(ui->chbAVRLow2);
	lowCheckBoxes.append(ui->chbAVRLow3);
	lowCheckBoxes.append(ui->chbAVRLow4);
	lowCheckBoxes.append(ui->chbAVRLow5);
	lowCheckBoxes.append(ui->chbAVRLow6);
	lowCheckBoxes.append(ui->chbAVRLow7);
	for(int i=0;i<8; i++)
		connect(lowCheckBoxes.at(i), SIGNAL(clicked()), this, SLOT(onAVRCheckBoxChanged()));


	highCheckBoxes.append(ui->chbAVRHigh0);
	highCheckBoxes.append(ui->chbAVRHigh1);
	highCheckBoxes.append(ui->chbAVRHigh2);
	highCheckBoxes.append(ui->chbAVRHigh3);
	highCheckBoxes.append(ui->chbAVRHigh4);
	highCheckBoxes.append(ui->chbAVRHigh5);
	highCheckBoxes.append(ui->chbAVRHigh6);
	highCheckBoxes.append(ui->chbAVRHigh7);
	for(int i=0;i<8; i++)
		connect(highCheckBoxes.at(i), SIGNAL(clicked()), this, SLOT(onAVRCheckBoxChanged()));

	extendCheckBoxes.append(ui->chbAVRExt0);
	extendCheckBoxes.append(ui->chbAVRExt1);
	extendCheckBoxes.append(ui->chbAVRExt2);
	extendCheckBoxes.append(ui->chbAVRExt3);
	extendCheckBoxes.append(ui->chbAVRExt4);
	extendCheckBoxes.append(ui->chbAVRExt5);
	extendCheckBoxes.append(ui->chbAVRExt6);
	extendCheckBoxes.append(ui->chbAVRExt7);
	for(int i=0;i<8; i++)
		connect(extendCheckBoxes.at(i), SIGNAL(clicked()), this, SLOT(onAVRCheckBoxChanged()));

	lockCheckBoxes.append(ui->chbAVRLock0);
	lockCheckBoxes.append(ui->chbAVRLock1);
	lockCheckBoxes.append(ui->chbAVRLock2);
	lockCheckBoxes.append(ui->chbAVRLock3);
	lockCheckBoxes.append(ui->chbAVRLock4);
	lockCheckBoxes.append(ui->chbAVRLock5);
	lockCheckBoxes.append(ui->chbAVRLock6);
	lockCheckBoxes.append(ui->chbAVRLock7);
	for(int i=0;i<8; i++)
		connect(lockCheckBoxes.at(i), SIGNAL(clicked()), this, SLOT(onAVRCheckBoxChanged()));


	connect(ui->txtAVRLow, SIGNAL(textChanged()), this, SLOT(avrSettingsTextChanged));
	connect(ui->txtAVRHigh, SIGNAL(textChanged()), this, SLOT(avrSettingsTextChanged));
	connect(ui->txtAVRExtend, SIGNAL(textChanged()), this, SLOT(avrSettingsTextChanged));
	connect(ui->txtAVRLock, SIGNAL(textChanged()), this, SLOT(avrSettingsTextChanged));


	progAVR->AVRfuse=10;
	progAVR->AVRfuse_h=20;
	progAVR->AVRfuse_x=30;
	progAVR->AVRlock=40;
	lblLow = lblHigh = lblExtend = lblLock = NULL;
}

FuseSettings::~FuseSettings()
{
	delete ui;

}

Ui::FuseSettings *FuseSettings::getUi()
{
	return ui;
}

void FuseSettings::setDeviceInfo(DeviceInfo *deviceInfo)
{
	this->deviceInfo=deviceInfo;
}
void FuseSettings::loadAVR1Fuse( QList<QCheckBox*> *checkBoxList, FuseByte *fuseByte, QTextEdit *textBox)
{
	int len = fuseByte->bitsInfo.length();
	textBox->setEnabled(len>0);
	int i=0;
	for (i=0; i < 8; ++i)
	{
		checkBoxList->at(i)->setEnabled(false);
		checkBoxList->at(i)->setChecked(len>0);
		checkBoxList->at(i)->setText("Not Used");
	}
	for (i = 0; i < len; ++i)
	{
		checkBoxList->at(fuseByte->bitsInfo.at(i).bitNumber)->setText(fuseByte->bitsInfo.at(i).bitName);
		checkBoxList->at(fuseByte->bitsInfo.at(i).bitNumber)->setEnabled(true);
	}
}

void FuseSettings::loadAVRFuseSettings()
{
	ReadXMLAVRList readXML;

	avrFuse  =readXML.ReadXML(deviceInfo->name);

	if(avrFuse==NULL)
	{
		GlobalVariables::printMessage("file "+deviceInfo->name+".xml not find");
		return;
	}
	loadAVR1Fuse(&lowCheckBoxes, &avrFuse->low, ui->txtAVRLow);
	loadAVR1Fuse(&highCheckBoxes, &avrFuse->high, ui->txtAVRHigh);
	loadAVR1Fuse(&extendCheckBoxes, &avrFuse->extend, ui->txtAVRExtend);
	loadAVR1Fuse(&lockCheckBoxes, &avrFuse->lock, ui->txtAVRLock);

	progAVR->AVRfuse=avrFuse->low.defualt;
	progAVR->AVRfuse_h=avrFuse->high.defualt;
	progAVR->AVRfuse_x=avrFuse->extend.defualt;
	progAVR->AVRlock=avrFuse->lock.defualt;


	//makeGroupCheckBoxes(&avrFuse->low, &lowCheckBoxes);
	int len=avrNavComboBoxes.count();
	for (int i = len-1; i >=0; --i)
	{
		QComboBox *comb=avrNavComboBoxes.at(i).comboBox;
		delete comb;
		comb=NULL;
	}
	avrNavComboBoxes.clear();


	len=avrNavCheckBoxes.count();
	for (int i = len-1; i >=0; --i)
	{
		QCheckBox *checkbox=avrNavCheckBoxes.at(i).checkBox;
		delete checkbox;
		checkbox=NULL;
	}
	avrNavCheckBoxes.clear();

	makeAvrNavigation(&avrFuse->low, lblLow, "Low", AvrFuseType::FuseLow);
	makeAvrNavigation(&avrFuse->high, lblHigh, "High", AvrFuseType::FuseHigh);
	makeAvrNavigation(&avrFuse->extend, lblExtend, "Extend", AvrFuseType::FuseExtend);
	makeAvrNavigation(&avrFuse->lock, lblLock, "Lock", AvrFuseType::FuseLock);

	setAvrCheckBoxFromFuse();
	onAVRCheckBoxChanged();
	for (int i = 0; i < lowCheckBoxes.count(); ++i)
	{
		if(lowCheckBoxes.at(i)->isEnabled())
			setAvrNavigationFromFuseCheckbox(lowCheckBoxes.at(i));
		if(highCheckBoxes.at(i)->isEnabled())
			setAvrNavigationFromFuseCheckbox(highCheckBoxes.at(i));
		if(extendCheckBoxes.at(i)->isEnabled())
			setAvrNavigationFromFuseCheckbox(extendCheckBoxes.at(i));
		if(lockCheckBoxes.at(i)->isEnabled())
			setAvrNavigationFromFuseCheckbox(lockCheckBoxes.at(i));
	}
}

bool FuseSettings::makeAvrNavigation(FuseByte *fuseByte, QLabel *&label, QString labelText, AvrFuseType::AvrFuseType fuseType)
{
	QList<QCheckBox*> *checks;
	switch (fuseType)
	{
		case AvrFuseType::FuseLow:      checks = &lowCheckBoxes;		break;
		case AvrFuseType::FuseHigh:     checks = &highCheckBoxes;		break;
		case AvrFuseType::FuseExtend:   checks = &extendCheckBoxes;		break;
		case AvrFuseType::FuseLock:     checks = &lockCheckBoxes;		break;
	}
	for (int i = 0; i < checks->count(); ++i)
	{
		checks->at(i)->setProperty("Type", NULL);
		checks->at(i)->setProperty("Navigation", NULL);
	}

	int navCount=fuseByte->navigationGroups.count();
	if(label)
	{
		ui->verticalLayout->removeWidget(label);
		delete label;
		label=NULL;
	}
	if(navCount>0)
	{
		label = new QLabel("\n"+labelText+" fuse bits choices:", ui->scrollAreaAVRNavigation);
		label->setMaximumHeight(30);
		ui->verticalLayout->addWidget(label);
	}

	for (int i = 0; i < navCount; ++i)
	{
		int memberCount=fuseByte->navigationGroups.at(i).members.count();
		if(memberCount==1)
		{
			QCheckBox *checkBoxNav=new QCheckBox(ui->scrollAreaAVRNavigation);

			checkBoxNav->setText(fuseByte->navigationGroups.at(i).members.at(0).text);
			connect(checkBoxNav, SIGNAL(clicked()), this, SLOT(onAvrNavCheckBoxChanged()));

			ui->verticalLayout->addWidget(checkBoxNav);

			int mask=fuseByte->navigationGroups.at(i).mask;
			avrNavCheckBoxes.append(CheckBoxNav(checkBoxNav, mask, fuseType));


			int index=-1;
			for(;mask;mask>>=1, index++);
			// در اینجا با توجه به اینکه نوع فیوز کدام بوده است  چک باکس متناظر با اندیس همان ارایه چک باکس های
			//ان فیوز را برابر با این چک باکس نویگیشن میکنیم
			QVariant control;
			control.setValue(checkBoxNav);
			QCheckBox *checkbox;
			switch (fuseType)
			{
				case AvrFuseType::FuseLow:      checkbox=lowCheckBoxes.at(index);       break;
				case AvrFuseType::FuseHigh:     checkbox=highCheckBoxes.at(index);      break;
				case AvrFuseType::FuseExtend:   checkbox=extendCheckBoxes.at(index);    break;
				case AvrFuseType::FuseLock:     checkbox=lockCheckBoxes.at(index);      break;
			}
			checkbox->setProperty("Type", "CheckBox");
			checkbox->setProperty("Navigation", control);
		}
		else
		{
			QComboBox *combobox=new QComboBox(ui->scrollAreaAVRNavigation);
			ui->verticalLayout->addWidget(combobox);
			quint8 mask=fuseByte->navigationGroups.at(i).mask;
			avrNavComboBoxes.append(ComboBoxNav(combobox, mask, fuseType));
			QList<QCheckBox*> fuseCheckBoxes;
			connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAvrNavComboBoxChanged(int)));
			for (int j = 0; j < memberCount; ++j)
			{
				quint8 value=fuseByte->navigationGroups.at(i).members.at(j).value;
				combobox->addItem(QString::number(value,2)+":"+ fuseByte->navigationGroups.at(i).members.at(j).text, value);
			}
			combobox->setProperty("Mask", mask);
			combobox->setProperty("FuseType", fuseType);
			for(int index=0;mask;mask>>=1, index++)
				if(mask&1)
				{
					// در اینجا با توجه به اینکه نوع فیوز کدام بوده است  چک باکس متناظر با اندیس همان ارایه چک باکس های
					//ان فیوز را برابر با این چک باکس نویگیشن میکنیم
					QVariant control;
					control.setValue(combobox);
					QCheckBox *checkbox;
					switch (fuseType)
					{
						case AvrFuseType::FuseLow:      checkbox=lowCheckBoxes.at(index);       break;
						case AvrFuseType::FuseHigh:     checkbox=highCheckBoxes.at(index);      break;
						case AvrFuseType::FuseExtend:   checkbox=extendCheckBoxes.at(index);    break;
						case AvrFuseType::FuseLock:     checkbox=lockCheckBoxes.at(index);      break;
					}
					fuseCheckBoxes.append(checkbox);
					checkbox->setProperty("Type", "ComboBox");
					checkbox->setProperty("Navigation", control);
				}

		}
	}

	return navCount>0;
}


void FuseSettings::onAvrNavCheckBoxChanged()
{
	QCheckBox *checkBox=(QCheckBox*)sender();
	int count=avrNavCheckBoxes.count();
	for (int i = 0; i < count; ++i)
	{
		//در اینجا در آرایه ای که برای چک باکس ها در نظر گرفتیم به دنبال کمبو باکسی که الان ایندکسش عوض شده میگردیم و
		//پس از یافتم مقدار الگوی آنرا برداشته و لگاریتم مبنای 2 آنرا برای بدست آوردن شماره چک باکس محاسبه میکنیم
		if(avrNavCheckBoxes.at(i).checkBox==checkBox)
		{
			int mask=avrNavCheckBoxes.at(i).mask;

			int index=-1;
			for(;mask;mask>>=1, index++);

			//مقدار چکی که برای توضیحات خورده باید برعکس مقدار چک باکس متناظر ان باشد
			bool check=!checkBox->isChecked();

			// در اینجا هم با توجه به اینکه نوع فیوز کدام بوده است مقدار چک باکس متناظر با اندیس همان ارایه چک باکس های
			//ان فیوز را تغییر می دهیم
			switch (avrNavCheckBoxes.at(i).fuseType)
			{
				case AvrFuseType::FuseLow:		lowCheckBoxes.at(index)->setChecked(check);		break;
				case AvrFuseType::FuseHigh:		highCheckBoxes.at(index)->setChecked(check);	break;
				case AvrFuseType::FuseExtend:	extendCheckBoxes.at(index)->setChecked(check);	break;
				case AvrFuseType::FuseLock:		lockCheckBoxes.at(index)->setChecked(check);	break;
			}

		}
	}
	onAVRCheckBoxChanged();
}



void FuseSettings::onAvrNavComboBoxChanged(int index)
{
	QComboBox *comboBox=(QComboBox*)sender();
	int count=avrNavComboBoxes.count();
	for (int i = 0; i < count; ++i)
	{
		//در اینجا در آرایه ای که برای چک باکس ها در نظر گرفتیم به دنبال کمبو باکسی که الان ایندکسش عوض شده میگردیم و
		//پس از یافتم مقدار الگوی آنرا برداشته و لگاریتم مبنای 2 آنرا برای بدست آوردن شماره چک باکس محاسبه میکنیم
		if(avrNavComboBoxes.at(i).comboBox==comboBox)
		{
			quint8 value=comboBox->itemData(index).toUInt();
			quint8 mask=avrNavComboBoxes.at(i).mask;

			// در اینجا هم با توجه به اینکه نوع فیوز کدام بوده است مقدار چک باکس متناظر با اندیس همان ارایه چک باکس های
			//ان فیوز را تغییر می دهیم
			for (int j = 0; mask  ; ++j, mask>>=1)
			{
				if( mask & 1)
				{
					bool check=false;
					switch (avrNavComboBoxes.at(i).fuseType)
					{
						case AvrFuseType::FuseLow:		lowCheckBoxes.at(j)->setChecked(check);		break;
						case AvrFuseType::FuseHigh:		highCheckBoxes.at(j)->setChecked(check);	break;
						case AvrFuseType::FuseExtend:	extendCheckBoxes.at(j)->setChecked(check);	break;
						case AvrFuseType::FuseLock:		lockCheckBoxes.at(j)->setChecked(check);	break;
					}
				}
			}
			//for (int j = 0; (value && j)|| (!j && !value) ; ++j, value>>=2)
			for (int j = 0; value; ++j, value>>=1)
			{
				bool check=(value & 1);//|| (!j && !value);
				switch (avrNavComboBoxes.at(i).fuseType)
				{
					case AvrFuseType::FuseLow:		lowCheckBoxes.at(j)->setChecked(check);		break;
					case AvrFuseType::FuseHigh:		highCheckBoxes.at(j)->setChecked(check);	break;
					case AvrFuseType::FuseExtend:	extendCheckBoxes.at(j)->setChecked(check);	break;
					case AvrFuseType::FuseLock:		lockCheckBoxes.at(j)->setChecked(check);	break;
				}
			}
		}
	}
	onAVRCheckBoxChanged();
}

void FuseSettings::makeGroupCheckBoxes(FuseByte *fuseByte, QList<QCheckBox*> *checkBoxList)
{
	//groupCheckBoxes.clear();
	for (int i = 0; i < fuseByte->navigationGroups.count(); ++i)
	{
		QString mask=QString::number(fuseByte->navigationGroups.at(i).mask, 2);
		for (int j = 0; i < mask.length(); ++i)
		{
			if(mask[j]=='1')
			{
				//groupCheckBoxes.append();
			}
		}
		//
	}
}


void FuseSettings::setAvrCheckBoxFromFuse()
{
	for (int i = 0, a=1; i < 8; i++, a*=2)
	{
		lowCheckBoxes.at(i)->setChecked(progAVR->AVRfuse & a);
		highCheckBoxes.at(i)->setChecked(progAVR->AVRfuse_h & a);
		extendCheckBoxes.at(i)->setChecked(progAVR->AVRfuse_x & a);
		lockCheckBoxes.at(i)->setChecked(progAVR->AVRlock & a);
	}
}

void FuseSettings::setAvrNavigationFromFuseCheckbox(QCheckBox *checkBox)
{
	if(checkBox->property("Type").isValid())
		if(checkBox->property("Type").toString()=="CheckBox")
		{
			QVariant control= checkBox->property("Navigation");
			QCheckBox *checkboxNav = control.value<QCheckBox*>();
			if(checkboxNav)
			{
				checkboxNav->setChecked(!checkBox->isChecked());
				qDebug()<<"checkBoxNav:"<<checkboxNav->text();
			}

		}
		else if(checkBox->property("Type").toString()=="ComboBox")
		{
			QVariant control= checkBox->property("Navigation");
			QComboBox *comboBoxNav = control.value<QComboBox*>();
			if(comboBoxNav)
			{
				quint8 mask=comboBoxNav ->property("Mask").toUInt();
				AvrFuseType::AvrFuseType fuseType = AvrFuseType::AvrFuseType(comboBoxNav ->property("FuseType").toUInt());
				quint8 fuseValue=0;
				switch (fuseType)
				{
					case AvrFuseType::FuseLow:		fuseValue=progAVR->AVRfuse;		break;
					case AvrFuseType::FuseHigh:		fuseValue=progAVR->AVRfuse_h;	break;
					case AvrFuseType::FuseExtend:	fuseValue=progAVR->AVRfuse_x;	break;
					case AvrFuseType::FuseLock:		fuseValue=progAVR->AVRlock;		break;
				}
				quint8 value=fuseValue&mask;

				int index=comboBoxNav->findData(value);
				if(index>=0)//if valid
					comboBoxNav->setCurrentIndex(index);
				else // invalid and reset
					checkBox->click();

			}

		}
}

void FuseSettings::onAVRCheckBoxChanged()
{


	progAVR->AVRfuse =
			ui->chbAVRLow0->isChecked()
			| ui->chbAVRLow1->isChecked()<<1
										   | ui->chbAVRLow2->isChecked()<<2
										   | ui->chbAVRLow3->isChecked()<<3
										   | ui->chbAVRLow4->isChecked()<<4
										   | ui->chbAVRLow5->isChecked()<<5
										   | ui->chbAVRLow6->isChecked()<<6
										   | ui->chbAVRLow7->isChecked()<<7;

	progAVR->AVRfuse_h =
			ui->chbAVRHigh0->isChecked()
			| ui->chbAVRHigh1->isChecked()<<1
											| ui->chbAVRHigh2->isChecked()<<2
											| ui->chbAVRHigh3->isChecked()<<3
											| ui->chbAVRHigh4->isChecked()<<4
											| ui->chbAVRHigh5->isChecked()<<5
											| ui->chbAVRHigh6->isChecked()<<6
											| ui->chbAVRHigh7->isChecked()<<7;

	progAVR->AVRfuse_x =
			ui->chbAVRExt0->isChecked()
			| ui->chbAVRExt1->isChecked()<<1
										   | ui->chbAVRExt2->isChecked()<<2
										   | ui->chbAVRExt3->isChecked()<<3
										   | ui->chbAVRExt4->isChecked()<<4
										   | ui->chbAVRExt5->isChecked()<<5
										   | ui->chbAVRExt6->isChecked()<<6
										   | ui->chbAVRExt7->isChecked()<<7;

	progAVR->AVRlock =
			ui->chbAVRLock0->isChecked()
			| ui->chbAVRLock1->isChecked()<<1
											| ui->chbAVRLock2->isChecked()<<2
											| ui->chbAVRLock3->isChecked()<<3
											| ui->chbAVRLock4->isChecked()<<4
											| ui->chbAVRLock5->isChecked()<<5
											| ui->chbAVRLock6->isChecked()<<6
											| ui->chbAVRLock7->isChecked()<<7;

	ui->txtAVRLow->setText(QString::number(progAVR->AVRfuse, 16).toUpper());
	ui->txtAVRHigh->setText(QString::number(progAVR->AVRfuse_h, 16).toUpper());
	ui->txtAVRExtend->setText(QString::number(progAVR->AVRfuse_x, 16).toUpper());
	ui->txtAVRLock->setText(QString::number(progAVR->AVRlock, 16).toUpper());

	// یکی از استفاده های این تابع برای رویداد کلیک بر روی چک باکس های فیوز بیت هاست
	if(sender())
	{
		QString className=sender()->metaObject()->className();
		// اگر چنین اتفاقی افتاد باید کنترل های بخش نویگشن هم اصلاح شود
		if(className=="QCheckBox")
		{
			QCheckBox *checkBox=(QCheckBox*)sender();
			//qDebug()<<"Type:"<<checkBox->property("Type");
			setAvrNavigationFromFuseCheckbox(checkBox);
		}
	}

}


void FuseSettings::avrSettingsTextChanged()
{
	qDebug()<<"avrSettingsTextChanged";




}

void FuseSettings::on_txtAVRLow_textChanged()
{
	bool ok;
	progAVR->AVRfuse = ui->txtAVRLow->toPlainText().toInt(&ok, 16);
	//qDebug()<<progAVR->AVRfuse;
	setAvrCheckBoxFromFuse();
}

void FuseSettings::on_txtAVRHigh_textChanged()
{
	bool ok;
	progAVR->AVRfuse_h = ui->txtAVRHigh->toPlainText().toInt(&ok, 16);
	setAvrCheckBoxFromFuse();
}

void FuseSettings::on_txtAVRExtend_textChanged()
{
	bool ok;
	progAVR->AVRfuse_x = ui->txtAVRExtend->toPlainText().toInt(&ok, 16);
	setAvrCheckBoxFromFuse();
}

void FuseSettings::on_txtAVRLock_textChanged()
{
	bool ok;
	progAVR->AVRlock = ui->txtAVRLock->toPlainText().toInt(&ok, 16);
	setAvrCheckBoxFromFuse();
}

void FuseSettings::on_btnAVRFuzeRead_clicked()
{
	switch (deviceInfo->readWriteInfo->readFunctionIndex)
	{
		case ReadFunctions::ReadAT:
			progAVR->ReadAT_Fuze(deviceInfo->readWriteInfo->readParam[0]);
			break;
		case ReadFunctions::ReadAT_HV:
			progAVR->ReadAT_HV_Fuze(deviceInfo->readWriteInfo->readParam[0]);
			break;
	}
	setAvrCheckBoxFromFuse();
	onAVRCheckBoxChanged();
}

void FuseSettings::on_btnAVRLockRead_clicked()
{
	switch (deviceInfo->readWriteInfo->readFunctionIndex)
	{
		case ReadFunctions::ReadAT:
			progAVR->ReadAT_Fuze(deviceInfo->readWriteInfo->readParam[0], true);
			break;
		case ReadFunctions::ReadAT_HV:
			progAVR->ReadAT_HV_Fuze(deviceInfo->readWriteInfo->readParam[0], true);
			break;
	}
	setAvrCheckBoxFromFuse();
	onAVRCheckBoxChanged();
}

void FuseSettings::on_btnAVRFuzeWrite_clicked()
{
	progAVR->WriteATmega_Fuze(deviceInfo->readWriteInfo->readParam[0]);
}


