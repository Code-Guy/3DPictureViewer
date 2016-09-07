#ifndef _SCENE_H
#define _SCENE_H

#include "picture.h"
#include "pictureshader.h"
#include "shadowmapshader.h"
#include "billboardshader.h"
#include "overlayshader.h"
#include "shadowmapfbo.h"
#include "camera.h"
#include "light.h"
#include "action.h"
#include "particleysytem.h"
#include "button.h"

#include <vector>
#include <deque>
#include <QObject>

class Scene : public QObject
{
	Q_OBJECT
public:
	Scene(int width, int height);
	~Scene();

	void logic(float deltaTime, int deltaMousePosX);
	void render();

	void mouseMove(int x, int y);
	void mousePress(int x, int y);
	void mouseRelease(int x, int y);

	void addEaseOutAction();
	QString getCenterPicturePath(QPoint mousePos);

	void load(QString dir);

	static void initSingletons(int width, int height);
	static void destorySingletons();

	static Camera *getCamera();
	static PointLight *getLight();

signals:
	void setFileName(QString fileName);
	void setResolution(int width, int height);
	void setAlpha(float alpha);
	void displayCenterPicture(QString centerPicturePath);
	void showEditPictureDialog(QString path, QString fileBaseName, QString fileSuffix, int width, int height);
	void showHelpDialog();

public slots:
	void reloadPicture(QString newCenterPicturePath);
	void prevBtnClicked();
	void nextBtnClicked();

private slots:
	void helpBtnClicked();
	void deleteBtnClicked();
	void displayBtnClicked();
	void editBtnClicked();
	void particleBtnClicked();
	
private:
	int width;
	int height;

	Picture *bgPicture;
	std::vector<Picture *> subthreadPictures;
	std::vector<Picture *> pictures;

	Picture *centerPicture;

	static Camera *camera;
	static PointLight *light;

	PictureShader *pictureShader;
	ShadowMapShader *shadowMapShader;
	BillboardShader *billboardShader;
	OverlayShader *overlayShader;
	ShadowMapFBO *shadowMapFBO;

	Texture *psTextures[3];
	ParticleSystem *lps, *rps;

	Button *buttons[7];
	Button *psButtons[3];

	int curPSIndex;

	float radius;
	float spanAngle;

	float lastDeltaMousePosX;

	int prevCenterPictureIndex;
	int curCenterPictureIndex;
	float centerPictureAngle;

	std::deque<Action> actions;

	bool loadFinish;//加载完成
	bool arrangeFinish;//排列完成
	bool isLoading;//正在加载

	void reset();

	void loadBgPicture();
	void loadPictures(QString dir);
	void arrangePictures();
	void rearrangePictures(int deletedPictureIndex);

	void initParticleSystems();
	void initButtons(int width, int height);
};

#endif //_SCENE_H