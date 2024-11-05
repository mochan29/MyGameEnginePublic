#include "TestScene.h"
#include "SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Sprite.h"
#include "Debug.h"

namespace
{
	const float TIMER = 30;
}
TestScene::TestScene(GameObject* parent)
	:GameObject(parent, "TestScene")
{
	
	noRendaTimer_ =TIMER;
}

TestScene::~TestScene()
{
}

void TestScene::Initialize()
{
	sp = new Sprite;
	sp->Load("Assets//Images//title.png");
}

void TestScene::Update()
{
	//タイトル画面表示
	if (noRendaTimer_ > 0)
	{
		noRendaTimer_--;
	}
	else
	{	
		if (Input::IsKey(DIK_RETURN))
		{
			noRendaTimer_ = TIMER;
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_PLAY);
		}
		
	}
	
}

void TestScene::Draw()
{
	Transform tr;
	sp->Draw(tr);
}

void TestScene::Release()
{
}
