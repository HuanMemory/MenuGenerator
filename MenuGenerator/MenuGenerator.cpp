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
	QString str = "Root";
	QStandardItem *item = new QStandardItem(str);
	MenuItemModel->appendRow(item);
	MenuList.append(str);
	ui.SubmenuIndex1->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex2->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex3->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex4->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex5->setMaximum(MenuList.count() - 1);
	Currentindex = 0;
	Refresh_Property();
	//QString str = MenuItemList[0].ExportItemStr();
	//qDebug(qPrintable(str));
	//connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(on_checkBox_clicked()));
}

void MenuGenerator::on_NewItemButton_clicked()
{	//新建菜单项
	int index;
	qDebug("New Item");
	QString str = "Menu" + QString::number(MenuList.count() + 1);
	QStandardItem *item = new QStandardItem(str);
	MenuItemModel->appendRow(item);
	MenuList.append(str);
	index = MenuItemList.count();
	MenuItemList.append(MenuItem(index, str));
	ui.SubmenuIndex1->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex2->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex3->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex4->setMaximum(MenuList.count() - 1);
	ui.SubmenuIndex5->setMaximum(MenuList.count() - 1);
	Currentindex = index;
	Refresh_Property();
}

void MenuGenerator::on_NewFuncButton_clicked()
{	//新建功能
	qDebug("New Function");
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
	qDebug("New Parameter");
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

void MenuGenerator::on_ClearButton_clicked()
{	//清楚多余项
	qDebug("Clear Items");

}

void MenuGenerator::on_SubmenuBox1_currentIndexChanged(int index)
{
	ui.SubmenuIndex1->setValue(index);
	MenuItemList[Currentindex].sub_index[0] = index;
}

void MenuGenerator::on_SubmenuBox2_currentIndexChanged(int index)
{
	ui.SubmenuIndex2->setValue(index);
	MenuItemList[Currentindex].sub_index[1] = index;
}

void MenuGenerator::on_SubmenuBox3_currentIndexChanged(int index)
{
	ui.SubmenuIndex3->setValue(index);
	MenuItemList[Currentindex].sub_index[2] = index;
}

void MenuGenerator::on_SubmenuBox4_currentIndexChanged(int index)
{
	ui.SubmenuIndex4->setValue(index);
	MenuItemList[Currentindex].sub_index[3] = index;
}

void MenuGenerator::on_SubmenuBox5_currentIndexChanged(int index)
{
	ui.SubmenuIndex5->setValue(index);
	MenuItemList[Currentindex].sub_index[4] = index;
}

void MenuGenerator::on_SubmenuIndex1_valueChanged(int val)
{
	ui.SubmenuBox1->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[0] = val;
}

void MenuGenerator::on_SubmenuIndex2_valueChanged(int val)
{
	ui.SubmenuBox2->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[1] = val;
}

void MenuGenerator::on_SubmenuIndex3_valueChanged(int val)
{
	ui.SubmenuBox3->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[2] = val;
}

void MenuGenerator::on_SubmenuIndex4_valueChanged(int val)
{
	ui.SubmenuBox4->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[3] = val;
}

void MenuGenerator::on_SubmenuIndex5_valueChanged(int val)
{
	ui.SubmenuBox5->setCurrentIndex(val);
	MenuItemList[Currentindex].sub_index[4] = val;
}

void MenuGenerator::Refresh_Property()
{
	ui.NameEdit->setText(MenuItemList[Currentindex].title);
	ui.IndexNum->display(Currentindex);
	ui.MenutypeBox->setCurrentIndex(MenuItemList[Currentindex].type);
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
		ui.SubmenuBox1->setCurrentIndex(MenuItemList[Currentindex].sub_index[0]);
		ui.SubmenuBox2->setCurrentIndex(MenuItemList[Currentindex].sub_index[1]);
		ui.SubmenuBox3->setCurrentIndex(MenuItemList[Currentindex].sub_index[2]);
		ui.SubmenuBox4->setCurrentIndex(MenuItemList[Currentindex].sub_index[3]);
		ui.SubmenuBox5->setCurrentIndex(MenuItemList[Currentindex].sub_index[4]);
		/*ui.SubmenuIndex1->setValue(MenuItemList[index].sub_index[0]);
		ui.SubmenuIndex2->setValue(MenuItemList[index].sub_index[1]);
		ui.SubmenuIndex3->setValue(MenuItemList[index].sub_index[2]);
		ui.SubmenuIndex4->setValue(MenuItemList[index].sub_index[3]);
		ui.SubmenuIndex5->setValue(MenuItemList[index].sub_index[4]);*/
		break;
	case MENUTYPE_FUNC:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(true);
		ui.SubParaGroup->setEnabled(false);
		ui.FuncBox->clear();
		ui.FuncBox->addItems(FuncList);
		break;
	case MENUTYPE_INT:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(false);
		ui.SubParaGroup->setEnabled(true);
		break;
	case MENUTYPE_BOOL:
		ui.SubmenuGroup->setEnabled(false);
		ui.SubFuncGroup->setEnabled(false);
		ui.SubParaGroup->setEnabled(true);
		break;
	}
}