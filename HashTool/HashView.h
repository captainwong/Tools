#pragma once

#include <QtWidgets>

class HashView : public QDialog
{
	Q_OBJECT
public:
	HashView(QWidget* parent = nullptr);

private slots:
	void slot_input_changed();
	void slot_algo_changed(int idx);

protected:
	void updateOutput(int algoIdx, const QString& input);

private:
	QPlainTextEdit * input_ = {};
	QComboBox* algo_ = {};
	QPlainTextEdit * output_ = {};
};