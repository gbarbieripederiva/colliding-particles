#ifndef __FILE_HANDLERS_H__
#define __FILE_HANDLERS_H__
#include <iostream>
#include <vector>
#include "particle/particle.h"

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
void saveParticles(std::ostream &f,std::vector<Particle> const &particles){
    int i = 0;
    int particleSize = particles.size();
    for(auto p : particles){
        f << p.getX() << " " << p.getY() << " " << p.getDX() << " " << p.getDY();
        if (i < particleSize-1)
        {
            f << std::endl;
        }
        i++;
    }
}

#endif