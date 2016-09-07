#include "button.h"
#include <QDebug>

const int ButtonRadius = 25;
const glm::ivec2 ButtonOffset = glm::ivec2(30, 36);
const float CommonEnhance = 1.0f;
const float HoverEnhance = 1.2f;
const float PressEnhance = 0.8f;

Button::Button(std::string texturePath, glm::ivec2 coord)
{
	overlay = new Overlay(texturePath, coord);
	overlay->setEnhance(1.0f);

	center = coord + ButtonOffset;

	isDisabled = false;
	isPressed = false;
}

Button::~Button()
{
	delete overlay;
}

void Button::render(OverlayShader *overlayShader)
{
	overlay->render(overlayShader);
}

void Button::mouseMove(int x, int y)
{
	if (!isDisabled)
	{
		if (contain(x, y))
		{
			if (isPressed)
			{
				overlay->setEnhance(PressEnhance);
			}
			else
			{
				overlay->setEnhance(HoverEnhance);
			}
		}
		else
		{
			overlay->setEnhance(CommonEnhance);
		}
	}
}

void Button::mousePress(int x, int y)
{
	if (!isDisabled)
	{
		isPressed = true;	
	}
}

void Button::mouseRelease(int x, int y)
{
	if (!isDisabled)
	{
		if (contain(x, y))
		{
			emit clicked();
		}
		isPressed = false;
	}
}

void Button::setDisable(bool isDisabled)
{
	this->isDisabled = isDisabled;
}

bool Button::contain(int x, int y)
{
	return glm::length(glm::vec2(x - center.x, y - center.y)) < ButtonRadius;
}