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
    int cc = 0;

public:
    
    // Constructors
    Particle(){};
    Particle(long id,double mass,double radius, double x, double y, double dx, double dy)
    : id(id),m(mass),r(radius), x(x), y(y), dx(dx), dy(dy) {};
    
    // Getters
    long getID() const { return this->id; };
    double getMass() const { return this->m; };
    double getRadius() const { return this->r; };
    double getX() const { return this->x; };
    double getY() const { return this->y; };
    double getDX() const { return this->dx; };
    double getDY() const { return this->dy; };
    int getCollisionCount() const { return this->cc; };

    // Setters
    void setID(long id) { this->id = id; };
    void setMass(double mass) { this->m = mass; };
    void setRadius(double radius) { this->r = radius; };
    void setX(double x) { this->x = x; };
    void setY(double y) { this->y = y; };
    void setDX(double dx) { this->dx = dx; };
    void setDY(double dy) { this->dy = dy; };
    void setCollisionCoung(int collisionCount) { this->cc = collisionCount; };

    // operators
    bool operator==(const Particle &p2) const;

    double distanceTo(const Particle &p) const;
    double collideX(double xWallLeft,double xWallRight) const;
    double collideY(double yWallDown,double yWallUp) const;
    double collide(const Particle &p) const;

    // update particle because of bouncing against walls
    void bounceX();
    void bounceY();
    /* 
        Update particle because of bouncing against a particle
        This also updates the other particle
    */
    void bounce(Particle &p);

    // update particle n time assuming no collisions in that time
    void advanceWithNoCollision(double time);
};

std::ostream &operator<<(std::ostream &os, Particle const &particle);

#endif