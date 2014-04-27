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
    struct ParticleVertex
    {
        float position[3];
        float color[4];
        float velocity[3];
        float size;
        float lifetime;
    };
    
    struct Particle
    {
    public:
        Particle()
        {
        }
        
        ~Particle()
        {
        }
        
        void Update(float elapsed, const glm::vec3 &gravity);
        
        static void RegisterScriptObject(asIScriptEngine *engine);
        
        float GetSize() { return mSize; }
        const glm::vec3& GetPosition() { return mPosition; }
        
        glm::vec3 mPosition;
        glm::vec4 mColor;
        glm::vec3 mVelocity;
        glm::vec3 mAcceleration;
        float mSize;
        float mLifetime;
    };
};


