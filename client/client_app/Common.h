#ifndef COMMON
#define COMMON

#include <windows.h>
#include <sstream>
#include <iostream>


 #define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

#endif