#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include "picturewidget.h"
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

	virtual void closeEvent(QCloseEvent *evt);
	virtual void changeEvent(QEvent *evt);

private slots:
	void setFileName(QString fileName);
	void setResolution(int width, int height);
	void setAlpha(float alpha);

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
};

#endif // MAINWIDGET_H
