#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MenuGenerator.h"

class MenuGenerator : public QMainWindow
{
	Q_OBJECT

public:
	MenuGenerator(QWidget *parent = Q_NULLPTR);
private:
	Ui::MenuGeneratorClass ui;
private slots:
	void on_NewItemButton_clicked();
	void on_NewFuncButton_clicked();
	void on_NewParaButton_clicked();
	void on_ClearButton_clicked();
};
