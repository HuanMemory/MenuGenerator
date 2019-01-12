#include "MenuGenerator.h"
#include <QInputDialog>
#include <QMessageBox>
MenuGenerator::MenuGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	MenuItemList.append(MenuItem(0, "Root"));				//��ʼ����Ŀ¼
	MenuItemList[0].type = MENUTYPE_MENU;
	MenuItemModel = new QStandardItemModel(this); 
	FuncItemModel = new QStandardItemModel(this);
	ParaItemModel = new QStandardItemModel(this);
	ui.ItemListView->setModel(MenuItemModel);
	ui.FuncListView->setModel(FuncItemModel);
	ui.ParaListView->setModel(ParaItemModel);

	QString menu_str = "Root";
	QStandardItem *menu_item = new QStandardItem(menu_str);
	MenuItemModel->appendRow(menu_item);

	QString func_str = "GUI_deInit";
	QStandardItem *func_item = new QStandardItem(func_str);
	FuncItemModel->appendRow(func_item);
	FuncList.append(func_str);

	QString para_str = "notused";
	QStandardItem *para_item = new QStandardItem(para_str);
	ParaItemModel->appendRow(para_item);
	ParaList.append(para_str);

	MenuList.append(menu_str);
	ui.SubmenuIndex1->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex2->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex3->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex4->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex5->setMaximum(MenuList.count() - 1);
	ui.LayersBox->setValue(0);
	Max_Layers = 0;
	Currentindex = 0;
	Refresh_Property();
}

void MenuGenerator::on_NewItemButton_clicked()
{	//�½��˵���
	int index;
	QString str = "Menu" + QString::number(MenuList.count() + 1);
	QStandardItem *item = new QStandardItem(str);
	MenuItemModel->appendRow(item);
	MenuList.append(str);
	index = MenuItemList.count();
	MenuItemList.append(MenuItem(index, str));
	Currentindex = index;
	Refresh_Property();
}

