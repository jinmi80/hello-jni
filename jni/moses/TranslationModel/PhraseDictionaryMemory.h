// $Id$
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

#ifndef moses_PhraseDictionaryMemory_h
#define moses_PhraseDictionaryMemory_h

#include "moses/TranslationModel/PhraseDictionary.h"
#include "moses/TranslationModel/PhraseDictionaryNode.h"

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
#endif

namespace Moses
{

/*** Implementation of a phrase table in a trie.  Looking up a phrase of
 * length n words requires n look-ups to find the TargetPhraseCollection.
 */
class PhraseDictionaryMemory : public PhraseDictionary
{
  typedef PhraseDictionary MyBase;
  friend std::ostream& operator<<(std::ostream&, const PhraseDictionaryMemory&);

protected:
  PhraseDictionaryNode m_collection;

  TargetPhraseCollection *CreateTargetPhraseCollection(const Phrase &source);

public:
  PhraseDictionaryMemory(size_t numScoreComponent, PhraseDictionaryFeature* feature)
    : PhraseDictionary(numScoreComponent,feature) {}
  virtual ~PhraseDictionaryMemory();

  bool Load(const std::vector<FactorType> &input
            , const std::vector<FactorType> &output
            , const std::string &filePath
            , const std::vector<float> &weight
            , size_t tableLimit
            , const LMList &languageModels
            , float weightWP);

  const TargetPhraseCollection *GetTargetPhraseCollection(const Phrase &source) const;

  // for mert
  virtual void InitializeForInput(InputType const&) {
    /* Don't do anything source specific here as this object is shared between threads.*/
  }

  virtual ChartRuleLookupManager *CreateRuleLookupManager(
    const InputType &,
    const ChartCellCollectionBase &) {
    CHECK(false);
    return 0;
  }

  TO_STRING();

};

}
#endif
