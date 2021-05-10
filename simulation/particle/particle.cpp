#include "particle.h"

// operators
bool Particle::operator==(const Particle &p2) const
{
    return p2.id == this->id;
}

double Particle::distanceTo(const Particle &p) const
{
    return sqrt(pow(this->getX() - p.getX(), 2) + pow(this->getY() - p.getY(), 2));
}

double Particle::collideX(double xWallLeft, double xWallRight) const
{
    if( this->dx == 0 ){
        return -1;
    }
    return this->dx > 0 ? (xWallRight - this->r - this->x) / this->dx : (xWallLeft + this->r - this->x) / this->dx;
}
double Particle::collideY(double yWallDown, double yWallUp) const
{
    if( this->dy == 0 ){
        return -1;
    }
    return this->dy > 0 ? (yWallUp - this->r - this->y) / this->dy : (yWallDown + this->r - this->y) / this->dy;
}

/* Dot product of two arrays of two doubles */
double dotProductCollide(double *v1, double *v2, int dim = 2)
{
    double res = 0;
    for (int i = 0; i < dim; i++)
    {
        res += v1[i] * v2[i];
    }
    return res;
}

double Particle::collide(const Particle &p) const
{
    double deltaR[2] = {p.x - this->x, p.y - this->y};
    double deltaV[2] = {p.dx - this->dx, p.dy - this->dy};
    double deltaVdotdeltaR = dotProductCollide(deltaV, deltaR);
    if (deltaVdotdeltaR >= 0)
        return -1;

    double deltaVdotdeltaV = dotProductCollide(deltaV, deltaV);
    double deltaRdotdeltaR = dotProductCollide(deltaR, deltaR);
    double sigma = p.r + this->r;
    double d = pow(deltaVdotdeltaR, 2) - (deltaVdotdeltaV * (deltaRdotdeltaR - pow(sigma, 2)));

    if (d < 0)
        return -1;

    return -(deltaVdotdeltaR + sqrt(d)) / (deltaVdotdeltaV);
}

void Particle::bounceX()
{
    this->dx = -this->dx;
    this->cc++;
}
void Particle::bounceY()
{
    this->dy = -this->dy;
    this->cc++;
}
void Particle::bounce(Particle &p)
{
    double deltaVdotdeltaR = (this->dx - p.dx) * (this->dy - p.dy) + (this->x - p.x) * (this->y - p.y);
    double sigma = this->r + p.r;
    // ((2*mi*mj*avar)/(sigma*(mi+mj)))
    double j = (2 * this->m * p.m * deltaVdotdeltaR) / (sigma * (this->m + p.m));
    double jx = (j * (this->x - p.x)) / sigma;
    double jy = (j * (this->y - p.y)) / sigma;

    this->dx = this->dx + jx / this->m;
    this->dy = this->dy + jy / this->m;
    p.dx = p.dx - jx / p.m;
    p.dy = p.dy - jy / p.m;

    p.cc++;
    this->cc++;
}

void Particle::advanceWithNoCollision(double time){
    this->x = this->x + time*this->dx;
    this->y = this->y + time*this->dy;
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
