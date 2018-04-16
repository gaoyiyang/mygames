#include "HelloWorldScene.h"
#include "Game.h"
#include "Role.h"
#include "AppDelegate.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <time.h>
#include <stdio.h>

USING_NS_CC;

using namespace cocostudio::timeline;

int helloFlag = 0;
int gameTime = -1;
int setN = 1;
extern int Myinyue;
extern int Myinxiao;
extern ssp allPlayer[20];
int opNum;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	auto menu = CSLoader::createNode("menu.csb");
	addChild(menu);
	Button* button = dynamic_cast<Button*>(menu->getChildByName("Button_1"));//根据名字获取按钮
	button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::Option, this));//添加监听
	Button* button1 = dynamic_cast<Button*>(menu->getChildByName("Button_2"));//根据名字获取按钮
	button1->addTouchEventListener(CC_CALLBACK_2(HelloWorld::Option2, this));//添加监听
	Button* button2 = dynamic_cast<Button*>(menu->getChildByName("Button_3"));//根据名字获取按钮
	button2->addTouchEventListener(CC_CALLBACK_2(HelloWorld::Option2, this));//添加监听
	Button* button3 = dynamic_cast<Button*>(menu->getChildByName("Button_4"));//根据名字获取按钮
	button3->addTouchEventListener(CC_CALLBACK_2(HelloWorld::Option3, this));//添加监听
    return true;
}

void HelloWorld::Option(Ref* pSender, Widget::TouchEventType type)
{
	auto scene = MapMenu::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
		CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

void HelloWorld::Option2(Ref* pSender, Widget::TouchEventType type)
{
	auto node = (Node*)pSender;
	if (node->getName() == "Button_2") {
		setN = 1;
	}
	else if (node->getName() == "Button_3"){
		setN = 2;
	}
	auto scene = MySet::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{

										  CCDirector::sharedDirector()->replaceScene(scene);
		 
	}break;
	default:break;
	}
}
void HelloWorld::Option3(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  CCDirector::sharedDirector()->end();
	}break;
	default:break;
	}
}

Scene* MapMenu::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MapMenu::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MapMenu::init()
{
	
	if (!Layer::init())
	{
		return false;
	}
	load();
	this->schedule(schedule_selector(MapMenu::loading), 0.2f);
	return true;
}

void MapMenu::mapTest(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  auto a = (Node*)pSender;
										  gameTime = sToN(a->getName());
										  auto scene = OP::createScene();
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}


void MapMenu::loading(float f)
{
	if (helloFlag == 0){
		LoadingBar* l = (LoadingBar*)this->getChildByTag(1);
		Text* t = (Text*)this->getChildByTag(2);
		auto d = this->getChildByTag(3);
		srand(time(NULL));
		int ti = rand()%20 + 10;
		l->setPercent(l->getPercent() + ti);
		d->setPosition(ccp((l->getPercent() / 100) * 960, 20));
		stringstream ss;
		string str;
		int b = l->getPercent();
		ss << b;
		ss >> str;
		str = str + "%";
		t->setText(str);
		if (l->getPercent() == 100){
			helloFlag = 1;
			this->removeChildByTag(1);
			this->removeChildByTag(2);
			this->removeChildByTag(3);
			this->removeChildByTag(5);
			auto menu = CSLoader::createNode("Map.csb");
			addChild(menu);
			Button* button1 = dynamic_cast<Button*>(menu->getChildByName("0"));
			button1->addTouchEventListener(CC_CALLBACK_2(MapMenu::mapTest, this));//添加监听
		}
	}
}

void MapMenu::load()
{
	helloFlag = 0;
	auto node = CSLoader::createNode("m_loading.csb");
	auto l = (LoadingBar*)node->getChildByName("LoadingBar_1");
	l->setPercent(0.0f);
	auto t = (Text*)node->getChildByTag(2);
	addChild(l, 99, 1);
	addChild(t, 100, 2);
	auto rootNode = CSLoader::createNode("Node3.csb");
	addChild(rootNode, 100, 3);
	rootNode->setPosition(ccp((l->getPercent() / 100) * 960, 20));
	ActionTimeline *action = CSLoader::createTimeline("Node3.csb");
	rootNode->runAction(action);
	action->gotoFrameAndPlay(0, 15, true);
	auto sc = CSLoader::createNode("load_1.csb");
	addChild(sc, 0, 5);
}

