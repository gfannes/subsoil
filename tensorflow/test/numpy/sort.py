import numpy as np

data = np.arange(1.0,-1.0,-0.1)
if 'reshape':
    data = data.reshape(4,5)
    data = data.flatten()
print(data)
data = np.sort(data)
print(data)
