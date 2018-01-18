#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "MagicEngineContext.h"


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
	float m_MagicRotate;
};


#endif 