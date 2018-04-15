#include "Skill.h"
#include "MapTest.h"

//创建一个新的技能类型，此处写一个Skill的子类，然后在编辑器中skillButtonsLayer.csd文件中添加一个按钮，
//按钮名称设置为自定义技能的成员变量csbFileName字符串内容一致，按钮文本随意
//然后再创建一个节点csd文件，名称为skill_(csbFileName).csd,如skill_testSkill1.csd这种

Skill::Skill()
{
}


Skill* Skill::nowSkills[5];
int Skill::power = 5;
void Skill::initNowSkills(){
	for (int i = 0; i < 5; i++){
		Skill::nowSkills[i] = NULL;
	}
}



Skill::Skill(int skillLevel){
}




cocos2d::Action *Skill::skillResult(ssp* players[]){
	return NULL;
}

/*测试技能*/
TestSkill::TestSkill(int Level){
	this->skillLevel = Level;
	this->csbFileName = "testSkill1";
	this->skillName = TEST1;
	this->skillType = MON;
	this->skillconsume = 10;
}

cocos2d::Action *TestSkill::skillResult(ssp* players[]){
	cocos2d::Action *action = NULL;
	//判断技能类型为对单体生效
	if (skillType == MON || skillType == FMON){
		//传入的数组参数players，第一个元素为施放技能的角色，其余元素为承受技能效果的角色
		auto me = players[0];
		auto he = players[1];
		he->p.hp = he->p.hp - (me->p.att * 1.5 + 30);
		for (auto buff : he->buffs){
			if (buff == NULL || buff->holdTime <= 0){
				buff = Buff::getBuff(SUOZU, 1, 1);
			}
		}
		power -= 10;
		//将自身可攻击以及可施放技能状态设为0
		me->p.jineng = 0;
		me->p.gongji = 0;
	}
	else {
		//群体效果
	}
	//返回一个动作
	return action;
}

//返回一个技能对象
Skill *Skill::getSkill(SkillNameEnum sne){
	Skill* skill = NULL;
	switch (sne)
	{
	case NULLNAME:
		break;
	case TEST1:
		skill = new TestSkill(1);
		break;
		//在这里添加
		//case SkillNameEnum:
		//	skill = new Skill(1);
		//break;
	default:
		break;
	}
	return skill;
}