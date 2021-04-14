import matplotlib.pyplot as plt
import data_parser as data_parser
import matplotlib.collections
import sys
import argparse

argp = argparse.ArgumentParser(description="Particle collision visualizer")
argp.add_argument('--particle-file-path', dest='particle_path', help='particle file path')
argp.add_argument('--output-file-path', dest='output_path', help='output file path')
args = argp.parse_args()

sim_data = data_parser.data_getter(
    particle_file_path=args.particle_path,
    output_file_path=args.output_path
    )

def show_particles():
    patches = [plt.Circle((p.x,p.y), p.r) for p  in  sim_data.events[0].particles]
    ax = plt.gca()
    ax.clear()
    ax.figure.set_size_inches(14,10)
    coll = matplotlib.collections.PatchCollection(patches, facecolors="black")
    ax.add_collection(coll)
    ax.margins(0.01)
    ax.figure.canvas.draw()
        
show_particles()
plt.show()