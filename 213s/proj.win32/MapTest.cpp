#include <math.h>
#include <iostream>
#include "MapTest.h"
#include "AppDelegate.h"
#include "Role.h"
#include "Skill.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"//音乐音效
#include "ui/CocosGUI.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <stdio.h>
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/filereadstream.h"
#include "json/stringbuffer.h"

#define READY 0
#define RUN 1
#define ATK 2
#define MOVE 3
#define SKILL 4

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;
using namespace std;

int fff = 0;

//移动方向
int nowMoveTag = 0;

int guanghuan = -1;
int skillOver;
int iMove[3];//前面为移动人的编号，后面为移动的方位
extern int gameTime;//当前关卡
int nowSkillTag = 0;//当前技能编号
int attNum1 = -1;
int nowPlayers = 0;
ssp allPlayer[20];
ssp oldPlayer[20];
int gameFlag = READY;//判断是否有棋子正在进行回合
int nowNum = 0;
float Myinliang = 0;//音乐音量
int Myinyue = 1;
int Myinxiao = 1;
int MSlider;//音乐音量的滑动条

float dx = 420.0;
float dy = 270.0;
int winTag[100][100] = { { 0 }, { 0 } };
int testMap[100][100] = { { 0 }, { 0 } };
int f0 = 0;
Player* nowTest;
cocos2d::Node nowT;
Dxy pxy[10];
int ddx[4] = { 0, 0, -1, 1 };
int ddy[4] = { -1, 1, 0, 0 };
int moveX = 0, moveY = 0;//现在的坐标
int targetX = 0, targetY = 0;//目标坐标
const int maxLevels = 1000;//最大移动步数
int maxAct = 4;//移动方向总数
int table[100][100] = { 0 };//标记是否已达到
int level = -1;//第几步
int levelComplete = 0;//这一步的搜索是否完成
int allComplete = 0;//全部搜索是否已完成
int Act[maxLevels] = { 0 };//每一步的移动方向（1、2、3、4）

Scene* MapTest::createScene()
{
	auto scene = Scene::create();

	auto layer = MapTest::create();

	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool MapTest::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initDFS();

	/*测试代码:
	 *测试Buff类的实现，为玩家控制角色添加BUFF
	 *2018年2月8日 18:37:09
	 */
	allPlayer[1].buffs[1] = Buff::getBuff(SUOZU, 1, 1);
	/*测试代码:
	*添加测试技能1给玩家角色
	*2018年2月20日 17:26:15
	*/
	allPlayer[2].p.skillNames[0] = SkillNameEnum::TEST1;

	/*地图参数初始化*/
	for (int i = 0; i < 20; i++){
		allPlayer[i].f = 0;
		allPlayer[i].a = 0;
		allPlayer[i].p.gongji = 1;
		allPlayer[i].p.jineng = 1;
		allPlayer[i].p.yidong = 1;
		//初始化BUFF列表
		for (int j = 0; j < 0; j++){
			allPlayer[i].buffs[j] = NULL;
		}
	}
	gameFlag = READY;
	for (int i = 0; i < 100; i++)
	for (int j = 0; j < 100; j++)
		winTag[i][j] = 0;
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	auto root = CSLoader::createNode("mapScence.csb");
	addChild(root, 0);
	auto rootNode = CSLoader::createNode("MapTest.csb");
	addChild(rootNode, 1);

	/*LabelTTF* label = LabelTTF::create("哈哈哈哈", "Marker Felt", 30);
	label->setPosition(ccp(100, 100));
	this->addChild(label, 100);*/
	setMap();

	//设置能量显示
	auto power = CSLoader::createNode("powerNode.csb");
	power->setPosition(Vec2(800,400));
	power->setName("power");
	((TextAtlas*)power->getChildByName("number"))->setString("100");
	this->addChild(power);

	auto back = CSLoader::createNode("Node.csb");
	this->addChild(back);
	Button* backButton = dynamic_cast<Button*>(back->getChildByName("Button_Back"));
	backButton->addTouchEventListener(CC_CALLBACK_2(MapTest::backMenu, this));//为Back按钮添加功能
	this->schedule(schedule_selector(MapTest::start), 0.5f);//schedule()函数，实现计时器，每0.2f秒执行一次MapTest::start()
	return true;
}

void MapTest::startTime(cocos2d::Layer* layer, ssp* sPlayer){
	auto buffs = sPlayer->buffs;
	auto player = &(sPlayer->p);
	for (int i = 0; i < 100; i++){
		if (buffs[i] != NULL){
			auto action = buffs[i]->start(player);
			if (buffs[i]->holdTime <= 0){
				delete buffs[i];
				buffs[i] = NULL;
			}
			if (action != NULL){
				layer->runAction(action);
			}
		}
	}
}

