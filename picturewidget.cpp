#include "picturewidget.h"
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <algorithm>
#include "tool.h"

const float BounceActionBaseValueRatio = 0.1f;
const float BounceActionTimeInterval = 0.8f;
const float ScaleActionSensitivity = 0.1f;
const float ScaleActionTimeInterval = 1.0f;
const float ScaleActionLowerBound = 0.1f;
const float ScaleActionUpperBound = 2.0f;

const QColor BackgroundColor(0, 0, 0, 100);

PictureWidget::PictureWidget(int fps, QWidget *parent) : fps(fps), QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	desktopRect = QApplication::desktop()->availableGeometry();

	//设置绘制计时器
	connect(&drawTimer, SIGNAL(timeout()), this, SLOT(update()));
	drawTimer.start(1000 / fps);

	fpsTime = new QTime;
	fpsTime->start();
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

	if (h > desktopRect.height())
	{
		w /= h / (0.8 * desktopRect.height());
		h /= h / (0.8 * desktopRect.height());
	}
	if (w > desktopRect.width())
	{
		w /= w / (0.8 * desktopRect.width());
		h /= w / (0.8 * desktopRect.width());
	}

	adaptedWidth = w;

	bounceAction.setBaseValue(adaptedWidth * BounceActionBaseValueRatio);
	bounceAction.setIncrementValue(adaptedWidth * (1 - BounceActionBaseValueRatio));
	bounceAction.setTimeInterval(BounceActionTimeInterval);
	bounceAction.setCurveShape(BounceCurve);
	bounceAction.start();

	scaleAction.setBaseValue(1.0f);
	scaleAction.setCurveShape(LinearCurve);
}

void PictureWidget::paintEvent(QPaintEvent* evt)
{
	tick();

	QPainter painter(this);
// 	QBrush brush(BackgroundColor);
// 	painter.setBrush(brush);
// 	painter.drawRect(rect());

	if (!picture.isNull())
	{
		if (bounceAction.isRunning())
		{
			bounceAction.logic(deltaTime);
			float displayWidth = bounceAction.getValue();
			displayPicture = picture.scaledToWidth(displayWidth);

			pos = QPoint((desktopRect.width() - displayPicture.width()) / 2, (desktopRect.height() - displayPicture.height()) / 2);
		}
		if (scaleAction.isRunning())
		{
			scaleAction.logic(deltaTime);
			float scale = scaleAction.getValue();

			displayPicture = picture.scaledToWidth(adaptedWidth * scale);
			pos = realCurMousePos - QPoint(displayPicture.width() * mousePosRatioX, displayPicture.height() * mousePosRatioY);
		}
		painter.drawPixmap(pos, displayPicture);
	}
}

void PictureWidget::keyPressEvent(QKeyEvent *evt)
{
	switch (evt->key())
	{
	case Qt::Key_Escape:
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
	pos += curMousePos - prevMousePos;
	mousePosRatioX = (float)(realCurMousePos.x() - pos.x()) / displayPicture.width();
	mousePosRatioY = (float)(realCurMousePos.y() - pos.y()) / displayPicture.height();
	prevMousePos = curMousePos;
}

void PictureWidget::wheelEvent(QWheelEvent *evt)
{
	realCurMousePos = mapFromGlobal(cursor().pos());
	if (!bounceAction.isRunning() && 
		QRect(pos, displayPicture.size()).contains(realCurMousePos))
	{
		int numSteps = evt->delta() / 120;
		
		mousePosRatioX = (float)(realCurMousePos.x() - pos.x()) / displayPicture.width();
		mousePosRatioY = (float)(realCurMousePos.y() - pos.y()) / displayPicture.height();

		float curValue = scaleAction.getValue();
		float restValue = scaleAction.getRestValue();
		float sumVal = curValue + restValue;

		float minIncrementValue = ScaleActionLowerBound - sumVal;
		float maxIncrementValue = ScaleActionUpperBound - sumVal;
		float incrementValue = numSteps * ScaleActionSensitivity;
		Tool::clamp(incrementValue, minIncrementValue, maxIncrementValue);

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