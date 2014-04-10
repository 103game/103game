#ifndef TRANSFORM_CLASS_DEF
#define TRANSFORM_CLASS_DEF

#include <windows.h>

#include <cinder/app/AppNative.h>
#include <cinder/gl/gl.h>

using namespace std;
using namespace ci;
using namespace ci::app;

class Transform {
public:
    Vec2f size;
    Vec2f position;
};

#endif