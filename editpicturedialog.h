#ifndef EDITPICTUREDIALOG_H
#define EDITPICTUREDIALOG_H

#include <QDialog>
#include <QVector>

namespace Ui {
class EditPictureDialog;
}

class EditPictureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPictureDialog(QWidget *parent = 0);
    ~EditPictureDialog();

	void set(QString path, QString fileBaseName, QString fileSuffix, int width, int height);

signals:
	void reloadPicture(QString newCenterPicturePath);

private slots:
	void resXSpinBoxValueChanged(int value);
	void resYSpinBoxValueChanged(int value);
	void okBtnClicked();
	void cancelBtnClicked();

private:
    Ui::EditPictureDialog *ui;

	QString path;
	QString fileBaseName;
	QString fileSuffix;
	int width;
	int height;
};

#endif // EDITPICTUREDIALOG_H
