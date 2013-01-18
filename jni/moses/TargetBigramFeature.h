#ifndef moses_TargetBigramFeature_h
#define moses_TargetBigramFeature_h

#include <string>
#include <map>

#include "FactorCollection.h"
#include "FeatureFunction.h"
#include "FFState.h"
#include "Word.h"

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
#endif

namespace Moses
{

class TargetBigramState : public FFState {
  public:
    TargetBigramState(const Word& word): m_word(word) {}
    const Word& GetWord() const {return m_word;}
    virtual int Compare(const FFState& other) const;

  private:
    Word m_word;
};

/** Sets the features of observed bigrams.
 */
class TargetBigramFeature : public StatefulFeatureFunction {
public:
	TargetBigramFeature(FactorType factorType = 0):
     StatefulFeatureFunction("dlmb", ScoreProducer::unlimited),
     m_factorType(factorType)
  {
    FactorCollection& factorCollection = FactorCollection::Instance();
    const Factor* bosFactor =
       factorCollection.AddFactor(Output,m_factorType,BOS_);
    m_bos.SetFactor(m_factorType,bosFactor);
  }
      

	bool Load(const std::string &filePath);

	std::string GetScoreProducerWeightShortName(unsigned) const;
  size_t GetNumInputScores() const;

	virtual const FFState* EmptyHypothesisState(const InputType &input) const;

	virtual FFState* Evaluate(const Hypothesis& cur_hypo, const FFState* prev_state,
	                          ScoreComponentCollection* accumulator) const;

  virtual FFState* EvaluateChart( const ChartHypothesis& /* cur_hypo */,
                                  int /* featureID */,
                                  ScoreComponentCollection* ) const
                                  {
                                  LOGE("[mgjang] before abort\n");
                                    abort();
                                  }
private:
  FactorType m_factorType;
  Word m_bos;
	std::set<std::string> m_vocab;
};

}

#endif // moses_TargetBigramFeature_h
