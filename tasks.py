from invoke import task
import sys

@task
def nn(c):
    print("running nn")
    sys.path.extend(['fri3d/2018/src'])
    import nn
    nn.app.run()
