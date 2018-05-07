#include "Role.h"
#include "MapTest.h"
#include "AppDelegate.h"
#include "Role.h"
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "json/document.h"
#include "json/prettywriter.h"
#include "json/filereadstream.h"
#include "json/stringbuffer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

extern int nowNum;


extern int winTag[100][100];
extern ssp allPlayer[20];
extern int skillOver;
extern int iMove[3];//前面为移动人的编号，后面为移动的方位


Player::Player()
{
}

//void Player::startTime(cocos2d::Layer* layer){
//	for (int j = 0; j < 100; j++){
//		if (this->buff[j][1] > 0){//遍历人物buff列表
//			switch (j){//buff实现功能
//			case 0:{//编号0，眩晕	
//					   this->yidong = this->gongji = this->jineng = 0;
//			}break;
//			case 1:{//编号1，锁足
//					   this->yidong = 0;
//			}break;
//			case 2:{//编号2，减速光环，全体敌人移动距离-1
//					   if (this->name == "player3"){
//						   for (int lj = 4; lj < 20; lj++){
//							   if (allPlayer[lj].p.buff[3][1] <= 0){
//								   allPlayer[lj].p.setBuff(3, 1);
//							   }
//						   }
//					   }
//			}
//			case 3:{//编号3，减速光环附加debuff
//					   if (this->buff[j][0] == 1){
//						   this->setMov(this->mov - 1);
//						   this->buff[j][0] = 0;
//					   }
//			}break;
//			case 4:{//编号4，治疗光环，全员行动时，按教师当前生命恢复血量
//					   if (this->name == "player3"){
//						   for (int lj = 0; lj < 4; lj++){
//							   if (allPlayer[lj].p.buff[5][1] <= 0){
//								   allPlayer[lj].p.setBuff(5, 1);
//							   }
//						   }
//					   }
//			}
//			case 5:{//编号5，治疗光环附加buff
//					   if (this->buff[j][0] == 1){
//						   if (true){
//							   this->hp += allPlayer[3].p.hp * 0.05;
//							   if (this->hp > this->maxHp) this->hp = this->maxHp;
//							   this->buff[j][0] = 0;
//						   }
//					   }
//			}break;
//			case 52:{//编号52，相位移动，通过检测僚机位置，将自己坐标随机设置在僚机上，再进行移动
//						if (true){//进入自己的回合
//							if (this->buff[j][0] == 1){//状态标志未归0
//								float lx[3] = { -1.0, -1.0, -1.0 };
//								float ly[3] = { -1.0, -1.0, -1.0 };
//								int lwx, lwy;
//								for (int li = 0; li < 20; li++){
//									for (int lj = 0; lj < 3; lj++){
//										if (allPlayer[li].p.name == "liaoji" + nToS(lj) && allPlayer[li].a != 0){
//											lx[lj] = allPlayer[li].p.x;
//											ly[lj] = allPlayer[li].p.y;
//										}
//									}
//								}
//								int a = rand() % 3;
//								if (lx[a] != -1.0 && ly[a] != -1.0){
//									lwx = lx[a] / 30;
//									lwy = ly[a] / 30;
//									if (winTag[lwx + 1][lwy] == 0)lwx += 1;
//									else if (winTag[lwx - 1][lwy] == 0)lwx -= 1;
//									else if (winTag[lwx][lwy + 1] == 0)lwy += 1;
//									else if (winTag[lwx][lwy - 1] == 0)lwy -= 1;
//									else {
//										lwx = this->wx;
//										lwy = this->wy;
//									}
//									if (lwx > 23 || lwx < 0 || lwy > 17 || lwy < 0){
//										lwx = this->wx;
//										lwy = this->wy;
//									}
//								}
//								else {
//									lwx = this->wx;
//									lwy = this->wy;
//								}
//								//移动角色
//								//动画未添加
//								winTag[this->wx][this->wy] = 0;
//								this->setPos(lwx, lwy);
//								winTag[this->wx][this->wy] = this->tag;
//								layer->getChildByTag(this->tag)->setPosition(ccp(this->x, this->y));
//							}
//						}
//			}break;
//			case 53:{//编号53，冰冻光环，僚机0周围2单位距离移动距离减少1
//						if (this->name == "liaoji0"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[54][1] <= 0){
//									allPlayer[lj].p.setBuff(54, 1);
//								}
//							}
//						}
//			}break;
//			case 54:{//编号54，冰冻光环附加debuff
//						if (this->buff[j][0] == 1){
//							this->setMov(this->mov - 1);
//							this->buff[j][0] = 0;
//						}
//			}break;
//			case 55:{//编号55，灼烧光环，僚机1周围2单位距离内所有敌人每回合损失5%当前生命
//						if (this->name == "liaoji1"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[56][1] <= 0){
//									allPlayer[lj].p.setBuff(56, 1);
//								}
//							}
//						}
//			}break;
//			case 56:{//编号56，灼烧光环附加debuff
//						this->hp = this->hp - (this->hp * 0.05) * this->sDef;
//			}break;
//			case 57:{//编号53，腐蚀光环，僚机2周围2单位距离减少3防御力
//						if (this->name == "liaoji2"){
//							for (int lj = 0; lj < 4; lj++){
//								if (abs(allPlayer[lj].p.wx - this->wx) + abs(allPlayer[lj].p.wy - this->wy) <= 2
//									&& allPlayer[lj].p.buff[58][1] <= 0){
//									allPlayer[lj].p.setBuff(58, 1);
//								}
//							}
//						}
//			}break;
//			case 58:{//编号58，腐蚀光环附加debuff
//						if (this->buff[j][0] == 1){
//							this->setDef(this->def - 3);
//							this->buff[j][0] = 0;
//						}
//			}break;
//			}
//
//		}
//	}
//}

