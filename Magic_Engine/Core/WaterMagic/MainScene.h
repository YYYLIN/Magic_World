#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "MagicEngineContext.h"
#include "Magic_Fonts.h"


class MainScene :public MagicScenesEx
{
public:
	MainScene();
	~MainScene();

protected:
	bool OnInitialize();
	void OnUpdata();
	void Draw();

private:
	MagicTexture pMagicTexture;
	Magic::Fonts m_Magic_Fonts;
	float m_MagicRotate;
};


#endif 