void MenuGenerator::on_NewFuncButton_clicked()
{	//�½�����
	bool ok = true;										//���뺯����
	QString str = QInputDialog::getText(this, "Function name", "Function:",			
		QLineEdit::Normal,"NewFunction" + QString::number(FuncList.count() + 1),&ok);
	if (!ok)											//�ж��Ƿ�ȡ��
		return;
	QRegExp reg("^[a-zA-Z_][a-zA-Z0-9_]*$");			//������ʽ�жϱ������������Ϸ�
	if (!reg.exactMatch(str))
	{
		qDebug("illegal function name");
		QMessageBox::critical(NULL, "ERROR", "Function name illegal!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QStandardItem *item = new QStandardItem(str);
	FuncItemModel->appendRow(item);
	FuncList.append(str);
}

void MenuGenerator::on_NewParaButton_clicked()
{	//�½�����
	bool ok = true;	
	QString str = QInputDialog::getText(this, "Parameter name", "Parameter:",
		QLineEdit::Normal, "NewParameter" + QString::number(ParaList.count() + 1), &ok);
	if (!ok)
		return;
	QRegExp reg("^[a-zA-Z_][a-zA-Z0-9_]*$");
	if (!reg.exactMatch(str))
	{
		qDebug("illegal variable name");
		QMessageBox::critical(NULL, "ERROR", "Variable name illegal!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QStandardItem *item = new QStandardItem(str);
	ParaItemModel->appendRow(item);
	ParaList.append(str);
}

void MenuGenerator::on_ExportButton_clicked()
{	//�����˵������ļ�
	QString path = QFileDialog::getSaveFileName(this, "Export Menu File", "./menu_data.h", tr("C Header Files(*.h)"));
	QFile f(path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(NULL, "ERROR", "Can not open/creat file!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QTextStream out(&f);
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString DataTime = current_date_time.toString(Qt::ISODate);
	out << "#ifndef __MENU_DATA_H__\n#define __MENU_DATA_H__\n"
		<< "/******************************\n"
		<< "*Creat Time : "
		<< DataTime
		<< "\n******************************/\n\n";
	out << "#define MENU_MAX_LAYERS " << QString::number(Max_Layers);
	out << "\n#define MENU_TITLE \"" << Title << "\"\n\n";
	out << "//Function Declaration\n";			//��������
	for (int i = 0; i < FuncList.count(); i++)
		out << "void " << FuncList[i] << "(void);\n";
	out << "\n";

	out << "//Function List\nvoid (*Func_list[])(void) = \n{\n";
	for (int i = 0; i < FuncList.count(); i++)	//�����б�
	{
		out << "  &" << FuncList[i];
		if (i != FuncList.count() - 1)
			out << ",\n";
		else
			out << "\n";
	}
	out << "};\n";

	out << "//Variable Definition\n";			//��������
	out << "struct\n{\n";
	for (int i = 0; i < ParaList.count(); i++)
		out << "  int " << ParaList[i] << ";\n";
	out << "}Initialization;\n";

	out << "//Variable List\nconst void *Para_list[] = \n{\n";
	for (int i = 0; i < ParaList.count(); i++)	//�����б�
	{
		out << "  &Initialization." << ParaList[i];
		if (i != ParaList.count() - 1)
			out << ",\n";
		else
			out << "\n";
	}
		
	out << "};\n";

	/*
	typedef struct
	{
		char Title[8];
		enum MENUTYPE Type;
		int Submenu[5];		//bool	//int min max step
	}MenuInfoItem;
	*/

	out << "//Menu List\nconst MenuInfoItem Menu_list[] = \n{\n";
	for (int i = 0; i < MenuItemList.count(); i++)	//�˵��б�
	{
		out << "  { .Title = \"" << MenuItemList[i].title << "\",";
		out << " .Type = " << MenuItemList[i].type << ",";
		out << " .Submenu = {";
		switch (MenuItemList[i].type)
		{
		case MENUTYPE_MENU:
			for (int j = 0; j < 5; j++)
			{
				out << QString::number(MenuItemList[i].sub_index[j]);
				if (j != 4)
					out << ",";
			}
			break;
		case MENUTYPE_FUNC:
			out << QString::number(MenuItemList[i].func_index);
			out << ",0,0,0,0";
			break;
		case MENUTYPE_BOOL:
			out << QString::number(MenuItemList[i].para_index);
			out << ",0,0,0,0";
			break;
		case MENUTYPE_INT:
			out << QString::number(MenuItemList[i].para_index) << ",";
			out << QString::number(MenuItemList[i].min) << ",";
			out << QString::number(MenuItemList[i].max) << ",";
			out << QString::number(MenuItemList[i].step) << ",0";
			break;
		}
		out << "} }";
		if (i != MenuItemList.count() - 1)
			out << ",";
		out << "\n";
	}
	out << "};\n";

	out << "#endif";
	f.close();
}



void MenuGenerator::on_SubmenuBox1_currentIndexChanged(int index)
{	//���Ӳ˵�ComboBox��Value����
	if (On_ItemEdit)
		return;
	ui.SubmenuIndex1->setValue(index);
	MenuItemList[Currentindex].sub_index[0] = index;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuBox2_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuIndex2->setValue(index);
	MenuItemList[Currentindex].sub_index[1] = index;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuBox3_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuIndex3->setValue(index);
	MenuItemList[Currentindex].sub_index[2] = index;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuBox4_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuIndex4->setValue(index);
	MenuItemList[Currentindex].sub_index[3] = index;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuBox5_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuIndex5->setValue(index);
	MenuItemList[Currentindex].sub_index[4] = index;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuIndex1_valueChanged(int val)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuBox1->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[0] = val;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuIndex2_valueChanged(int val)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuBox2->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[1] = val;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuIndex3_valueChanged(int val)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuBox3->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[2] = val;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuIndex4_valueChanged(int val)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuBox4->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[3] = val;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_SubmenuIndex5_valueChanged(int val)
{
	if (On_ItemEdit)
		return;
	ui.SubmenuBox5->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[4] = val;
	Refresh_SubmenuEnable();
}

void MenuGenerator::on_ItemListView_clicked()
{	//�л����ڱ༭����
	Currentindex = ui.ItemListView->currentIndex().row();
	Refresh_Property();
}

void MenuGenerator::on_NameEdit_editingFinished()
{	//���Ĳ˵�����
	QString newName = ui.NameEdit->text();
	QStandardItem *item = new QStandardItem(newName);
	MenuList[Currentindex] = newName;
	MenuItemList[Currentindex].title = newName;
	MenuItemModel->setItem(Currentindex, item);
	Refresh_Property();
}

void MenuGenerator::on_MenutypeBox_currentIndexChanged(int index)
{	//�ı�������
	if (On_ItemEdit)
		return;
	switch (index)
	{
	case 0:
		MenuItemList[Currentindex].type = MENUTYPE_MENU;
		break;
	case 1:
		MenuItemList[Currentindex].type = MENUTYPE_FUNC;
		break;
	case 2:
		MenuItemList[Currentindex].type = MENUTYPE_INT;
		break;
	case 3:
		MenuItemList[Currentindex].type = MENUTYPE_BOOL;
		break;
	}
	Refresh_Property();
}

void MenuGenerator::on_FuncBox_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.FuncIndex->setValue(index);
	MenuItemList[Currentindex].func_index = index;
}

void MenuGenerator::on_FuncIndex_valueChanged(int val)
{	//Ŀ�꺯������
	if (On_ItemEdit)
		return;
	ui.FuncBox->setCurrentIndex(val);
	MenuItemList[Currentindex].func_index = val;
}

void MenuGenerator::on_ParaBox_currentIndexChanged(int index)
{
	if (On_ItemEdit)
		return;
	ui.ParaIndex->setValue(index);
	MenuItemList[Currentindex].para_index = index;
}

void MenuGenerator::on_ParaIndex_valueChanged(int val)
{	//Ŀ���������
	if (On_ItemEdit)
		return;
	ui.ParaBox->setCurrentIndex(val);
	MenuItemList[Currentindex].para_index = val;
}

void MenuGenerator::on_ParaMax_valueChanged(int val)
{
	//���õ������ֵ
	if (On_ItemEdit)
		return;
	MenuItemList[Currentindex].max = val;
	if (val < MenuItemList[Currentindex].min)
	{
		ui.ParaMax->setStyleSheet("background-color:yellow;");
		ui.ParaMin->setStyleSheet("background-color:yellow;");
	}
	else
	{
		ui.ParaMax->setStyleSheet("background-color:white;");
		ui.ParaMin->setStyleSheet("background-color:white;");
	}
}

void MenuGenerator::on_ParaMin_valueChanged(int val)
{	//���õ�����Сֵ
	if (On_ItemEdit)
		return;
	MenuItemList[Currentindex].min = val;
	if (val > MenuItemList[Currentindex].max)
	{
		ui.ParaMax->setStyleSheet("background-color:yellow;");
		ui.ParaMin->setStyleSheet("background-color:yellow;");
	}
	else
	{
		ui.ParaMax->setStyleSheet("background-color:white;");
		ui.ParaMin->setStyleSheet("background-color:white;");
	}
}

void MenuGenerator::on_ParaStep_valueChanged(int val)
{	//���õ�������
	if (On_ItemEdit)
		return;
	MenuItemList[Currentindex].step = val;
}

void MenuGenerator::on_SaveButton_clicked()
{
	int MenuItemList_size = MenuItemList.size();
	int MenuList_size = 0;
	int MenuItemModel_size = 0;
	int ParaList_size = 0;
	int ParaItemModel_size = 0;
	int FuncList_size = 0;
	int FuncItemModel_size = 0;
	QList<MenuItem> MenuItemList;
	QList<QString> MenuList;
	QStandardItemModel *MenuItemModel;
	QList<QString> ParaList;
	QStandardItemModel *ParaItemModel;
	QList<QString> FuncList;
	QStandardItemModel *FuncItemModel;
}

void MenuGenerator::Refresh_SubmenuEnable()
{	//�����Ӳ˵�����ʹ��
	bool SubmenuEnable[5] = { false,false,false,false,false };
	for (int i = 0; i < 4; i++)
	{
		if (MenuItemList[Currentindex].sub_index[i])
			SubmenuEnable[i] = true;
		else
		{
			SubmenuEnable[i] = true;
			break;
		}
	}
	ui.SubmenuBox1->setEnabled(SubmenuEnable[0]);
	ui.SubmenuIndex1->setEnabled(SubmenuEnable[0]);
	ui.SubmenuBox2->setEnabled(SubmenuEnable[1]);
	ui.SubmenuIndex2->setEnabled(SubmenuEnable[1]);
	ui.SubmenuBox3->setEnabled(SubmenuEnable[2]);
	ui.SubmenuIndex3->setEnabled(SubmenuEnable[2]);
	ui.SubmenuBox4->setEnabled(SubmenuEnable[3]);
	ui.SubmenuIndex4->setEnabled(SubmenuEnable[3]);
	ui.SubmenuBox5->setEnabled(SubmenuEnable[4]);
	ui.SubmenuIndex5->setEnabled(SubmenuEnable[4]);
}

void MenuGenerator::on_TitleEdit_editingFinished()
{
	Title = ui.TitleEdit->text();
}
void MenuGenerator::on_LayersBox_valueChanged(int val)
{
	Max_Layers = val;
}

void MenuGenerator::Refresh_Property()
{	//ˢ������
	On_ItemEdit = true;
	ui.NameEdit->setText(MenuItemList[Currentindex].title);
	ui.IndexNum->display(Currentindex);
	ui.MenutypeBox->setCurrentIndex(MenuItemList[Currentindex].type);
	if (Currentindex)	//���˵�
	{
		ui.NameEdit->setEnabled(true);
		ui.MenutypeBox->setEnabled(true);
	}
	else
	{
		ui.NameEdit->setEnabled(false);
		ui.MenutypeBox->setEnabled(false);
	}
	switch (MenuItemList[Currentindex].type)
	{
	case MENUTYPE_MENU:
		ui.SubmenuGroup->setEnabled(true);
		ui.SubFuncGroup->setEnabled(false);
		ui.SubParaGroup->setEnabled(false);
		ui.SubmenuBox1->clear();
		ui.SubmenuBox2->clear();
		ui.SubmenuBox3->clear();
		ui.SubmenuBox4->clear();
		ui.SubmenuBox5->clear();
		ui.SubmenuBox1->addItems(MenuList);
		ui.SubmenuBox2->addItems(MenuList);
		ui.SubmenuBox3->addItems(MenuList);
		ui.SubmenuBox4->addItems(MenuList);
		ui.SubmenuBox5->addItems(MenuList);
		ui.SubmenuIndex1->setMaximum(MenuList.count() - 1);
		ui.SubmenuIndex2->setMaximum(MenuList.count() - 1);
		ui.SubmenuIndex3->setMaximum(MenuList.count() - 1);
		ui.SubmenuIndex4->setMaximum(MenuList.count() - 1);
		ui.SubmenuIndex5->setMaximum(MenuList.count() - 1);
		ui.SubmenuBox1->setCurrentIndex(MenuItemList[Currentindex].sub_index[0]);
		ui.SubmenuBox2->setCurrentIndex(MenuItemList[Currentindex].sub_index[1]);
		ui.SubmenuBox3->setCurrentIndex(MenuItemList[Currentindex].sub_index[2]);
		ui.SubmenuBox4->setCurrentIndex(MenuItemList[Currentindex].sub_index[3]);
		ui.SubmenuBox5->setCurrentIndex(MenuItemList[Currentindex].sub_index[4]);
		ui.SubmenuIndex1->setValue(MenuItemList[Currentindex].sub_index[0]);
		ui.SubmenuIndex2->setValue(MenuItemList[Currentindex].sub_index[1]);
		ui.SubmenuIndex3->setValue(MenuItemList[Currentindex].sub_index[2]);
		ui.SubmenuIndex4->setValue(MenuItemList[Currentindex].sub_index[3]);
		ui.SubmenuIndex5->setValue(MenuItemList[Currentindex].sub_index[4]);
		Refresh_SubmenuEnable();
		break;
	case MENUTYPE_FUNC:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(true);
		ui.SubParaGroup->setEnabled(false);
		ui.FuncBox->clear();
		ui.FuncBox->addItems(FuncList);
		ui.FuncBox->setCurrentIndex(MenuItemList[Currentindex].func_index);
		ui.FuncIndex->setMaximum(FuncList.count() - 1);
		ui.FuncIndex->setValue(MenuItemList[Currentindex].func_index);
		break;
	case MENUTYPE_INT:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(false);
		ui.SubParaGroup->setEnabled(true);
		ui.ParaMax->setEnabled(true);
		ui.ParaMin->setEnabled(true);
		ui.ParaStep->setEnabled(true);
		ui.ParaBox->clear();
		ui.ParaBox->addItems(ParaList);
		ui.ParaBox->setCurrentIndex(MenuItemList[Currentindex].para_index);
		ui.ParaIndex->setMaximum(ParaList.count() - 1);
		ui.ParaIndex->setValue(MenuItemList[Currentindex].para_index);
		ui.ParaMax->setValue(MenuItemList[Currentindex].max);
		ui.ParaMin->setValue(MenuItemList[Currentindex].min);
		ui.ParaStep->setValue(MenuItemList[Currentindex].step);
		break;
	case MENUTYPE_BOOL:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(false);
		ui.SubParaGroup->setEnabled(true);
		ui.ParaMax->setEnabled(false);
		ui.ParaMin->setEnabled(false);
		ui.ParaStep->setEnabled(false);
		ui.ParaBox->clear();
		ui.ParaBox->addItems(ParaList);
		ui.ParaBox->setCurrentIndex(MenuItemList[Currentindex].para_index);
		ui.ParaIndex->setMaximum(ParaList.count() - 1);
		ui.ParaIndex->setValue(MenuItemList[Currentindex].para_index);
		break;
	}
	On_ItemEdit = false;
}