Scene* OP::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = OP::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool OP::init()
{

	if (!Layer::init())
	{
		return false;
	}
	gameLoad();
	auto node = CSLoader::createNode("Load.csb");
	auto b_k = (Button *)node->getChildByName("kaishi");
	b_k->addTouchEventListener(CC_CALLBACK_2(OP::kai, this));
	for (int i = 0; i < 4; i++){
		if (allPlayer[i].a == 1){
			auto p1_s = (Button *)node->getChildByTag(i)->getChildByName("shuxing");
			p1_s->addTouchEventListener(CC_CALLBACK_2(OP::p1Sx, this));
			auto p1_sk = (Button *)node->getChildByTag(i)->getChildByName("jineng");
			p1_sk->addTouchEventListener(CC_CALLBACK_2(OP::p1Sk, this));
		}
		if (allPlayer[i].a == 0){
			node->removeChildByTag(i + 1);
		}
	}
	addChild(node);
	auto l = CSLoader::createNode("all_map.csb")->getChildByName("map" + nToS(gameTime));
	addChild(l);//添加任务说明
	l->setPosition(ccp(660, 100));
	return true;
}

void OP::p1Sx(Ref* pSender, Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	opNum = node->getTag();
	cocos2d::Scene* scene = SX::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

void OP::p1Sk(Ref* pSender, Widget::TouchEventType type)
{
	Node* node = (Node*)pSender;
	opNum = node->getTag() - 10;
	cocos2d::Scene* scene = SK::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

void OP::kai(Ref* pSender, Widget::TouchEventType type)
{
	cocos2d::Scene* scene;
	gameSave();
	scene = MapTest::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

Scene* SX::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SX::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SX::init()
{

	if (!Layer::init())
	{
		return false;
	}
	Node* node;
	if(opNum == 0)
		node = CSLoader::createNode("player0_sx.csb");
	if (opNum == 1)
		node = CSLoader::createNode("player1_sx.csb");
	if (opNum == 2)
		node = CSLoader::createNode("player2_sx.csb");
	if (opNum == 3)
		node = CSLoader::createNode("player3_sx.csb");
	addChild(node);
	auto t = (Text*)node->getChildByName("maxHp");
	t->setText(nToS(allPlayer[opNum].p.maxHp));
	t = (Text*)node->getChildByName("att");
	t->setText(nToS(allPlayer[opNum].p.att));
	t = (Text*)node->getChildByName("def");
	t->setText(nToS(allPlayer[opNum].p.def));
	t = (Text*)node->getChildByName("sp");
	t->setText(nToS(allPlayer[opNum].p.sp));
	t = (Text*)node->getChildByName("mov");
	t->setText(nToS(allPlayer[opNum].p.mov));
	t = (Text*)node->getChildByName("atMov");
	t->setText(nToS(allPlayer[opNum].p.atMov));
	auto p1_back = (Button *)node->getChildByName("back");
	p1_back->addTouchEventListener(CC_CALLBACK_2(SX::back, this));
	return true;
}

void SX::back(Ref* pSender, Widget::TouchEventType type)
{
	auto scene = OP::createScene();
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

Scene* SK::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SK::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SK::init()
{

	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::createNode("skill_shezhi.csb");
	addChild(rootNode, 0);
	for (int i = 0; i < 12; i++){
		if (allPlayer[opNum].p.skill[i] != 0){
			auto node = CSLoader::createNode("all_jineng.csb");
			auto b = (Button*)node->getChildByName(nToS(allPlayer[opNum].p.skill[i]));
			addChild(b, 2);
			b->setPosition(ccp(105 + (i % 4) * 105, (3 - i % 3) * 40 + 10));
			b->addTouchEventListener(CC_CALLBACK_2(SK::getSkill, this));
		}
	}
	nSkill();
	auto over = (Button*)rootNode->getChildByName("over");
	over->addTouchEventListener(CC_CALLBACK_2(SK::over, this));
	return true;
}

void SK::nSkill()
{
	for (int i = 0; i < 5; i++){
		this->removeChildByTag(5000 + i);
	}
	for (int i = 0; i < 5; i++){
		if (allPlayer[opNum].p.nSkill[i][0] != 0){
			auto node = CSLoader::createNode("all_jineng.csb");
			auto b = (Button*)node->getChildByName(nToS(allPlayer[opNum].p.nSkill[i][0]));
			addChild(b, 2, 5000 + i);
			b->setPosition(ccp(300, (5 - i % 5) * 40 + 300));
			b->addTouchEventListener(CC_CALLBACK_2(SK::deSkill, this));
		}
	}
}

void SK::getSkill(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
	this->removeChildByName("skillText");
	auto node = (Node*)pSender;
	string str = node->getName();
	str = str  + "_t";
	auto rootNode = CSLoader::createNode("all_jineng.csb");
	auto n = rootNode->getChildByName(str);
	addChild(n, 1);
	n->setName("skillText");
	n->setPosition(ccp(690, 270));
	int f = 1;
	for (int i = 0; i < 5; i++){
		if (allPlayer[opNum].p.nSkill[i][0] == sToN(node->getName()))
			f = 0;
	}
	if (f == 1){
		for (int i = 0; i < 5; i++){
			if (allPlayer[opNum].p.nSkill[i][0] == 0){
				allPlayer[opNum].p.nSkill[i][0] = sToN(node->getName());
				nSkill();
				break;
			}
		}
	}
	}break;
	default:break;
	}
}

void SK::deSkill(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  auto node = (Node*)pSender;
										  for (int i = 0; i < 5; i++){
											  if (allPlayer[opNum].p.nSkill[i][0] == sToN(node->getName()))
												  allPlayer[opNum].p.nSkill[i][0] = 0;
										  }

										  this->removeChild(node);

	}break;
	default:break;
	}
}

