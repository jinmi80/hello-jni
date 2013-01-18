# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CFLAGS += -x c++ -D LM_INTERNAL -DTRACE_ENABLE  -D _FILE_OFFSET_BITS=64 -D _LARGE_FILES -D __USE_FILE_OFFSET64 -D _USE_FILE_OFFSET64 -I irstlm/src
#LOCAL_CFLAGS += -D LM_INTERNAL -DTRACE_ENABLE  -D _FILE_OFFSET_BITS=64 -D _LARGE_FILES -D _USE_FILE_OFFSET64 -I irstlm/src
LOCAL_CFLAGS += -D LM_INTERNAL -DTRACE_ENABLE -I irstlm/src
#LOCAL_CFLAGS += -D MAX_NUM_FACTORS=4 -D KENLM_MAX_ORDER=6 -D BOOST_NO_EXCEPTIONS -D ANDROID -D __LITTLE_ENDIAN__ -D LM_IRST
LOCAL_CFLAGS += -D MAX_NUM_FACTORS=4 -D KENLM_MAX_ORDER=6 -D BOOST_NO_EXCEPTIONS -D __LITTLE_ENDIAN__ -D LM_IRST

LOCAL_CFLAGS += -I boost/include
LOCAL_CPPFLAGS += -fexceptions -frtti

LOCAL_CFLAGS += -I irstlm-5.70.04/src

#LOCAL_CFLAGS += -I android-ndk-r8c/sources/cxx-stl/gnu-bilstdc++/include \
#		-I android-ndk-r8c/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/include \



LOCAL_LDLIBS += -L boost/lib/ -lboost_system -lboost_date_time -lboost_filesystem -lboost_iostreams -lboost_program_options -lboost_regex -lboost_signals -lboost_thread
LOCAL_LDLIBS += -llog
LOCAL_LDLIBS += \
             -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/libs/armeabi 
#             -lgnustl_static
#LOCAL_LDLIBS += lboost_system-gcc-md lboost_thread-gcc-md \

LOCAL_LDLIBS += -lz -ldl

#LOCAL_LDFLAGS := android-ndk-r7-crystax-5.beta3/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/libstdc++.a \ 
#	      android-ndk-r7-crystax-5.beta3/sources/cxx-stl/gnu-libstdc++/libs/armeabi/4.6.3/libsupc++.a



#LOCAL_MODULE := moses-jni
LOCAL_MODULE    := hello-jni

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
									$(LOCAL_PATH)/moses \
									$(LOCAL_PATH)/boost_1_46_1 \

#									moses/LM/LDHT.cpp					
#									moses/LM/ParallelBackoff.cpp
#									moses/LM/Rand.cpp
#									moses/LM/SRI.cpp
#
#								moses/SyntacticLanguageModel.cpp \
#								moses/MockHypothesis.cpp \

