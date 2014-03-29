#ifndef COMMON
#define COMMON

#include <sstream>
#include <iostream>


 #define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s << "\n";                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

#endif