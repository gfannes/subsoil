from gubg.neural.simulator import Simulator
from abc import ABC, abstractmethod
import numpy as np

class Interface(ABC):
    def __init__(self):
        self.simulator = None
        self.input_six = None
        self.bias_six = None
        self.output_six = None

    @abstractmethod
    def __str__(self):
        pass
    @abstractmethod
    def _show(self, ctx):
        pass
    @abstractmethod
    def _setup(self, simulator):
        pass

    def show(self, ctx):
        self._show(ctx)
    def setup(self):
        self.simulator = Simulator()

        self._setup()
        assert self.input_six is not None
        assert self.bias_six is not None
        assert self.output_six is not None

        self.states = np.zeros(self.simulator.nr_states)
        self.weights = np.zeros(self.simulator.nr_weights)

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
    @property
    def weight(self): return self.weights[self.weight_wix]
    @weight.setter
    def weight(self, value): self.weights[self.weight_wix] = value

    @property
    def bias(self): return self.weights[self.bias_wix]
    @bias.setter
    def bias(self, value): self.weights[self.bias_wix] = value

    def __str__(self):
        if (self.simulator is None):
            return "Model: Perceptron, Simulator not valid"
        return "Model: Perceptron, NrStates: {s}, NrWeights: {w}".format(s=self.simulator.nr_states, w=self.simulator.nr_weights)
    def _show(self, ctx):
        pass
    def _setup(self):
        assert self.simulator
        self.input_six = self.simulator.add_external(1)
        self.bias_six  = self.simulator.add_external(1)
        self.output_six, self.weight_wix = self.simulator.add_neuron("tanh", [self.input_six, self.bias_six])
        self.bias_wix = self.weight_wix+1

class MLP(Interface):
    def __str__(self):
        return "MLP"
    def _show(self, ctx):
        pass
    def _setup(self):
        pass
