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
    class Particle
    {
    public:
        Particle()
        {
        }
        
        ~Particle()
        {
        }
        
        static void RegisterScriptObject(asIScriptEngine *engine);
        
        float GetSize() { return mSize; }
        const glm::vec3& GetPosition() { return mPosition; }
        
    private:
        glm::vec3 mPosition;
        float mSize;
        glm::vec4 mColor;
        glm::vec3 mVelocity;
        uint32_t mLifetime;
    };
};


