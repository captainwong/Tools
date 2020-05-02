#include "HashView.h"
#include <Qvector>
#include <qcryptographichash.h>

const QVector<QPair<QString, QCryptographicHash::Algorithm>> ALGOS = {
	{ "Md4", QCryptographicHash::Md4,},
	{ "Md5", QCryptographicHash::Md5, },
	{ "Sha1", QCryptographicHash::Sha1, },
	{ "Sha224", QCryptographicHash::Sha224, },
	{ "Sha256", QCryptographicHash::Sha256, },
	{ "Sha384", QCryptographicHash::Sha384, },
	{ "Sha512", QCryptographicHash::Sha512, },
	{ "Sha3_224", QCryptographicHash::Sha3_224, },
	{ "Sha3_256", QCryptographicHash::Sha3_256, },
	{ "Sha3_384", QCryptographicHash::Sha3_384, },
	{ "Sha3_512", QCryptographicHash::Sha3_512, },
};

HashView::HashView(QWidget* parent)
	: QDialog(parent)
{
	setFixedSize(800, 600);

	auto vbox = new QVBoxLayout();

	input_ = new QPlainTextEdit(this);
	connect(input_, &QPlainTextEdit::textChanged, this, &HashView::slot_input_changed);

	algo_ = new QComboBox(this);
	for (const auto& algo : ALGOS) {
		algo_->addItem(algo.first);
	}
	connect(algo_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HashView::slot_algo_changed);

	output_ = new QPlainTextEdit(this);

	vbox->addWidget(input_, 1);
	vbox->addWidget(algo_);
	vbox->addWidget(output_, 1);
	setLayout(vbox);
}

void HashView::slot_input_changed()
{
	updateOutput(algo_->currentIndex(), input_->toPlainText());
}

void HashView::slot_algo_changed(int idx)
{
	updateOutput(idx, input_->toPlainText());
}

void HashView::updateOutput(int algoIdx, const QString & input)
{
	if (algoIdx < 0 || ALGOS.size() <= algoIdx) { return; }
	output_->setPlainText(QCryptographicHash::hash(input.toUtf8(), ALGOS[algoIdx].second).toHex());
}
