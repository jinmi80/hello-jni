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
 */
package com.example.hellojni;

import java.io.File;
import java.util.Vector;

import android.app.Activity;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.os.Bundle;
import android.os.Environment;


public class HelloJni extends Activity
{
	private Vector<File> m_storage = new Vector<File>();
	
	private Vector<String> m_arrModels = new Vector<String>(); 
	private ArrayAdapter<String> m_listAdapter;
	
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        TextView  tv = new TextView(this);
     //   tv.setText( stringFromJNI() );
       // setContentView(tv);
        
//    	Log.v(TAG, "Load " + model);
        
		String appPath = Environment.getExternalStorageDirectory() + "/DioVoice/moses";
		String iniPath = Environment.getExternalStorageDirectory() + "/DioVoice/moses/moses.kj.ini";
		String source = "SSS";
		String target = "TTT";
		String descr = "DDDD";
		
		//Environment.getExternalStorageDirectory() + 
			//"/DioVoice/DB/ASR/Kor.AM.Clt.Release.110812_KR.T1_23.L1_23.JEJU.130K.g24.11101.noOpt.quant.kr.cfg"

		int ret = loadModel(appPath, iniPath, source, target, descr);
		String input = "할머#N 이#c 니#e 앉#V 으세요#e";
		//String input = "っていう#j か#e それ#P が#j 日本#O の#e ルール#y って#j もん#o な#W の#e";
		String lang = "kj";
		String result = translateSentence(input, 1);
//    	Log.v(TAG, Integer.toString(ret));
//    	m_txtProgress.setText(Integer.toString(ret));

    	//enableTranslate(ret == 0);
		 tv.setText( result);
	     setContentView(tv);
    }

    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    public native String  stringFromJNI();
    
    public native int	loadModel(String appPath
			, String iniPath
			, String source
			, String target
			, String description);
    
    public native String translateSentence(String input, int lang);

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("hello-jni");
    }
}
