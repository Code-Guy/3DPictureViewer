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