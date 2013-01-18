/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>
#include <android/log.h>

//#include "moses/staticData.h"
#include "MosesUIInterface.h"
#include "moses/TypeDef.h"
#include "moses/Util.h"
#include "moses/Timer.h"
#include "moses/StaticData.h"
#include "moses/Manager.h"
#include "moses/UserMessage.h"

using namespace Moses;
using namespace std;

#include <android/log.h>
#ifndef LOGE
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", "%s : %d :: %s", __FILE__, __LINE__, __VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "libnav", __VA_ARGS__)
#endif

extern "C" {
	JNIEXPORT jstring Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz );
	JNIEXPORT jint Java_com_example_hellojni_HelloJni_loadModel(JNIEnv * env, jobject obj
							, jstring appPath
							, jstring iniPath
							, jstring source
							, jstring target
							, jstring description);

	JNIEXPORT jstring Java_com_example_hellojni_HelloJni_translateSentence(JNIEnv * env, jobject obj
						, jstring source, int lang);	
};

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
JNIEXPORT jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
	Moses::Parameter* params = new Moses::Parameter();
	if (!Moses::StaticData::LoadDataStatic(params, "")) {
		return env->NewStringUTF("return 0!");
	}
//	return (*env)->NewStringUTF(env, "Hello from JNI !");
	return env->NewStringUTF("Hello from JNI !");    
}

jint Java_com_example_hellojni_HelloJni_loadModel(JNIEnv * env, jobject obj
						, jstring appPath
						, jstring iniPath
						, jstring source
						, jstring target
						, jstring description)
{

	LOGE("in load Model function\n");

	const char *appPathChar = env->GetStringUTFChars(appPath, NULL);
    assert(NULL != appPathChar);

	LOGE("appPathChar : %s\n", appPathChar);

    const char *iniPathChar = env->GetStringUTFChars(iniPath, NULL);
    assert(NULL != iniPathChar);
	LOGE("iniPathChar : %s\n", iniPathChar);

    chdir(appPathChar);

	LOGE("after chdir function\n");

	// load data structures
//	UserMessage::SetOutput(false, true);
	Moses::Parameter *param = new Moses::Parameter();

	LOGE("after new Parameter\n");

	if (!param->LoadParam(iniPathChar))
	{
		LOGE("fail load param\n");
		delete param;
		return 1;
	}

	LOGE("success load param\n");

	const Moses::StaticData &staticData = Moses::StaticData::Instance();
	LOGE("after new staticData\n");

	if (!staticData.LoadDataStatic(param, appPathChar))
	{
		LOGE("fail loadDataStatic\n");
		return 2;
	}

	LOGE("success loadData Static\n");
	return 0;
}

string StringToLower(string strToConvert)
{//change each element of the string to lower case
	for(unsigned int i=0;i<strToConvert.length();i++)
	{
		strToConvert[i] = tolower(strToConvert[i]);
	}
	return strToConvert;//return the converted string
}

void HypoToString(const Hypothesis &hypo, char *output)
{
	const StaticData &staticData = StaticData::Instance();
	const std::vector<FactorType> outFactor = staticData.GetOutputFactorOrder();
	assert(outFactor.size() == 1);

	stringstream strme;
	size_t hypoSize = hypo.GetSize();

	for (size_t pos = 0 ; pos < hypoSize ; ++pos)
	{
		const Word &word = hypo.GetWord(pos);
		strme << *word[outFactor[0]] << " ";
	}

	string str = strme.str();
	strcat(output, str.c_str());
}

void GetErrorMessages(char *msg)
{
	strcpy(msg, UserMessage::GetQueue().c_str());
}

jstring Java_com_example_hellojni_HelloJni_translateSentence(JNIEnv * env, jobject obj
						, jstring source, int lang)
{
#if (_FILE_OFFSET_BITS==64)
	LOGE("[mgjang] _FILE_OFFSET_BITS=64");
#else
	LOGE("[mgjang] _FILE_OFFSET_BITS=64 is not defined");
#endif
//  LOGE("[mgjang] _FILE_OFFSET_BITS= %d", _FILE_OFFSET_BITS);

  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 1");
    // convert Java string to UTF-8
	const char *sourceChar = env->GetStringUTFChars(source, NULL);
    assert(NULL != sourceChar);
    LOGE("Java_mosesui_app_JNIWrapper_translateSentence 2");

	const Moses::StaticData &staticData = Moses::StaticData::Instance();
//	std::vector<FactorType> factorOrder;
//	factorOrder.push_back(0);
	
  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 3");
	string inputStr = StringToLower(sourceChar);
  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 4");
//	vector<string> sentences = SegmentSentenceAndWord(inputStr);
  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 5");
	char output[1000];
	strcpy(output, "");

////////// added
//	Sentence *sentence = new Sentence(inputStr);

	//string sys("jk");
	string langsys;
	if(lang == 0)
		langsys = "jk";
	else
		langsys = "kj";

	LOGE("[mgjang] langsys is %s\n", langsys.begin());
	
	const TranslationSystem& system = staticData.GetTranslationSystem(TranslationSystem::DEFAULT);
//	stringstream out, graphInfo, transCollOpts;
//	map<string, xmlrpc_c::value> retData;

	LOGE("[mgjang] after GetTranslationSystem\n");

	Sentence sentence;
	const vector<FactorType> &inputFactorOrder =
	  staticData.GetInputFactorOrder();

	LOGE("[mgjang] after GetInputFactorOrder\n");
	
	stringstream in(inputStr + "\n");
	sentence.Read(in,inputFactorOrder);
	// need to check in
	LOGE("[mgjang] after sentence.Read\n");
	
	size_t lineNumber = 0; // TODO: Include sentence request number here?

	Manager manager(lineNumber, sentence, staticData.GetSearchAlgorithm(), &system);
	LOGE("[mgjang] after manager\n");
	manager.ProcessSentence();
	LOGE("[mgjang] after ProcessSentence\n");
	const Hypothesis* hypo = manager.GetBestHypothesis();
	LOGE("[mgjang] after GetBestHypothesis\n");

	if (hypo != NULL)
	{
		LOGE("[mgjang] hypo is not null\n");
		HypoToString(*hypo, output);
		LOGE("[mgjang] after HypoToString\n");		
	}
	else
	{
		LOGE("[mgjang] hypo is null\n");
	}

#if 0
	vector<string>::iterator iterSentences;
	for (iterSentences = sentences.begin() ; iterSentences != sentences.end() ; ++iterSentences)
	{
		const string &input = *iterSentences;
	  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 6");
		Sentence *sentence = new Sentence(Input);
		sentence->CreateFromString(factorOrder, input, "|");
	  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 7");
		const TranslationSystem& system = staticData.GetTranslationSystem(TranslationSystem::DEFAULT);

		Manager manager(*sentence, staticData.GetSearchAlgorithm(), &system);
	  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 8");
		manager.ProcessSentence();
		const Hypothesis *hypo = manager.GetBestHypothesis();
		if (hypo != NULL)
		{
			HypoToString(*hypo, output);
		}
	  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 9");
		delete sentence;
	}
  LOGE("Java_mosesui_app_JNIWrapper_translateSentence 10");
  #endif
  return env->NewStringUTF(output);
}



#if 0
std::vector<std::string> SegmentSentenceAndWord(const std::string &sentence)
{
	std::vector<std::string> ret;
	Tokenizer tokenizer;

	ret = tokenizer.Tokenize(sentence, SentenceInput);

	return ret;
}
#endif


