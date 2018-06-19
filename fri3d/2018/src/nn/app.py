import bimpy
from . import io

class Modes:
    names = ["<mode>", "Single Perceptron", "Multi-layer Perceptron", "Learn NN", "Genetic NN"]
    classes = [None, io.Perceptron, io.MLP, None, None]

    def __init__(self):
        self.ix = bimpy.Int(0)
        self.mode = None
    def __str__(self):
        return Modes.names[self.ix.value]
    def update(self):
        t = Modes.classes[self.ix.value]
        self.mode = None
        if (t is not None):
            self.mode = t()
    def imgui(self, ctx):
        if (self.mode is None):
            bimpy.text("This mode is not supported.")
            return
        self.mode.imgui(ctx)

def run():
    ctx = bimpy.Context()
    ctx.init(600, 600, "Neural Network")

    modes = Modes()
    modes.ix.value = 1
    modes.update()
    selection = bimpy.Bool(True)
    while (not ctx.should_close()):
        with ctx:
            bimpy.begin("Control Center")
            if (bimpy.collapsing_header("Mode selection ({str})".format(str=str(modes)))):
                if bimpy.combo("Selected mode", modes.ix, Modes.names):
                    modes.update()
                    print("Mode changed to {str}".format(str=str(modes)))
            if (bimpy.collapsing_header("Mode processing")):
                modes.imgui(ctx)
            bimpy.end()
