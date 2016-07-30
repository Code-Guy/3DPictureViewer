#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>
#include "ui_mainwidget.h"

#include "scene.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(int fps = 60, QWidget *parent = 0);
	~MainWidget();

protected:
	void logic(float deltaTime);//�߼�
	void render();//����

	virtual QPaintEngine* paintEngine() const { return NULL; } //���������˸
	virtual void paintEvent(QPaintEvent* evt);
	virtual void wheelEvent(QWheelEvent *evt);

private:
	Ui::MainWidgetClass ui;

	void initWidgetProp();//��ʼ��widget��һЩ����
	void initOpenGLContext();//��ʼ��window����context
	bool initMultiSample();//����MultiSample
	void initGlew();//��ʼ��Glew
	void initGLStates();//��ʼ��opengl����

	bool setMultisample(HWND hWnd, int sampleNum, int &pixelFormat);
	bool wglIsExtensionSupported(const char *extension);

	void tick();

	QTimer drawTimer;//֡ˢ�¼�ʱ��
	QTime *fpsTime;//FPS��ʱ��
	float deltaTime;//��һ֡��������Ҫ��֮��

	Scene *scene;
};

#endif // MAINWIDGET_H
