#include "Game.h"
#include "HelloWorldScene.h"
#include "AppDelegate.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* StartGame::createScene()
{
	auto scene = Scene::create();

	auto layer = StartGame::create();

	scene->addChild(layer);
	
	return scene;
}

// on "init" you need to initialize your instance
bool StartGame::init()
{
	if (!Layer::init())
	{
		return false;
	}
	MapTest mapT;
	auto rootNode = CSLoader::createNode("Scene.csb");

	addChild(rootNode);
	auto back = CSLoader::createNode("Node.csb");
	this->addChild(back);
	Button* backButton = dynamic_cast<Button*>(back->getChildByName("Button_Back"));
	backButton->addTouchEventListener(CC_CALLBACK_2(StartGame::backMenu, this));//为Back按钮添加功能
	return true;
}
void StartGame::backMenu(Ref* pSender, Widget::TouchEventType type)
{
	auto scene = HelloWorld::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
		 CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}