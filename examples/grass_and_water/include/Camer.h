#ifndef CAMER_CLASS_DEF
#define CAMER_CLASS_DEF

#define step 100

class Camer {
public:
	int getCameraX();
	int getCameraY();
	void setCameraCoordinates(int NewX, int NewY);
protected:
	int CameraX;
	int CameraY;
};

int Camer::getCameraX() {
	return CameraX;
}
int Camer::getCameraY() {
	return CameraY;
}
void Camer::setCameraCoordinates(int NewX, int NewY) {
	CameraX=NewX;
	CameraY=NewY;
}
#endif