Player dongsan(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("dongsan");
	p.setPictureName("rw00.csb");
	p.setHp(300 + Lv * 50);
	p.setAtt(20 + Lv * 10);
	p.setAtMov(2);
	p.setDef(5 + Lv);
	p.setSp(11);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player nanliu(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("nanliu");
	p.setPictureName("rw01.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(20 + Lv * 10);
	p.setAtMov(2);
	p.setDef(2 + Lv);
	p.setSp(15);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	return p;
}

Player xijiu(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("xijiu");
	p.setPictureName("rw02.csb");
	p.setHp(150 + Lv * 50);
	p.setAtt(30 + Lv * 10);
	p.setAtMov(3);
	p.setDef(1 + Lv);
	p.setSp(12);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player beishier(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("beishier");
	p.setPictureName("rw03.csb");
	p.setHp(300 + Lv * 50);
	p.setAtt(25 + Lv * 10);
	p.setAtMov(3);
	p.setDef(3 + Lv);
	p.setSp(13);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player jiashi(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("jiashi");
	p.setPictureName("rw04.csb");
	p.setHp(1500 + Lv * 50);
	p.setAtt(40 + Lv * 10);
	p.setAtMov(1);
	p.setDef(10 + Lv);
	p.setSp(10);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player wumianzhe(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("wumianzhe");
	p.setPictureName("rw05.csb");
	p.setHp(500 + Lv * 50);
	p.setAtt(60 + Lv * 10);
	p.setAtMov(100);
	p.setDef(1 + Lv);
	p.setSp(15);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(5);
	return p;
}

Player mowang(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("mowang");
	p.setPictureName("rw06.csb");
	p.setHp(1200 + Lv * 50);
	p.setAtt(100 + Lv * 10);
	p.setAtMov(3);
	p.setDef(6 + Lv);
	p.setSp(14);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	return p;
}

Player jinzhan(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("jinzhan");
	if (rand() % 2 != 0){
		p.setPictureName("rw07.csb");
	}
	else{
		p.setPictureName("rw08.csb");
	}
	p.setHp(200 + Lv * 50);
	p.setAtt(20 + Lv * 10);
	p.setAtMov(1);
	p.setDef(3 + Lv);
	p.setSp(10);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player yuancheng(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("yuancheng");
	if (rand() % 2 != 0){
		p.setPictureName("rw09.csb");
	}
	else{
		p.setPictureName("rw10.csb");
	}
	p.setHp(100 + Lv * 50);
	p.setAtt(30 + Lv * 10);
	p.setAtMov(3);
	p.setDef(1 + Lv);
	p.setSp(12);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player roudun(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("roudun");
	p.setPictureName("rw11.csb");
	p.setHp(800 + Lv * 50);
	p.setAtt(30 + Lv * 10);
	p.setAtMov(1);
	p.setDef(5 + Lv);
	p.setSp(10);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	return p;
}

Player ying(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("ying");
	p.setPictureName("rw12.csb");
	p.setHp(300 + Lv * 50);
	p.setAtt(40 + Lv * 10);
	p.setAtMov(2);
	p.setDef(2 + Lv);
	p.setSp(15);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(5);
	return p;
}

Player quan(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("quan");
	p.setPictureName("rw13.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(50 + Lv * 10);
	p.setAtMov(1);
	p.setDef(3 + Lv);
	p.setSp(14);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	return p;
}
Player jianke(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("jianke");
	p.setPictureName("rw14.csb");
	p.setHp(500 + Lv * 50);
	p.setAtt(80 + Lv * 10);
	p.setAtMov(2);
	p.setDef(3 + Lv);
	p.setSp(12);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	return p;
}



void newGame()
{
	ifstream iFin;
	ofstream oFin;
	string str;
	iFin.open("..\\sav.txt", ios::out);
	if (!iFin.is_open()){
		/*iFin.close();
		oFin.open("..\\sav.txt");
		str = "player0 rw00.csb 100001 1000 240 8 20 3 2 101 102 103 104 105 106 107 108 109 110 111 112 0 0 0 0 0";
		oFin << str << "\n";
		str = "player1 rw01.csb 100002 1200 200 10 18 4 1 205 207 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		str = "player2 rw02.csb 100003 950 250 6 19 2 3 309 312 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		str = "player3 rw03.csb 100004 1000 230 10 18 4 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
		oFin << str << "\n";
		oFin.close();*/
		iFin.close();
		oFin.open("..\\sav.txt");
		str = "1";
		oFin << str << "\n";
	}
}

void gameLoad()
{
	/*for (int i = 0; i < 20; i++){
		allPlayer[i].a = 0;
		for (int j = 0; j < 100; j++){
			allPlayer[i].p.buff[j][0] = allPlayer[i].p.buff[j][1] = 0;
		}
	}
	newGame();
	ifstream fin;
	fin.open("..\\sav.txt", ios::in);
	char line[1024];
	string name, pictureName;
	int hp, att, def, sp, mov, tag, atMov;
	int skill[12];
	int nSkill[5];
	while (fin.getline(line, sizeof(line))){
		stringstream ss(line);
		ss >> name;
		ss >> pictureName;
		ss >> tag;
		ss >> hp;
		ss >> att;
		ss >> def;
		ss >> sp;
		ss >> mov;
		ss >> atMov;
		for (int i = 0; i < 12; i++){
			ss >> skill[i];
		}
		for (int i = 0; i < 5; i++){
			ss >> nSkill[i];
		}
		for (int i = 0; i < 4; i++){
			if (allPlayer[i].a == 0){
				allPlayer[i].a = 1;
				allPlayer[i].p.setName(name);
				allPlayer[i].p.setPictureName(pictureName);
				allPlayer[i].p.setTag(tag);
				allPlayer[i].p.setHp(hp);
				allPlayer[i].p.setAtt(att);
				allPlayer[i].p.setDef(def);
				allPlayer[i].p.setSp(sp);
				allPlayer[i].p.setMov(mov);
				allPlayer[i].p.setAtMov(atMov);
				allPlayer[i].p.setSkill(skill);
				allPlayer[i].p.setNSkill(nSkill);
				break;
			}
		}
	}
	fin.close();*/
	newGame();
	ifstream fin;
	fin.open("..\\sav.txt", ios::in);
	char line[1024];
	if (fin.getline(line, sizeof(line))){
		stringstream ss(line);
		ss >> MapTest::mapNum;
	}
	fin.close();
	getMapFile(MapTest::mapNum);	
}

#include "Skill.h"
void addSkill(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 5; j++)
		allPlayer[i].p.skillNames[j] = 0;
	}
	allPlayer[0].p.skillNames[0] = SkillNameEnum::SKILL102;
}

string getMapFile(int mapNum)
{
	switch (mapNum){
	case 1:{
			   for (size_t i = 0; i < 4; i++)
			   {
				   allPlayer[i].a = 1;
			   }
			   allPlayer[0].p = nanliu(1, 0, 0, 100001);
			   allPlayer[1].p = dongsan(1, 0, 0, 100002);
			   allPlayer[2].p = xijiu(1, 0, 0, 100003);
			   allPlayer[3].p = beishier(1, 0, 0, 100004);
			   allPlayer[4].a = 1;
			   allPlayer[4].p = jinzhan(1, 11, 11, 120001);
			   allPlayer[5].a = 1;
			   allPlayer[5].p = jinzhan(1, 11, 8, 120002);
			   allPlayer[6].a = 1;
			   allPlayer[6].p = jinzhan(1, 17, 9, 120003);
			   setPlayers(mapNum);
			   addSkill();
			   return "map01.csb";
	}break;
	case 2:{
			   for (size_t i = 0; i < 4; i++)
			   {
				   allPlayer[i].a = 1;
			   }
			   allPlayer[0].p = nanliu(2, 0, 0, 100001);
			   allPlayer[1].p = dongsan(2, 0, 0, 100002);
			   allPlayer[2].p = xijiu(2, 0, 0, 100003);
			   allPlayer[3].p = beishier(2, 0, 0, 100004);
			   allPlayer[4].a = 1;
			   allPlayer[4].p = jinzhan(1, 14, 14, 120001);
			   allPlayer[5].a = 1;
			   allPlayer[5].p = jinzhan(1, 14, 13, 120002);
			   allPlayer[6].a = 1;
			   allPlayer[6].p = jinzhan(1, 16, 12, 120003);
			   allPlayer[7].a = 1;
			   allPlayer[7].p = yuancheng(1, 13, 7, 120004);
			   allPlayer[8].a = 1;
			   allPlayer[8].p = yuancheng(1, 15, 7, 120005);
			   setPlayers(mapNum);
			   addSkill();
			   return "map02.csb";
	}break;
	case 3:{
			   for (size_t i = 0; i < 4; i++)
			   {
				   allPlayer[i].a = 1;
			   }
			   allPlayer[0].p = nanliu(3, 0, 0, 100001);
			   allPlayer[1].p = dongsan(3, 0, 0, 100002);
			   allPlayer[2].p = xijiu(3, 0, 0, 100003);
			   allPlayer[3].p = beishier(3, 0, 0, 100004);
			   allPlayer[4].a = 1;
			   allPlayer[4].p = jinzhan(1, 13, 12, 120001);
			   allPlayer[5].a = 1;
			   allPlayer[5].p = jinzhan(1, 13, 15, 120002);
			   allPlayer[6].a = 1;
			   allPlayer[6].p = jinzhan(1, 13, 9, 120003);
			   allPlayer[7].a = 1;
			   allPlayer[7].p = jinzhan(1, 13, 5, 120004);
			   allPlayer[8].a = 1;
			   allPlayer[8].p = yuancheng(1, 17, 7, 120005);
			   allPlayer[9].a = 1;
			   allPlayer[9].p = yuancheng(1, 17, 11, 120007);
			   allPlayer[10].a = 1;
			   allPlayer[10].p = jiashi(1, 20, 9, 120008);
			   setPlayers(mapNum);
			   addSkill();
			   return "map03.csb";
	}break;
	case 4:{
			   for (size_t i = 0; i < 4; i++)
			   {
				   allPlayer[i].a = 1;
			   }
			   allPlayer[0].p = nanliu(4, 0, 0, 100001);
			   allPlayer[1].p = dongsan(4, 0, 0, 100002);
			   allPlayer[2].p = xijiu(4, 0, 0, 100003);
			   allPlayer[3].p = beishier(4, 0, 0, 100004);
			   allPlayer[4].a = 1;
			   allPlayer[4].p = jinzhan(1, 14, 10, 120001);
			   allPlayer[5].a = 1;
			   allPlayer[5].p = jinzhan(1, 5, 16, 120002);
			   allPlayer[6].a = 1;
			   allPlayer[6].p = jinzhan(1, 7, 7, 120003);
			   allPlayer[7].a = 1;
			   allPlayer[7].p = jinzhan(1, 10, 10, 120004);
			   allPlayer[8].a = 1;
			   allPlayer[8].p = jinzhan(1, 15, 4, 120005);
			   allPlayer[9].a = 1;
			   allPlayer[9].p = yuancheng(1, 6, 2, 120007);
			   allPlayer[10].a = 1;
			   allPlayer[10].p = yuancheng(1, 14, 7, 120008);
			   allPlayer[11].a = 1;
			   allPlayer[11].p = yuancheng(1, 16, 4, 120009);
			   allPlayer[11].a = 1;
			   allPlayer[11].p = jiashi(1, 22, 4, 120010);
			   allPlayer[11].a = 1;
			   allPlayer[11].p = jiashi(1, 3, 3, 120011);
			   setPlayers(mapNum);
			   addSkill();
			   return "map04.csb";
	}break;
	case 5:{
			   for (size_t i = 0; i < 4; i++)
			   {
				   allPlayer[i].a = 1;
			   }
			   allPlayer[0].p = nanliu(4, 0, 0, 100001);
			   allPlayer[1].p = dongsan(4, 0, 0, 100002);
			   allPlayer[2].p = xijiu(4, 0, 0, 100003);
			   allPlayer[3].p = beishier(4, 0, 0, 100004);
			   allPlayer[4].a = 1;
			   allPlayer[4].p = jiashi(1, 10, 10, 120001);
			   allPlayer[5].a = 1;
			   allPlayer[5].p = jiashi(1, 13, 13, 120002);
			   allPlayer[6].a = 1;
			   allPlayer[6].p = jiashi(1, 10, 7, 120003);
			   allPlayer[7].a = 1;
			   allPlayer[7].p = wumianzhe(1, 13, 7, 120004);
			   setPlayers(9);
			   addSkill();
			   return "map09.csb";
	}break;
	default:
		return "map00.csb";
		break;
	}
}

void setPlayers( int mapNum)
{
	/**
	*	设置坐标
	*/
	string filename = "config/map";
	filename += nToS(mapNum);	
	filename += ".json";
	rapidjson::Document doc;
	//读取文件数据，初始化doc  
	std::string data = FileUtils::getInstance()->getStringFromFile(filename);

	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	for (unsigned int i = 0; i < doc.Size(); i++)

	{

		//逐个提取数组元素（声明的变量必须为引用）  

		rapidjson::Value &v = doc[i];
		allPlayer[v["tag"].GetInt()].p.setPos(v["x"].GetInt(), v["y"].GetInt());
		if (v["x"].GetInt() == 0 && v["y"].GetInt() == 0){
			allPlayer[v["tag"].GetInt()].a = 0;
		}
	}
	data.clear();
}

void gameSave()
{
	ofstream fin;
	fin.open("..\\sav.txt", ios::out | ios::trunc);//以写方式打开文件，如果文件存在，长度设为0
	fin << MapTest::mapNum;
	fin.close();
}

void Player::setName(string s){ this->name = s; }
void Player::setPictureName(string s){ this->pictureName = s; }
void Player::setMov(int i){ this->mov = i; }
void Player::setTag(int i){ this->tag = i; }
void Player::setHp(int h){ this->hp = this->maxHp = h; }
void Player::setAtt(int a){ this->att = a; }
void Player::setDef(int d){ this->def = d; this->sDef = (1.0 - (float)d / (10 + d)); }
void Player::setSp(int s){ this->sp = s; }
void Player::setPos(int x, int y){ this->wx = x; this->wy = y; this->x = x * 30; this->y = y * 30; }
void Player::setAtMov(int n){ this->atMov = n; }
void Player::setSkill(int *n){ for (int i = 0; i < 12; i++)this->skill[i] = n[i]; }
void Player::setNSkill(int *n){ 
	for (int i = 0; i < 5; i++)
	{
		this->nSkill[i][0] = this->nSkill[i][1] = 0;
	}
	for (int i = 0; i < 5; i++)
	{
		this->nSkill[i][0] = n[i];
		string str = nToS(n[i]);
		str[0] = '0';
		int num = sToN(str);
		if (num % 4 != 0)this->nSkill[i][1] = 5;
		else if ( this->nSkill[i][0] != 0 ) this->nSkill[i][1] = 2;
	}
}
void Player::setBuff(int n, int time){

	this->buff[n][0] = 1;
	if (this->buff[n][1] <= time) this->buff[n][1] = time;
}

Player d1(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("xiaobing1");
	p.setPictureName("rw04.csb");
	p.setHp(50 + Lv * 30);
	p.setAtt(5 + Lv * 5);
	p.setAtMov(2);
	p.setDef(5);
	p.setSp(18);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	return p;
}

Player d2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("xiaobing2");
	p.setPictureName("rw04.csb");
	p.setHp(50 + Lv * 30);
	p.setAtt(5 + Lv * 5);
	p.setAtMov(1);
	p.setDef(5);
	p.setSp(20);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(3);
	return p;
}

Player boss2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("boss2");
	p.setPictureName("rw04.csb");
	p.setHp(100 + Lv * 100);
	p.setAtt(25 + Lv * 5);
	p.setAtMov(200);
	p.setDef(8);
	p.setSp(17);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(4);
	p.setBuff(52, 999);
	return p;
}
Player liaoji0(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji0");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(53, 999);
	return p;
}

Player liaoji1(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji1");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(55, 999);
	return p;
}
Player liaoji2(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji2");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	p.setBuff(57, 999);
	return p;
}
Player liaoji3(int Lv, int x, int y, int tag)
{
	Player p;
	p.setName("liaoji3");
	p.setPictureName("rw04.csb");
	p.setHp(200 + Lv * 50);
	p.setAtt(0);
	p.setAtMov(0);
	p.setDef(0);
	p.setSp(5);
	p.setPos(x, y);
	p.setTag(tag);
	p.setMov(2);
	return p;
}




