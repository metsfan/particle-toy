//
//  Particle.cpp
//  ParticleToy
//
//  Created by Adam on 4/18/14.
//
//

#include "Objects/Particle.h"

namespace ptoy
{
    void Particle::RegisterScriptObject(asIScriptEngine *engine)
    {
        const char *className = "Particle";
        
        engine->RegisterObjectType(className, sizeof(Particle), asOBJ_VALUE | asOBJ_POD);
        
        engine->RegisterObjectProperty(className, "vec3 position", asOFFSET(Particle, mPosition));
        engine->RegisterObjectProperty(className, "vec4 color", asOFFSET(Particle, mColor));
        engine->RegisterObjectProperty(className, "float size", asOFFSET(Particle, mSize));
        engine->RegisterObjectProperty(className, "vec3 velocity", asOFFSET(Particle, mVelocity));
        engine->RegisterObjectProperty(className, "float lifetime", asOFFSET(Particle, mLifetime));
    }
    
    void Particle::Update(float elapsed)
    {
        mPosition += mPosition * (mVelocity * elapsed);
    }
}