void MapTest::start(float f)
{
	winL();

	this->removeChildByName("change");

	//获取当前能量值，如果与实际不同则修改为实际值
	auto powerNumber = (TextAtlas*)this->getChildByName("power")->getChildByName("number");
	if (powerNumber->getString() != nToS(Skill::power)){
		powerNumber->setString(nToS(Skill::power));
	}

	auto node = CSLoader::createNode("p_change.csb");
	if (gameFlag == READY){
		node->setPosition(ccp(-100, -100));
		this->removeChildByName("touxiang");
		this->removeChildByName("nowxuetiao");//删除血条 头像
	}
	if (gameFlag != 0){
		if (nowNum < 4){
			auto tx = CSLoader::createNode("all_player.csb")->getChildByName(allPlayer[nowNum].p.name);
			auto txe = (Text*)CSLoader::createNode("xuetiao.csb")->getChildByTag(2);
			//添加头像
			if (this->getChildByName("touxiang") == 0){
				this->addChild(tx);
				this->addChild(txe);
				tx->setPosition(ccp(750, 260));
				tx->setName("touxiang");
				txe->setPosition(ccp(825, 240));
				txe->setName("nowxuetiao");
				txe->setText(nToS(allPlayer[nowNum].p.hp) + "/" + nToS(allPlayer[nowNum].p.maxHp));
			}
			else {
				txe = (Text*)this->getChildByName("nowxuetiao");
				txe->setText(nToS(allPlayer[nowNum].p.hp) + "/" + nToS(allPlayer[nowNum].p.maxHp));
			}
		}
		node->setPosition(ccp(allPlayer[nowNum].p.x, allPlayer[nowNum].p.y));//设置指示器
	}

	node->setName("change");
	addChild(node, 100);
	//显示血量增减动画
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].p.hp != oldPlayer[i].p.hp){
			changeHp(allPlayer[i].p.hp - oldPlayer[i].p.hp, allPlayer[i].p.x, allPlayer[i].p.y);
		}

		
	}

	//保存当前状态
	for (int i = 0; i < 20; i++){
		oldPlayer[i] = allPlayer[i];
	}
	//实时更新血量信息
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].a){
			auto xt1 = this->getChildByTag(allPlayer[i].p.tag + 1000);
			auto xuet = (LoadingBar*)xt1->getChildByName("LoadingBar_1");
			xuet->setPercent(float(allPlayer[i].p.hp) / allPlayer[i].p.maxHp * 100);
			if (allPlayer[i].p.y == 0)xt1->setPosition(ccp(allPlayer[i].p.x, allPlayer[i].p.y));
			else xt1->setPosition(ccp(allPlayer[i].p.x, allPlayer[i].p.y));
			if (allPlayer[i].p.hp <= 0){//血量归0之后
				oldPlayer[i].a = 0;
				allPlayer[i].a = 0;
				this->removeChildByTag(allPlayer[i].p.tag);
				this->removeChildByTag(allPlayer[i].p.tag + 10000);
				winTag[allPlayer[i].p.wx][allPlayer[i].p.wy] = 0;
			}
		}
	}
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].a){
			winTag[allPlayer[i].p.wx][allPlayer[i].p.wy] = allPlayer[i].p.tag;
		}
	}

	if (gameFlag == READY){
		for (int i = 0; i < 20; i++){
			if (allPlayer[i].a)
			if (allPlayer[i].f >= 100){
				this->removeChildByName("loadingBarLayer");
				for (int j = 0; j < 20; j++)
					this->removeChildByName("flag" + nToS(j));
				gameFlag = RUN;
				allPlayer[i].f = 0;
				nowNum = i;
				this->startTime(this, (allPlayer+nowNum));
				//buff();//进行一次buff检测
				break;
			}
		}
		if (gameFlag == READY)
		for (int i = 0; i < 20; i++){//人物进度增加，添加进度动画
			if (allPlayer[i].a){
				allPlayer[i].f += allPlayer[i].p.sp;
				if (this->getChildByName("loadingBarLayer") == NULL){
					auto loadingBarLayer = CSLoader::createNode("loadingBarLayer.csb");
					this->addChild(loadingBarLayer, 1000);
					loadingBarLayer->setName("loadingBarLayer");
				}
				loadingBar(this->getChildByName("loadingBarLayer")->getChildByName("loadingBar"));
			}
		}
	}
	if (nowNum < 4){
		if (gameFlag == RUN){
			gameFlag = -1;
			if (nowNum < 4){
				unStartButton();
				startButton();
			}
			else AIstartButton();
		}
		if (gameFlag == ATK){
			gameFlag = -1;
			if (nowNum < 4)
				ddMove(allPlayer[nowNum].p.x,
				allPlayer[nowNum].p.y, allPlayer[nowNum].p.mov);
			else {
				AIddMove(allPlayer[nowNum].p.x,
					allPlayer[nowNum].p.y, allPlayer[nowNum].p.mov);
				AImove();
			}
		}
		if (gameFlag == MOVE){
			gameFlag = -1;
			if (nowNum < 4)attMove(allPlayer[nowNum].p.x, allPlayer[nowNum].p.y, allPlayer[nowNum].p.atMov);
		}
		if (gameFlag == SKILL){
			gameFlag = -1;
			if (nowNum < 3){
				//显示玩家技能按钮
				auto player = allPlayer[nowNum].p;
				//初始化技能数组
				Skill::initNowSkills();
				//加入一个空的图层放入按钮
				this->removeChildByName("VoidLayer");
				auto buttonLayer = CSLoader::createNode("VoidLayer.csb");
				buttonLayer->setName("VoidLayer");
				this->addChild(buttonLayer, 200);
				for (int i = 0; i < 5; i++){
					auto skill = Skill::getSkill((SkillNameEnum)player.skillNames[i]);
					if (skill != NULL){
						Skill::nowSkills[i] = skill;
						auto skillButton = (Button*)CSLoader::createNode("skillButtonsLayer.csb")->getChildByName(skill->csbFileName);
						skillButton->setPosition(Vec2(750, 190 - ((i % 5) + 1) * 30));
						skillButton->setName(skill->csbFileName);
						skillButton->addTouchEventListener(CC_CALLBACK_2(MapTest::nSkill, this));
						if (skill->skillconsume > Skill::power){
							//当前能量低于技能消耗时，技能按钮设为不可用
							skillButton->setTouchEnabled(false);
						}
						buttonLayer->addChild(skillButton);
					}
				}
			}
			else if (nowNum == 3){
				auto g1 = (Button*)CSLoader::createNode("all_jineng.csb")->getChildByName("400");
				g1->setPosition(ccp(750, 160));
				g1->setName("400");
				g1->addTouchEventListener(CC_CALLBACK_2(MapTest::nSkill, this));
				addChild(g1, 100);
				if (guanghuan == 0)g1->setEnabled(false);
				auto g2 = (Button*)CSLoader::createNode("all_jineng.csb")->getChildByName("401");
				g2->setPosition(ccp(750, 130));
				g2->setName("401");
				g2->addTouchEventListener(CC_CALLBACK_2(MapTest::nSkill, this));
				addChild(g2, 100);
				if (guanghuan == 1)g2->setEnabled(false);
			}
		}
	}
	else {
		if (gameFlag == RUN){
			gameFlag = -1;
			AIddMove(allPlayer[nowNum].p.x, allPlayer[nowNum].p.y, allPlayer[nowNum].p.mov);
			AImove();
		}
		if (gameFlag == ATK){
			gameFlag = -1;
			AIatt();
		}
		if (gameFlag == MOVE){
			gameFlag = -1;
			auto mov = CCSequence::create(CCMoveBy::create(1.0f, ccp(0, 0)), CCCallFunc::create(this, callfunc_selector(MapTest::AIzhongzhi)), NULL, NULL);
			this->getChildByTag(allPlayer[nowNum].p.tag)->runAction(mov);
		}
	}
}
void MapTest::setMap()
{
	nowPlayers = 0;
	guanghuan = -1;
	if (Myinyue == 1)SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music001.mp3", true);//加载音乐文件,true表示循环播放
	//SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(Myinliang);
	srand(time(NULL));
	Node*diban;
	Vec2 vpos;
	Node *nod;
	Player test;
	if (gameTime == 0){
		
		gameLoad();


		/**
		*	设置坐标
		*/

		string filename = "config/setting.json";

		rapidjson::Document doc;
		//读取文件数据，初始化doc  
		std::string data = FileUtils::getInstance()->getStringFromFile(filename);

		doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

		for (unsigned int i = 0; i < doc.Size(); i++)

		{

			//逐个提取数组元素（声明的变量必须为引用）  

			rapidjson::Value &v = doc[i];
			allPlayer[v["tag"].GetInt()].p.setPos(v["x"].GetInt(), v["y"].GetInt());

		}
		data.clear();
		/*设置地图*/
		rapidjson::Document doc1;
		std::string data1 = FileUtils::getInstance()->getStringFromFile("config/setting_map.json");

		doc1.Parse<rapidjson::kParseDefaultFlags>(data1.c_str());

		for (unsigned int i = 0; i < doc1.Size(); i++)

		{

			//逐个提取数组元素（声明的变量必须为引用）  

			rapidjson::Value &v = doc1[i];
			diban = CSLoader::createNode(v["mapFile"].GetString());
			this->addChild(diban, 0);
		}
		data1.clear();

		//diban = CSLoader::createNode("MapTest2.csb");
		//diban = CSLoader::createNode("Map02.csb");
		//this->addChild(diban, 0);
		/*allPlayer[0].p.setPos(2, 2);
		allPlayer[1].p.setPos(2, 3);
		allPlayer[2].p.setPos(4, 2);
		allPlayer[3].p.setPos(3, 2);*/
		allPlayer[4].a = 1;
		allPlayer[4].p = liaoji0(20, 15, 5, 120001);
		allPlayer[5].a = 1;
		allPlayer[5].p = liaoji1(20, 15, 15, 120002);
		allPlayer[6].a = 1;
		allPlayer[6].p = liaoji2(20, 5, 15, 120003);
		allPlayer[7].a = 1;
		allPlayer[7].p = boss2(20, 10, 10, 120000);
		allPlayer[8].a = 1;
		allPlayer[8].p = d1(20, 5, 5, 120007);
		allPlayer[9].a = 1;
		allPlayer[9].p = d1(20, 5, 6, 120006);
		allPlayer[10].a = 1;
		allPlayer[10].p = d2(20, 6, 5, 120005);
		allPlayer[11].a = 1;
		allPlayer[11].p = d2(20, 6, 6, 120004);
	}
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].a == 1){
			auto t = CSLoader::createNode(allPlayer[i].p.pictureName);
			t->setPosition(ccp(allPlayer[i].p.x, allPlayer[i].p.y));
			addChild(t, 2, allPlayer[i].p.tag);
		}
	}
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].a){
			CCNode* xt;
			if (i<4)xt = CSLoader::createNode("xuetiao2.csb");
			else xt = CSLoader::createNode("xuetiao3.csb");
			xt->setPosition(ccp(allPlayer[i].p.x, allPlayer[i].p.y));
			addChild(xt, 100, allPlayer[i].p.tag + 1000);
		}
	}
	for (int i = 2; i < 10000; i++){
		if (nod = diban->getChildByTag(i)){
			vpos = nod->getPosition();
			winTag[int(vpos.x + 15) / 30][int(vpos.y + 15) / 30] = 2;
		}
	}
	for (int i = 0; i < 100; i++)
	for (int j = 0; j < 100; j++)
		testMap[i][j] = winTag[i][j];
	for (int i = 0; i < 20; i++){
		if (allPlayer[i].a){
			winTag[allPlayer[i].p.wx][allPlayer[i].p.wy] = allPlayer[i].p.tag;
		}
	}
}
void MapTest::backMenu(Ref* pSender, Widget::TouchEventType type)
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
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


