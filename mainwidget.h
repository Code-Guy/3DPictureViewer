#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include "picturewidget.h"
#include "editpicturedialog.h"

#include "ui_mainwidget.h"

#include "scene.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(int fps = 80, QWidget *parent = 0);
	~MainWidget();

protected:
	void logic(float deltaTime);//�߼�
	void render();//����

	virtual QPaintEngine* paintEngine() const { return NULL; } //���������˸
	virtual void paintEvent(QPaintEvent* evt);
	virtual void keyPressEvent(QKeyEvent *evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseReleaseEvent(QMouseEvent *evt);
	virtual void mouseDoubleClickEvent(QMouseEvent *evt);
	virtual void dragEnterEvent(QDragEnterEvent *evt);
	virtual void dropEvent(QDropEvent *evt);
	virtual void closeEvent(QCloseEvent *evt);
	virtual void changeEvent(QEvent *evt);

private slots:
	void setFileName(QString fileName);
	void setResolution(int width, int height);
	void setAlpha(float alpha);
	void displayCenterPicture(QString centerPicturePath);
	void showEditPictureDialog(QString path, QString fileBaseName, QString fileSuffix, int width, int height);
	void showHelpDialog();

private:
	Ui::MainWidgetClass ui;
	int fps;

	void initWidgetProp();//��ʼ��widget��һЩ����
	void initOpenGLContext();//��ʼ��window����context
	bool initMultiSample();//����MultiSample
	void initGlew();//��ʼ��Glew
	void initGLStates();//��ʼ��opengl����
	void initLabels();//��ʼ��label

	void renderLabels();

	bool setMultisample(HWND hWnd, int sampleNum, int &pixelFormat);
	bool wglIsExtensionSupported(const char *extension);

	void tick();

	QPoint prevMousePos;
	QPoint curMousePos;

	QTimer drawTimer;//֡ˢ�¼�ʱ��
	QTime *fpsTime;//FPS��ʱ��
	float deltaTime;//��һ֡��������Ҫ��֮��

	bool isMousePress;

	Scene *scene;

	QLabel *fileNameLabel;
	QLabel *resolutionLabel;
	
	PictureWidget *pictureWidget;
	EditPictureDialog *editPictureDialog;
	QDialog *helpDialog;
};

#endif // MAINWIDGET_H
