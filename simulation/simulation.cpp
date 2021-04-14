#include <iostream>
#include <fstream>
#include <filesystem>
#include <queue>

#include "particle/particle.h"
#include "particleCollideEvent/particleCollideEvent.h"
#include "simData.h"
#include "utils.h"

#define PARTICLE_FILENAME "particles.txt"
#define OUTPUT_FILENAME "output.txt"
#define DATA_PATH "./data"

int main(int argc, char const *argv[])
{
    namespace fs = std::filesystem;
    SimData simData = SimData(10,10,2);

    std::string dataPath = DATA_PATH;
    if (argc > 1)
    {
        dataPath = argv[1];
    }

    fs::path particlePath = fs::path(dataPath) / fs::path(PARTICLE_FILENAME);
    std::vector<Particle *> particles;
    if (fs::exists(particlePath))
    {
        std::fstream particleFile;
        particleFile.open(particlePath, std::ios::in);
        particles = getParticles(particleFile);
    }
    else
    {
        fs::create_directories(dataPath);
        std::ofstream particleFile;
        particleFile.open(particlePath);
        particles = generateParticles(10, simData, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
        printState(particles, particleFile);
        particleFile.close();
    }

    std::ofstream outputfile;
    outputfile.open(dataPath / fs::path(OUTPUT_FILENAME), std::ios::trunc);
    printState(particles, outputfile);
    std::priority_queue<ParticleCollideEvent,std::vector<ParticleCollideEvent>,ParticleCollideEvent::TimeComparator> events;
    initEventVector(simData,particles,events);
    ParticleCollideEvent lastEvent = ParticleCollideEvent();
    lastEvent.setTime(0);
    for (int i = 0; i < simData.eventsToProcess; i++)
    {
        lastEvent = advanceEvents(simData,particles,events,lastEvent.getTime());
        outputfile << std::endl;
        printState(particles, outputfile);
    }
    outputfile.close();
    
    // final cleaning
    for (Particle *p:particles)
    {
        delete p;
    }
    
    return 0;
}
