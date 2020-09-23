import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0.0, 5.0, 0.1)
y = np.sin(x)+np.random.normal(scale=0.1, size=x.shape)
xy = np.c_[x,y]
plt.plot(xy[:,0], xy, 'r.')
plt.show()
print( f'{xy}' )