void MapTest::move(Ref* pSender, Widget::TouchEventType type)
{
	CCNode* node = dynamic_cast<CCNode*>(pSender);
	//auto pos = CCNode::getChildByTag(node->getTag() - 600);//获取按钮Tag 由于按钮tag与节点不同，所以设定节点tag为按钮tag-600
	auto t = this->getChildByTag(allPlayer[nowNum].p.tag);
	moveX = t->getPositionX() / 30;
	moveY = t->getPositionY() / 30;
	targetX = node->getPositionX() / 30;
	targetY = node->getPositionY() / 30;
	//SimpleAudioEngine::sharedEngine()->playEffect("M怕.mp3",true);   //播放 
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{

										  DFS();
										  CCSequence* mov = CCSequence::create(CCMoveTo::create(0.0, t->getPosition()), NULL, NULL, NULL);
										  int ox = t->getPositionX() / 30;
										  int oy = t->getPositionY() / 30;
										  for (int i = 0; i < 10; i++){
											  if (pxy[i].num == 1){
												  if ((int)(pxy[i].x / 30) - ox > 0){
													  mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movRight,this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
												  }
												  if ((int)(pxy[i].x / 30) - ox < 0){
													  mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movLeft, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
												  }
												  if ((int)(pxy[i].y / 30) - oy > 0){
													  mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movUp, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
												  }
												  if ((int)(pxy[i].y / 30) - oy < 0){
													  mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movDown, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
												  }
												  
											  }
											 
										  }
										  
										  t->runAction(CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movOver, this)), NULL, NULL));
										  
										  this->getChildByTag(allPlayer[nowNum].p.tag)->setPosition(node->getPosition());
										 allPlayer[nowNum].p.setPos(int(node->getPositionX() / 30),
											  int(node->getPositionY() / 30));
										  MapTest::deMove();
										  gameFlag = RUN;
										  allPlayer[nowNum].p.yidong = 0;
										  //确认地图上人物位置
										  initDFS();
	}break;
	default:break;
	}
}

CCAction *cca = NULL;

void MapTest::movUp()
{
	if (nowMoveTag != 1){
		nowMoveTag = 1;
		auto action = CSLoader::createTimeline(allPlayer[nowNum].p.pictureName);
		action->gotoFrameAndPlay(120, 150, false);
		action->setTimeSpeed(0.2f);
		this->getChildByTag(allPlayer[nowNum].p.tag)->runAction(action);
	}
}
void MapTest::movDown()
{
	if (nowMoveTag != 2){
		nowMoveTag = 2;
		auto action = CSLoader::createTimeline(allPlayer[nowNum].p.pictureName);
		action->gotoFrameAndPlay(0, 30,false);

		action->setTimeSpeed(0.2f);
		this->getChildByTag(allPlayer[nowNum].p.tag)->runAction(action);
	}
}
void MapTest::movRight()
{
	if (nowMoveTag != 3){
		nowMoveTag = 3;
	auto action = CSLoader::createTimeline(allPlayer[nowNum].p.pictureName);
	action->gotoFrameAndPlay(80, 110,false);

	action->setTimeSpeed(0.2f);
	this->getChildByTag(allPlayer[nowNum].p.tag)->runAction(action);
	}
}
void MapTest::movLeft()
{
	if (nowMoveTag != 4){
		nowMoveTag = 4;
		auto action = CSLoader::createTimeline(allPlayer[nowNum].p.pictureName);
		action->gotoFrameAndPlay(40, 70,false);

		action->setTimeSpeed(0.2f);
		this->getChildByTag(allPlayer[nowNum].p.tag)->runAction(action);
	}
}
void MapTest::movOver()
{
	nowMoveTag = 0;
}


void MapTest::deMove()
{
	for (int i = 102; i < 10000; i++)
		this->removeChildByName("move" + nToS(i));
	for (int i = 0; i < 100; i++)
	for (int j = 0; j < 100; j++)
	if (winTag[i][j] != 2)winTag[i][j] = 0;

}

void MapTest::ddMove(float x, float y, int n)
{

	if (winTag[int(x / 30)][int(y / 30)] == 0 || winTag[int(x / 30)][int(y / 30)] == 1
		|| winTag[int(x / 30)][int(y / 30)] == allPlayer[nowNum].p.tag){
		if (winTag[int(x / 30)][int(y / 30)] == 0 && x < 720 && y < 540 && x >= 0 && y >= 0){
			auto tMoveButton = CSLoader::createNode("MoveButton_test1.csb");
			/*tMoveButton->setPosition(ccp(x, y));
			addChild(tMoveButton, 1, 102 + int(y / 30 * 32) + int(x / 30));*/
			Button* moveButton = dynamic_cast<Button*>(tMoveButton->getChildByName("Button_MoveTest1"));
			moveButton->setName("move" + nToS((102 + int(y / 30 * 32) + int(x / 30) + 600)));
			moveButton->addTouchEventListener(CC_CALLBACK_2(MapTest::move, this));
			moveButton->setPosition(ccp(x, y));
			addChild(moveButton, 1);
			winTag[int(x / 30)][int(y / 30)] = 1;
		}
		int m = n - 1;
		if (m >= 0){
			MapTest::ddMove(x + 30, y, m);
			MapTest::ddMove(x, y + 30, m);
			MapTest::ddMove(x - 30, y, m);
			MapTest::ddMove(x, y - 30, m);
		}

	}


}


void test()//测试是否已达到目标
{
	if (moveX == targetX && moveY == targetY)
	{
		levelComplete = allComplete = 1;
	}
}

