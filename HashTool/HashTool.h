#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HashTool.h"

class HashTool : public QMainWindow
{
	Q_OBJECT

public:
	HashTool(QWidget *parent = Q_NULLPTR);

private:
	Ui::HashToolClass ui;
};
