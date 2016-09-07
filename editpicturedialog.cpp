#include "editpicturedialog.h"
#include "ui_editpicturedialog.h"

#include <QFile>
#include <QMessageBox>
#include <QImage>

EditPictureDialog::EditPictureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPictureDialog)
{
    ui->setupUi(this);

	setWindowTitle(QString::fromLocal8Bit("编辑图片"));

	connect(ui->resXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resXSpinBoxValueChanged(int)));
	connect(ui->resYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resYSpinBoxValueChanged(int)));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(okBtnClicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(cancelBtnClicked()));
}

EditPictureDialog::~EditPictureDialog()
{
    delete ui;
}

void EditPictureDialog::set(QString path, QString fileBaseName, QString fileSuffix, int width, int height)
{
	this->path = path;
	this->fileBaseName = fileBaseName;
	this->fileSuffix = fileSuffix;
	this->width = width;
	this->height = height;

	ui->nameLineEdit->setText(fileBaseName);
	ui->suffixLabel->setText("." + fileSuffix);
	ui->resXSpinBox->setValue(width);
	ui->resYSpinBox->setValue(height);
}

void EditPictureDialog::resXSpinBoxValueChanged(int value)
{
	disconnect(ui->resYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resYSpinBoxValueChanged(int)));
	ui->resYSpinBox->setValue(height * ui->resXSpinBox->value() / width);
	connect(ui->resYSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resYSpinBoxValueChanged(int)));
}

void EditPictureDialog::resYSpinBoxValueChanged(int value)
{
	disconnect(ui->resXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resXSpinBoxValueChanged(int)));
	ui->resXSpinBox->setValue(width * ui->resYSpinBox->value() / height);
	connect(ui->resXSpinBox, SIGNAL(valueChanged(int)), this, SLOT(resXSpinBoxValueChanged(int)));
}

void EditPictureDialog::okBtnClicked()
{
	QString oldFilePath = path + "/" + fileBaseName + "." + fileSuffix;
	QString newFilePath = path + "/" + ui->nameLineEdit->text() + "." + fileSuffix;

	int w = ui->resXSpinBox->value();
	int h = ui->resYSpinBox->value();

	if (oldFilePath == newFilePath)
	{
		if (w != width)
		{
			QImage image(newFilePath);
			image = image.scaledToWidth(w);
			QFile::remove(newFilePath);
			image.save(newFilePath);

			emit reloadPicture(newFilePath);
		}
	}
	else
	{
		if (QFile::exists(newFilePath))
		{
			QMessageBox::warning(this, QString::fromLocal8Bit("温馨提示"),
				QString::fromLocal8Bit("该文件夹已有同名图片"),
				QMessageBox::Ok);
			return;
		}
		else if (w == width)
		{
			QFile::rename(oldFilePath, newFilePath);

			emit reloadPicture(newFilePath);
		}
		else
		{
			QImage image(oldFilePath);
			image = image.scaledToWidth(w);
			QFile::remove(oldFilePath);
			image.save(newFilePath);

			emit reloadPicture(newFilePath);
		}
	}

	hide();
}

void EditPictureDialog::cancelBtnClicked()
{
	hide();
}