int actOK()//判断移动方向是否合理
{
	int nextX = moveX + ddx[Act[level] - 1];
	int nextY = moveY + ddy[Act[level] - 1];

	if (Act[level] > maxAct)//方向是否错误
		return 0;
	if (nextX >= 960 / 30 || nextX < 0)//x坐标是否越界
		return 0;
	if (nextY >= 720 / 30 || nextY < 0)//y坐标是否越界
		return 0;
	if (table[nextX][nextY] == 1)//是否已达到过
		return 0;
	if (winTag[nextX][nextY] != 1)//是否有障碍
		return 0;
	if (level > allPlayer[nowNum].p.mov - 1)
		return 0;
	moveX = nextX;
	moveY = nextY;//移动
	table[nextX][nextY] = 1;//标记已达到
	pxy[level].num = 1;
	pxy[level].x = nextX * 30;
	pxy[level].y = nextY * 30;
	return 1;
}

void back()
{
	table[moveX][moveY] = 0;
	pxy[level].num = 0;
	moveX -= ddx[Act[level - 1] - 1];
	moveY -= ddy[Act[level - 1] - 1];//回退到原来的坐标
	Act[level] = 0;//清除方向
	--level;//回到上一层
}

void DFS()
{
	table[moveX][moveY] = 1;
	while (!allComplete)
	{
		++level;//搜索下一步
		levelComplete = 0;//这一步的搜索还未完成
		while (!levelComplete)
		{
			++Act[level];//改变移动方向
			if (abs(targetY - (nowT.getPositionY() / 30)) == 0){
				if (targetX - (nowT.getPositionX() / 30) == 1)Act[level] = 4;
				if (targetX - (nowT.getPositionX() / 30) == -1)Act[level] = 3;
			}
			else if (abs(targetX - (nowT.getPositionX() / 30)) == 0){
				if (targetY - (nowT.getPositionY() / 30) == 1)Act[level] = 2;
				if (targetY - (nowT.getPositionY() / 30) == -1)Act[level] = 1;
			}

			if (actOK())//方向合理
			{
				test();
				levelComplete = 1;//该步搜索完成
			}
			else
			{
				if (Act[level] > 4)//已经搜索完所有方向
				{
					back();//回退,到上一个分支
				}
				if (level < 0)//全部搜索完，仍然没有搜索到目标
				{
					levelComplete = allComplete = 1;//退出
				}
			}
		}
	}
}

void initDFS()
{
	for (int i = 0; i < 10; i++)
		pxy[i] = { 0, 0, 0 };
	moveX = moveY = targetX = targetY = 0;
	level = -1;
	levelComplete = 0;
	allComplete = 0;
	for (int i = 0; i < maxLevels; i++)
		Act[i] = 0;
	for (int i = 0; i < 100; i++)
	for (int j = 0; j < 100; j++)
		table[i][j] = 0;
}


void MapTest::startButton()
{
	float x = allPlayer[nowNum].p.x;
	float y = allPlayer[nowNum].p.y;
	auto b1 = CSLoader::createNode("m_zhongzhi.csb");
	auto b2 = CSLoader::createNode("m_jineng.csb");
	auto b3 = CSLoader::createNode("m_gongji.csb");
	auto b4 = CSLoader::createNode("m_yidong.csb");
	b1->setPosition(ccp(750, 100));
	b2->setPosition(ccp(750, 130));
	b3->setPosition(ccp(750, 160));
	b4->setPosition(ccp(750, 190));
	Button* b1Button = dynamic_cast<Button*>(b1->getChildByName("Button_zhongzhi"));
	b1Button->addTouchEventListener(CC_CALLBACK_2(MapTest::zhongzhi, this));
	b1Button->setName("start200005");
	Button* b2Button = dynamic_cast<Button*>(b2->getChildByName("Button_jineng"));
	b2Button->addTouchEventListener(CC_CALLBACK_2(MapTest::jineng, this));
	b2Button->setName("start200006");
	Button* b3Button = dynamic_cast<Button*>(b3->getChildByName("Button_gongji"));
	b3Button->addTouchEventListener(CC_CALLBACK_2(MapTest::gongji, this));
	b3Button->setName("start200007");
	Button* b4Button = dynamic_cast<Button*>(b4->getChildByName("Button_yidong"));
	b4Button->addTouchEventListener(CC_CALLBACK_2(MapTest::yidong, this));
	b4Button->setName("start200008");
	if (allPlayer[nowNum].p.yidong == 0){
		b4Button->setEnabled(false);
	}
	if (allPlayer[nowNum].p.gongji == 0){
		b3Button->setEnabled(false);
	}
	if (allPlayer[nowNum].p.jineng == 0){
		b2Button->setEnabled(false);
	}
	this->addChild(b1, 2000, 200001);
	b1->setName("start200001");
	this->addChild(b2, 2000, 200002);
	b2->setName("start200002");
	this->addChild(b3, 2000, 200003);
	b3->setName("start200003");
	this->addChild(b4, 2000, 200004);
	b4->setName("start200004");
}
void MapTest::unStartButton()
{
	for (int i = 200001; i < 200008; i++){
		this->removeChildByName("start" + nToS(i));
	}
	this->removeChildByName("fanhui");
}

void MapTest::AI()
{
	//优先级第一，移动

	int i = 1;
}

void MapTest::AIzhongzhi()
{
	allPlayer[nowNum].p.gongji = 1;
	allPlayer[nowNum].p.yidong = 1;
	allPlayer[nowNum].p.jineng = 1;
	buffTime();
	gameFlag = READY;
}

void MapTest::AIatt()
{
	if (allPlayer[nowNum].p.gongji == 1){
		int mf[4] = { 0, 0, 0, 0 };
		int x, y;
		for (int i = 0; i < 4; i++){
			if (allPlayer[i].a == 1 && abs(allPlayer[i].p.wx - allPlayer[nowNum].p.wx) + abs(allPlayer[i].p.wy - allPlayer[nowNum].p.wy)
				<= allPlayer[nowNum].p.atMov){
				mf[i] += 50;
				mf[i] += 100 - (allPlayer[i].p.hp * 100) / allPlayer[i].p.maxHp;
			}
		}
		if (allPlayer[nowNum].p.name == "boss2"){//boss2随机攻击目标
			for (int i = 0; i < 4; i++)if (allPlayer[i].a == 1)mf[i] = rand() % 100;
		}
		int a = 0;
		for (int i = 0; i < 4; i++){
			if (mf[a] < mf[i])a = i;
		}
		attNum1 = a;
		if (mf[0] == 0 && mf[1] == 0 && mf[2] == 0 && mf[3] == 0){
			attNum1 = -1;
			gameFlag = MOVE;
		}
		else {
			if (allPlayer[nowNum].p.name != "boss2")att();
			else boss2Att();
		}
	}
}

