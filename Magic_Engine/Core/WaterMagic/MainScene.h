#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "UI_Control/BaseWindow.h"
#include "Render/Magic_Fonts.h"


class MainScene : public Magic::UI::BaseWindow
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

	static MainScene* pMainScene;
};


#endif 