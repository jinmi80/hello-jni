// vim:tabstop=2

/***********************************************************************
Moses - factored phrase-based language decoder
Copyright (C) 2006 University of Edinburgh

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
***********************************************************************/

#include "moses/TranslationModel/PhraseDictionary.h"
#include "moses/TranslationModel/PhraseDictionaryTreeAdaptor.h"
#include "moses/TranslationModel/RuleTable/PhraseDictionarySCFG.h"
#include "moses/TranslationModel/RuleTable/PhraseDictionaryOnDisk.h"
#include "moses/TranslationModel/RuleTable/PhraseDictionaryALSuffixArray.h"
#include "moses/TranslationModel/RuleTable/PhraseDictionaryFuzzyMatch.h"

#ifndef WIN32
#include "moses/TranslationModel/PhraseDictionaryDynSuffixArray.h"
#include "moses/TranslationModel/CompactPT/PhraseDictionaryCompact.h"
#endif
#include "moses/TranslationModel/RuleTable/UTrie.h"

#include "moses/StaticData.h"
#include "moses/InputType.h"
#include "moses/TranslationOption.h"
#include "moses/UserMessage.h"

using namespace std;

namespace Moses
{

const TargetPhraseCollection *PhraseDictionary::
GetTargetPhraseCollection(InputType const& src,WordsRange const& range) const
{
  return GetTargetPhraseCollection(src.GetSubString(range));
}

size_t PhraseDictionary::GetDictIndex() const 
{ 
  return m_feature->GetDictIndex(); 
}

PhraseDictionaryFeature::PhraseDictionaryFeature
(PhraseTableImplementation implementation
 , SparsePhraseDictionaryFeature* spdf
 , size_t numScoreComponent
 , unsigned numInputScores
 , const std::vector<FactorType> &input
 , const std::vector<FactorType> &output
 , const std::string &filePath
 , const std::vector<float> &weight
 , size_t dictIndex
 , size_t tableLimit
 , const std::string &targetFile  // default param
 , const std::string &alignmentsFile) // default param
  :DecodeFeature("PhraseModel",numScoreComponent,input,output),
  m_dictIndex(dictIndex),
  m_numInputScores(numInputScores),
  m_filePath(filePath),
  m_tableLimit(tableLimit),
  m_implementation(implementation),
  m_targetFile(targetFile),
  m_alignmentsFile(alignmentsFile),
  m_sparsePhraseDictionaryFeature(spdf)
{
  if (implementation == Memory || implementation == SCFG || implementation == SuffixArray ||
      implementation==Compact) { // || implementation==FuzzyMatch ) {
    m_useThreadSafePhraseDictionary = true;
    if (implementation == SuffixArray) {
      cerr << "Warning: implementation holds cached weights!" << endl;
      exit(1);
    }
  } else {
    m_useThreadSafePhraseDictionary = false;
  }
}

PhraseDictionary* PhraseDictionaryFeature::LoadPhraseTable(const TranslationSystem* system)
{
	LOGE("[mgjang] in LoadPhraseTable function. \n");
  const StaticData& staticData = StaticData::Instance();
  	LOGE("[mgjang] 1 in LoadPhraseTable function. \n");
  std::vector<float> weightT = staticData.GetWeights(this);
  	LOGE("[mgjang] 2 in LoadPhraseTable function. \n");  
	
  if (m_implementation == Memory) {
  	LOGE("[mgjang] 3 in LoadPhraseTable function. \n");	
    // memory phrase table
    VERBOSE(2,"using standard phrase tables" << std::endl);
  	LOGE("[mgjang] file path is %s. \n", m_filePath.begin());
    if (!FileExists(m_filePath) && FileExists(m_filePath + ".gz")) {
  	LOGE("[mgjang] 4 in LoadPhraseTable function. \n");		
      m_filePath += ".gz";
      VERBOSE(2,"Using gzipped file" << std::endl);
    }
  	LOGE("[mgjang] 5 in LoadPhraseTable function. \n");			
    if (staticData.GetInputType() != SentenceInput) {
  	LOGE("[mgjang] 6 in LoadPhraseTable function. \n");				
      UserMessage::Add("Must use binary phrase table for this input type");
      CHECK(false);
    }

  	LOGE("[mgjang] 7 in LoadPhraseTable function. \n");		

    PhraseDictionaryMemory* pdm  = new PhraseDictionaryMemory(GetNumScoreComponents(),this);
    bool ret = pdm->Load(GetInput(), GetOutput()
                         , m_filePath
                         , weightT
                         , m_tableLimit
                         , system->GetLanguageModels()
                         , system->GetWeightWordPenalty());
    CHECK(ret);
  	LOGE("[mgjang] 8 in LoadPhraseTable function. \n");			
    return pdm;
  } else if (m_implementation == Binary) {
	  LOGE("[mgjang] 9 in LoadPhraseTable function. \n");	  
	PhraseDictionaryTreeAdaptor* pdta = new PhraseDictionaryTreeAdaptor(GetNumScoreComponents(), m_numInputScores,this);
	LOGE("[mgjang] 9-1 in LoadPhraseTable function. \n");
    bool ret = pdta->Load(                    GetInput()
               , GetOutput()
               , m_filePath
               , weightT
               , m_tableLimit
               , system->GetLanguageModels()
               , system->GetWeightWordPenalty());
	LOGE("[mgjang] 9-2 in LoadPhraseTable function. \n");
    CHECK(ret);
  	LOGE("[mgjang] 10 in LoadPhraseTable function. \n");			
    return pdta;
  } else if (m_implementation == SCFG || m_implementation == Hiero) {
  	LOGE("[mgjang] 11 in LoadPhraseTable function. \n");		  
    // memory phrase table
    if (m_implementation == Hiero) {
      VERBOSE(2,"using Hiero format phrase tables" << std::endl);
    } else {
      VERBOSE(2,"using Moses-formatted SCFG phrase tables" << std::endl);
    }
    if (!FileExists(m_filePath) && FileExists(m_filePath + ".gz")) {
      m_filePath += ".gz";
      VERBOSE(2,"Using gzipped file" << std::endl);
    }

  	LOGE("[mgjang] 12 in LoadPhraseTable function. \n");		

    RuleTableTrie *dict;
    if (staticData.GetParsingAlgorithm() == ParseScope3) {
      dict = new RuleTableUTrie(GetNumScoreComponents(), this);
    } else {
      dict = new PhraseDictionarySCFG(GetNumScoreComponents(), this);
    }
    bool ret = dict->Load(GetInput()
                         , GetOutput()
                         , m_filePath
                         , weightT
                         , m_tableLimit
                         , system->GetLanguageModels()
                         , system->GetWordPenaltyProducer());
  	LOGE("[mgjang] 13 in LoadPhraseTable function. \n");			
    CHECK(ret);
  	LOGE("[mgjang] 14 in LoadPhraseTable function. \n");				
    return dict;
  } else if (m_implementation == ALSuffixArray) {
    // memory phrase table
  	LOGE("[mgjang] 15 in LoadPhraseTable function. \n");			    
    cerr << "Warning: Implementation holds cached weights!" << endl;
    exit(1);
    VERBOSE(2,"using Hiero format phrase tables" << std::endl);
    if (!FileExists(m_filePath) && FileExists(m_filePath + ".gz")) {
      m_filePath += ".gz";
      VERBOSE(2,"Using gzipped file" << std::endl);
    }

  	LOGE("[mgjang] 16 in LoadPhraseTable function. \n");				
    PhraseDictionaryALSuffixArray* pdm  = new PhraseDictionaryALSuffixArray(GetNumScoreComponents(),this);
    bool ret = pdm->Load(GetInput()
                         , GetOutput()
                         , m_filePath
                         , weightT
                         , m_tableLimit
                         , system->GetLanguageModels()
                         , system->GetWordPenaltyProducer());
    CHECK(ret);
  	LOGE("[mgjang] 17 in LoadPhraseTable function. \n");				
    return pdm;
  } else if (m_implementation == OnDisk) {

    PhraseDictionaryOnDisk* pdta = new PhraseDictionaryOnDisk(GetNumScoreComponents(), this);
    bool ret = pdta->Load(GetInput()
                          , GetOutput()
                          , m_filePath
                          , weightT
                          , m_tableLimit
                          , system->GetLanguageModels()
                          , system->GetWordPenaltyProducer());
    CHECK(ret);
    return pdta;
  } else if (m_implementation == SuffixArray) {
    cerr << "Warning: Implementation holds cached weights!" << endl;
    exit(1);
#ifndef WIN32
    PhraseDictionaryDynSuffixArray *pd = new PhraseDictionaryDynSuffixArray(GetNumScoreComponents(), this);
    if(!(pd->Load(
           GetInput()
           ,GetOutput()
           ,m_filePath
           ,m_targetFile
           ,m_alignmentsFile
           ,weightT, m_tableLimit
           ,system->GetLanguageModels()
	   ,system->GetWeightWordPenalty()))) {
      std::cerr << "FAILED TO LOAD\n" << endl;
      delete pd;
      pd = NULL;
    }
    std::cerr << "Suffix array phrase table loaded" << std::endl;
  	LOGE("[mgjang] 18 in LoadPhraseTable function. \n");				
    return pd;
#else
    CHECK(false);
#endif
  } else if (m_implementation == FuzzyMatch) {
    
    PhraseDictionaryFuzzyMatch *dict = new PhraseDictionaryFuzzyMatch(GetNumScoreComponents(), this);

    bool ret = dict->Load(GetInput()
                          , GetOutput()
                          , m_filePath
                          , weightT
                          , m_tableLimit
                          , system->GetLanguageModels()
                          , system->GetWordPenaltyProducer());
    CHECK(ret);
  	LOGE("[mgjang] 19 in LoadPhraseTable function. \n");			

    return dict;    
  } else if (m_implementation == Compact) {
#ifndef WIN32
    VERBOSE(2,"Using compact phrase table" << std::endl);                                                                                                                               
                                                                                                                                      
    PhraseDictionaryCompact* pd  = new PhraseDictionaryCompact(GetNumScoreComponents(), m_implementation, this);                         
    bool ret = pd->Load(GetInput(), GetOutput()                                                                                      
                         , m_filePath                                                                                                 
                         , weightT                                                                                                  
                         , m_tableLimit                                                                                               
                         , system->GetLanguageModels()                                                                                
                         , system->GetWeightWordPenalty());                                                                           
    CHECK(ret);
  	LOGE("[mgjang] 20 in LoadPhraseTable function. \n");				
    return pd;                                                                                                                       
#else
  	LOGE("[mgjang] 21 in LoadPhraseTable function. \n");			

    CHECK(false);
#endif
  }  
  else {
  	LOGE("[mgjang] 22 in LoadPhraseTable function. \n");			  	
    std::cerr << "Unknown phrase table type " << m_implementation << endl;
    CHECK(false);
  }
}

void PhraseDictionaryFeature::InitDictionary(const TranslationSystem* system)
{
  //Thread-safe phrase dictionaries get loaded now
  if (m_useThreadSafePhraseDictionary && !m_threadSafePhraseDictionary.get()) {
    IFVERBOSE(1)
    PrintUserTime("Start loading phrase table from " +  m_filePath);
    m_threadSafePhraseDictionary.reset(LoadPhraseTable(system));
    IFVERBOSE(1)
    PrintUserTime("Finished loading phrase tables");
  }
  //Other types will be lazy loaded
}

//Called when we start translating a new sentence
void PhraseDictionaryFeature::InitDictionary(const TranslationSystem* system, const InputType& source)
{
	LOGE("[mgjang] in InitDictionary function\n");
  PhraseDictionary* dict;
  if (m_useThreadSafePhraseDictionary) {
	LOGE("[mgjang] translationmodel 1\n");  	
    //thread safe dictionary should already be loaded
    dict = m_threadSafePhraseDictionary.get();
	LOGE("[mgjang] translationmodel 2\n");  		
  } else {
    //thread-unsafe dictionary may need to be loaded if this is a new thread.
	LOGE("[mgjang] translationmodel 3\n");  	    
    if (!m_threadUnsafePhraseDictionary.get()) {
	LOGE("[mgjang] translationmodel 4\n");  			
      m_threadUnsafePhraseDictionary.reset(LoadPhraseTable(system));
    }
		LOGE("[mgjang] translationmodel 5\n");  	
    dict = m_threadUnsafePhraseDictionary.get();
	LOGE("[mgjang] translationmodel 6\n");  		
  }
	LOGE("[mgjang] before check dict\n");  
  CHECK(dict);
	LOGE("[mgjang] after check dict\n");    
  dict->InitializeForInput(source);
}

const PhraseDictionary* PhraseDictionaryFeature::GetDictionary() const
{
  PhraseDictionary* dict;
  if (m_useThreadSafePhraseDictionary) {
    dict = m_threadSafePhraseDictionary.get();
  } else {
    dict = m_threadUnsafePhraseDictionary.get();
  }
  CHECK(dict);
  return dict;
}

PhraseDictionary* PhraseDictionaryFeature::GetDictionary()
{
  PhraseDictionary* dict;
  if (m_useThreadSafePhraseDictionary) {
    dict = m_threadSafePhraseDictionary.get();
  } else {
    dict = m_threadUnsafePhraseDictionary.get();
  }
  CHECK(dict);
  return dict;
}


PhraseDictionaryFeature::~PhraseDictionaryFeature()
{}


std::string PhraseDictionaryFeature::GetScoreProducerWeightShortName(unsigned idx) const
{
  if (idx < GetNumInputScores()){
    return "I";
  }else{
    return "tm";
  }
}


size_t PhraseDictionaryFeature::GetNumInputScores() const
{
  return m_numInputScores;
}

bool PhraseDictionaryFeature::ComputeValueInTranslationOption() const
{
  return true;
}

const PhraseDictionaryFeature* PhraseDictionary::GetFeature() const
{
  return m_feature;
}

size_t PhraseDictionaryFeature::GetDictIndex() const 
{
  return m_dictIndex;
}

}