void MapTest::yidong(Ref* pSender, Widget::TouchEventType type){
	gameFlag = ATK;
	unStartButton();
	auto fanhui = CSLoader::createNode("fanhui.csb");
	Button* t = dynamic_cast<Button*>(fanhui->getChildByName("Button_1"));
	t->setName("fanhui");
	t->addTouchEventListener(CC_CALLBACK_2(MapTest::fanhui, this));
	t->setPosition(ccp(750, 190));
	addChild(t);
}
void MapTest::gongji(Ref* pSender, Widget::TouchEventType type){
	gameFlag = MOVE;
	unStartButton();
	auto fanhui = CSLoader::createNode("fanhui.csb");
	Button* t = dynamic_cast<Button*>(fanhui->getChildByName("Button_1"));
	t->setName("fanhui");
	t->addTouchEventListener(CC_CALLBACK_2(MapTest::fanhui, this));
	t->setPosition(ccp(750, 190));
	addChild(t);
}
void MapTest::jineng(Ref* pSender, Widget::TouchEventType type){
	gameFlag = SKILL;
	unStartButton();
	auto fanhui = CSLoader::createNode("fanhui.csb");
	Button* t = dynamic_cast<Button*>(fanhui->getChildByName("Button_1"));
	t->setName("fanhui");
	t->addTouchEventListener(CC_CALLBACK_2(MapTest::fanhui, this));
	t->setPosition(ccp(750, 190));
	addChild(t);
}
void MapTest::zhongzhi(Ref* pSender, Widget::TouchEventType type)
{
	allPlayer[nowNum].p.gongji = 1;
	allPlayer[nowNum].p.yidong = 1;
	allPlayer[nowNum].p.jineng = 1;
	buffTime();
	gameFlag = READY;
	unStartButton();
	deSkillMove();
	this->removeChildByName("VoidLayer");
	this->removeChildByName("fanhui");
}

void MapTest::sheZhi(Ref* pSender, Widget::TouchEventType type)
{
	auto sz = CSLoader::createNode("m_shezhi.csb");
	auto yl = (Slider*)sz->getChildByName("Slider_1");
	yl->setPercent(Myinliang * 100);
	yl->setTag(999991);
	yl->addEventListener(CC_CALLBACK_2(MapTest::yinliang, this));//添加滑动条回调函数
	this->addChild(sz, 9999, 999999);
	Button* over = dynamic_cast<Button*>(sz->getChildByName("Button_1"));
	over->addTouchEventListener(CC_CALLBACK_2(MapTest::overSZ, this));//为Back按钮添加功能
	Button* ok = dynamic_cast<Button*>(sz->getChildByName("Button_2"));
	ok->addTouchEventListener(CC_CALLBACK_2(MapTest::ok, this));
}

void MapTest::overSZ(Ref* pSender, Widget::TouchEventType type)
{
	removeChildByTag(999999);
}
void MapTest::ok(Ref* pSender, Widget::TouchEventType type)
{
	Myinliang = MSlider;
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(Myinliang);
}

void MapTest::yinliang(cocos2d::Ref * ref, Slider::EventType type)
{

	switch (type)
	{
	case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
	{
																  Slider* node = dynamic_cast<Slider*>(ref);
																  MSlider = node->getPercent();
	}
		break;
	default:
		break;
	}
}

void MapTest::AIstartButton()
{
	float x = allPlayer[nowNum].p.x;
	float y = allPlayer[nowNum].p.y;
	auto b1 = CSLoader::createNode("m_zhongzhi.csb");
	auto b2 = CSLoader::createNode("m_jineng.csb");
	auto b3 = CSLoader::createNode("m_gongji.csb");
	auto b4 = CSLoader::createNode("m_yidong.csb");
	if (x + 30 < 720)b1->setPosition(ccp(x + 30, y));
	else b1->setPosition(ccp(x - 60, y));
	if (y + 30 < 540)b2->setPosition(ccp(x, y + 30));
	else b2->setPosition(ccp(x, y - 60));
	if (x - 30 >= 0)b3->setPosition(ccp(x - 30, y));
	else b3->setPosition(ccp(x + 60, y));
	if (y - 30 >= 0)b4->setPosition(ccp(x, y - 30));
	else b4->setPosition(ccp(x, y + 60));
	Button* b1Button = dynamic_cast<Button*>(b1->getChildByName("Button_zhongzhi"));
	b1Button->setTag(200005);
	Button* b2Button = dynamic_cast<Button*>(b2->getChildByName("Button_jineng"));
	b2Button->setTag(200006);
	Button* b3Button = dynamic_cast<Button*>(b3->getChildByName("Button_gongji"));
	b3Button->setTag(200007);
	Button* b4Button = dynamic_cast<Button*>(b4->getChildByName("Button_yidong"));
	b4Button->setTag(200008);
	if (allPlayer[nowNum].p.yidong == 0){
		b4Button->setEnabled(false);
	}
	this->addChild(b1, 2000, 200001);
	this->addChild(b2, 2000, 200002);
	this->addChild(b3, 2000, 200003);
	this->addChild(b4, 2000, 200004);
	if (allPlayer[nowNum].p.yidong == 1)gameFlag = ATK;
}

void MapTest::AIddMove(float x, float y, int n)
{
	if (winTag[int(x / 30)][int(y / 30)] == 0 || winTag[int(x / 30)][int(y / 30)] == 1
		|| winTag[int(x / 30)][int(y / 30)] == allPlayer[nowNum].p.tag){
		if (winTag[int(x / 30)][int(y / 30)] == 0 && x < 720 && y < 540 && x >= 0 && y >= 0){
			auto tMoveButton = CSLoader::createNode("MoveButton_ai.csb");
			tMoveButton->setPosition(ccp(x, y));
			addChild(tMoveButton, 1);
			tMoveButton->setName("move" + nToS(102 + int(y / 30 * 32) + int(x / 30)));
			winTag[int(x / 30)][int(y / 30)] = 1;
		}
		int m = n - 1;
		if (m >= 0){
			MapTest::AIddMove(x + 30, y, m);
			MapTest::AIddMove(x, y + 30, m);
			MapTest::AIddMove(x - 30, y, m);
			MapTest::AIddMove(x, y - 30, m);
		}
	}
}

void MapTest::AImove()
{
	//移动优先级
	if (allPlayer[nowNum].p.yidong == 1){
		int mf[4] = { 0, 0, 0, 0 };
		Player  ap = allPlayer[nowNum].p;
		for (int i = 0; i < 4; i++){
			if (allPlayer[i].a){
				mf[i] += 50 - (float)allPlayer[i].p.hp / allPlayer[i].p.maxHp * 50;//目标每损失2%的血量，优先级点数增加1点
				mf[i] += 100 - abs(ap.wx - allPlayer[i].p.wx) - abs(ap.wy - allPlayer[i].p.wy) * 8;//与目标距离每提升1点，优先级点数减少1点
			}
		}

		int a = 0;
		for (int i = 0; i < 4; i++){
			if (mf[a] < mf[i])a = i;
		}
		int x = allPlayer[nowNum].p.wx;
		int y = allPlayer[nowNum].p.wy;
		for (int i = 0; i < 100; i++){
			for (int j = 0; j < 100; j++){
				if (winTag[i][j] == 1){
					if (abs(allPlayer[a].p.wx - x) + abs(allPlayer[a].p.wy - y) >
						abs(allPlayer[a].p.wx - i) + abs(allPlayer[a].p.wy - j)){
						x = i;
						y = j;
					}
				}
			}
		}
		if (allPlayer[nowNum].p.name == "boss2"){//boss2移动机制,远离目标
			for (int i = 0; i < 100; i++){
				for (int j = 0; j < 100; j++){
					if (winTag[i][j] == 1){
						if (abs(allPlayer[a].p.wx - x) + abs(allPlayer[a].p.wy - y) <
							abs(allPlayer[a].p.wx - i) + abs(allPlayer[a].p.wy - j)){
							x = i;
							y = j;
						}
					}
				}
			}
		}
		if (allPlayer[nowNum].p.name == "liaoji0" || allPlayer[nowNum].p.name == "liaoji1" || allPlayer[nowNum].p.name == "liaoji2"){//僚机随机移动
			int lwx[12];
			int lwy[12];
			int fl = 0;
			for (int i = 0; i < 100; i++){
				for (int j = 0; j < 100; j++){
					if (winTag[i][j] == 1){
						lwx[fl] = i;
						lwy[fl] = j;
						fl++;
					}
				}
			}
			if (fl != 0){
				fl = rand() % fl;
				x = lwx[fl]; y = lwy[fl];
			}
		}
		moveX = allPlayer[nowNum].p.wx;
		moveY = allPlayer[nowNum].p.wy;
		targetX = x;
		targetY = y;
		auto t = this->getChildByTag(allPlayer[nowNum].p.tag);
		DFS();
		CCSequence* mov = CCSequence::create(CCMoveTo::create(0.0, t->getPosition()), NULL, NULL, NULL);
		int ox = t->getPositionX() / 30;
		int oy = t->getPositionY() / 30;
		for (int i = 0; i < 10; i++){
			if (pxy[i].num == 1){
				if ((int)(pxy[i].x / 30) - ox > 0){
					mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movRight, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
				}
				if ((int)(pxy[i].x / 30) - ox < 0){
					mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movLeft, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
				}
				if ((int)(pxy[i].y / 30) - oy > 0){
					mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movUp, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
				}
				if ((int)(pxy[i].y / 30) - oy < 0){
					mov = CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movDown, this)), CCMoveTo::create(0.2f, ccp(pxy[i].x, pxy[i].y)), NULL);
				}

			}

		}

		t->runAction(CCSequence::create(mov, CallFunc::create(CC_CALLBACK_0(MapTest::movOver, this)), CallFunc::create(CC_CALLBACK_0(MapTest::deMove, this)), NULL));
		//t->runAction(mov);
		this->getChildByTag(allPlayer[nowNum].p.tag)->setPosition(ccp(x * 30, y * 30));
		allPlayer[nowNum].p.setPos(x, y);
		allPlayer[nowNum].p.yidong = 0;
		//确认地图上人物位置
		initDFS();
	}
	gameFlag = ATK;

}


