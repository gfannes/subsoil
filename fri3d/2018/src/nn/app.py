import bimpy
from . import io

class App:
    class Sub:
        def __init__(self):
            self.names, self.types = [], []
            self._add("<app>", None)
            self._add("Perceptron", io.Perceptron)
            self._add("Multi-Layer Perceptron", io.MLP)

            self.ix = bimpy.Int(1)
            self.update()
        def __str__(self):
            return self.names[self.ix.value]
        def update(self):
            t = self.types[self.ix.value]
            self.app = None
            if (t is not None):
                self.app = t()
        def show(self, ctx):
            if (bimpy.collapsing_header("Subapp selection ({str})".format(str=str(self)))):
                if bimpy.combo("Selected app", self.ix, self.names):
                    self.update()
                    print("App changed to {str}".format(str=str(self)))
            if (bimpy.collapsing_header("App processing")):
                if (self.app is None):
                    bimpy.text("This subapp is not supported.")
                    return
                self.app.show(ctx)
        def _add(self, name, type):
            self.names.append(name)
            self.types.append(type)

    def __init__(self):
        self.sub = self.Sub()

    def run(self):
        ctx = bimpy.Context()
        ctx.init(600, 600, "Neural Network")
        while (not ctx.should_close()):
            with ctx:
                bimpy.begin("Control Center")
                self.sub.show(ctx)
                bimpy.end()

def run():
    app = App()
    app.run()
