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
    static int kRandMax = RAND_MAX;
    
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
    
    static inline int randomInt(int min, int max)
    {
        return (rand() % (max - min)) + min;
    }
    
    static inline float randomZeroToOne()
    {
        return (float)rand() / (float)RAND_MAX;
    }
    
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
        mCamera(nullptr)
    {
        mScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
        mScriptContext = mScriptEngine->CreateContext();
        
        RegisterStdString(mScriptEngine);
        RegisterScriptArray(mScriptEngine, true);
        RegisterScriptDictionary(mScriptEngine);
        RegisterScriptMath(mScriptEngine);
        
        auto random = [](float min, float max) -> float {
            
        };
        
        mScriptEngine->RegisterGlobalFunction("int randomInt(int, int)", asFUNCTION(randomInt), asCALL_CDECL);
        mScriptEngine->RegisterGlobalFunction("float randomZeroToOne()", asFUNCTION(randomZeroToOne), asCALL_CDECL);
        
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
        mCamera = std::unique_ptr<citymaps::PerspectiveCamera>(new citymaps::PerspectiveCamera(aspect, 45.0f, 1.0, 1000.0, citymaps::Size(width, height)));
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
                mParticleUpdateFunction = mScriptModule->GetFunctionByDecl("void updateParticles(array<Particle> &particles)");
            }
            
            //mScriptContext->Prepare(mParticleUpdateFunction);
            //mScriptContext->SetArgObject(0, &scriptArray);
            //mScriptContext->Execute();
            
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
            
            std::vector<ParticleVertex> verts;
            verts.resize(mParticles.size() * 6);
            
            citymaps::Logger::Log("Rendering %i particles", mParticles.size());
            
            int c = 0;
            for (const Particle &particle : mParticles) {
                this->CopyParticleToBuffer(verts, particle, c);
            }
            
            mRenderShape->UpdateData(&verts[0], sizeof(ParticleVertex) * verts.size(), verts.size());
            
            mCamera->MoveTo(citymaps::Vector3(0, 0, 100));
            
            mCamera->Update();
            
            citymaps::RenderState state;
            state.SetProjection(mCamera->GetProjectionMatrix());
            state.PushTransform(mCamera->GetViewMatrix());
            
            mRenderShape->Draw(device, state, citymaps::PrimitiveTypeTriangleList);
        }
        
    }
    
    void Effect::CopyParticleToBuffer(std::vector<ParticleVertex> &verts, const Particle &particle, int &c)
    {
        int c2 = c;
        
        for (int i = 0; i < 6; i++) {
            verts[c2].position[0] = particle.mPosition.x;
            verts[c2].position[1] = particle.mPosition.y;
            verts[c2].position[2] = particle.mPosition.z;
            
            verts[c2].color[0] = particle.mColor.x;
            verts[c2].color[1] = particle.mColor.y;
            verts[c2].color[2] = particle.mColor.z;
            verts[c2].color[3] = particle.mColor.w;
            
            verts[c2].velocity[0] = particle.mVelocity.x;
            verts[c2].velocity[1] = particle.mVelocity.y;
            verts[c2].velocity[2] = particle.mVelocity.z;
            
            verts[c2].size = particle.mSize;
            
            verts[c2].lifetime = particle.mLifetime;
            
            c2++;
        }
        
        verts[c].position[0] = particle.mPosition.x - (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y - (verts[c].size * 0.5);
        c++;
        
        verts[c].position[0] = particle.mPosition.x + (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y - (verts[c].size * 0.5);
        c++;
        
        verts[c].position[0] = particle.mPosition.x + (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y + (verts[c].size * 0.5);
        c++;
        
        verts[c].position[0] = particle.mPosition.x - (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y - (verts[c].size * 0.5);
        c++;
        
        verts[c].position[0] = particle.mPosition.x - (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y + (verts[c].size * 0.5);
        c++;
        
        verts[c].position[0] = particle.mPosition.x + (verts[c].size * 0.5);
        verts[c].position[1] = particle.mPosition.y + (verts[c].size * 0.5);
        c++;
        
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
        }
        
//        int mvpIndex = device->GetContext()->GetGlobalDataIndex("u_mvp");
//        
//        citymaps::File vertexShaderData(mVertexShaderFile);
//        vertexShaderData.Load();
//        mVertexShader = device->CreateShader(vertexShaderData.Data(), citymaps::ShaderTypeVertexShader);
//        mVertexShader->AddGlobalData(mvpIndex, "u_mvp");
//        
//        
//        citymaps::File fragmentShaderData(mFragmentShaderFile);
//        fragmentShaderData.Load();
//        mFragmentShader = device->CreateShader(fragmentShaderData.Data(), citymaps::ShaderTypePixelShader);
//        //mFragmentShader->AddGlobalData(mvpIndex, "u_mvp");
//        
//        
//        citymaps::Logger::Log("Shader Program: %p", mShaderProgram);
//        
//        mShaderProgram->SetShader(mVertexShader);
//        mShaderProgram->SetShader(mFragmentShader);
//        
//        mShaderProgram->Build();
        
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
        mScriptContext->SetArgDWord(0, mMaxParticles);
        
        status = mScriptContext->Execute();
        
        CScriptArray *particles = reinterpret_cast<CScriptArray *>(mScriptContext->GetReturnObject());
        
        mParticles.clear();
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
        engine->RegisterObjectProperty("vec3", "float r", asOFFSET(glm::vec3, r));
        engine->RegisterObjectProperty("vec3", "float g", asOFFSET(glm::vec3, g));
        engine->RegisterObjectProperty("vec3", "float b", asOFFSET(glm::vec3, b));
        engine->RegisterObjectProperty("vec3", "float s", asOFFSET(glm::vec3, s));
        engine->RegisterObjectProperty("vec3", "float t", asOFFSET(glm::vec3, t));
        engine->RegisterObjectProperty("vec3", "float p", asOFFSET(glm::vec3, p));
        engine->RegisterObjectMethod("vec3", "float& opIndex(int)", asMETHODPR(glm::vec3, operator[], (int), float&), asCALL_THISCALL);
        
        engine->RegisterObjectType("vec4", sizeof(glm::vec4), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectProperty("vec4", "float x", asOFFSET(glm::vec4, x));
        engine->RegisterObjectProperty("vec4", "float y", asOFFSET(glm::vec4, y));
        engine->RegisterObjectProperty("vec4", "float z", asOFFSET(glm::vec4, z));
        engine->RegisterObjectProperty("vec4", "float w", asOFFSET(glm::vec4, w));
        engine->RegisterObjectProperty("vec4", "float r", asOFFSET(glm::vec4, r));
        engine->RegisterObjectProperty("vec4", "float g", asOFFSET(glm::vec4, g));
        engine->RegisterObjectProperty("vec4", "float b", asOFFSET(glm::vec4, b));
        engine->RegisterObjectProperty("vec4", "float a", asOFFSET(glm::vec4, a));
        engine->RegisterObjectProperty("vec4", "float s", asOFFSET(glm::vec4, s));
        engine->RegisterObjectProperty("vec4", "float t", asOFFSET(glm::vec4, t));
        engine->RegisterObjectProperty("vec4", "float p", asOFFSET(glm::vec4, p));
        engine->RegisterObjectProperty("vec4", "float q", asOFFSET(glm::vec4, q));
        engine->RegisterObjectMethod("vec4", "float& opIndex(int)", asMETHODPR(glm::vec4, operator[], (int), float&), asCALL_THISCALL);
        
        engine->RegisterObjectType("mat3", sizeof(glm::mat3), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectMethod("mat3", "vec3& opIndex(int)", asMETHODPR(glm::mat3, operator[], (int), glm::vec3&), asCALL_THISCALL);
        
        engine->RegisterObjectType("mat4", sizeof(glm::mat4), asOBJ_VALUE | asOBJ_POD);
        engine->RegisterObjectMethod("mat4", "vec4& opIndex(int)", asMETHODPR(glm::mat4, operator[], (int), glm::vec4&), asCALL_THISCALL);
    }
}
