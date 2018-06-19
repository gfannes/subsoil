# Nr steps on the X-axis when creating plots
NR = 100

# Maps [0, 100] into [-v, v]
def ix2f(ix):
    v = 3.0
    return (ix/NR*2-1)*v

# Min-max range as a tuple
mm = (ix2f(0), ix2f(NR))

# Array of X-axis values
def x_ary():
    return [ix2f(ix) for ix in range(NR+1)]

