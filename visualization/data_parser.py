import os
from particle_system import *

DATA_PATH = "../data"
PARTICLE_FILE = "particles.txt"
OUTPUT_FILE = "output.txt"

def particle_file_getter(sim_data,particle_file_path):
    pfile = open(particle_file_path,'r')
    line = pfile.readline()
    line = line.strip().split(" ")
    sim_data.simulation_side_x = float(line[0])
    sim_data.simulation_side_y = float(line[1])
    sim_data.num_particles = int(pfile.readline().strip().split(" ")[0])

    pfile.close()


def output_file_getter(sim_data,output_file_path):
    # open output file
    ofile = open(output_file_path,'r')

    # parse particles
    line = ofile.readline()
    while line:
        pce = ParticleCollideEvent()
        
        line = line.strip().split(" ")
        particles = int(line[0])
        pce.time = float(line[1])
        
        line = ofile.readline()
        while line.__len__() > 0 and not line.isspace():
            line = line.strip().split(" ")
            pce.particles.append(Particle(
                float(line[0]),
                float(line[1]),
                float(line[2]),
                float(line[3]),
                float(line[4]),
                float(line[5])
            ))
            line = ofile.readline()
        
        sim_data.events.append(pce)

        # advance loop
        line = ofile.readline()
        
    # close file
    ofile.close()


def data_getter( particle_file_path = None,output_file_path = None ):
    if particle_file_path == None:
        particle_file_path = os.path.join(DATA_PATH,PARTICLE_FILE)
    if output_file_path == None:
        output_file_path = os.path.join(DATA_PATH,OUTPUT_FILE)

    data = SimulationData()
    particle_file_getter( data, particle_file_path )
    output_file_getter( data, output_file_path )
    return data