void SK::over(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  gameSave();
										  auto scene = OP::createScene();
										  CCDirector::sharedDirector()->replaceScene(scene);
	}break;
	default:break;
	}
}

Scene* MySet::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MySet::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MySet::init()
{

	if (!Layer::init())
	{
		return false;
	}
	Node* rootNode;
	if (setN == 1) {
		rootNode = CSLoader::createNode("Scene_set.csb");
		auto b_yinyue = (Button*)rootNode->getChildByName("yinyue");
		b_yinyue->addTouchEventListener(CC_CALLBACK_2(MySet::yinyue, this));
		auto b = (Button*)rootNode->getChildByName("over");
		b->addTouchEventListener(CC_CALLBACK_2(MySet::over, this));
		addChild(rootNode, 1, 1);
		auto t_yinyue = (Text*)this->getChildByTag(1)->getChildByName("yinyue_t");
		if (Myinyue == 1)t_yinyue->setText("ON");
		else if (Myinyue == 0)t_yinyue->setText("OFF");
	}
	if (setN == 2){
		rootNode = CSLoader::createNode("Scene_name.csb");
		auto b = (Button*)rootNode->getChildByName("over");
		b->addTouchEventListener(CC_CALLBACK_2(MySet::over, this));
		addChild(rootNode);
	}
	return true;
}

void MySet::over(Ref* pSender, Widget::TouchEventType type)
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

void MySet::yinyue(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  if (Myinyue == 1){
											  Myinyue = 0;
											  auto text = (Text*)this->getChildByTag(1)->getChildByName("yinyue_t");
											  text->setText("OFF");
										  }
										  else if (Myinyue == 0){
											  Myinyue = 1;
											  auto text = (Text*)this->getChildByTag(1)->getChildByName("yinyue_t");
											  text->setText("ON");
										  }
	}break;
	default:break;
	}
}


Scene* WIN::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = WIN::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool WIN::init()
{

	if (!Layer::init())
	{
		return false;
	}
	auto s = CSLoader::createNode("gameWin.csb");
	addChild(s);
	auto mov = CCSequence::create(CCMoveBy::create(3.0f, ccp(0, 0)), CCCallFunc::create(this, callfunc_selector(WIN::goMenu)), NULL, NULL);
	this->runAction(mov);
	return true;
}

void WIN::goMenu()
{
	auto scene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(scene);
}

Scene* LOSE::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LOSE::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LOSE::init()
{

	if (!Layer::init())
	{
		return false;
	}
	auto s = CSLoader::createNode("gameLose.csb");
	addChild(s);
	auto mov = CCSequence::create(CCMoveBy::create(3.0f, ccp(0, 0)), CCCallFunc::create(this, callfunc_selector(LOSE::goMenu)), NULL, NULL);
	this->runAction(mov);
	return true;
}

void LOSE::goMenu()
{
	auto scene = HelloWorld::createScene();
	CCDirector::sharedDirector()->replaceScene(scene);
}


string nToS(int i)
{
	string str;
	stringstream ss;
	ss << i;
	ss >> str;
	return str;
}

int sToN(string s)
{
	int i;
	stringstream ss;
	ss << s;
	ss >> i;
	return i;
}
