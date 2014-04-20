//
//  Particle.h
//  ParticleToy
//
//  Created by Adam on 4/18/14.
//
//

#pragma once

#include "Globals.h"

namespace ptoy
{
    class Particle
    {
    public:
        Particle();
        ~Particle();
        
    private:
        glm::vec3 mPosition;
        float mSize;
        glm::vec4 mColor;
        glm::vec3 mVelocity;
        uint32_t mLifetime;
    };
};


