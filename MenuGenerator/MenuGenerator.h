#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MenuGenerator.h"
#include "MenuItem.h"
#include <QStandardItem>
#include <QFile>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

class MenuGenerator : public QMainWindow
{
	Q_OBJECT

public:
	MenuGenerator(QWidget *parent = Q_NULLPTR);
private:
	Ui::MenuGeneratorClass ui;
	QList<MenuItem> MenuItemList;
	QList<QString> MenuList;
	QStandardItemModel *MenuItemModel;
	QList<QString> ParaList;
	QStandardItemModel *ParaItemModel;
	QList<QString> FuncList;
	QStandardItemModel *FuncItemModel;
	QString Title;
	int Max_Layers;
	int Currentindex;
	bool On_ItemEdit = false;
	void LoadItemfromJsonArray(QJsonArray);
	void Refresh_Property(void);
	void Refresh_SubmenuEnable(void);
	void Refresh_All(void);
private slots:
	void on_NewItemButton_clicked();
	void on_NewFuncButton_clicked();
	void on_NewParaButton_clicked();
	void on_ExportButton_clicked();
	void on_SubmenuBox1_currentIndexChanged(int index);
	void on_SubmenuBox2_currentIndexChanged(int index);
	void on_SubmenuBox3_currentIndexChanged(int index);
	void on_SubmenuBox4_currentIndexChanged(int index);
	void on_SubmenuBox5_currentIndexChanged(int index);
	void on_SubmenuIndex1_valueChanged(int val);
	void on_SubmenuIndex2_valueChanged(int val);
	void on_SubmenuIndex3_valueChanged(int val);
	void on_SubmenuIndex4_valueChanged(int val);
	void on_SubmenuIndex5_valueChanged(int val);
	void on_ItemListView_clicked();
	void on_NameEdit_editingFinished();
	void on_MenutypeBox_currentIndexChanged(int index);
	void on_FuncBox_currentIndexChanged(int index);
	void on_FuncIndex_valueChanged(int val);
	void on_ParaBox_currentIndexChanged(int index);
	void on_ParaIndex_valueChanged(int val);
	void on_ParaMax_valueChanged(int val);
	void on_ParaMin_valueChanged(int val);
	void on_ParaStep_valueChanged(int val);
	void on_SaveButton_clicked();
	void on_OpenButton_clicked();
	void on_TitleEdit_editingFinished();
	void on_LayersBox_valueChanged(int val);
};
