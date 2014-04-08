#ifndef OBJPROTO_CLASS_DEF
#define OBJPROTO_CLASS_DEF

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ObjProto {
public:
	int getLife();
	void setLife(int NewLife);
	void setX(int NewX);
	void setY(int NewY);
	int getX();
	int getY();
protected:
	int ObjX;
	int ObjY;
	int life;
};

void ObjProto::setX(int NewX) {
	ObjX = NewX;
}
void ObjProto::setY(int NewY) {
	ObjY = NewY;
}

int ObjProto::getX() {
	return ObjX;
}

int ObjProto::getY() {
	return ObjY;
}
int ObjProto::getLife() {
	return life;
}

void ObjProto::setLife(int NewLife) {
	life = NewLife;
}

#endif