#include "MapTest.h"
#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace std;
//����ö������
enum SkillNameEnum{
	NULLNAME,
	//���Լ���1
	TEST1,
	SKILL102
};
enum SkillTypeEnum{
	NULLTYPE,
	//����
	MON,
	//Ⱥ��
	AOE,
	//�ѷ�����
	FMON,
	//�ѷ�Ⱥ��
	FAOE
};
/*2018��2��10�� 19:15:10 ���������*/
class Skill
{
public:
	Skill();
	Skill(int Level);
	//ͨ��������ʵ�������ܶ���
	static Skill *getSkill(SkillNameEnum skillName);
	static void initNowSkills();
	static Skill *nowSkills[5];
	//����ֵ
	static int power;
	//һ��һ��һ�Զ��ͷż���
	virtual cocos2d::Action *skillResult(ssp* players[]);
	//������
	int skillName;
	//���ܷ�Χ��csb�ļ���
	string csbFileName;
	//��������
	int skillType;
	//���ܵȼ�
	int skillLevel;
	//��������
	int skillconsume;
};

class TestSkill : public Skill{
public:
	//��Ҫ��дһ�����캯����һ��skilResult����
	TestSkill(int skillLevel);
	cocos2d::Action *skillResult(ssp* players[]);
};

class SKILL102 : public Skill{
public:
	SKILL102(int skillLevel);
	cocos2d::Action *skillResult(ssp* players[]);
};
class Skill102 : public Skill{
public:
	//��Ҫ��дһ�����캯����һ��skilResult����
	Skill102(int skillLevel);
	cocos2d::Action *skillResult(ssp* players[]);
};