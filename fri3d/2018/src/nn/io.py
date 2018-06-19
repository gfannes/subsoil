from gubg.neural.simulator import Simulator
from gubg.neural.mlp import MLP as gubg_MLP
import bimpy
import numpy as np

NR = 100
def ix2f(ix):
    v = 3.0
    return (ix/NR*2-1)*v
mm = (ix2f(0), ix2f(NR-1))

class Perceptron:
    class ImState:
        def __init__(self):
            self.input = bimpy.Float(0.0)
            self.weight = bimpy.Float(0.0)
            self.bias = bimpy.Float(0.0)
            self.output = bimpy.Float(0.0)

    def __init__(self):
        self.simulator = Simulator()
        self.imstate = self.ImState()

        simulator = self.simulator
        self.input = simulator.add_external(1)
        self.bias  = simulator.add_external(1)
        self.output, self.weight = simulator.add_neuron("tanh", [self.input, self.bias])
        self.states = np.zeros(simulator.nr_states)
        self.weights = np.zeros(simulator.nr_weights)
        self.states[self.bias] = 1.0

    def imgui(self, ctx):
        bimpy.text("Nr states: {a}, nr weight: {b}".format(a=self.simulator.nr_states, b=self.simulator.nr_weights))
        im = self.imstate

        bimpy.slider_float("Weight", im.weight, *mm)
        self.weights[self.weight] = im.weight.value

        bimpy.slider_float("Bias", im.bias, *mm)
        self.weights[self.bias] = im.bias.value

        bimpy.slider_float("Input", im.input, *mm)

        self._compute_outputs()
        bimpy.plot_lines("", self._outputs, 0, "", -1.0, 1.0, bimpy.Vec2(0,100))

        bimpy.same_line()

        im.output.value = self._compute_output(im.input.value)
        bimpy.v_slider_float("Output", bimpy.Vec2(40,100), im.output, -1.0, 1.0)

    def _compute_output(self, input):
        self.states[self.input] = input
        self.simulator.forward(self.states, self.weights)
        return self.states[self.output]
    def _compute_outputs(self):
        self._outputs = [self._compute_output(ix2f(ix)) for ix in range(NR)]

class MLP:
    def __init__(self):
        self.mlp = gubg_MLP(1)
        self.layer = bimpy.Int(0)
        self.layer_names = ["abc", "def"]
    def imgui(self, cxt):
        if (bimpy.combo("Layer", self.layer, self.layer_names)):
            pass
