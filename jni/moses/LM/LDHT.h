//
// Oliver Wilson <oliver.wilson@ed.ac.uk>
//

#ifndef moses_LanguageModelLDHT_h
#define moses_LanguageModelLDHT_h

#include "moses/TypeDef.h"

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
#endif

namespace Moses {

class ScoreIndexManager;
class LanguageModel;

LanguageModel* ConstructLDHTLM(const std::string& file,
                               ScoreIndexManager& manager,
                               FactorType factorType);
}  // namespace Moses.

#endif  // moses_LanguageModelLDHT_h

