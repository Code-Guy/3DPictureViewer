#include "action.h"
#include <cstdio>

Action::Action()
{
	running = false;
	curveShape = LinearCurve;
}

Action::~Action()
{

}

void Action::logic(float deltaTime)
{
	if (running)
	{
		t += deltaTime;
		if (t > timeInterval)
		{
			running = false;
		}
		else
		{
			float lastVal = val;
			switch (curveShape)
			{
			case LinearCurve:
				linear();
				break;
			case EaseInCurve:
				easeIn();
				break;
			case EaseOutCurve:
				easeOut();
				break;
			case EaseInOutCurve:
				easeInOut();
				break;
			case BounceCurve:
				bounce();
				break;
			default:
				break;
			}
			deltaVal = val - lastVal;
		}
	}
}

void Action::start()
{
	t = 0;
	running = true;
}

void Action::setBaseValue(float baseValue)
{
	this->baseValue = baseValue;
	this->val = baseValue;
}

void Action::setIncrementValue(float incrementVal)
{
	if (running)
	{
		incrementVal += this->incrementVal * (1 - t / timeInterval);
	}
	this->incrementVal = incrementVal;
}

void Action::setTimeInterval(float timeInterval)
{
	this->timeInterval = timeInterval;
}

void Action::setCurveShape(CurveShape curveShape)
{
	this->curveShape = curveShape;
}

CurveShape Action::getCurveShape()
{
	return curveShape;
}

float Action::getValue()
{
	return val;
}

float Action::getDeltaValue()
{
	return deltaVal;
}

bool Action::isRunning()
{
	return running;
}

void Action::linear()
{
	val = incrementVal  * t / timeInterval + baseValue;
}

void Action::easeIn()
{
	float tt = t / timeInterval;
	val = incrementVal  * tt * tt + baseValue;
}

void Action::easeOut()
{
	float tt = t / timeInterval;
	val = -incrementVal  * tt * (tt - 2) + baseValue;
}

void Action::easeInOut()
{
	float tt = 2 * t / timeInterval;
	if (tt < 1)
	{
		val = incrementVal  / 2 * tt * tt + baseValue;
		return;
	}
	tt--;
	val = -incrementVal  / 2 * (tt * (tt - 2) - 1) + baseValue;
}

void Action::bounce()
{
	float tt = t;
	if ((tt /= timeInterval) < (1 / 2.75))
		val = incrementVal * (7.5625 * tt * tt) + baseValue;
	else if (tt < (2 / 2.75))
		val = incrementVal * (7.5625 * (tt -= (1.5 / 2.75)) * tt + 0.75) + baseValue;
	else if (tt < (2.5 / 2.75))
		val = incrementVal * (7.5625 * (tt -= (2.25 / 2.75)) * tt + 0.9375) + baseValue;
	else
		val = incrementVal * (7.5625 * (tt -= (2.625 / 2.75)) * tt + 0.984375) + baseValue;
}