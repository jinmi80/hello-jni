#ifndef moses_Search_h
#define moses_Search_h

#include <vector>
#include "TypeDef.h"
#include "Phrase.h"

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
#endif

namespace Moses
{

class HypothesisStack;
class Hypothesis;
class InputType;
class TranslationOptionCollection;
class Manager;

/** Abstract class used in the phrase-based decoder. 
 *  Cube pruning and normal searches are the classes that inherits from this class
 */
class Search
{
public:
  virtual const std::vector < HypothesisStack* >& GetHypothesisStacks() const = 0;
  virtual const Hypothesis *GetBestHypothesis() const = 0;
  virtual void ProcessSentence() = 0;
  Search(Manager& manager) : m_manager(manager) {}
  virtual ~Search()
  {}

  // Factory
  static Search *CreateSearch(Manager& manager, const InputType &source, SearchAlgorithm searchAlgorithm,
                              const TranslationOptionCollection &transOptColl);

protected:

  const Phrase *m_constraint;
  Manager& m_manager;

};


}
#endif
