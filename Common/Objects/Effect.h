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
#include <CitymapsEngine/Core/Graphics/Shader.h>
#include <CitymapsEngine/Core/Graphics/ShaderProgram.h>
#include <CitymapsEngine/Core/Graphics/VertexBuffer.h>

namespace ptoy
{
    class Effect : public std::enable_shared_from_this<Effect>,
                    public citymaps::IApplicationListener,
                    public citymaps::Validatable
    {
    public:
        Effect(std::shared_ptr<citymaps::IApplication> app);
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
        
        CScriptArray *mParticles;
        
        citymaps::IShader *mVertexShader;
        citymaps::IShader *mFragmentShader;
        citymaps::IShaderProgram *mShaderProgram;
        bool mShaderProgramBuilt;
        std::shared_ptr<citymaps::IApplication> mApp;
        
        asIScriptEngine *mScriptEngine;
        
        static void RegisterGLMTypes(asIScriptEngine *engine);
    };
};
