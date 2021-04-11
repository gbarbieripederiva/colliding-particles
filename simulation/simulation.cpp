#include <iostream>
#include <fstream>
#include <filesystem>

#include "particle/particle.h"
#include "particleCollideEvent/particleCollideEvent.h"
#include "utils.h"
#include "fileHandlers.h"

#define PARTICLE_FILENAME "particles.txt"
#define OUTPUT_FILENAME "output.txt"
#define DATA_PATH "./data"

int main(int argc, char const *argv[])
{
    namespace fs = std::filesystem;

    const double SIM_SIDE_X = 10;
    const double SIM_SIDE_Y = 10;
    const int EVENT_TO_PROCESS = 2;

    std::string dataPath = DATA_PATH;
    if (argc > 1)
    {
        dataPath = argv[1];
    }

    fs::path particlePath = fs::path(dataPath) / fs::path(PARTICLE_FILENAME);
    std::vector<Particle> particles;
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
        particles = generateParticles(10, SIM_SIDE_X, SIM_SIDE_Y, 0.1, 0.1, 0.1, 0.1);
        saveParticles(particleFile, particles);
        particleFile.close();
    }

    std::ofstream outputfile;
    outputfile.open(dataPath/fs::path(OUTPUT_FILENAME),std::ios::trunc);
    printState(particles,outputfile);
    for (int i = 0; i < EVENT_TO_PROCESS; i++)
    {
        ParticleCollideEvent nextEvent = calculateNextCollision(SIM_SIDE_X,SIM_SIDE_Y, particles);
        advanceParticlesToNextEvent(particles,nextEvent);
        outputfile << std::endl;
        printState(particles,outputfile);
    }
    outputfile.close();
    
    return 0;
}
