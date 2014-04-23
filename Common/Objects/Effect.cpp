//
//  Effect.cpp
//  ParticleToyNative
//
//  Created by Adam on 4/20/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#include "Objects/Effect.h"
#include "Lib/angelscript/scriptstdstring/scriptstdstring.h"
#include "Lib/angelscript/scriptdictionary/scriptdictionary.h"
#include "Lib/angelscript/scriptbuilder/scriptbuilder.h"
#include "Lib/angelscript/scriptmath/scriptmath.h"

#include <CitymapsEngine/Core/Disk/File.h>
#include <CitymapsEngine/Core/Disk/Resource.h>

#ifdef __APPLE__
#include <JavaVM/jni.h>
#else
#include <jni.h>
#endif

namespace ptoy
{
    static inline void PrintExceptionInfo(asIScriptContext *ctx)
    {
        asIScriptEngine *engine = ctx->GetEngine();
        // Determine the exception that occurred
        printf("desc: %s\n", ctx->GetExceptionString());
        // Determine the function where the exception occurred
        const asIScriptFunction *function = ctx->GetExceptionFunction();
        printf("func: %s\n", function->GetDeclaration());
        printf("modl: %s\n", function->GetModuleName());
        printf("sect: %s\n", function->GetScriptSectionName());
        // Determine the line number where the exception occurred
        printf("line: %d\n", ctx->GetExceptionLineNumber());
    }
    
    static inline void MessageCallback(const asSMessageInfo *msg, void *param)
    {
        citymaps::Logger::Log("Script Error: %s, Type: %d, Row: %d, Col: %d, Section: %s", msg->message, msg->type, msg->row, msg->col, msg->section);
        //citymaps::Logger::Log("Message callback called for some reason");
    }
    
    static const char *kScriptModuleName = "particles";
    
    Effect::Effect(std::shared_ptr<citymaps::IApplication> app, int width, int height) :
        mShaderProgram(nullptr),
        mVertexShader(nullptr),
        mFragmentShader(nullptr),
        mShaderProgramBuilt(false),
        mApp(app),
        mScriptModule(NULL),
        mParticleType(NULL),
        mParticleUpdateFunction(NULL),
        mRenderShape(NULL),
        mRunning(false),
        mCamera(NULL)
    {
        mScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
        mScriptContext = mScriptEngine->CreateContext();
        
        RegisterStdString(mScriptEngine);
        RegisterScriptArray(mScriptEngine, true);
        RegisterScriptDictionary(mScriptEngine);
        RegisterScriptMath(mScriptEngine);
        
        
        RegisterGLMTypes(mScriptEngine);
        Particle::RegisterScriptObject(mScriptEngine);
        
        int status;
        
        status = mScriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(citymaps::Logger::Log), asCALL_CDECL);
        citymaps::Logger::Log("Registered Print: %d", status);
        
