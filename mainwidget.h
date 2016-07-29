#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>
#include "ui_mainwidget.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

protected:
	void logic(float deltaTime);//逻辑
	void render();//绘制

	virtual QPaintEngine* paintEngine() const { return NULL; } //避免界面闪烁
	virtual void paintEvent(QPaintEvent* evt);

private:
	Ui::MainWidgetClass ui;

	void initWidgetProp();//初始化widget的一些属性
	void initOpenGLContext();//初始化window界面context
	bool initMultiSample();//设置MultiSample
	void initGlew();//初始化Glew

	bool setMultisample(HWND hWnd, int sampleNum, int &pixelFormat);
	bool wglIsExtensionSupported(const char *extension);

	void tickClock();

	QTimer drawTimer;//帧刷新计时器
	QTime *fpsTime;//FPS计时器
	float deltaTime;//上一帧绘制所需要的之间
};

#endif // MAINWIDGET_H
