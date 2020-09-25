import tensorflow as tf
from tensorflow import keras
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

print('tf',tf.__version__)
print('keras',keras.__version__)

(X_train_full, y_train_full), (X_test, y_test) = keras.datasets.fashion_mnist.load_data()
class_names = ['T-shirt','Trouser','Pullover','Dress','Coat','Sandal','Shirt','Sneaker','Bag','Boot']

def rescale(full, scale=None):
    train, test = full[5000:], full[:5000]
    if scale is not None:
        def div_scale(data):
            return data/scale
        train, test = div_scale(train), div_scale(test)
    return train, test
X_train, X_valid = rescale(X_train_full, scale=255.0)
y_train, y_valid = rescale(y_train_full)

if 0*'plot':
    n = 10
    image = np.concatenate([np.concatenate(X_train[i:i+n], axis=1) for i in range(0, n*n, n)], axis=0)
    print([class_names[y] for y in y_train[0:n*n]])
    plt.imshow(image, cmap=mpl.cm.binary)
    plt.show()

model = keras.models.Sequential()
model.add(keras.layers.Flatten(input_shape=(28,28)))
model.add(keras.layers.Dense(3000, activation='relu'))
model.add(keras.layers.Dense(300, activation='relu'))
model.add(keras.layers.Dense(10, activation='softmax'))
model.summary()
print([w.shape for w in model.layers[1].get_weights()])

model.compile(loss='sparse_categorical_crossentropy', optimizer='sgd', metrics=['accuracy'])

hist = None
epochs = 3
plt.ion()
plt.show()
for i in range(0, 10, epochs):
    history = model.fit(X_train, y_train, epochs=epochs, validation_data=(X_valid, y_valid))
    myhist = pd.DataFrame(history.history, index=range(i,i+epochs))
    if hist is None:
        hist = myhist
    else:
        hist = hist.append(myhist)
    hist.info()

    # hist.plot()
    plt.plot(hist['accuracy'])
    plt.draw()
    plt.pause(0.1)

