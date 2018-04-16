#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <string>
#include <sstream>
#include "MapTest.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;


class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	//按钮响应函数
	void Option(Ref* pSender, Widget::TouchEventType type);
	void Option2(Ref* pSender, Widget::TouchEventType type);
	void Option3(Ref* pSender, Widget::TouchEventType type);

};

class MapMenu : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MapMenu);

	//按钮响应函数
	void mapTest(Ref* pSender, Widget::TouchEventType type);
	void loading(float f);//载入动画
	void load();//删除载入动画回调函数
};

class OP : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(OP);

	//按钮响应函数
	void kai(Ref* pSender, Widget::TouchEventType type);
	void p1Sx(Ref* pSender, Widget::TouchEventType type);
	void p1Sk(Ref* pSender, Widget::TouchEventType type);
};

class SX : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SX);

	//按钮响应函数
	void back(Ref* pSender, Widget::TouchEventType type);
};

class SK : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SK);
	void getSkill(Ref* pSender, Widget::TouchEventType type);
	void nSkill();
	void deSkill(Ref* pSender, Widget::TouchEventType type);
	void over(Ref* pSender, Widget::TouchEventType type);//完成按钮
	//按钮响应函数
};

class MySet : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MySet);
	void yinyue(Ref* pSender, Widget::TouchEventType type);
	void yinxiao(Ref* pSender, Widget::TouchEventType type);
	void over(Ref* pSender, Widget::TouchEventType type);//完成按钮
	//按钮响应函数
};

class WIN : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(WIN);
	void goMenu();
};

class LOSE : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LOSE);
	void goMenu();
};


string nToS(int i);
int sToN(string s);

#endif // __HELLOWORLD_SCENE_H__
