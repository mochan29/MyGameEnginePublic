#include "ClearScene.h"
#include "SceneManager.h"
#include "Engine/Input.h"
#include "Engine/Sprite.h"

ClearScene::ClearScene(GameObject* parent)
	:GameObject(parent, "ClearScene")
{
	sp = new Sprite;
	sp->Load("Assets//Images//clear.png");
}

ClearScene::~ClearScene()
{
}

void ClearScene::Initialize()
{
}

void ClearScene::Update()
{
	if (Input::IsKey(DIK_RETURN))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TEST);
	}
}

void ClearScene::Draw()
{
	Transform tr;
	sp->Draw(tr);
}

void ClearScene::Release()
{
}
