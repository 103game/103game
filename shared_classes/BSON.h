#include "CompilerOptions.h"

#ifdef SERVER_APP
#include <mongo/bson/bson.h>
using namespace mongo;
#else 
#include <bson/bson.h>
#include <bson/util/json.h>
using namespace bson;
#endif