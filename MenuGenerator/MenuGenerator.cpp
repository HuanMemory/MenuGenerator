#include "MenuGenerator.h"

MenuGenerator::MenuGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(on_checkBox_clicked()));
}

void MenuGenerator::on_NewItemButton_clicked()
{	//新建菜单项
	qDebug("New Item");
	
}

void MenuGenerator::on_NewFuncButton_clicked()
{	//新建功能
	qDebug("New Function");

}

void MenuGenerator::on_NewParaButton_clicked()
{	//新建变量
	qDebug("New Parameter");

}

void MenuGenerator::on_ClearButton_clicked()
{	//清楚多余项
	qDebug("Clear Items");

}