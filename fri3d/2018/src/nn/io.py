from gubg.neural.simulator import Simulator
from gubg.neural.mlp import MLP as gubg_MLP
import bimpy
import numpy as np

NR = 100
def ix2f(ix):
    v = 3.0
    return (ix/NR*2-1)*v
mm = (ix2f(0), ix2f(NR))

class Perceptron:
    class UI:
        def __init__(self):
            self.input = bimpy.Float(0.0)
            self.weight = bimpy.Float(0.0)
            self.bias = bimpy.Float(0.0)
            self.output = bimpy.Float(0.0)

    def __init__(self):
        self.simulator = Simulator()
        self.ui = self.UI()

        self.input = self.simulator.add_external(1)
        self.bias  = self.simulator.add_external(1)
        self.output, self.weight = self.simulator.add_neuron("tanh", [self.input, self.bias])
        self.states = np.zeros(self.simulator.nr_states)
        self.weights = np.zeros(self.simulator.nr_weights)
        self.states[self.bias] = 1.0

    def show(self, ctx):
        bimpy.text("Nr states: {a}, nr weight: {b}".format(a=self.simulator.nr_states, b=self.simulator.nr_weights))
        ui = self.ui

        bimpy.slider_float("Weight", ui.weight, *mm)
        self.weights[self.weight] = ui.weight.value

        bimpy.slider_float("Bias", ui.bias, *mm)
        self.weights[self.bias] = ui.bias.value

        bimpy.slider_float("Input", ui.input, *mm)

        self._compute_outputs()
        bimpy.plot_lines("", self.outputs, 0, "", -1.0, 1.0, bimpy.Vec2(0,100))

        bimpy.same_line()

        ui.output.value = self._compute_output(ui.input.value)
        bimpy.v_slider_float("Output", bimpy.Vec2(40,100), ui.output, -1.0, 1.0)

    def _compute_output(self, input):
        self.states[self.input] = input
        self.simulator.forward(self.states, self.weights)
        return self.states[self.output]
    def _compute_outputs(self):
        self.outputs = [self._compute_output(ix2f(ix)) for ix in range(NR+1)]

class MLP:
    class UI:
        def __init__(self):
            self.nr = bimpy.Int(1)
            self.transfer = bimpy.Int(1)
            self.transfer_names = ["linear", "tanh"]
            self.layer = bimpy.Int(0)

    def __init__(self):
        self.ui = self.UI()
        self.mlp = gubg_MLP(1)
        self.simulator = None
    def show(self, cxt):
        ui = self.ui

        bimpy.push_item_width(80)
        bimpy.input_int("nr neurons", ui.nr)
        bimpy.same_line()
        bimpy.combo("transfer", ui.transfer, ui.transfer_names)
        if (ui.nr.value > 0):
            bimpy.same_line()
            if (bimpy.button("add layer")):
                transfer = ui.transfer_names[ui.transfer.value]
                self.mlp.add_layer(transfer, ui.nr.value)
                self.simulator = None
        bimpy.pop_item_width()

        if (not self.mlp.layers):
            bimpy.text("Warning: you need at least one layer")
            return

        names = ["{ix} {str}".format(ix=ix, str=str(layer)) for ix,layer in enumerate(self.mlp.layers)]
        bimpy.combo("Layer", ui.layer, names)

        layer_ix = ui.layer.value
        if (layer_ix < len(self.mlp.layers)):
            layer = self.mlp.layers[layer_ix]

        if (self.mlp.layers[-1].nr_outputs != 1):
            bimpy.text("Warning: last layer should have only one output")
            return

        if (not self.simulator):
            self.simulator = Simulator()
            self.mlp.setup_simulator(self.simulator)
            self.states = np.zeros(self.simulator.nr_states)
            self.weights = np.zeros(self.simulator.nr_weights)
            self.output = self.simulator.nr_states-1
            self.states[self.mlp.bias] = 1.0
        bimpy.text(str(self.simulator))

        self._compute_outputs()
        print(self.outputs)
        bimpy.plot_lines("Function", self.outputs)

    def _compute_output(self, input):
        self.states[self.mlp.input0] = input
        self.simulator.forward(self.states, self.weights)
        return self.states[self.output]
    def _compute_outputs(self):
        self.outputs = [self._compute_output(ix2f(ix)) for ix in range(NR+1)]