void MapTest::deAtt()
{
	for (int i = 3000; i < 5000; i++)
		removeChildByTag(i);
}

void MapTest::attMove(float x, float y, int n)
{
	float i, j;
	int a = 3000;
	for (i = 0; i < 960.0; i = i + 30){
		for (j = 0; j < 540.0; j = j + 30){
			if (int(fabsf(i - x) + fabsf(j - y)) / 30 <= n && int(fabsf(i - x) + fabsf(j - y)) / 30 != 0){
				auto tMoveButton = CSLoader::createNode("AttMove_test1.csb");
				tMoveButton->setPosition(ccp(i, j));
				Button* t = dynamic_cast<Button*>(tMoveButton->getChildByName("Button_1"));
				t->setTag(a + 1000);
				t->addTouchEventListener(CC_CALLBACK_2(MapTest::attNum, this));
				addChild(tMoveButton, 5, a++);
			}
		}
	}
}

void MapTest::attNum(Ref* pSender, Widget::TouchEventType type)
{
	CCNode* node = dynamic_cast<CCNode*>(pSender);
	auto p = this->getChildByTag(node->getTag() - 1000)->getPosition();
	int i;
	if (attNum1 == -2){ attNum1 = -1; }
	else for (i = 0; i < 20; i++){
		if (allPlayer[i].a == 1 && i != nowNum
			&& allPlayer[i].p.x == p.x && allPlayer[i].p.y == p.y
			&& int(fabsf(allPlayer[i].p.x - allPlayer[nowNum].p.x) + fabsf(allPlayer[i].p.y - allPlayer[nowNum].p.y)) / 30 <= allPlayer[nowNum].p.atMov){
			if ((nowNum < 4 && i >= 4) || (nowNum >= 4 && i < 4)){
				attNum1 = i;
				att();
			}
			break;
		}
	}
}

int MapTest::att()
{
	auto aNode = CSLoader::createNode("m_gjtexiao.csb");
	this->addChild(aNode, 99);
	aNode->setPosition(ccp(allPlayer[nowNum].p.x, allPlayer[nowNum].p.y));
	aNode->setName("texiao");
	float sf = 0;
	if (nowNum >= 4)sf = 1;
	auto mov = CCSequence::create(CCMoveBy::create(sf, ccp(0, 0)), CCMoveTo::create(0.2f, ccp(allPlayer[attNum1].p.x, allPlayer[attNum1].p.y)),
		CCCallFunc::create(this, callfunc_selector(MapTest::delTx)), NULL);
	aNode->runAction(mov);
	float df = float(allPlayer[attNum1].p.def) / (allPlayer[attNum1].p.def + 10);//防御力减少的承受伤害
	allPlayer[attNum1].p.hp = allPlayer[attNum1].p.hp - (allPlayer[nowNum].p.att * allPlayer[attNum1].p.sDef);
	deAtt();
	if (nowNum < 4)gameFlag = RUN;
	else gameFlag = MOVE;
	allPlayer[nowNum].p.gongji = allPlayer[nowNum].p.jineng = 0;
	attNum1 = -2;
	return 1;
}
int MapTest::boss2Att()
{
	auto aNode = CSLoader::createNode("boss2_gjtexiao.csb");
	this->addChild(aNode, 99);
	aNode->setPosition(ccp(allPlayer[nowNum].p.x, allPlayer[nowNum].p.y));
	aNode->setName("texiao");
	float sf = 0;
	if (nowNum >= 4)sf = 1;
	Sequence* mov = CCSequence::create(CCMoveBy::create(sf, ccp(0, 0)), NULL, NULL, NULL);
	for (int i = 0; i < 4; i++){
		if (allPlayer[i].a == 1){
			mov = CCSequence::create(mov, CCMoveTo::create(0.1f, ccp(allPlayer[i].p.x, allPlayer[i].p.y)),
				NULL, NULL);
			allPlayer[i].p.hp = allPlayer[i].p.hp - allPlayer[nowNum].p.att * allPlayer[i].p.sDef;
		}
	}
	mov = CCSequence::create(mov, CCCallFunc::create(this, callfunc_selector(MapTest::delTx)), NULL, NULL);
	aNode->runAction(mov);
	deAtt();
	if (nowNum < 4)gameFlag = RUN;
	else gameFlag = MOVE;
	allPlayer[nowNum].p.gongji = allPlayer[nowNum].p.jineng = 0;
	attNum1 = -2;
	return 1;
}

void MapTest::delTx()
{
	this->removeChildByName("texiao");
}

void MapTest::fanhui(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  deAtt();
										  deMove();
										  deSkillMove();
										  this->removeChildByName("VoidLayer");
										  this->removeChildByName("fanhui");
										  gameFlag = RUN;
	}break;
	default:break;
	}
}

void MapTest::skillMove(string kn)
{
	string csbFile = "skill_" + kn + ".csb";
	auto a = CSLoader::createNode(csbFile);
	float x, y;
	//获取csb文件中的所有按钮
	auto buttons = (Vector<Button*>&)a->getChildren();
	//加入一个空图层
	this->removeChildByName("SkillLayer");
	auto layer = CSLoader::createNode("VoidLayer.csb");
	layer->setName("SkillLayer");
	this->addChild(layer, 100);
	//将技能范围的按钮加入空图层中，并为每一个按钮设置回调函数
	for (auto button : buttons){
		x = button->getPositionX();
		y = button->getPositionY();
		button->setPosition(Vec2(allPlayer[nowNum].p.x + x, allPlayer[nowNum].p.y + y));
		button->addTouchEventListener(CC_CALLBACK_2(MapTest::skill, this));
		button->setName(kn);
		layer->addChild(button);
	}
	//for (int i = 0; i < 100; i++){
	//	if (a->getChildByTag(i)){
	//		x = a->getChildByTag(i)->getPositionX();
	//		y = a->getChildByTag(i)->getPositionY();
	//		Button* t = dynamic_cast<Button*>(a->getChildByTag(i));
	//		addChild(t, 99);
	//		t->setPosition(ccp(allPlayer[nowNum].p.x + x, allPlayer[nowNum].p.y + y));
	//		t->addTouchEventListener(CC_CALLBACK_2(MapTest::skill, this));
	//		t->setName("skill" + nToS(i));
	//	}
	//}
}

