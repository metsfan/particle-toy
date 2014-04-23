//
//  Particle.h
//  ParticleToy
//
//  Created by Adam on 4/18/14.
//
//

#pragma once

#include "Globals.h"

#include "Lib/angelscript/angelscript.h"

namespace ptoy
{
    struct Particle
    {
    public:
        Particle()
        {
        }
        
        ~Particle()
        {
        }
        
        void Update(float elapsed);
        
        static void RegisterScriptObject(asIScriptEngine *engine);
        
        float GetSize() { return mSize; }
        const glm::vec3& GetPosition() { return mPosition; }
        
        glm::vec3 mPosition;
        glm::vec4 mColor;
        glm::vec3 mVelocity;
        float mSize;
        float mLifetime;
    };
};


