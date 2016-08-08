#include "picturewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <algorithm>
#include "tool.h"

const float BounceActionBaseValueRatio = 0.1f;
const float BounceActionTimeInterval = 0.8f;
const float ScaleActionSensitivity = 0.1f;
const float ScaleActionTimeInterval = 1.0f;
const float ScaleActionLowerBound = 0.1f;
const float ScaleActionUpperBound = 5.0f;

const QColor BackgroundColor(0, 0, 0, 100);

PictureWidget::PictureWidget(int fps, QWidget *parent) : fps(fps), QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//设置绘制计时器
	connect(&drawTimer, SIGNAL(timeout()), this, SLOT(update()));
	drawTimer.start(1000 / fps);

	fpsTime = new QTime;
	fpsTime->start();

	desktopRect = QApplication::desktop()->geometry();
}

PictureWidget::~PictureWidget()
{
	delete fpsTime;
}

void PictureWidget::setPicturePath(QString filePath)
{
	picture = QPixmap(filePath);
	
	int w = picture.width();
	int h = picture.height();

	if ((float)h / w > desktopRect.height() / desktopRect.width())
	{
		if (h > desktopRect.height())
		{
			w /= h / (0.8 * desktopRect.height());
			h /= h / (0.8 * desktopRect.height());
		}
	}
	else
	{
		if (w > desktopRect.width())
		{
			w /= w / (0.8 * desktopRect.width());
			h /= w / (0.8 * desktopRect.width());
		}
	}

	
	float baseScaleFactor = (float)w / picture.width();
	
	offsetPos = QPoint((desktopRect.width() - w) / 2, (desktopRect.height() - h) / 2);
	
	bounceAction.setBaseValue(baseScaleFactor * BounceActionBaseValueRatio);
	bounceAction.setIncrementValue(baseScaleFactor * (1 - BounceActionBaseValueRatio));
	bounceAction.setTimeInterval(BounceActionTimeInterval);
	bounceAction.setCurveShape(BounceCurve);
	bounceAction.start();

	scaleAction.setBaseValue(baseScaleFactor);
	scaleAction.setCurveShape(LinearCurve);
}

void PictureWidget::paintEvent(QPaintEvent* evt)
{
	tick();

	QPainter painter(this);

	if (bounceAction.isRunning())
	{
		bounceAction.logic(deltaTime);
		scaleFactor = bounceAction.getValue();
	}

	if (scaleAction.isRunning())
	{
		scaleAction.logic(deltaTime);
		scaleFactor = scaleAction.getValue();

		offsetPos.setX(curCursorPos.x() - curPosRatioX * scaleFactor);
		offsetPos.setY(curCursorPos.y() - curPosRatioY * scaleFactor);
	}

	painter.translate(offsetPos);
	painter.scale(scaleFactor, scaleFactor);

	painter.drawPixmap(0, 0, picture);
}

void PictureWidget::keyPressEvent(QKeyEvent *evt)
{
	switch (evt->key())
	{
	case Qt::Key_Escape:
		bounceAction.stop();
		scaleAction.stop();
		close();
		break;
	}
}

void PictureWidget::mousePressEvent(QMouseEvent *evt)
{
	prevMousePos = evt->pos();
	curMousePos = evt->pos();
}

void PictureWidget::mouseMoveEvent(QMouseEvent *evt)
{
	curMousePos = evt->pos();

	offsetPos += curMousePos - prevMousePos;
	curCursorPos = cursor().pos();
	curPosRatioX = (curCursorPos.x() - offsetPos.x()) / scaleFactor;
	curPosRatioY = (curCursorPos.y() - offsetPos.y()) / scaleFactor;

	prevMousePos = curMousePos;
}

void PictureWidget::wheelEvent(QWheelEvent *evt)
{
	if (!bounceAction.isRunning())
	{
		int numSteps = evt->delta() / 120;

		float curValue = scaleAction.getValue();
		float restValue = scaleAction.getRestValue();
		float sumVal = curValue + restValue;

		float minIncrementValue = ScaleActionLowerBound - sumVal;
		float maxIncrementValue = ScaleActionUpperBound - sumVal;

		float incrementValue = numSteps * ScaleActionSensitivity;
		Tool::clamp(incrementValue, minIncrementValue, maxIncrementValue);

		curCursorPos = cursor().pos();
		curPosRatioX = (curCursorPos.x() - offsetPos.x()) / scaleFactor;
		curPosRatioY = (curCursorPos.y() - offsetPos.y()) / scaleFactor;

		scaleAction.setBaseValue(curValue);
		scaleAction.setIncrementValue(incrementValue);
		scaleAction.setTimeInterval(ScaleActionTimeInterval);
		scaleAction.start();
	}
}

void PictureWidget::closeEvent(QCloseEvent *evt)
{
	emit closing();
}

void PictureWidget::tick()
{
	deltaTime = fpsTime->elapsed();
	deltaTime /= 1000.0f;
	fpsTime->restart();

	if (deltaTime > 2.0 / fps)
	{
		deltaTime = 1.0 / fps;
	}
}