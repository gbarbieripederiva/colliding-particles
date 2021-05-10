import copy
import math

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
        self.curr_event = 0
        self.curr_time = 0
        self.delta_time = 0
        self.particles = []

    def __repr__(self):
        return "{{ num_particles:{}, simulation_side_x:{}, simulation_side_y:{} }}".format(
            self.num_particles,
            self.simulation_side_x,
            self.simulation_side_y
        )

    def reset_particles(self):
        self.particles = self.events[0].particles
        self.curr_time = 0
        self.curr_event = 0

    def update_particles(self):
        # delta time without events
        dt_without_e = self.delta_time
        time_changed = False
        while (self.curr_event + 1 < len(self.events) 
            and self.events[self.curr_event + 1].time < self.curr_time + self.delta_time):

                time_changed = True
                self.curr_event += 1
        
        if time_changed:
            self.particles = copy.deepcopy(self.events[self.curr_event].particles)
            dt_without_e = (self.curr_time + self.delta_time) - self.events[self.curr_event].time
            
        for i,p in enumerate(self.particles):
            self.particles[i].x += self.particles[i].dx * dt_without_e
            self.particles[i].y += self.particles[i].dy * dt_without_e
        
        self.curr_time += self.delta_time
    
    def get_frames(self):
        return math.ceil(self.events[-1].time/self.delta_time)