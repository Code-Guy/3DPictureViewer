#ifndef _PICTURELABEL_H
#define _PICTURELABEL_H

#include <QtWidgets/QWidget>
#include <QTime>
#include <QTimer>

#include "action.h"

class PictureWidget : public QWidget
{
	Q_OBJECT
public:
	PictureWidget(int fps = 60, QWidget *parent = 0);
	~PictureWidget();

	void setPicturePath(QString filePath);

protected:
	virtual void paintEvent(QPaintEvent* evt);
	virtual void keyPressEvent(QKeyEvent *evt);
	virtual void mousePressEvent(QMouseEvent *evt);
	virtual void mouseMoveEvent(QMouseEvent *evt);
	virtual void wheelEvent(QWheelEvent *evt);
	virtual void closeEvent(QCloseEvent *evt);

signals:
	void closing();

private:
	int fps;
	QPixmap picture;
	QPixmap displayPicture;

	float adaptedWidth;

	QPoint pos;

	float mousePosRatioX, mousePosRatioY;

	QRect desktopRect;

	QTimer drawTimer;//֡ˢ�¼�ʱ��
	QTime *fpsTime;//FPS��ʱ��
	float deltaTime;//��һ֡��������Ҫ��֮��

	Action bounceAction;
	Action scaleAction;

	QPoint prevMousePos;
	QPoint curMousePos;

	QPoint realCurMousePos;

	void tick();
};

#endif //_PICTURELABEL_H