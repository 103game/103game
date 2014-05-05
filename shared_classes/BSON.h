#include "CompilerOptions.h"

#ifdef SERVER_APP
#include <mongo/bson/bson.h>
#include <mongo/db/json.h>
using namespace mongo;
#else 
#include <bson/bson.h>
#include <bson/util/json.h>
using namespace bson;
#endif