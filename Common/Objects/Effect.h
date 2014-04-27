//
//  Effect.h
//  ParticleToyNative
//
//  Created by Adam on 4/20/14.
//  Copyright (c) 2014 Adam. All rights reserved.
//

#pragma once

#include "Globals.h"
#include "Lib/angelscript/angelscript.h"
#include "Objects/Particle.h"
#include "Lib/angelscript/scriptarray/scriptarray.h"

#include <CitymapsEngine/Core/Application.h>
#include <CitymapsEngine/Core/Graphics/GraphicsDevice.h>
#include <CitymapsEngine/Core/Util/Platform/Android/JavaObject.h>
#include <CitymapsEngine/Core/Graphics/Shape/MeshShape.h>
#include <CitymapsEngine/Core/Graphics/Camera/PerspectiveCamera.h>

namespace ptoy
{
    class Effect : public std::enable_shared_from_this<Effect>,
                    public citymaps::IApplicationListener,
                    public citymaps::Validatable
    {
    public:
        Effect(std::shared_ptr<citymaps::IApplication> app, int width, int height);
        ~Effect();
        
        void Enable();
        
        
        bool Compile(citymaps::IGraphicsDevice *device, citymaps::JavaObject &object);
        void Run(std::shared_ptr<citymaps::IApplication> app);
        
        /* IApplicationListener methods */
        void OnResize(int width, int height);
        void OnUpdate(citymaps::IApplication* application, int deltaMS);
        void OnRender(citymaps::IApplication* application, int deltaMS);
        void OnNetworkStatusChanged(citymaps::IApplication* application, citymaps::NetworkStatus status);
        
    private:
        std::string mVertexShaderFile;
        std::string mFragmentShaderFile;
        std::string mScriptFile;
        
        asIScriptModule *mScriptModule;
        asIScriptContext *mScriptContext;
        
        asIObjectType *mParticleType;
        asIScriptFunction *mParticleUpdateFunction;
        
        int mMaxParticles;
        CScriptArray *mParticles;
        std::vector<ParticleVertex> mVerts;
        glm::vec3 mGravity;
        
        citymaps::IShader *mVertexShader;
        citymaps::IShader *mFragmentShader;
        citymaps::IShaderProgram *mShaderProgram;
        bool mShaderProgramBuilt;
        std::shared_ptr<citymaps::IApplication> mApp;
        
        asIScriptEngine *mScriptEngine;
        
        std::unique_ptr<citymaps::PerspectiveCamera> mCamera;
        citymaps::MeshShape *mRenderShape;
        
        bool mRunning;
        int mFPSCounter;
        int mFPSTime;
        
        void InitializeScriptEngine();
        void CopyParticleToBuffer(std::vector<ParticleVertex> &verts, const Particle &particle, int &c);
        
        static void RegisterGLMTypes(asIScriptEngine *engine);
    };
};