LOCAL_SRC_FILES := \
			lm/read_arpa.cc \
			moses/TranslationOptionCollection.cpp \
			moses/manager.cpp \
			moses/LM/Base.cpp \
			moses/LM/Factory.cpp \
			moses/LM/Implementation.cpp \
			moses/LM/IRST.cpp \
			moses/LM/Joint.cpp \
			moses/LM/KEN.cpp \
			moses/LM/MultiFactor.cpp \
			moses/LM/ORLM.cpp \
			moses/LM/Remote.cpp \
			moses/LM/SingleFactor.cpp \
			moses/StaticData.cpp	\
			moses/Parameter.cpp	\
			hellojni.cpp \
			irstlm-5.70.04/src/lmclass.cpp \
			irstlm-5.70.04/src/Htable.cpp \
			irstlm-5.70.04/src/Lmmacro.cpp \
			irstlm-5.70.04/src/lmtable.cpp \
			irstlm-5.70.04/src/N_gram.cpp \
			irstlm-5.70.04/src/util.cpp \
			irstlm-5.70.04/src/timer.cpp \
			irstlm-5.70.04/src/dictionary.cpp 	\
			irstlm-5.70.04/src/mempool.cpp		 \
			irstlm-5.70.04/src/Mfstream.cpp 	\
			irstlm-5.70.04/src/lmContainer.cpp	\
			irstlm-5.70.04/src/lmInterpolation.cpp \
			lm/bhiksha.cc \
			lm/binary_format.cc \
			lm/config.cc \
			lm/lm_exception.cc \
			lm/model.cc \
			lm/quantize.cc \
			lm/search_hashed.cc \
			lm/search_trie.cc \
			lm/trie.cc \
			lm/trie_sort.cc \
			lm/value_build.cc \
			lm/virtual_interface.cc \
			lm/vocab.cc \
			OnDiskPt/OnDiskWrapper.cpp \
			OnDiskPt/SourcePhrase.cpp \
			OnDiskPt/TargetPhrase.cpp \
			OnDiskPt/Word.cpp \
			OnDiskPt/Phrase.cpp \
			OnDiskPt/PhraseNode.cpp \
			OnDiskPt/TargetPhraseCollection.cpp \
			OnDiskPt/Vocab.cpp \
			OnDiskPt/OnDiskQuery.cpp \
			search/nbest.cc \
			search/rule.cc \
			search/vertex.cc \
			search/vertex_generator.cc \
			search/edge_generator.cc \
			util/double-conversion/bignum-dtoa.cc \
			util/double-conversion/cached-powers.cc \
			util/double-conversion/double-conversion.cc \
			util/double-conversion/fixed-dtoa.cc \
			util/double-conversion/bignum.cc \
			util/double-conversion/diy-fp.cc \
			util/double-conversion/fast-dtoa.cc \
			util/double-conversion/strtod.cc \
			util/bit_packing.cc \
			util/ersatz_progress.cc \
			util/exception.cc \
			util/file.cc \
			util/file_piece.cc \
			util/mmap.cc \
			util/murmur_hash.cc \
			util/pool.cc \
			util/read_compressed.cc \
			util/string_piece.cc \
			util/usage.cc \
			moses/AlignmentInfo.cpp \
			moses/AlignmentInfoCollection.cpp \
			moses/BitmapContainer.cpp \
			moses/BleuScoreFeature.cpp \
			moses/ChartCell.cpp \
			moses/ChartCellCollection.cpp \
			moses/ChartHypothesis.cpp \
			moses/ChartHypothesisCollection.cpp \
			moses/ChartManager.cpp	\
			moses/ChartParser.cpp \
			moses/ChartTranslationOptionList.cpp \
			moses/ChartTranslationOptions.cpp \
			moses/ChartTrellisDetour.cpp \
			moses/ChartTrellisDetourQueue.cpp \
			moses/ChartTrellisNode.cpp \
			moses/ChartTrellisPath.cpp \
			moses/ConfusionNet.cpp \
			moses/DecodeFeature.cpp \
			moses/DecodeGraph.cpp \
			moses/DecodeStep.cpp \
			moses/DecodeStepGeneration.cpp \
			moses/DecodeStepTranslation.cpp \
			moses/Dictionary.cpp \
			moses/DummyScoreProducers.cpp \
			moses/Factor.cpp \
			moses/FactorCollection.cpp \
			moses/FactorTypeSet.cpp \
			moses/FeatureFunction.cpp \
			moses/FeatureVector.cpp \
			moses/FFState.cpp \
			moses/File.cpp \
			moses/FloydWarshall.cpp \
			moses/GenerationDictionary.cpp \
			moses/GlobalLexicalModel.cpp \
			moses/GlobalLexicalModelUnlimited.cpp \
			moses/Hypothesis.cpp \
			moses/HypothesisStack.cpp \
			moses/HypothesisStackCubePruning.cpp \
			moses/HypothesisStackNormal.cpp \
			moses/Incremental.cpp \
			moses/InputFileStream.cpp \
			moses/InputType.cpp \
			moses/LexicalReorderingTable.cpp \
			moses/LexicalReordering.cpp \
			moses/LexicalReorderingState.cpp \
			moses/LMList.cpp \
			moses/LVoc.cpp \
			moses/NonTerminal.cpp \
			moses/PartialTranslOptColl.cpp \
			moses/PCNTools.cpp \
			moses/Phrase.cpp \
			moses/PhraseBoundaryFeature.cpp \
			moses/PhraseLengthFeature.cpp \
			moses/PhrasePairFeature.cpp \
			moses/PrefixTreeMap.cpp \
			moses/ReorderingConstraint.cpp \
			moses/ReorderingStack.cpp \
			moses/RuleCube.cpp \
			moses/RuleCubeItem.cpp \
			moses/RuleCubeQueue.cpp \
			moses/ScoreComponentCollection.cpp \
			moses/ScoreProducer.cpp \
			moses/Search.cpp \
			moses/SearchCubePruning.cpp \
			moses/SearchNormal.cpp \
			moses/SearchNormalBatch.cpp \
			moses/Sentence.cpp \
			moses/SentenceStats.cpp \
			moses/SourceWordDeletionFeature.cpp \
			moses/SparsePhraseDictionaryFeature.cpp \
			moses/SquareMatrix.cpp \
			moses/TargetBigramFeature.cpp \
			moses/TargetNgramFeature.cpp \
			moses/TargetPhrase.cpp \
			moses/TargetPhraseCollection.cpp \
			moses/TargetWordInsertionFeature.cpp \
			moses/ThreadPool.cpp \
			moses/Timer.cpp \
			moses/TranslationOption.cpp \
			moses/TranslationOptionCollectionConfusionNet.cpp \
			moses/TranslationOptionCollectionText.cpp \
			moses/TranslationOptionList.cpp \
			moses/TranslationSystem.cpp \
			moses/TreeInput.cpp \
			moses/TrellisPath.cpp \
			moses/TrellisPathCollection.cpp \
			moses/UserMessage.cpp \
			moses/Util.cpp \
			moses/Word.cpp \
			moses/WordLattice.cpp \
			moses/WordsBitmap.cpp \
			moses/WordsRange.cpp \
			moses/WordTranslationFeature.cpp \
			moses/XmlOption.cpp \
			moses/TranslationModel/CompactPT/BlockHashIndex.cpp \
			moses/TranslationModel/CompactPT/CmphStringVectorAdapter.cpp \
			moses/TranslationModel/CompactPT/LexicalReorderingTableCompact.cpp \
			moses/TranslationModel/CompactPT/LexicalReorderingTableCreator.cpp \
			moses/TranslationModel/CompactPT/MurmurHash3.cpp \
			moses/TranslationModel/CompactPT/PhraseDecoder.cpp \
			moses/TranslationModel/CompactPT/PhraseDictionaryCompact.cpp \
			moses/TranslationModel/CompactPT/PhraseTableCreator.cpp \
			moses/TranslationModel/CompactPT/ThrowingFwrite.cpp \
			moses/TranslationModel/CYKPlusParser/ChartRuleLookupManagerCYKPlus.cpp \
			moses/TranslationModel/CYKPlusParser/ChartRuleLookupManagerMemory.cpp \
			moses/TranslationModel/CYKPlusParser/ChartRuleLookupManagerMemoryPerSentence.cpp \
			moses/TranslationModel/CYKPlusParser/ChartRuleLookupManagerOnDisk.cpp \
			moses/TranslationModel/CYKPlusParser/DotChartInMemory.cpp \
			moses/TranslationModel/CYKPlusParser/DotChartOnDisk.cpp \
			moses/TranslationModel/DynSAInclude/FileHandler.cpp \
			moses/TranslationModel/DynSAInclude/params.cpp \
			moses/TranslationModel/DynSAInclude/vocab.cpp \
			moses/TranslationModel/fuzzy-match/Alignments.cpp \
			moses/TranslationModel/fuzzy-match/create_xml.cpp \
			moses/TranslationModel/fuzzy-match/FuzzyMatchWrapper.cpp \
			moses/TranslationModel/fuzzy-match/SentenceAlignment.cpp \
			moses/TranslationModel/fuzzy-match/SuffixArray.cpp \
			moses/TranslationModel/fuzzy-match/Vocabulary.cpp \
			moses/TranslationModel/RuleTable/LoaderCompact.cpp \
			moses/TranslationModel/RuleTable/LoaderFactory.cpp \
			moses/TranslationModel/RuleTable/LoaderHiero.cpp \
			moses/TranslationModel/RuleTable/LoaderStandard.cpp \
			moses/TranslationModel/RuleTable/PhraseDictionaryALSuffixArray.cpp \
			moses/TranslationModel/RuleTable/PhraseDictionaryFuzzyMatch.cpp \
			moses/TranslationModel/RuleTable/PhraseDictionaryNodeSCFG.cpp \
			moses/TranslationModel/RuleTable/PhraseDictionaryOnDisk.cpp \
			moses/TranslationModel/RuleTable/PhraseDictionarySCFG.cpp \
			moses/TranslationModel/RuleTable/Trie.cpp \
			moses/TranslationModel/RuleTable/UTrie.cpp \
			moses/TranslationModel/RuleTable/UTrieNode.cpp \
			moses/TranslationModel/Scope3Parser/ApplicableRuleTrie.cpp \
			moses/TranslationModel/Scope3Parser/Parser.cpp \
			moses/TranslationModel/Scope3Parser/StackLatticeBuilder.cpp \
			moses/TranslationModel/Scope3Parser/VarSpanTrieBuilder.cpp \
			moses/TranslationModel/BilingualDynSuffixArray.cpp \
			moses/TranslationModel/DynSuffixArray.cpp \
			moses/TranslationModel/PhraseDictionary.cpp \
			moses/TranslationModel/PhraseDictionaryDynSuffixArray.cpp \
			moses/TranslationModel/PhraseDictionaryMemory.cpp \
			moses/TranslationModel/PhraseDictionaryNode.cpp \
			moses/TranslationModel/PhraseDictionaryTree.cpp \
			moses/TranslationModel/PhraseDictionaryTreeAdaptor.cpp \

			


									
								

include $(BUILD_SHARED_LIBRARY)
