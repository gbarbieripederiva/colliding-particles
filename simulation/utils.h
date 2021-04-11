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

std::vector<Particle> generateParticles(int qty, double sideX, double sideY, double minRadius = 0, double maxRadius = 0, double minVel = 0, double maxVel = 0, long seed = -1)
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
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 0; j < particles.size(); j++)
        {
            if (i != j)
            {
                double dr[2] = {
                    particles[j].getX() - particles[i].getX(),
                    particles[j].getY() - particles[i].getY()};
                double dv[2] = {
                    particles[j].getDX() - particles[i].getDX(),
                    particles[j].getDY() - particles[i].getDY()};

                double dvdotdr = std::inner_product(dv, dv + 2, dr, 0);
                double dvdotdv = std::inner_product(dv, dv + 2, dv, 0);
                double drdotdr = std::inner_product(dr, dr + 2, dr, 0);
                double sigma = particles[i].getRadius() + particles[j].getRadius();
                double d = pow(dvdotdr, 2) - (dvdotdv * (drdotdr - pow(sigma, 2)));

                if (dvdotdr < 0 && d >= 0)
                {
                    events.push_back(ParticleCollideEvent(
                        particles[i],
                        particles[j],
                        -(dvdotdr + sqrt(d)) / dvdotdv));
                }
            }
            else
            {
                if (particles[i].getDX() > 0)
                {
                    events.push_back(ParticleCollideEvent(
                        particles[i],
                        WALL_X_POSITIVO,
                        (simSideX - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX()));
                }
                else if (particles[i].getDX() < 0)
                {
                    events.push_back(ParticleCollideEvent(
                        particles[i],
                        WALL_X_NEGATIVO,
                        (0 - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX()));
                }

                if (particles[i].getDY() > 0)
                {
                    events.push_back(ParticleCollideEvent(
                        particles[i],
                        WALL_Y_POSITIVO,
                        (simSideY - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX()));
                }
                else if (particles[i].getDY() < 0)
                {
                    events.push_back(ParticleCollideEvent(
                        particles[i],
                        WALL_Y_NEGATIVO,
                        (0 - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX()));
                }
            }
        }
    }
}

ParticleCollideEvent calculateNextCollision(double simSideX, double simSideY, std::vector<Particle> &particles)
{
    ParticleCollideEvent res;
    for (int i = 0; i < particles.size(); i++)
    {
        for (int j = 1; j < particles.size(); j++)
        {
            if (i != j)
            {
                double dr[2] = {
                    particles[j].getX() - particles[i].getX(),
                    particles[j].getY() - particles[i].getY()};
                double dv[2] = {
                    particles[j].getDX() - particles[i].getDX(),
                    particles[j].getDY() - particles[i].getDY()};

                double dvdotdr = std::inner_product(dv, dv + 2, dr, 0);
                double dvdotdv = std::inner_product(dv, dv + 2, dv, 0);
                double drdotdr = std::inner_product(dr, dr + 2, dr, 0);
                double sigma = particles[i].getRadius() + particles[j].getRadius();
                double d = pow(dvdotdr, 2) - (dvdotdv * (drdotdr - pow(sigma, 2)));
                double timeToEvent = -(dvdotdr + sqrt(d)) / dvdotdv;

                if (dvdotdr < 0 && d >= 0 && res.getTime() < timeToEvent)
                {
                    res = ParticleCollideEvent(
                        particles[i],
                        particles[j],
                        timeToEvent);
                }
            }
            else
            {
                if (particles[i].getDX() > 0)
                {
                    double timeToEvent = (simSideX - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX();
                    if (timeToEvent)
                    {
                        ParticleCollideEvent(particles[i], WALL_X_POSITIVO, timeToEvent);
                    }
                }
                else if (particles[i].getDX() < 0)
                {
                    double timeToEvent = (0 - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX();
                    if (timeToEvent < res.getTime())
                    {
                        ParticleCollideEvent(particles[i], WALL_X_NEGATIVO, timeToEvent);
                    }
                }

                if (particles[i].getDY() > 0)
                {
                    double timeToEvent = (simSideY - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX();
                    if (timeToEvent < res.getTime())
                    {
                        ParticleCollideEvent(particles[i], WALL_Y_POSITIVO, timeToEvent);
                    }
                }
                else if (particles[i].getDY() < 0)
                {
                    double timeToEvent = (0 - particles[i].getRadius() - particles[i].getX()) / particles[i].getDX();
                    if (timeToEvent < res.getTime())
                    {
                        ParticleCollideEvent(particles[i], WALL_Y_NEGATIVO, timeToEvent);
                    }
                }
            }
        }
    }
    return res;
}

void advanceParticle(Particle &particle, double time)
{
    particle.setX(particle.getX() + time * particle.getDX());
    particle.setY(particle.getY() + time * particle.getDY());
}

void advanceParticlesToNextEvent(std::vector<Particle> &particles, ParticleCollideEvent const &event)
{
    std::vector<double> vels = event.newVelsAfterEvent();
    for (int i = 0; i < particles.size(); i++)
    {
        advanceParticle(particles[i],event.getTime());
    }
    for (int i = 0; i < particles.size(); i++)
    {
        if(!event.getNoEvent()){
            if(event.getP1()==particles[i]){
                particles[i].setDX(vels[0]);
                particles[i].setDY(vels[1]);
            }else if(event.getIsWall() && event.getP2() == particles[i]){
                particles[i].setDX(vels[2]);
                particles[i].setDY(vels[3]);
            }
        }
    }
    
}
void printState(std::vector<Particle> &particles, std::ostream &os = std::cout)
{
    for (Particle p : particles)
    {
        os << p.getX() << " " << p.getY() << " " << p.getDX() << " " << p.getDY() << std::endl;
    }
}

#endif