// $Id$


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

#include <iostream>
#include "Factory.h"
#include "moses/UserMessage.h"
#include "moses/TypeDef.h"
#include "moses/FactorCollection.h"

// include appropriate header
#ifdef LM_SRI
#  include "SRI.h"
#include "ParallelBackoff.h"
#endif
#ifdef LM_IRST
#  include "IRST.h"
#endif
#ifdef LM_RAND
#  include "Rand.h"
#endif

#include "ORLM.h"

#ifdef LM_REMOTE
#	include "Remote.h"
#endif

#include "Ken.h"

#ifdef LM_LDHT
#   include "LDHT.h"
#endif

#include "Base.h"
#include "Joint.h"

#include <android/log.h>

#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", __VA_ARGS__)
#endif


using namespace std;

namespace Moses
{

namespace LanguageModelFactory
{

LanguageModel* CreateLanguageModel(LMImplementation lmImplementation
                                   , const std::vector<FactorType> &factorTypes
                                   , size_t nGramOrder
                                   , const std::string &languageModelFile
                                   , int dub)
{

  LOGE("[mgjang] in CreateLanguageModel function \n");

  if (lmImplementation == Ken || lmImplementation == LazyKen) {
    return ConstructKenLM(languageModelFile, factorTypes[0], lmImplementation == LazyKen);
  }
  LanguageModelImplementation *lm = NULL;
  switch (lmImplementation) {
  case RandLM:
#ifdef LM_RAND
    lm = NewRandLM();
#endif
    break;
  case ORLM:
    lm = new LanguageModelORLM();
    break;
  case Remote:
#ifdef LM_REMOTE
    lm = new LanguageModelRemote();
#endif
    break;

  case SRI:
#ifdef LM_SRI
    lm = new LanguageModelSRI();
#endif
    break;
  case IRST:
  	LOGE("[mgjang] case IRST\n");
#ifdef LM_IRST
  	LOGE("[mgjang] check definition IRST\n");
    lm = new LanguageModelIRST(dub);
#endif
    break;
  case Joint:
#ifdef LM_SRI
    lm = new LanguageModelJoint(new LanguageModelSRI());
#endif
    break;
  case ParallelBackoff:
#ifdef LM_SRI
    lm = NewParallelBackoff();
#endif
    break;
  case LDHTLM:
#ifdef LM_LDHT
    return ConstructLDHTLM(languageModelFile,
                           scoreIndexManager,
                           factorTypes[0]);
#endif
    break;
  default:
    break;
  }

  if (lm == NULL) {
    UserMessage::Add("Language model type unknown. Probably not compiled into library");
  	LOGE("[mgjang] Language model type unknown. Probably not compiled into library\n");	
    return NULL;
  } else {
  	LOGE("[mgjang] LM instance is created\n");  
    switch (lm->GetLMType()) {
    case SingleFactor:
		LOGE("[mgjang] case SingleFactor\n");
      if (! static_cast<LanguageModelSingleFactor*>(lm)->Load(languageModelFile, factorTypes[0], nGramOrder)) {
        cerr << "single factor model failed" << endl;
        delete lm;
        lm = NULL;
		LOGE("[mgjang] single factor model failed\n");
      }
      break;
    case MultiFactor:
		LOGE("[mgjang] case MultiFactor\n");
      if (! static_cast<LanguageModelMultiFactor*>(lm)->Load(languageModelFile, factorTypes, nGramOrder)) {
        cerr << "multi factor model failed" << endl;
        delete lm;
        lm = NULL;
		LOGE("[mgjang] multi factor model failed\n");
      }
      break;
    }
  }

  return new LMRefCount(lm);
}
}

}

