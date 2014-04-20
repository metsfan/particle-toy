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

#include <CitymapsEngine/Core/Application.h>
#include <CitymapsEngine/Core/Graphics/GraphicsDevice.h>
#include <CitymapsEngine/Core/Util/Platform/Android/JavaObject.h>
#include <CitymapsEngine/Core/Graphics/Shader.h>
#include <CitymapsEngine/Core/Graphics/ShaderProgram.h>

namespace ptoy
{
    class Effect
    {
    public:
        Effect();
        ~Effect();
        
        bool Compile(citymaps::IGraphicsDevice *device, citymaps::JavaObject &object);
        void Run(std::shared_ptr<citymaps::IApplication> app);
        
    private:
        std::unique_ptr<citymaps::IShader> mVertexShader;
        std::unique_ptr<citymaps::IShader> mFragmentShader;
        std::unique_ptr<citymaps::IShaderProgram> mShaderProgram;
        bool mShaderProgramBuilt;
        
        asIScriptEngine *mScriptEngine;
    };
};
