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

//����ʱ����ɫ״̬����
class playerSP{
public:
	Player p;
	int a = 0;//����������Ƿ����
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
	void setMap();//���õ�ǰ��ͼ�ĸ������ݣ���ҪΪ�������ӵĳ�ʼ״̬�����Լ�����ϰ���
	void start(float);//��ʼ����ս��
	void backMenu(Ref* pSender, Widget::TouchEventType type);
	void move(Ref* pSender, Widget::TouchEventType type);//��ť����:�ƶ�
	void AImove();
	void dMoveCallBack();//dMove�Ļص�����
	void deMove();//ɾ���ƶ���Χָʾɫ��
	void ddMove( float x, float y, int n);//�����ƶ���Χ
	void AIddMove(float x, float y, int n);
	void startButton();//�����ж���ť
	void AIstartButton();
	void unStartButton();//ɾ���ж���ť


	void AI();
	void AIzhongzhi();
	void AIatt();
	
	void sheZhi(Ref* pSender, Widget::TouchEventType type);
	void overSZ(Ref* pSender, Widget::TouchEventType type);
	void yinliang(cocos2d::Ref * ref, Slider::EventType type);
	void ok(Ref* pSender, Widget::TouchEventType type);

	void yidong(Ref* pSender, Widget::TouchEventType type);//��ť�ƶ�
	void gongji(Ref* pSender, Widget::TouchEventType type);//��ť����
	void jineng(Ref* pSender, Widget::TouchEventType type);//��ť����
	void zhongzhi(Ref* pSender, Widget::TouchEventType type);//��ť��ֹ
	void fanhui(Ref* pSender, Widget::TouchEventType type);//��ť����

	void deAtt();//ɾ��������Χָʾɫ��
	void attMove(float x, float y, int n);//���ƹ�����Χ
	void attNum(Ref* pSender, Widget::TouchEventType type);//�����ص�����
	int att();//����
	int	boss2Att();//boss2��������

	void delTx();//ɾ����Ч���ص�����

	void buff();//����BUFF�ļ��
	void buffTime();//buffʱ�����Լ��Ƴ�

	void nSkill(Ref* pSender, Widget::TouchEventType type);
	void skillMove(string kn);
	void deSkillMove();
	void skill(Ref* pSender, Widget::TouchEventType type);
	void changeHp(int i, float x, float y);
	void deCgHp();
	void loadingBar(cocos2d::CCNode*);//���ƽ�����

	//�غϽ�������
	//void overTime();
	//�غϿ�ʼ����
	void startTime(cocos2d::Layer* layer, ssp* sPlayer);

	void winL();//ս������
	void moveI();
};

//�Թ�Ѱ·����


void test();//�����Ƿ��Ѿ�����
int actOK();//�ж��ƶ������Ƿ����
void backXY();//���˵�ԭ������
void DFS();//�����������
void initDFS();//��ʼ��DFS�����ı���



#endif //_MAPTEST_H__
