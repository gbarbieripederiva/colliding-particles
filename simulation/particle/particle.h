#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include <iostream>
#include <string>
#include <math.h>

class Particle
{
private:
    long id = 0;
    double m = 0;
    double r = 0;
    double x = 0;
    double y = 0;
    double dx = 0;
    double dy = 0;
public:
    // Constructors
    Particle();
    Particle(long id,double mass,double radius,double x,double y,double dx = 0,double dy = 0);
    
    // Getters
    long getID() const;
    double getMass() const;
    double getRadius() const;
    double getX() const;
    double getY() const;
    double getDX() const;
    double getDY() const;

    // Setters
    void setID(long id);
    void setMass(double mass);
    void setRadius(double radius);
    void setX(double x);
    void setY(double y);
    void setDX(double dx);
    void setDY(double dy);

    // operators
    bool operator ==(const Particle &p2) const;

    double distanceTo(Particle &p);

};

std::ostream &operator<<(std::ostream &os, Particle const &particle);

#endif