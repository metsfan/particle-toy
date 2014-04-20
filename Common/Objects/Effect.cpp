//
//  Effect.cpp
//  ParticleToyNative
//
//  Created by Adam on 4/20/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "Objects/Effect.h"
#include "Lib/angelscript/scriptstdstring/scriptstdstring.h"
#include "Lib/angelscript/scriptarray/scriptarray.h"
#include "Lib/angelscript/scriptdictionary/scriptdictionary.h"
#include "Lib/angelscript/scriptbuilder/scriptbuilder.h"
#include "Lib/angelscript/scriptmath/scriptmath.h"

#include <CitymapsEngine/Core/Disk/File.h>

#ifdef __APPLE__
#include <JavaVM/jni.h>
#else
#include <jni.h>
#endif

namespace ptoy
{
    Effect::Effect() :
        mShaderProgram(nullptr),
        mVertexShader(nullptr),
        mFragmentShader(nullptr),
        mShaderProgramBuilt(false)
    {
        mScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
        
        RegisterStdString(mScriptEngine);
        RegisterScriptArray(mScriptEngine, true);
        RegisterScriptDictionary(mScriptEngine);
        RegisterScriptMath(mScriptEngine);
    }
    
    Effect::~Effect()
    {
    }
    
    bool Effect::Compile(citymaps::IGraphicsDevice *device, citymaps::JavaObject &object)
    {
        int length;
        
        char *vertexShader = object.GetUTFString("mVertexShaderFilepath", &length);
        std::string vertexShaderFilepath(vertexShader, length);
        citymaps::Logger::Log("Vertex Shader: %s", vertexShaderFilepath.c_str());
        
        char *fragmentShader = object.GetUTFString("mFragmentShaderFilepath", &length);
        std::string fragmentShaderFilepath(fragmentShader, length);
        citymaps::Logger::Log("Fragment Shader: %s", fragmentShaderFilepath.c_str());
        
        char *script = object.GetUTFString("mScriptFilepath", &length);
        std::string scriptFilepath(script, length);
        citymaps::Logger::Log("Script: %s", scriptFilepath.c_str());
        
        free(vertexShader);
        free(fragmentShader);
        free(script);
        
        mShaderProgram = std::unique_ptr<citymaps::IShaderProgram>(device->CreateShaderProgram());
        
        citymaps::File vertexShaderFile(vertexShaderFilepath);
        if (!vertexShaderFile.Load()) {
            citymaps::Logger::Log("Failed to load vertex shader");
            return false;
        }
        
        citymaps::File fragmentShaderFile(fragmentShaderFilepath);
        if (!fragmentShaderFile.Load()) {
            citymaps::Logger::Log("Failed to load fragment shader");
            return false;
        }
        
        mVertexShader = std::unique_ptr<citymaps::IShader>(device->CreateShader(vertexShaderFile.Data(), citymaps::ShaderTypeVertexShader));
        mFragmentShader = std::unique_ptr<citymaps::IShader>(device->CreateShader(fragmentShaderFile.Data(), citymaps::ShaderTypePixelShader));
        
        mShaderProgram->SetShader(mVertexShader.get());
        mShaderProgram->SetShader(mFragmentShader.get());
        
        mShaderProgramBuilt = false;
        
        return true;
    }
    
    void Effect::Run(std::shared_ptr<citymaps::IApplication> app)
    {
        
    }
}
