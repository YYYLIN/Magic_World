#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "MagicScene.h"
#include "Magic_Fonts.h"


class MainScene : public Magic::SceneCommon
{
public:
	MainScene();
	~MainScene();

protected:
	bool OnInitialize();
	static void OnUpdata(EntityCommon _Entity);
	static void Draw(EntityCommon _Entity);

private:
	MagicTexture pMagicTexture;
	Magic::Fonts m_Magic_Fonts;
	float m_MagicRotate;

	static MainScene* pMainScene;
};


#endif 