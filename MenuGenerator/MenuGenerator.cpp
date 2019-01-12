#include "MenuGenerator.h"
#include <QInputDialog>
#include <QMessageBox>
MenuGenerator::MenuGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	MenuItemList.append(MenuItem(0, "Root"));				//初始化根目录
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
	Max_Layers = 3;
	Currentindex = 0;
	Refresh_Property();
}

void MenuGenerator::on_NewItemButton_clicked()
{	//新建菜单项
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
{	//新建功能
	bool ok = true;										//输入函数名
	QString str = QInputDialog::getText(this, "Function name", "Function:",			
		QLineEdit::Normal,"NewFunction" + QString::number(FuncList.count() + 1),&ok);
	if (!ok)											//判断是否取消
		return;
	QRegExp reg("^[a-zA-Z_][a-zA-Z0-9_]*$");			//正则表达式判断变量、函数名合法
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
{	//新建变量
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
{	//导出菜单数据文件
	QString path = QFileDialog::getSaveFileName(this, "Export Menu File", "./menu_data.h", tr("C Header Files(*.h)"));
	if (path.isEmpty())
		return;
	QFile f(path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::critical(NULL, "ERROR", "Can not open/creat file!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QTextStream out(&f);
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString DataTime = current_date_time.toString(Qt::ISODate);
	out << "#ifndef __MENU_DATA_H__\n#define __MENU_DATA_H__\n\n"
		<< "/******************************\n"
		<< "*Creat Time : "
		<< DataTime
		<< "\n******************************/\n\n";
	out << "#define MENU_MAX_LAYERS " << QString::number(Max_Layers);
	out << "\n#define MENU_TITLE \"" << Title << "\"\n\n";
	out << "//Function Declaration\n";			//函数声明
	for (int i = 0; i < FuncList.count(); i++)
		out << "void " << FuncList[i] << "(void);\n";
	out << "\n";

	out << "//Function List\nvoid (*Func_list[])(void) = \n{\n";
	for (int i = 0; i < FuncList.count(); i++)	//函数列表
	{
		out << "  &" << FuncList[i];
		if (i != FuncList.count() - 1)
			out << ",\n";
		else
			out << "\n";
	}
	out << "};\n\n";

	out << "//Variable Definition\n";			//变量定义
	out << "struct\n{\n";
	for (int i = 0; i < ParaList.count(); i++)
		out << "  int " << ParaList[i] << ";\n";
	out << "}Initialization;\n\n";

	out << "//Variable List\nconst void *Para_list[] = \n{\n";
	for (int i = 0; i < ParaList.count(); i++)	//变量列表
	{
		out << "  &Initialization." << ParaList[i];
		if (i != ParaList.count() - 1)
			out << ",\n";
		else
			out << "\n";
	}
		
	out << "};\n\n";

	out << "//Menu List\nconst MenuInfoItem Menu_list[] = \n{\n";
	for (int i = 0; i < MenuItemList.count(); i++)	//菜单列表
	{
		out << MenuItemList[i].ExportItemStr();
		if (i != MenuItemList.count() - 1)
			out << ",";
		out << "\n";
	}
	out << "};\n\n";

	out << "#endif";
	f.close();
}

void MenuGenerator::on_SubmenuBox1_currentIndexChanged(int index)
{	//将子菜单ComboBox和Value联动
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
{	//切换正在编辑的项
	Currentindex = ui.ItemListView->currentIndex().row();
	Refresh_Property();
}

void MenuGenerator::on_NameEdit_editingFinished()
{	//更改菜单标题
	QString newName = ui.NameEdit->text();
	QStandardItem *item = new QStandardItem(newName);
	MenuList[Currentindex] = newName;
	MenuItemList[Currentindex].title = newName;
	MenuItemModel->setItem(Currentindex, item);
	Refresh_Property();
}

void MenuGenerator::on_MenutypeBox_currentIndexChanged(int index)
{	//改变项类型
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
{	//目标函数设置
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
{	//目标变量设置
	if (On_ItemEdit)
		return;
	ui.ParaBox->setCurrentIndex(val);
	MenuItemList[Currentindex].para_index = val;
}

void MenuGenerator::on_ParaMax_valueChanged(int val)
{
	//设置调整最大值
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
{	//设置调整最小值
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
{	//设置调整步长
	if (On_ItemEdit)
		return;
	MenuItemList[Currentindex].step = val;
}

void MenuGenerator::on_SaveButton_clicked()
{
	QString path = QFileDialog::getSaveFileName(this, "Save", "./Menu.json", tr("Menu Json File(*.json)"));
	if (path.isEmpty())
		return;
	QFile file(path);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QMessageBox::critical(NULL, "ERROR", "Can not open/creat file!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QTextStream save(&file);
	QJsonObject json;
	QJsonArray FuncArray;
	QJsonArray ParaArray;
	QJsonArray ItemArray;
	QJsonDocument document;
	QByteArray byte_array;
	QString JsonStr;
	json.insert("title", Title);
	json.insert("layers", Max_Layers);
	QString description = QInputDialog::getText(this, tr("Log"),
		tr("Description:"), QLineEdit::Normal);
	json.insert("description", description);
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString DataTime = current_date_time.toString(Qt::ISODate);
	json.insert("time", DataTime);
	for (int i = 0; i < FuncList.count(); i++)
		FuncArray.append(FuncList[i]);
	for (int i = 0; i < ParaList.count(); i++)
		ParaArray.append(ParaList[i]);
	for (int i = 0; i < MenuItemList.count(); i++)
		ItemArray.append(MenuItemList[i].ExportItemJson());
	json.insert("Item", ItemArray);
	json.insert("Func", FuncArray);
	json.insert("Para", ParaArray);
	document.setObject(json);
	byte_array = document.toJson(QJsonDocument::Indented);
	JsonStr = QString(byte_array);
	save << JsonStr;
	file.close();
}

void MenuGenerator::on_OpenButton_clicked()
{
	QString path = QFileDialog::getOpenFileName(this, "Open", ".", tr("Menu Json File(*.json)"));
	if (path.isEmpty())
		return;
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(NULL, "ERROR", "Can not open file!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QByteArray byteArray = file.readAll();
	file.close();
	QJsonParseError json_error;
	QJsonDocument jsonDocument(QJsonDocument::fromJson(byteArray, &json_error));
	if (json_error.error != QJsonParseError::NoError)
	{
		QMessageBox::critical(NULL, "ERROR", "Wrong File Content!", QMessageBox::Yes, QMessageBox::Yes);
		return;
	}
	QJsonObject json = jsonDocument.object();
	QJsonArray FuncArray;
	QJsonArray ParaArray;
	QJsonArray ItemArray;
	MenuItemModel->clear();
	FuncItemModel->clear();
	ParaItemModel->clear();
	MenuItemList.clear();
	MenuList.clear();
	FuncList.clear();
	ParaList.clear();
	Title = json.value("title").toString();
	Max_Layers = json.value("layers").toInt();
	ItemArray = json.value("Item").toArray();
	LoadItemfromJsonArray(ItemArray);
	FuncArray = json.value("Func").toArray();
	for (int i = 0; i < FuncArray.count(); i++)
	{
		QString Func_name = FuncArray[i].toString();
		QStandardItem *item = new QStandardItem(Func_name);
		FuncItemModel->appendRow(item);
		FuncList.append(Func_name);
	}
	ParaArray = json.value("Para").toArray();
	for (int i = 0; i < ParaArray.count(); i++)
	{
		QString Para_name = ParaArray[i].toString();
		QStandardItem *item = new QStandardItem(Para_name);
		ParaItemModel->appendRow(item);
		ParaList.append(Para_name);
	}
	Currentindex = 0;
	Refresh_All();
	QString time = json.value("time").toString();
	QString description = json.value("description").toString();
	QMessageBox msgBox;
	msgBox.setIcon(QMessageBox::Information);
	msgBox.setText("Success.");
	msgBox.setInformativeText("Last saved : " + time);
	msgBox.setDetailedText(description);
	msgBox.setStandardButtons(QMessageBox::Yes);
	msgBox.setDefaultButton(QMessageBox::Yes);
	msgBox.exec();
}

void MenuGenerator::Refresh_SubmenuEnable()
{	//设置子菜单设置使能
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
	//调整标题
	if (On_ItemEdit)
		return;
	Title = ui.TitleEdit->text();
}
void MenuGenerator::on_LayersBox_valueChanged(int val)
{
	//调整最大层数
	if (On_ItemEdit)
		return;
	Max_Layers = val;
}

void MenuGenerator::LoadItemfromJsonArray(QJsonArray ItemArray)
{
	for (int i = 0; i < ItemArray.count(); i++)
	{
		MenuItemList.append(MenuItem(ItemArray[i].toObject()));
		QStandardItem *item = new QStandardItem(MenuItemList[i].title);
		MenuItemModel->appendRow(item);
		MenuList.append(MenuItemList[i].title);
	}
}

void MenuGenerator::Refresh_Property()
{	//刷新属性
	On_ItemEdit = true;
	ui.NameEdit->setText(MenuItemList[Currentindex].title);
	ui.IndexNum->display(Currentindex);
	ui.MenutypeBox->setCurrentIndex(MenuItemList[Currentindex].type);
	if (Currentindex)	//根菜单
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

void MenuGenerator::Refresh_All()
{
	On_ItemEdit = true;
	ui.TitleEdit->setText(Title);
	ui.LayersBox->setValue(Max_Layers);
	Refresh_Property();
	On_ItemEdit = false;
}