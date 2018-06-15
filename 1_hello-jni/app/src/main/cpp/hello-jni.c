/*
 * Copyright (C) 2016 The Android Open Source Project
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

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   hello-jni/app/src/main/java/com/example/hellojni/HelloJni.java
 */
#include <android/log.h>
static const char* kTAG = "hello-jniCallback";
#define LOGE(...)\
  ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))
JNIEXPORT jstring JNICALL
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
#if defined(__arm__)
    #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    return (*env)->NewStringUTF(env, "Hello from JNI !  Compiled with ABI " ABI ".");
}
/*
 写法一
 */

JNIEXPORT void JNICALL
Java_com_example_hellojni_HelloJni_onClick( JNIEnv* env,
                                                  jobject thiz )
{


    jobject  obj = (*env)->NewGlobalRef(env, thiz);
    // 开始：methodId
    // 获取clz方式一：
//    jclass clz = (*env)->FindClass(env, "com/example/hellojni/HelloJni");
    // 获取clz方式二：
    jclass clz=(*env)->GetObjectClass(env, thiz);
    jmethodID methodId = (*env)->GetMethodID(env, clz, "onClick2", "()V");
    // 结束：methodId
    (*env)->CallVoidMethod(env, obj, methodId); // 调用方法
    (*env)->DeleteGlobalRef(env, obj); // 垃圾回收

}


/*
 写法二：
 这种写法在Activity中调用findViewById将出错
 出错原因暂时不知晓
 */
/*
JNIEXPORT void JNICALL
Java_com_example_hellojni_HelloJni_onClick( JNIEnv* env,
                                            jobject thiz )
{


    // 开始：methodId
    jclass clz = (*env)->FindClass(env, "com/example/hellojni/HelloJni");

    jclass  classInstance  =(*env)->NewGlobalRef(env, clz);
    jmethodID constructor = (*env)->GetMethodID(env, classInstance, "<init>", "()V"); // 构造方法
    jobject  objPre = (*env)->NewObject(env, classInstance, constructor);

    jobject  obj = (*env)->NewGlobalRef(env, objPre);
    // 结束：methodId
    jmethodID methodId = (*env)->GetMethodID(env, classInstance, "onClick2", "()V"); // 构造方法
    (*env)->CallVoidMethod(env, obj, methodId); // 调用方法
    (*env)->DeleteLocalRef(env, objPre); // 垃圾回收
    (*env)->DeleteGlobalRef(env, obj); // 垃圾回收
    (*env)->DeleteGlobalRef(env, classInstance); // 垃圾回收
    LOGE("%d",
         __LINE__);
}
*/