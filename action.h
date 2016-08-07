#ifndef _ACTION_H
#define _ACTION_H

enum CurveShape
{
	LinearCurve, 
	EaseInCurve,
	EaseOutCurve,
	EaseInOutCurve,
	BounceCurve
};

class Action
{
public:
	Action();
	~Action();

	void logic(float deltaTime);
	void start();
	void pause();
	
	void setBaseValue(float baseValue);
	void setIncrementValue(float incrementVal);

	void setTimeInterval(float timeInterval);
	void setCurveShape(CurveShape curveShape);

	CurveShape getCurveShape();
	float getValue();
	float getDeltaValue();
	float getRestValue();

	bool isRunning();

private:
	float baseValue;
	float incrementVal;
	float timeInterval;

	float t;
	float deltaVal;
	float val;
	CurveShape curveShape;

	bool running;

	void linear();
	void easeIn();
	void easeOut();
	void easeInOut();
	void bounce();
};

#endif //_ACTION_H