void MapTest::deSkillMove()
{
	//删除技能范围图层
	this->removeChildByName("SkillLayer");
}

void MapTest::skill(Ref* pSender, Widget::TouchEventType type)
{
	CCNode* a = (CCNode*)pSender;
	skillOver = 0;
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  int x, y;
										  x = a->getPositionX() / 30;
										  y = a->getPositionY() / 30;
										  if (winTag[x][y] > 0){
											  for (int j = 0; j < 20; j++){
												  //判断点击位置是否存在一个角色
												  if (allPlayer[j].p.wx == x && allPlayer[j].p.wy == y){
													  Skill* skill;
													  for (int i = 0; i < 5; i++){
														  auto skills = Skill::nowSkills;
														  if (skills[i]->csbFileName == a->getName()){
															  skill = skills[i];
															  break;
														  }
													  }
													  if (skill->skillType == SkillTypeEnum::MON){
														  ssp *players[] = { &(allPlayer[nowNum]), &(allPlayer[j]) };
														  auto action = skill->skillResult(players);
														  if (action != NULL){
															  this->runAction(action);
														  }
														  if (allPlayer[nowNum].p.jineng == 0){
															  deSkillMove();
															  gameFlag = RUN;
														  }
													  }
												  }
											  }
											  
											  
										  }
										  //触发技能
	}break;
	default:break;
	}
}



//点击技能按钮回调函数
void MapTest::nSkill(Ref* pSender, Widget::TouchEventType type){
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
	{
										  auto node = (Node*)pSender;
										  skillMove(node->getName());
										  //if (sToN(node->getName()) < 400){
											 // skillMove("skill" + node->getName() + ".csb");
											 // nowSkillTag = sToN(node->getName());
											 // for (int i = 0; i < 9999; i++){
												//  this->removeChildByName(nToS(i));
												//  this->removeChildByName("num" + nToS(i));
											 // }
										  //}
										  //else {
											 // if (sToN(node->getName()) == 400){//老师光环
												//  guanghuan = 0;
												//  /*添加光环图片*/
												//  this->removeChildByName("guanghuan");
												//  auto g = CSLoader::createNode("guanghuan0.csb");
												//  g->setPosition(ccp(770, 450));
												//  addChild(g, 1);
												//  g->setName("guanghuan");
												//  /*添加光环图片*/
												//  for (int i = 0; i < 100; i++){
												//	  if (allPlayer[3].p.buff[i][1] > 100){
												//		  allPlayer[3].p.buff[i][1] = allPlayer[3].p.buff[i][0] = 0;
												//	  }
												//  }
												//  allPlayer[3].p.setBuff(2, 999);
											 // }
											 // if (sToN(node->getName()) == 401){
												//  guanghuan = 1;
												//  /*添加光环图片*/
												//  this->removeChildByName("guanghuan");
												//  auto g = CSLoader::createNode("guanghuan1.csb");
												//  g->setPosition(ccp(770, 450));
												//  addChild(g, 1);
												//  g->setName("guanghuan");
												//  /*添加光环图片*/
												//  for (int i = 0; i < 100; i++){
												//	  if (allPlayer[3].p.buff[i][1] > 100){
												//		  allPlayer[3].p.buff[i][1] = allPlayer[3].p.buff[i][0] = 0;
												//	  }
												//  }
												//  allPlayer[3].p.setBuff(4, 999);
											 // }
											 // deAtt();
											 // deMove();
											 // deSkillMove();
											 // for (int i = 0; i < 9999; i++){
												//  this->removeChildByName(nToS(i));
												//  this->removeChildByName("num" + nToS(i));
											 // }
											 // this->removeChildByName("fanhui");
											 // gameFlag = RUN;
										  //}

	}break;
	default:break;
	}
}

void MapTest::changeHp(int i, float x, float y)
{
	CCSequence* mov;
	if (i < 0){
		auto a = (Text*)CSLoader::createNode("m_cghp.csb")->getChildByName("Text_1");
		a->setText(nToS(i));
		a->setPosition(ccp(x, y));
		a->setName("cghp");
		addChild(a, 9999999);
		mov = CCSequence::create(CCMoveTo::create(2.0f, ccp(x, y + 30)),
			CCCallFunc::create(this, callfunc_selector(MapTest::deCgHp)), NULL, NULL);
		a->runAction(mov);
	}
	if (i > 0){
		auto a = (Text*)CSLoader::createNode("m_cghp2.csb")->getChildByName("Text_1");
		a->setText("+" + nToS(i));
		a->setPosition(ccp(x, y));
		a->setName("cghp");
		addChild(a, 9999999);
		mov = CCSequence::create(CCMoveTo::create(2.0f, ccp(x, y + 30)),
			CCCallFunc::create(this, callfunc_selector(MapTest::deCgHp)), NULL, NULL);
		a->runAction(mov);
	}
}
void MapTest::deCgHp()
{
	this->removeChildByName("cghp");
}

