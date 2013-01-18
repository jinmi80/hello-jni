/* People have been abusing assert by assuming it will always execute.  To
 * rememdy the situation, asserts were replaced with CHECK.  These should then
 * be manually replaced with assert (when used correctly) or UTIL_THROW (for
 * runtime checks).  
 */
#ifndef UTIL_CHECK__
#define UTIL_CHECK__

#include <stdlib.h>
#include <iostream>

#include <cassert>

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
#endif

//    abort(); \  // mgjang check for abort

#define CHECK(Condition) do { \
  if (!(Condition)) { \
    std::cerr << "Check " << #Condition << " failed in " << __FILE__ << ":" << __LINE__ << std::endl; \
    LOGE("[CHECK][mgjang] before abort\n"); \
  } \
} while (0) // swallow ;

#endif // UTIL_CHECK__
