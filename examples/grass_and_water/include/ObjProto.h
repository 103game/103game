#ifndef OBJPROTO_CLASS_DEF
#define OBJPROTO_CLASS_DEF

#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ObjProto
{
public:
	void ChangeX(int NewX);
	void ChangeY(int NewY);
	int getX();
	int getY();
protected:
	int ObjX;
	int ObjY;
};

void ObjProto::ChangeX(int NewX)
{
	ObjX = NewX;
}
void ObjProto::ChangeY(int NewY)
{
	ObjY = NewY;
}

int ObjProto::getX()
{
	return ObjX;
}

int ObjProto::getY()
{
	return ObjY;
}


#endif