#ifndef _MAPTEST_H__
#define _MAPTEST_H__

#include "Buff.h"
#include <vector>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Game.h"
#include "Role.h"



using namespace cocostudio::timeline;
using namespace cocos2d::ui;


typedef struct dxy{
	float x;
	float y;
	int num;
}Dxy;

//运行时，角色状态类型
class playerSP{
public:
	Player p;
	int a = 0;//代表该棋子是否存在
	int f = 0;
	Buff *buffs[100];
	//int yidong = 1;
	//int gongji = 1;
	//int jineng = 1;
};

typedef playerSP ssp;






class MapTest : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(MapTest);
public:
	void setMap();//设置当前地图的各项数据，主要为各个棋子的初始状态坐标以及标记障碍物
	void start(float);//开始进行战斗
	void backMenu(Ref* pSender, Widget::TouchEventType type);
	void move(Ref* pSender, Widget::TouchEventType type);//按钮功能:移动
	void AImove();
	void dMoveCallBack();//dMove的回调函数
	void deMove();//删除移动范围指示色块
	void ddMove( float x, float y, int n);//绘制移动范围
	void AIddMove(float x, float y, int n);
	void startButton();//绘制行动按钮
	void AIstartButton();
	void unStartButton();//删除行动按钮


	void AI();
	void AIzhongzhi();
	void AIatt();
	
	void sheZhi(Ref* pSender, Widget::TouchEventType type);
	void overSZ(Ref* pSender, Widget::TouchEventType type);
	void yinliang(cocos2d::Ref * ref, Slider::EventType type);
	void ok(Ref* pSender, Widget::TouchEventType type);

	void yidong(Ref* pSender, Widget::TouchEventType type);//按钮移动
	void gongji(Ref* pSender, Widget::TouchEventType type);//按钮攻击
	void jineng(Ref* pSender, Widget::TouchEventType type);//按钮技能
	void zhongzhi(Ref* pSender, Widget::TouchEventType type);//按钮终止
	void fanhui(Ref* pSender, Widget::TouchEventType type);//按钮返回

	void deAtt();//删除攻击范围指示色块
	void attMove(float x, float y, int n);//绘制攻击范围
	void attNum(Ref* pSender, Widget::TouchEventType type);//攻击回调函数
	int att();//攻击
	int	boss2Att();//boss2攻击函数

	void delTx();//删除特效，回调函数

	void buff();//人物BUFF的检测
	void buffTime();//buff时间检测以及移除

	void nSkill(Ref* pSender, Widget::TouchEventType type);
	void skillMove(string kn);
	void deSkillMove();
	void skill(Ref* pSender, Widget::TouchEventType type);
	void changeHp(int i, float x, float y);
	void deCgHp();
	void loadingBar(cocos2d::CCNode*);//绘制进度条

	//回合结束清算
	//void overTime();
	//回合开始清算
	void startTime(cocos2d::Layer* layer, ssp* sPlayer);

	void winL();//战斗结算
	void moveI();
};

//迷宫寻路函数


void test();//测试是否已经到达
int actOK();//判断移动方向是否合理
void backXY();//回退到原来坐标
void DFS();//深度优先搜索
void initDFS();//初始化DFS函数的变量



#endif //_MAPTEST_H__
