#ifndef CURSOR_CLASS_DEF
#define CURSOR_CLASS_DEF

#include "Transform.h"
#include "cinder/app/KeyEvent.h"
using namespace ci;
using namespace ci::app;
using namespace std;
class Cursor {
public:
    Transform transform;
    Timer blink;
	
    int height;

    void Render() {
        if((int)blink.getSeconds() % 2 == 0) {
            return;
        }

        gl::drawLine(
            transform.position,
            transform.position + Vec2f(0, height)
        );
		

    }

    Cursor() : height(0) { blink.start(); }
};

#endif