import gubg.neural
import bimpy
import numpy as np

class ImState:
    input = bimpy.Float(0.0)
    weight = bimpy.Float(0.0)
    output = bimpy.Float(0.0)

class IO:
    network = gubg.neural.Network()
    imstate = ImState()

    def __init__(self):
        network = self.network
        self.input = network.add_external(1)
        self.bias  = network.add_external(1)
        self.output, self.weight = network.add_neuron("tanh", [self.input, self.bias])
        self.states = np.zeros(network.nr_states)
        self.weights = np.zeros(network.nr_weights)
        self.states[self.bias] = 1.0

    def imgui(self, ctx):
        bimpy.show_test_window()
        return
        bimpy.text("Nr states: {a}, nr weight: {b}".format(a=self.network.nr_states, b=self.network.nr_weights))
        im = self.imstate
        bimpy.slider_float("Input", im.input, -2.0, 2.0)
        bimpy.slider_float("Weight", im.weight, -2.0, 2.0)
        self.states[self.input] = im.input.value
        self.weights[self.weight] = im.weight.value
        self.network.forward(self.states, self.weights)
        im.output.value = self.states[self.output]
        bimpy.slider_float("Output", im.output, -1.0, 1.0)
