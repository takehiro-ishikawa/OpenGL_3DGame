#include "Scene.h"
#include "Game.h"

BaseScene::BaseScene(Game* game)
	:mGame(game)
	,mIsLoad(false)
{

}

BaseScene::~BaseScene()
{
}