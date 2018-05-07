#include "MapTest.h"
#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocostudio::timeline;
using namespace cocos2d::ui;
using namespace std;
//技能枚举类型
enum SkillNameEnum{
	NULLNAME,
	//测试技能1
	TEST1,
	SKILL102
};
enum SkillTypeEnum{
	NULLTYPE,
	//单体
	MON,
	//群体
	AOE,
	//友方单体
	FMON,
	//友方群体
	FAOE
};
/*2018年2月10日 19:15:10 技能类基类*/
class Skill
{
public:
	Skill();
	Skill(int Level);
	//通过技能名实例化技能对象
	static Skill *getSkill(SkillNameEnum skillName);
	static void initNowSkills();
	static Skill *nowSkills[5];
	//能量值
	static int power;
	//一对一，一对多释放技能
	virtual cocos2d::Action *skillResult(ssp* players[]);
	//技能名
	int skillName;
	//技能范围的csb文件名
	string csbFileName;
	//技能类型
	int skillType;
	//技能等级
	int skillLevel;
	//技能消耗
	int skillconsume;
};

class TestSkill : public Skill{
public:
	//需要重写一个构造函数，一个skilResult函数
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
	//需要重写一个构造函数，一个skilResult函数
	Skill102(int skillLevel);
	cocos2d::Action *skillResult(ssp* players[]);
};