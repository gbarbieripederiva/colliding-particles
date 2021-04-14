#ifndef __MY_UTILS_H__
#define __MY_UTILS_H__
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <queue>
#include "particle/particle.h"
#include "particleCollideEvent/particleCollideEvent.h"
#include "simData.h"

double randomDouble(double min = 0, double max = 1)
{
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}

std::vector<Particle *> generateParticles(int qty,const SimData &simData, double minMass = 0, double maxMass = 0, double minRadius = 0, double maxRadius = 0, double minVel = 0, double maxVel = 0, long seed = -1)
{
    auto vp = std::vector<Particle *>();

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
                                  randomDouble(minMass, maxMass),
                                  randomDouble(minRadius, maxRadius),
                                  randomDouble(0, simData.simSideX), randomDouble(0, simData.simSideY),
                                  randomDouble(minVel, maxVel), randomDouble(minVel, maxVel));
            bool superimposed = false;
            for (int j = 0; j < vp.size() && !superimposed; j++)
            {
                if (pow(p.distanceTo(*(vp[j])), 2) <= pow(vp[j]->getRadius() + p.getRadius(), 2))
                {
                    superimposed = true;
                }
            }
            if (!superimposed)
            {
                vp.push_back(new Particle(p));
                added = true;
            }
        }
    }
    return vp;
}

void initEventVector(
    const SimData &simData,
    std::vector<Particle *> &particles,
    std::priority_queue<
        ParticleCollideEvent,
        std::vector<ParticleCollideEvent>,
        ParticleCollideEvent::TimeComparator> &events)
{
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = i; j < particles.size(); j++)
        {
            if (i == j)
            {
                double tx = particles[i]->collideX(0, simData.simSideX);
                if (tx >= 0)
                {
                    events.push(ParticleCollideEvent(tx, particles[i],ParticleCollideEventConstants::WALL_X));
                }
                double ty = particles[i]->collideX(0, simData.simSideY);
                if (ty >= 0)
                {
                    events.push(ParticleCollideEvent(ty, particles[i],ParticleCollideEventConstants::WALL_Y));
                }
            }
            else
            {
                double t = particles[i]->collide(*(particles[j]));
                if(t >= 0){
                    events.push(ParticleCollideEvent(t,particles[i],particles[j]));
                }
            }
        }
    }
}

ParticleCollideEvent advanceEvents(
    const SimData &simData,
    std::vector<Particle *> &particles,
    std::priority_queue<
        ParticleCollideEvent,
        std::vector<ParticleCollideEvent>,
        ParticleCollideEvent::TimeComparator> &events,
        double lastEventTime){
    
    ParticleCollideEvent res;
    bool finished = false;
    while (!finished)
    {
        if(events.empty()){
            res = ParticleCollideEvent();
            finished = true;
            continue;
        }
        res = ParticleCollideEvent(events.top());
        events.pop();
        for (int i = 0; i < particles.size(); i++)
        {
            particles[i]->advanceWithNoCollision(res.getTime() - lastEventTime);
        }
        if(!res.getNoEvent()){
            if(res.getCollisionCountP1()==res.getP1()->getCollisionCount()){
                if(res.getIsWall()){
                    if(res.getWall()==ParticleCollideEventConstants::WALL_X){
                        res.getP1()->bounceX();
                    }else{
                        res.getP1()->bounceY();
                    }
                    // add all new events for particle p1
                    for (int i = 0; i < particles.size(); i++)
                    {
                        if (particles[i]->getID() == res.getP1()->getID())
                        {
                            double tx = res.getP1()->collideX(0, simData.simSideX);
                            if (tx >= 0)
                            {
                                events.push(ParticleCollideEvent(tx, res.getP1(),ParticleCollideEventConstants::WALL_X));
                            }
                            double ty = res.getP1()->collideX(0, simData.simSideY);
                            if (ty >= 0)
                            {
                                events.push(ParticleCollideEvent(ty, res.getP1(),ParticleCollideEventConstants::WALL_Y));
                            }
                        }
                        else
                        {
                            double t = res.getP1()->collide(*(particles[i]));
                            if(t >= 0){
                                events.push(ParticleCollideEvent(t,res.getP1(),particles[i]));
                            }
                        }
                    }
                    finished = true;
                } else{
                    if(res.getCollisionCountP2() == res.getP2()->getCollisionCount()){
                        res.getP1()->bounce(*res.getP2());
                        // Add all new events for particles 1 and 2
                        for (int i = 0; i < particles.size(); i++)
                        {
                            if (particles[i]->getID() == res.getP1()->getID())
                            {
                                double tx = res.getP1()->collideX(0, simData.simSideX);
                                if (tx >= 0)
                                {
                                    events.push(ParticleCollideEvent(tx, res.getP1(),ParticleCollideEventConstants::WALL_X));
                                }
                                double ty = res.getP1()->collideX(0, simData.simSideY);
                                if (ty >= 0)
                                {
                                    events.push(ParticleCollideEvent(ty, res.getP1(),ParticleCollideEventConstants::WALL_Y));
                                }
                            }else if (particles[i]->getID() == res.getP2()->getID()){
                                double tx = res.getP2()->collideX(0, simData.simSideX);
                                if (tx >= 0)
                                {
                                    events.push(ParticleCollideEvent(tx, res.getP2(),ParticleCollideEventConstants::WALL_X));
                                }
                                double ty = res.getP2()->collideX(0, simData.simSideY);
                                if (ty >= 0)
                                {
                                    events.push(ParticleCollideEvent(ty, res.getP2(),ParticleCollideEventConstants::WALL_Y));
                                }
                            }
                            else
                            {
                                double t = res.getP1()->collide(*(particles[i]));
                                if(t >= 0){
                                    events.push(ParticleCollideEvent(t,res.getP1(),particles[i]));
                                }
                                double t2 = res.getP2()->collide(*(particles[i]));
                                if(t2 >= 0){
                                    events.push(ParticleCollideEvent(t,res.getP2(),particles[i]));
                                }
                            }
                        }
                        finished = true;
                    }
                }
            }
        }else{
            finished = true;
        }
    }

    // else if p is no event return that
    return res;
}

void printState(std::vector<Particle *> &particles, std::ostream &os = std::cout, double time = 0)
{
    os << particles.size() << " " << time << std::endl;
    for (Particle* p : particles)
    {
        os  << p->getX() << " " 
            << p->getY() << " " 
            << p->getDX() << " " 
            << p->getDY() << " "
            << p->getMass() << " "
            << p->getRadius() << " "
            << 0 << " "
            << 0 << " "
            << 0 << " "<< std::endl;
    }
}

void printState(std::vector<Particle> &particles, std::ostream &os = std::cout, double time = 0)
{
    os << particles.size() << " " << time << std::endl;
    for (Particle p : particles)
    {
        os  << p.getX() << " " 
            << p.getY() << " " 
            << p.getDX() << " " 
            << p.getDY() << " "
            << p.getMass() << " "
            << p.getRadius() << " "
            << 0 << " "
            << 0 << " "
            << 0 << " "<< std::endl;
    }
}

std::vector<Particle *> getParticles(std::istream &f)
{
    int particles = 0;
    double time = 0;
    f >> particles >> time;
    std::vector<Particle *> vec;
    int id = 0;
    double x, y, dx, dy, mass, radius;
    int r,g,b;
    while (f >> x >> y >> dx >> dy >> mass >> radius >> r >> g >> b)
    {
        vec.push_back(new Particle(id, mass, radius, x, y, dx, dy));
        id++;
    }
    return vec;
}

#endif