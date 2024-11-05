#include "OverScene.h"
#include "SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Sprite.h"

OverScene::OverScene(GameObject* parent)
	:GameObject(parent, "OverScene")
{
	sp = new Sprite;
	sp->Load("Assets//Images//over.png");
}

OverScene::~OverScene()
{
}

void OverScene::Initialize()
{
}

void OverScene::Update()
{
	if (Input::IsKey(DIK_RETURN))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

void OverScene::Draw()
{
	Transform tr;
	sp->Draw(tr);
}

void OverScene::Release()
{
}
