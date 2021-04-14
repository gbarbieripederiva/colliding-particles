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
#define LOG_FILENAME "log.txt"
#define DATA_PATH "./data"

#define LOG2_FILENAME "log2.txt"

int main(int argc, char const *argv[])
{
    namespace fs = std::filesystem;
    SimData simData = SimData(10,10,10);

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
        particleFile >> simData.simSideX >> simData.simSideY;
        particles = getParticles(particleFile);
    }
    else
    {
        fs::create_directories(dataPath);
        std::ofstream particleFile;
        particleFile.open(particlePath);
        particles = generateParticles(10, simData, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
        particleFile << simData.simSideX << " " << simData.simSideY << std::endl;
        printState(particles, particleFile, 0);
        particleFile.close();
    }

    std::ofstream outputfile;
    outputfile.open(dataPath / fs::path(OUTPUT_FILENAME), std::ios::trunc);

    std::ofstream logfile;
    logfile.open(dataPath / fs::path(LOG_FILENAME), std::ios::trunc);

    #ifdef LOG2_FILENAME
    std::ofstream logfile2;
    logfile2.open(dataPath / fs::path("log2.txt"), std::ios::trunc);
    #endif

    printState(particles, outputfile,0);
    
    std::priority_queue<ParticleCollideEvent,std::vector<ParticleCollideEvent>,ParticleCollideEvent::TimeComparator> events;
    initEventVector(simData,particles,events);
    
    ParticleCollideEvent lastEvent = ParticleCollideEvent();
    lastEvent.setTime(0);
    for (int i = 0; i < simData.eventsToProcess; i++)
    {
        lastEvent = advanceEvents(simData,particles,events,lastEvent.getTime());
        outputfile << std::endl;
        logfile << "Event " << i << ": " << lastEvent << std::endl;
        
        #ifdef LOG2_FILENAME
        std::priority_queue<ParticleCollideEvent,std::vector<ParticleCollideEvent>,ParticleCollideEvent::TimeComparator> copy = std::priority_queue<ParticleCollideEvent,std::vector<ParticleCollideEvent>,ParticleCollideEvent::TimeComparator>(events);
        while(!copy.empty()){
            logfile2 << copy.top() << std::endl;
            copy.pop();
        }
        logfile2 << std::endl;
        #endif
        
        printState(particles, outputfile,lastEvent.getTime());
    }
    
    #ifdef LOG2_FILENAME
    logfile2.close();
    #endif

    logfile.close();
    outputfile.close();
    
    // final cleaning
    for (Particle *p:particles)
    {
        delete p;
    }
    
    return 0;
}
