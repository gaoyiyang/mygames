#include "Skill.h"
#include "MapTest.h"

//����һ���µļ������ͣ��˴�дһ��Skill�����࣬Ȼ���ڱ༭����skillButtonsLayer.csd�ļ������һ����ť��
//��ť��������Ϊ�Զ��弼�ܵĳ�Ա����csbFileName�ַ�������һ�£���ť�ı�����
//Ȼ���ٴ���һ���ڵ�csd�ļ�������Ϊskill_(csbFileName).csd,��skill_testSkill1.csd����

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

/*���Լ���*/
TestSkill::TestSkill(int Level){
	this->skillLevel = Level;
	this->csbFileName = "testSkill1";
	this->skillName = TEST1;
	this->skillType = MON;
	this->skillconsume = 10;
}

cocos2d::Action *TestSkill::skillResult(ssp* players[]){
	cocos2d::Action *action = NULL;
	//�жϼ�������Ϊ�Ե�����Ч
	if (skillType == MON || skillType == FMON){
		//������������players����һ��Ԫ��Ϊʩ�ż��ܵĽ�ɫ������Ԫ��Ϊ���ܼ���Ч���Ľ�ɫ
		auto me = players[0];
		auto he = players[1];
		he->p.hp = he->p.hp - (me->p.att * 1.5 + 30);
		for (auto buff : he->buffs){
			if (buff == NULL || buff->holdTime <= 0){
				buff = Buff::getBuff(SUOZU, 1, 1);
			}
		}
		power -= 10;
		//������ɹ����Լ���ʩ�ż���״̬��Ϊ0
		me->p.jineng = 0;
		me->p.gongji = 0;
	}
	else {
		//Ⱥ��Ч��
	}
	//����һ������
	return action;
}

//����һ�����ܶ���
Skill *Skill::getSkill(SkillNameEnum sne){
	Skill* skill = NULL;
	switch (sne)
	{
	case NULLNAME:
		break;
	case TEST1:
		skill = new TestSkill(1);
		break;
		//���������
		//case SkillNameEnum:
		//	skill = new Skill(1);
		//break;
	default:
		break;
	}
	return skill;
}