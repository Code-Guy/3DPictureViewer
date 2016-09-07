#ifndef _BUTTON_H
#define _BUTTON_H

#include "overlay.h"

#include <QObject>

class Button : public QObject
{
	Q_OBJECT
public:
	Button(std::string texturePath, glm::ivec2 coord);
	~Button();

	void render(OverlayShader *overlayShader);

	void mouseMove(int x, int y);
	void mousePress(int x, int y);
	void mouseRelease(int x, int y);

	void setDisable(bool isDisabled);

signals:
	void clicked();

private:
	Overlay *overlay;

	glm::ivec2 center;

	bool isDisabled;
	bool isPressed;

	bool contain(int x, int y);
};

#endif //_BUTTON_H