//void MapTest::buff()
//{
//	for (int i = 0; i < 20; i++){//遍历人物
//		if (allPlayer[i].a){
//			for (int j = 0; j < 100; j++){
//				if (allPlayer[i].p.buff[j][1] > 0){//遍历人物buff列表
//					switch (j){//buff实现功能
//					case 0:{//编号0，眩晕	
//							   allPlayer[i].yidong = allPlayer[i].gongji = allPlayer[i].jineng = 0;
//					}break;
//					case 1:{//编号1，锁足
//							   allPlayer[i].yidong = 0;
//					}break;
//					case 2:{//编号2，减速光环，全体敌人移动距离-1
//							   if (allPlayer[i].p.name == "player3" && allPlayer[i].a == 1){
//								   for (int lj = 4; lj < 20; lj++){
//									   if (allPlayer[lj].p.buff[3][1] <= 0){
//										   allPlayer[lj].p.setBuff(3, 1);
//									   }
//								   }
//							   }
//					}
//					case 3:{//编号3，减速光环附加debuff
//							   if (allPlayer[i].p.buff[j][0] == 1){
//								   allPlayer[i].p.setMov(allPlayer[i].p.mov - 1);
//								   allPlayer[i].p.buff[j][0] = 0;
//							   }
//					}break;
//					case 4:{//编号4，治疗光环，全员行动时，按教师当前生命恢复血量
//							   if (allPlayer[i].p.name == "player3" && allPlayer[i].a == 1){
//								   for (int lj = 0; lj < 4; lj++){
//									   if (allPlayer[lj].p.buff[5][1] <= 0){
//										   allPlayer[lj].p.setBuff(5, 1);
//									   }
//								   }
//							   }
//					}
//					case 5:{//编号5，治疗光环附加buff
//							   if (allPlayer[i].p.buff[j][0] == 1){
//								   if (i == nowNum){
//									   allPlayer[i].p.hp += allPlayer[3].p.hp * 0.05;
//									   if (allPlayer[i].p.hp > allPlayer[i].p.maxHp) allPlayer[i].p.hp = allPlayer[i].p.maxHp;
//									   allPlayer[i].p.buff[j][0] = 0;
//								   }
//							   }
//					}break;
//					case 52:{//编号52，相位移动，通过检测僚机位置，将自己坐标随机设置在僚机上，再进行移动
//								if (i == nowNum){//进入自己的回合
//									if (allPlayer[i].p.buff[j][0] == 1){//状态标志未归0
//										float lx[3] = { -1.0, -1.0, -1.0 };
//										float ly[3] = { -1.0, -1.0, -1.0 };
//										int lwx, lwy;
//										for (int li = 0; li < 20; li++){
//											for (int lj = 0; lj < 3; lj++){
//												if (allPlayer[li].p.name == "liaoji" + nToS(lj) && allPlayer[li].a != 0){
//													lx[lj] = allPlayer[li].p.x;
//													ly[lj] = allPlayer[li].p.y;
//												}
//											}
//										}
//										int a = rand() % 3;
//										if (lx[a] != -1.0 && ly[a] != -1.0){
//											lwx = lx[a] / 30;
//											lwy = ly[a] / 30;
//											if (winTag[lwx + 1][lwy] == 0)lwx += 1;
//											else if (winTag[lwx - 1][lwy] == 0)lwx -= 1;
//											else if (winTag[lwx][lwy + 1] == 0)lwy += 1;
//											else if (winTag[lwx][lwy - 1] == 0)lwy -= 1;
//											else {
//												lwx = allPlayer[i].p.wx;
//												lwy = allPlayer[i].p.wy;
//											}
//											if (lwx > 23 || lwx < 0 || lwy > 17 || lwy < 0){
//												lwx = allPlayer[i].p.wx;
//												lwy = allPlayer[i].p.wy;
//											}
//										}
//										else {
//											lwx = allPlayer[i].p.wx;
//											lwy = allPlayer[i].p.wy;
//										}
//										//移动角色
//										//动画未添加
//										winTag[allPlayer[i].p.wx][allPlayer[i].p.wy] = 0;
//										allPlayer[i].p.setPos(lwx, lwy);
//										winTag[allPlayer[i].p.wx][allPlayer[i].p.wy] = allPlayer[i].p.tag;
//										this->getChildByTag(allPlayer[i].p.tag)->setPosition(ccp(allPlayer[i].p.x, allPlayer[i].p.y));
//									}
//								}
//					}break;
//					case 53:{//编号53，冰冻光环，僚机0周围2单位距离移动距离减少1
//								if (allPlayer[i].p.name == "liaoji0" && allPlayer[i].a == 1){
//									for (int lj = 0; lj < 4; lj++){
//										if (abs(allPlayer[lj].p.wx - allPlayer[i].p.wx) + abs(allPlayer[lj].p.wy - allPlayer[i].p.wy) <= 2
//											&& allPlayer[lj].p.buff[54][1] <= 0){
//											allPlayer[lj].p.setBuff(54, 1);
//										}
//									}
//								}
//					}break;
//					case 54:{//编号54，冰冻光环附加debuff
//								if (allPlayer[i].p.buff[j][0] == 1){
//									allPlayer[i].p.setMov(allPlayer[i].p.mov - 1);
//									allPlayer[i].p.buff[j][0] = 0;
//								}
//					}break;
//					case 55:{//编号55，灼烧光环，僚机1周围2单位距离内所有敌人每回合损失5%当前生命
//								if (allPlayer[i].p.name == "liaoji1" && allPlayer[i].a == 1){
//									for (int lj = 0; lj < 4; lj++){
//										if (abs(allPlayer[lj].p.wx - allPlayer[i].p.wx) + abs(allPlayer[lj].p.wy - allPlayer[i].p.wy) <= 2
//											&& allPlayer[lj].p.buff[56][1] <= 0){
//											allPlayer[lj].p.setBuff(56, 1);
//										}
//									}
//								}
//					}break;
//					case 56:{//编号56，灼烧光环附加debuff
//								if (i == nowNum)allPlayer[i].p.hp = allPlayer[i].p.hp - (allPlayer[i].p.hp * 0.05) * allPlayer[i].p.sDef;
//					}break;
//					case 57:{//编号53，腐蚀光环，僚机2周围2单位距离减少3防御力
//								if (allPlayer[i].p.name == "liaoji2" && allPlayer[i].a == 1){
//									for (int lj = 0; lj < 4; lj++){
//										if (abs(allPlayer[lj].p.wx - allPlayer[i].p.wx) + abs(allPlayer[lj].p.wy - allPlayer[i].p.wy) <= 2
//											&& allPlayer[lj].p.buff[58][1] <= 0){
//											allPlayer[lj].p.setBuff(58, 1);
//										}
//									}
//								}
//					}break;
//					case 58:{//编号58，腐蚀光环附加debuff
//								if (allPlayer[i].p.buff[j][0] == 1){
//									allPlayer[i].p.setDef(allPlayer[i].p.def - 3);
//									allPlayer[i].p.buff[j][0] = 0;
//								}
//					}break;
//					}
//
//				}
//			}
//		}
//	}
//}

void MapTest::buffTime()
{
	for (int i = 0; i < 100; i++){
		if (allPlayer[nowNum].p.buff[i][1] > 0){
			allPlayer[nowNum].p.buff[i][1] -= 1;
			switch (i)
			{
			case 3:{
					   if (allPlayer[nowNum].p.buff[i][1] == 0)allPlayer[nowNum].p.setMov(allPlayer[nowNum].p.mov + 1);
			}break;
			case 52:{
						allPlayer[nowNum].p.buff[i][0] = 1;
			}break;
			case 54:{
						if (allPlayer[nowNum].p.buff[i][1] == 0)allPlayer[nowNum].p.setMov(allPlayer[nowNum].p.mov + 1);//移除54号debuff影响
			}break;
			case 58:{
						if (allPlayer[nowNum].p.buff[i][1] == 0)allPlayer[nowNum].p.setDef(allPlayer[nowNum].p.def + 3);
			}break;
			default:
				break;
			}
		}
	}
}

void MapTest::winL()
{
	switch (gameTime)
	{
	case 0:{
			   int l = 0;
			   int w = 0;
			   for (int i = 0; i < 4; i++){
				   if (allPlayer[i].p.name == "player3" &&allPlayer[i].a == 0){
					   l = 1;
					   break;
				   }
			   }
			   for (int i = 0; i < 20; i++){
				   if (allPlayer[i].p.name == "boss2" && allPlayer[i].a == 0){
					   w = 1;
					   break;
				   }
			   }
			   if (l == 1){
				   SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
				   auto scene = LOSE::createScene();
				   CCDirector::sharedDirector()->replaceScene(scene);
			   }
			   else if (w == 1){
				   SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
				   auto scene = WIN::createScene();
				   CCDirector::sharedDirector()->replaceScene(scene);
			   }
	}break;
	default:
		break;
	}
}

void MapTest::loadingBar(cocos2d::CCNode* node){
	for (int i = 0; i < 20; i++){
		node->removeChildByName("tempNode" + i);
		auto player = allPlayer[i];
		if (player.a == 1){
			auto tempNode = CSLoader::createNode(player.p.pictureName);
			tempNode->setName("tempNode" + i);
			tempNode->setPosition(Vec2(400.0f * ((float)player.f/100.0f),5.0f));
			node->addChild(tempNode);
		}
	}
}





