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
        engine->RegisterObjectType("Particle", sizeof(Particle), asOBJ_VALUE | asOBJ_POD);
        
        engine->RegisterObjectProperty("Particle", "vec3 position", asOFFSET(Particle, mPosition));
        engine->RegisterObjectProperty("Particle", "vec4 color", asOFFSET(Particle, mColor));
        engine->RegisterObjectProperty("Particle", "float size", asOFFSET(Particle, mSize));
        engine->RegisterObjectProperty("Particle", "vec3 velocity", asOFFSET(Particle, mVelocity));
        engine->RegisterObjectProperty("Particle", "float lifetime", asOFFSET(Particle, mLifetime));
    }
}