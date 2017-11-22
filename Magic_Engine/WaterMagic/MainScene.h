#ifndef _MAINSCENE_H_
#define _MAINSCENE_H_

#include "MagicEngineContext.h"


class MainScene :public MagicScene
{
public:
	MainScene();
	~MainScene();

protected:
	bool OnInitialize();
	void Draw();
};


#endif 