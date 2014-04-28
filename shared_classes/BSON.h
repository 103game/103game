#include "CompilerOptions.h"

#ifdef SERVER_APP
#include <mongo/bson/bson.h>
using namespace mongo;
#else 
#include <bson/bson.h>
using namespace bson;
#endif