        status = mScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), this, asCALL_CDECL);
        citymaps::Logger::Log("Registered message callback: %d", status);
        
        this->OnResize(width, height);
    }
    
    Effect::~Effect()
    {
        mScriptContext->Release();
        mScriptEngine->Release();
    }
    
    void Effect::Enable()
    {
        mApp->SetListener(shared_from_this());
        mApp->RunMainLoop(true);
        
        this->Invalidate();
    }
    
    void Effect::OnResize(int width, int height)
    {
        float aspect = (float)width / (float)height;
        citymaps::Logger::Log("Building Camera: %d, %d", width, height);
        mCamera = new citymaps::PerspectiveCamera(aspect, glm::radians(45.0f), 1.0, 100.0, citymaps::Size(width, height));
    }
    
    void Effect::OnUpdate(citymaps::IApplication* application, int deltaMS)
    {
        if (mShaderProgramBuilt) {
            float elapsed = deltaMS * 0.001;
            
            if (!mParticleType) {
                mParticleType = mScriptEngine->GetObjectTypeByName("Particle");
            }
            
            CScriptArray *scriptArray = CScriptArray::Create(mParticleType, mParticles.size());
            
            for (int i = 0; i < mParticles.size(); i++) {
                Particle &particle = mParticles[i];
                //particle.Update(elapsed);
                scriptArray->InsertAt(i, &particle);
            }
            
            if (!mParticleUpdateFunction) {
                mParticleUpdateFunction = mScriptModule->GetFunctionByDecl("void updateParticles(array<Particle> &in particles)");
                mParticleUpdateFunction->AddRef();
            }
            
            mScriptContext->Prepare(mParticleUpdateFunction);
            mScriptContext->SetArgObject(0, &scriptArray);
            mScriptContext->Execute();
            
            scriptArray->Release();
            
            application->Invalidate();
        }
    }
    
    void Effect::OnRender(citymaps::IApplication* application, int deltaMS)
    {
        if (mShaderProgramBuilt && mCamera) {
            citymaps::IGraphicsDevice *device = application->GetGraphicsDevice();
            
            if (!mRenderShape) {
                mRenderShape = new citymaps::MeshShape(device, "particles");
            }
            
            mRenderShape->UpdateData(&mParticles[0], sizeof(Particle) * mParticles.size(), mParticles.size());
            
            citymaps::RenderState state;
            state.SetProjection(mCamera->GetProjectionMatrix());
            state.PushTransform(mCamera->GetViewMatrix());
            
            mRenderShape->Draw(device, state, citymaps::PrimitiveTypePointList);
        }
        
    }
    
    void Effect::OnNetworkStatusChanged(citymaps::IApplication* application, citymaps::NetworkStatus status)
    {
    }
    
    bool Effect::Compile(citymaps::IGraphicsDevice *device, citymaps::JavaObject &object)
    {
        int length;
        
        char *vertexShader = object.GetUTFString("mVertexShaderFilepath", &length);
        mVertexShaderFile = std::string(vertexShader, length);
        citymaps::Logger::Log("Vertex Shader: %s", mVertexShaderFile.c_str());
        
        char *fragmentShader = object.GetUTFString("mFragmentShaderFilepath", &length);
        mFragmentShaderFile = std::string(fragmentShader, length);
        citymaps::Logger::Log("Fragment Shader: %s", mFragmentShaderFile.c_str());
        
        char *script = object.GetUTFString("mScriptFilepath", &length);
        mScriptFile = std::string(script, length);
        citymaps::Logger::Log("Script: %s", mScriptFile.c_str());
        
        free(vertexShader);
        free(fragmentShader);
        free(script);
        
        device->ReleaseGraphicsObject(mVertexShader);
        device->ReleaseGraphicsObject(mFragmentShader);
        
        citymaps::File vertexShaderFile(mVertexShaderFile);
        if (!vertexShaderFile.Load()) {
            citymaps::Logger::Log("Failed to load vertex shader");
            //return false;
        }
        
        citymaps::File fragmentShaderFile(mFragmentShaderFile);
        if (!fragmentShaderFile.Load()) {
            citymaps::Logger::Log("Failed to load fragment shader");
            //return false;
        }
        
        /* Build shader program */
        //mVertexShader = device->CreateShader(vertexShaderFile.Data(), citymaps::ShaderTypeVertexShader);
        //mFragmentShader = device->CreateShader(fragmentShaderFile.Data(), citymaps::ShaderTypePixelShader);
        
        mShaderProgram = device->GetContext()->GetShaderProgram("particles");
        if (!mShaderProgram) {
            mApp->LoadShaderConfig("pt_shaders.xml");
            mApp->LoadEffectsConfig("pt_effects.xml");
            
            mShaderProgram = device->GetContext()->GetShaderProgram("particles");
            
            device->GetContext()->RegisterGlobalData("u_mvp");
        }
        
        int mvpIndex = device->GetContext()->GetGlobalDataIndex("u_mvp");
        
        mVertexShader = device->CreateShader("test_vert.glsl", citymaps::ShaderTypeVertexShader);
        mVertexShader->AddGlobalData(mvpIndex, "u_mvp");
        
        mFragmentShader = device->CreateShader("test_frag.glsl", citymaps::ShaderTypePixelShader);
        mFragmentShader->AddGlobalData(mvpIndex, "u_mvp");
        
        
        citymaps::Logger::Log("Shader Program: %p", mShaderProgram);
        
        mShaderProgram->SetShader(mVertexShader);
        mShaderProgram->SetShader(mFragmentShader);
        
        mShaderProgram->Build();
        
        /* Build script */
        
        if (mScriptModule) {
            mScriptEngine->DiscardModule(kScriptModuleName);
        }
        
        CScriptBuilder builder;
        
        int status = builder.StartNewModule(mScriptEngine, kScriptModuleName);
        if (status < 0) {
            citymaps::Logger::Log("Failed to create module: %d", status);
        }
        
        status = builder.AddSectionFromFile(mScriptFile.c_str());
        if (status < 0) {
            citymaps::Logger::Log("Failed to load script file: %d", status);
        }
        
        status = builder.BuildModule();
        if (status < 0) {
            citymaps::Logger::Log("Failed to build module: %d", status);
        }
        
        mScriptModule = mScriptEngine->GetModule(kScriptModuleName);
        
        asIScriptFunction *function = mScriptModule->GetFunctionByDecl("int getMaxParticles()");
        status = mScriptContext->Prepare(function);
        mScriptContext->Execute();
        mMaxParticles = mScriptContext->GetReturnDWord();
        
        function = mScriptModule->GetFunctionByDecl("array<Particle> initializeParticles(int)");
        status = mScriptContext->Prepare(function);
        mScriptContext->SetArgDWord(0, 10);
        
        status = mScriptContext->Execute();
        
        CScriptArray *particles = reinterpret_cast<CScriptArray *>(mScriptContext->GetReturnObject());
        for (int i = 0; i < particles->GetSize(); i++) {
            mParticles.push_back(*(Particle *)particles->At(i));
        }
        
        this->Invalidate();
        
        mShaderProgramBuilt = true;
        
        return true;
    }
    
    void Effect::Run(std::shared_ptr<citymaps::IApplication> app)
    {
        mRunning = true;
    }
    
    void Effect::RegisterGLMTypes(asIScriptEngine *engine)
    {
        engine->RegisterObjectType("vec3", sizeof(glm::vec3), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectProperty("vec3", "float x", asOFFSET(glm::vec3, x));
        engine->RegisterObjectProperty("vec3", "float y", asOFFSET(glm::vec3, y));
        engine->RegisterObjectProperty("vec3", "float z", asOFFSET(glm::vec3, z));
        engine->RegisterObjectMethod("vec3", "float& opIndex(int)", asMETHODPR(glm::vec3, operator[], (int), float&), asCALL_THISCALL);
        
        engine->RegisterObjectType("vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4, x));
        engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4, y));
        engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4, z));
        engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4, w));
        engine->RegisterObjectMethod("vec4", "float& opIndex(int)", asMETHODPR(glm::vec4, operator[], (int), float&), asCALL_THISCALL);
        
        engine->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectMethod("mat4", "vec4& opIndex(int)", asMETHODPR(glm::mat4, operator[], (int), glm::vec4&), asCALL_THISCALL);
    }
}
