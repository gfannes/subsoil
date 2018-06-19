from . import util
from gubg.neural.simulator import Simulator
from gubg.neural.mlp import MLP as gubg_MLP
from abc import ABC, abstractmethod
import numpy as np
import bimpy

class Interface(ABC):
    def __init__(self):
        print("ABC")
        self.simulator = None
        self.input_six = None
        self.bias_six = None
        self.output_six = None

    @abstractmethod
    def _str(self): pass
    @abstractmethod
    def _show(self, ctx): pass
    @abstractmethod
    def _setup(self, simulator): pass

    def __str__(self):
        str = "Model: {m}".format(m=self._str())
        if (self.simulator is not None):
            str += ", NrStates: {s}, NrWeights: {w}".format(s=self.simulator.nr_states, w=self.simulator.nr_weights)
        return str
    def show(self, ctx):
        self._show(ctx)
    def setup(self):
        if (self.simulator):
            # Nothing todo
            return True

        simulator = Simulator()
        if (not self._setup(simulator)):
            return False
        self.simulator = simulator
        
        assert self.input_six is not None
        assert self.bias_six is not None
        assert self.output_six is not None

        self.states = np.zeros(self.simulator.nr_states)
        self.weights = np.zeros(self.simulator.nr_weights)
        return True

    @property
    def input(self): return self.states[self.input_six]
    @input.setter
    def input(self, value): self.states[self.input_six] = value

    @property
    def output(self): return self.states[self.output_six]

    def compute_output(self, input):
        self.states[self.bias_six] = 1.0
        self.input = input
        self.simulator.forward(self.states, self.weights)
        return self.output
    def compute_outputs(self, inputs):
        return [self.compute_output(input) for input in inputs]

class Perceptron(Interface):
    class UI:
        def __init__(self):
            self.weight = bimpy.Float(0.0)
            self.bias = bimpy.Float(0.0)

    def __init__(self):
        super().__init__()
        self.ui = self.UI()

    @property
    def weight(self): return self.weights[self.weight_wix]
    @weight.setter
    def weight(self, value): self.weights[self.weight_wix] = value

    @property
    def bias(self): return self.weights[self.bias_wix]
    @bias.setter
    def bias(self, value): self.weights[self.bias_wix] = value

    # Interface implementation
    def _str(self): return "Perceptron"
    def _show(self, ctx):
        uif = bimpy.Float()

        uif.value = self.weight
        bimpy.slider_float("Weight", uif, *util.mm)
        self.weight = uif.value

        uif.value = self.bias
        bimpy.slider_float("Bias", uif, *util.mm)
        self.bias = uif.value

    def _setup(self, simulator):
        self.input_six = simulator.add_external(1)
        self.bias_six  = simulator.add_external(1)
        self.output_six, self.weight_wix = simulator.add_neuron("tanh", [self.input_six, self.bias_six])
        self.bias_wix = self.weight_wix+1
        return True

class MLP(Interface):
    class UI:
        def __init__(self):
            self.layer_ix = bimpy.Int(0)
            self.neuron_ix = bimpy.Int(0)

    def __init__(self):
        super().__init__()
        self.ui = self.UI()
        self.mlp = gubg_MLP(1)

    def add_layer(self, transfer, nr_neurons):
        self.mlp.add_layer(transfer, nr_neurons)
        self.simulator = None

    # Interface implementation
    def _str(self): return "MLP"
    def _show(self, ctx):
        # Show/adjust the weights
        if (not self.mlp.layers):
            bimpy.text("Warning: you need at least one layer")
            return

        # Select layer
        names = ["{ix} {str}".format(ix=ix, str=str(layer)) for ix,layer in enumerate(self.mlp.layers)]
        if (bimpy.combo("Layer", self.ui.layer_ix, names)):
            self.ui.neuron_ix.value = 0
        layer_ix = self.ui.layer_ix.value
        if (layer_ix < len(self.mlp.layers)):
            layer = self.mlp.layers[layer_ix]
            # Select neuron
            names = ["{ix}".format(ix=ix) for ix,neuron in enumerate(layer.neurons)]
            bimpy.combo("Neuron", self.ui.neuron_ix, names)
            neuron_ix = self.ui.neuron_ix.value
            if (neuron_ix < len(layer.neurons)):
                neuron = layer.neurons[neuron_ix]
                weights = [bimpy.Float(self.weights[neuron.weight_offset+ix]) for ix,_ in enumerate(neuron.input_ixs)]
                for ix,weight in enumerate(weights):
                    bimpy.slider_float("Weight {ix}".format(ix=ix), weight, *util.mm)
                    self.weights[neuron.weight_offset+ix] = weight.value

        if (self.mlp.layers[-1].nr_neurons != 1):
            bimpy.text("Warning: last layer should have only one output")
            return

        if (not self.setup()):
            return
            

    def _setup(self, simulator):
        if (not self.mlp.layers):
            return False
        if (self.mlp.layers[-1].nr_neurons != 1):
            return False
        self.mlp.setup_simulator(simulator)
        self.input_six  = self.mlp.input_offset
        self.bias_six   = self.mlp.bias
        self.output_six = simulator.nr_states-1
        return True
