#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include "ui_mainwidget.h"

#include "scene.h"

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	MainWidget(int fps = 60, QWidget *parent = 0);
	~MainWidget();

protected:
	void logic(float deltaTime);//逻辑
	void render();//绘制

	virtual QPaintEngine* paintEngine() const { return NULL; } //避免界面闪烁
	virtual void paintEvent(QPaintEvent* evt);
	virtual void wheelEvent(QWheelEvent *evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseReleaseEvent(QMouseEvent *evt);
	virtual void closeEvent(QCloseEvent *evt);
	virtual void changeEvent(QEvent *evt);

private slots:
	void setFileName(QString fileName);
	void setResolution(int width, int height);
	void setAlpha(float alpha);

private:
	Ui::MainWidgetClass ui;

	void initWidgetProp();//初始化widget的一些属性
	void initOpenGLContext();//初始化window界面context
	bool initMultiSample();//设置MultiSample
	void initGlew();//初始化Glew
	void initGLStates();//初始化opengl参数
	void initLabels();//初始化label

	void renderLabels();

	bool setMultisample(HWND hWnd, int sampleNum, int &pixelFormat);
	bool wglIsExtensionSupported(const char *extension);

	void tick();

	QPoint prevMousePos;
	QPoint curMousePos;

	QTimer drawTimer;//帧刷新计时器
	QTime *fpsTime;//FPS计时器
	float deltaTime;//上一帧绘制所需要的之间

	bool isMousePress;

	Scene *scene;

	QLabel *fileNameLabel;
	QLabel *resolutionLabel;
};

#endif // MAINWIDGET_H
