#include "particleCollideEvent.h"

// Constructors
ParticleCollideEvent::ParticleCollideEvent() : noEvent(true) {}
ParticleCollideEvent::ParticleCollideEvent(Particle p1, Particle p2, double time)
    : p1(p1), p2(p2), t(time) {}
ParticleCollideEvent::ParticleCollideEvent(Particle p, Wall w, double time):p1(p),w(w),t(time),isWall(true){}

// Getters
Particle ParticleCollideEvent::getP1() const { return this->p1; }
Particle ParticleCollideEvent::getP2() const { return this->p2; }
Wall ParticleCollideEvent::getWall() const { return this->w; }
bool ParticleCollideEvent::getIsWall() const { return this->isWall; }
double ParticleCollideEvent::getTime() const { return this->t; }
bool ParticleCollideEvent::getNoEvent() const { return this->noEvent; }

// Setters
void ParticleCollideEvent::setP1(Particle p1) { this->p1 = p1; }
void ParticleCollideEvent::setP2(Particle p2) { this->p2 = p2; }
void ParticleCollideEvent::setWall(Wall wall) { this->w = wall; }
void ParticleCollideEvent::setIsWall(bool isWall) { this->isWall = isWall; }
void ParticleCollideEvent::setTime(double t) { this->t = t; }
void ParticleCollideEvent::setNoEvent(bool noEvent) { this->noEvent = noEvent; }

/* 
    Calculates velocities after collision. For now its an ellastic collision
    bettween particles of the same mass or an unmovable wall
*/
std::vector<double> ParticleCollideEvent::newVelsAfterEvent() const {
    std::vector<double> vels;
    if (this->getNoEvent())
    {
        return vels;
    }
    if(this->getIsWall()){
        switch (this->getWall())
        {
        case WALL_X_POSITIVO:
        case WALL_X_NEGATIVO:
            vels.push_back( - this->getP1().getDX() );
            vels.push_back(  this->getP1().getDY() );
            break;
        case WALL_Y_POSITIVO:
        case WALL_Y_NEGATIVO:
            vels.push_back(  this->getP1().getDX() );
            vels.push_back( - this->getP1().getDY() );
            break;
        }
    }else{
        // TODO: calculate alfa
        double alfap1 = 0;
        double alfap2 = 0;

        // P1 DX
        vels.push_back(
            -cos(2*alfap1)*this->getP1().getDX() +
            -sin(2*alfap1)*this->getP1().getDY()
        );
        // P1 DY
        vels.push_back(
            -sin(2*alfap1)*this->getP1().getDX() +
            cos(2*alfap1)*this->getP1().getDY() 
        );
        // P2 DX
        vels.push_back(
            -cos(2*alfap2)*this->getP1().getDX() +
            -sin(2*alfap2)*this->getP1().getDX()
        );
        // P2 DY
        vels.push_back(
            -sin(2*alfap2)*this->getP1().getDX() +
            cos(2*alfap2)*this->getP1().getDY() 
        );
    }
    return vels;
}

std::ostream &operator<<(std::ostream &os, ParticleCollideEvent const &pce)
{
    if(pce.getNoEvent()){
        return os   << "{ noEvent:true }";
    }
    else if(pce.getIsWall()){
        return os   << "{ p:" << pce.getP1()
                    << ", wall:" << pce.getWall()
                    << ", time:" << pce.getTime()
                    << " }";
    }else{
        return os   << "{ p1:" << pce.getP1()
                    << ", p2:" << pce.getP2()
                    << ", time:" << pce.getTime()
                    << " }";
    }
}
