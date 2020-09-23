import numpy as np

data = np.random.randint(0,10,size=(20,20))
print(data)
print(np.unique(data.flatten()))
