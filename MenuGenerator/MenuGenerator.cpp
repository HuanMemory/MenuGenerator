#include "MenuGenerator.h"

MenuGenerator::MenuGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//connect(ui.checkBox, SIGNAL(clicked()), this, SLOT(on_checkBox_clicked()));
}

void MenuGenerator::on_NewItemButton_clicked()
{	//�½��˵���
	qDebug("New Item");
	
}

void MenuGenerator::on_NewFuncButton_clicked()
{	//�½�����
	qDebug("New Function");

}

void MenuGenerator::on_NewParaButton_clicked()
{	//�½�����
	qDebug("New Parameter");

}

void MenuGenerator::on_ClearButton_clicked()
{	//���������
	qDebug("Clear Items");

}