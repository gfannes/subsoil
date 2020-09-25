import numpy as np
import matplotlib.pyplot as plt
import math

x = np.arange(0.0, 2*math.pi, 0.1)
y = np.sin(x)+np.random.normal(scale=0.1, size=x.shape)
xy = np.c_[x,y]
plt.plot(xy[:,0], xy[:,1], 'r.')
plt.show()
print( f'{xy}' )
