import tensorflow as tf
from tensorflow import keras
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

sample_cnt = 1000
if 0:
    stddev = 0.05
    radius = 1.0
else:
    stddev = 0.005
    radius = 0.1

def my_gen():
    for angle in np.linspace(0.0, 1.8*np.pi, sample_cnt):
        x,y = radius*np.cos(angle),radius*np.sin(angle)
        yield {'angle':angle, 'xy':[x,y]}
ds = tf.data.Dataset.from_generator(my_gen, {'angle':tf.float32, 'xy':tf.float32}, {'angle':tf.TensorShape([]), 'xy':tf.TensorShape([2])})
ds = ds.map(lambda d:{**d, 'xyn':d['xy']+tf.random.normal([2],stddev=stddev)})

if 0*'show xyn data':
    for batch in ds.batch(sample_cnt):
        xy = batch['xyn']
        x,y = xy[:,0],xy[:,1]
        plt.plot(x,y,'.')
        plt.show()

n = 20
activation=keras.layers.LeakyReLU()
kernel_initializer='he_normal'
kernel_initializer=None
model = keras.models.Sequential([
    keras.layers.Dense(n, activation=activation, kernel_initializer=kernel_initializer),
    keras.layers.Dense(n, activation=activation, kernel_initializer=kernel_initializer),
    keras.layers.Dense(1, activation='linear'),
    keras.layers.Dense(n, activation=activation, kernel_initializer=kernel_initializer),
    keras.layers.Dense(n, activation=activation, kernel_initializer=kernel_initializer),
    keras.layers.Dense(2, activation='linear'),
    ])
model.compile(optimizer='adam', loss='mse')
train_ds = ds.shuffle(sample_cnt).batch(32).map(lambda d:(d['xyn'],d['xyn']))
model.fit(train_ds, epochs=50)
print(model.summary())
w,b = model.layers[0].get_weights()
print(w,b)

if 1*'show reco':
    for batch in ds.batch(sample_cnt):
        xy = batch['xyn']
        reco = model.predict(xy)
        x,y = xy[:,0],xy[:,1]
        px,py = reco[:,0],reco[:,1]
        plt.plot(x,y,'.')
        plt.plot(px,py,'r.')
        plt.show()

