class Particle:
    def __init__( self, x = 0, y = 0,dx = 0, dy = 0,  m = 0, r = 0):
        self.x = x
        self.y = y
        self.dx = dx
        self.dy = dy
        self.r = r
        self.m = m
    def __repr__(self):
        return "{{x:{}, y:{}, dx:{}, dy:{}, m:{}, r:{}}}".format(
            self.x , self.y , self.dx , self.dy , self.m , self.r 
            )

class ParticleCollideEvent:
    def __init__(self, particles = None, time = 0):
        self.particles = [] if particles == None else particles
        self.time = time

class SimulationData:
    def __init__(self):
        self.num_particles = 0
        self.simulation_side_x = 0
        self.simulation_side_y = 0
        self.events = []

    def __repr__(self):
        return "{{ num_particles:{}, simulation_side_x:{}, simulation_side_y:{} }}".format(
            self.num_particles,
            self.simulation_side_x,
            self.simulation_side_y
        )
