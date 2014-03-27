#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include <Vector>

using namespace ci;
using namespace ci::app;
using namespace std;

class Cam
{
public:
	int getCameraX();
	int getCameraY();
	void ChangeCameraCoordinates(int NewX, int NewY);
protected:
	int CameraX;
	int CameraY;
};

int Cam::getCameraX()
{
	return CameraX;
}
int Cam::getCameraY()
{
	return CameraY;
}
void Cam::ChangeCameraCoordinates(int NewX, int NewY)
{
	CameraX=NewX;
	CameraY=NewY;
}