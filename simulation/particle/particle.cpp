#include "particle.h"

// Constructors
Particle::Particle() : id(0),r(0), x(0), y(0), dx(0), dy(0) {}
Particle::Particle(long id,double radius, double x, double y, double dx, double dy)
    : id(id),r(radius), x(x), y(y), dx(dx), dy(dy) {}

// Getters
long Particle::getID() const { return this->id; }
long Particle::getRadius() const { return this->r; }
double Particle::getX() const { return this->x; }
double Particle::getY() const { return this->y; }
double Particle::getDX() const { return this->dx; }
double Particle::getDY() const { return this->dy; }

// Setters
void Particle::setID(long id) { this->id = id; }
void Particle::setRadius(double radius) { this->r = radius; }
void Particle::setX(double x) { this->x = x; }
void Particle::setY(double y) { this->y = y; }
void Particle::setDX(double dx) { this->dx = dx; }
void Particle::setDY(double dy) { this->dy = dy; }

double Particle::distanceTo(Particle &p){
    return sqrt(pow(this->getX()-p.getX(),2) + pow(this->getY()-p.getY(),2));
}


std::ostream &operator<<(std::ostream &os, Particle const &particle)
{
    return os << "{ id:" << particle.getID()
              << ", radius:" << particle.getRadius()
              << ", x:" << particle.getX()
              << ", y:" << particle.getY()
              << ", dx:" << particle.getDX()
              << ", dy:" << particle.getDY()
              << " }";
}
