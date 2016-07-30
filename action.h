#ifndef _ACTION_H
#define _ACTION_H

enum CurveShape
{
	LinearCurve, 
	EaseInCurve,
	EaseOutCurve,
	EaseInOutCurve
};

class Action
{
public:
	Action();
	~Action();

	void logic(float deltaTime);
	void start();

	void setBaseValue(float baseValue);
	void setIncrementValue(float incrementVal);
	void setTimeInterval(float timeInterval);
	void setCurveShape(CurveShape curveShape);

	float getValue();

	bool isRunning();

private:
	float baseValue;
	float incrementVal;
	float timeInterval;

	float t;
	float val;
	CurveShape curveShape;

	bool running;

	void linear();
	void easeIn();
	void easeOut();
	void easeInOut();
};

#endif //_ACTION_H