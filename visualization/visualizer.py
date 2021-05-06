import matplotlib.pyplot as plt
import data_parser as data_parser
from matplotlib.animation import FuncAnimation
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

def get_circles(i):
    global sim_data
    circles = []
    for p in sim_data.events[i].particles:
        circles.append(plt.Circle((p.x,p.y),p.r))
    return circles

def show_particles(i):
    global ax
    global coll
    patches = get_circles(i)
    coll.set_paths(patches)
    ax.set_title(f"Frame {i}", fontdict={'fontsize': 20})
    return coll

ax = plt.gca()
ax.figure.set_size_inches(14,10)
ax.margins(0.01)
patches = get_circles(0)
coll = matplotlib.collections.PatchCollection(patches)
ax.add_collection(coll)

plt.xlim([0, sim_data.simulation_side_x])
plt.ylim([0, sim_data.simulation_side_y])

ani = FuncAnimation(plt.gcf(),show_particles,frames=len(sim_data.events),blit=False)

plt.show()