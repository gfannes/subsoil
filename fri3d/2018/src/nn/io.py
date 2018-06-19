from . import util, model
from gubg.neural.simulator import Simulator
import bimpy
import numpy as np

# Helper function that show the io plot
def show_io_plot(ui_input, model):
    if (not model.simulator):
        return
    bimpy.slider_float("Input", ui_input, *util.mm)

    outputs = model.compute_outputs(util.x_ary())
    bimpy.plot_lines("", outputs, 0, "", -1.0, 1.0, bimpy.Vec2(0,100))

    bimpy.same_line()

    output = bimpy.Float(model.compute_output(ui_input.value))
    bimpy.v_slider_float("Output", bimpy.Vec2(40,100), output, -1.0, 1.0)

class Perceptron:
    class UI:
        def __init__(self):
            self.input = bimpy.Float(0.0)
            self.weight = bimpy.Float(0.0)
            self.bias = bimpy.Float(0.0)

    def __init__(self):
        self.ui = self.UI()
        self.model = model.Perceptron()
        assert self.model.setup()

    def show(self, ctx):
        bimpy.text(str(self.model))
        self.model.show(ctx)
        show_io_plot(self.ui.input, self.model)

class MLP:
    class UI:
        def __init__(self):
            self.nr_neurons = bimpy.Int(1)
            self.transfer = bimpy.Int(1)
            self.transfer_names = ["linear", "tanh"]
            self.input = bimpy.Float()

    def __init__(self):
        self.ui = self.UI()
        self.model = model.MLP()
    def show(self, ctx):
        # Add a layer to the MLP
        bimpy.push_item_width(80)
        bimpy.input_int("nr neurons", self.ui.nr_neurons)
        bimpy.same_line()
        bimpy.combo("transfer", self.ui.transfer, self.ui.transfer_names)
        if (self.ui.nr_neurons.value > 0):
            bimpy.same_line()
            if (bimpy.button("add layer")):
                transfer = self.ui.transfer_names[self.ui.transfer.value]
                self.model.add_layer(transfer, self.ui.nr_neurons.value)
        bimpy.pop_item_width()

        self.model.show(ctx)

        show_io_plot(self.ui.input, self.model)
