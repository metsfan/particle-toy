//
//  EffectRendererWindow.cpp
//  ParticleToyNative
//
//  Created by Adam on 4/20/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "Globals.h"

#ifdef __APPLE__
#include <JavaVM/jni.h>
#else
#include <jni.h>
#endif

#include <CitymapsEngine/Core/Application.h>
#include <CitymapsEngine/Core/Util/Platform/Android/JavaObject.h>
#include "Objects/Effect.h"

extern "C"
{
    JNIEXPORT jlong JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeInitialize(JNIEnv * env, jobject obj);
    JNIEXPORT jboolean JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeCompile(JNIEnv * env, jobject obj, jlong appPtr, jlong effectPtr, jobject effectJObj);
    JNIEXPORT void JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeRun(JNIEnv * env, jobject obj, jlong appPtr, jlong effectPtr);
};

JNIEXPORT jlong JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeInitialize(JNIEnv * env, jobject obj)
{
    ptoy::Effect *effectPtr = new ptoy::Effect();
    
    return (jlong)(int64_t)effectPtr;
}

JNIEXPORT jboolean JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeCompile(JNIEnv * env, jobject obj, jlong appPtr, jlong effectPtr, jobject effectJObj)
{
    citymaps::JavaObject effectObj(env, effectJObj);
    
    ptoy::Effect *effect = reinterpret_cast<ptoy::Effect *>(effectPtr);
    std::shared_ptr<citymaps::IApplication> *app = reinterpret_cast<std::shared_ptr<citymaps::IApplication> *>(appPtr);
    
    return effect->Compile((*app)->GetGraphicsDevice(), effectObj);
}

JNIEXPORT void JNICALL Java_com_aeskreis_particletoy_view_EffectRendererWindow_nativeRun(JNIEnv * env, jobject obj, jlong appPtr, jlong effectPtr)
{
    //ptoy::Effect *effect = reinterpret_cast<ptoy::Effect *>(effectPtr);
    //std::shared_ptr<citymaps::IApplication> *app = reinterpret_cast<std::shared_ptr<citymaps::IApplication> *>(appPtr);
    
    //effect->Run(*app);
}

