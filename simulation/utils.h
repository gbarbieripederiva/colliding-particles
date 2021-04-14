#ifndef __MY_UTILS_H__
#define __MY_UTILS_H__
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include "particle/particle.h"
#include "particleCollideEvent/particleCollideEvent.h"

double randomDouble(double min = 0, double max = 1)
{
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}

std::vector<Particle> generateParticles(int qty, double sideX, double sideY,double minMass = 0, double maxMass = 0, double minRadius = 0, double maxRadius = 0, double minVel = 0, double maxVel = 0, long seed = -1)
{
    auto vp = std::vector<Particle>();

    unsigned int realSeed = 0;
    if (seed < 0)
    {
        realSeed = (unsigned)std::time(NULL);
    }
    else
    {
        realSeed = seed % UINTMAX_MAX;
    }
    srand(realSeed);

    for (int i = 0; i < qty; i++)
    {
        bool added = false;
        for (int retries = 0; retries < 10 && !added; retries++)
        {
            Particle p = Particle(i,
                                randomDouble(minMass,maxMass),
                                randomDouble(minRadius, maxRadius),
                                randomDouble(0, sideX), randomDouble(0, sideY),
                                randomDouble(minVel, maxVel), randomDouble(minVel, maxVel));
            bool superimposed = false;
            for (int j = 0; j < vp.size() && !superimposed; j++)
            {
                if (pow(p.distanceTo(vp[j]), 2) <= pow(vp[j].getRadius() + p.getRadius(), 2))
                {
                    superimposed = true;
                }
            }
            if (!superimposed)
            {
                vp.push_back(p);
                added = true;
            }
        }
    }
    return vp;
}

void initEventVector(double simSideX, double simSideY, std::vector<Particle> &particles, std::vector<ParticleCollideEvent> &events)
{
    
}

void printState(std::vector<Particle> &particles, std::ostream &os = std::cout)
{
    os << particles.size();
    for (Particle p : particles)
    {
        os << p.getX() << " " << p.getY() << " " << p.getDX() << " " << p.getDY() << std::endl;
    }
}

std::vector<Particle> getParticles(std::istream &f){
    std::vector<Particle> vec;
    int id = 0;
    double x,y,dx,dy;
    while (f >> x >> y >> dx >> dy)
    {
        vec.push_back(Particle(id,x,y,dx,dy));
        id++;
    }
    return vec;
}

#endif