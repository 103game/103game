#ifndef STATICOBJ_CLASS_DEF
#define STATICOBJ_CLASS_DEF

#include "Camer.h"
#include "StaticObj.h"


class StaticObj {
public:
	StaticObj(){};
	~StaticObj(){};
	bool isBroken();
	void GoLeft();
	void GoRight();
	void GoUp();
	void GoDown();
	int getX();
	int getY();
	void setX(int NewX);
	void setY(int NewY);

protected:
	int ObjX;
	int ObjY;
	bool broken;
};

bool StaticObj::isBroken() {
	return broken;
}
void StaticObj::GoRight() {

	ObjX += step;
}
void StaticObj::GoDown() {

	ObjY += step;
}
void StaticObj::GoLeft() {

	ObjX -= step;
}
void StaticObj::GoUp() {

	ObjY -= step;
}

int StaticObj::getX() {
	return ObjX;
}

int StaticObj::getY() {
	return ObjY;
}
void StaticObj::setX(int NewX) {
	ObjX = NewX;
}

void StaticObj::setY(int NewY) {
	ObjY=NewY;
}
#endif