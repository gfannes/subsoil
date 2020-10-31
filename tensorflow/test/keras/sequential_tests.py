import tensorflow as tf
from tensorflow import keras
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

if 'Create sin+noise dataset':
    stddev = 0.05
    sample_cnt = 1000
    ds = tf.data.Dataset.from_tensor_slices(tf.cast(tf.linspace(-5,5,sample_cnt), dtype=tf.float32))
    ds = ds.map(lambda a:([a],[tf.sin(a)+tf.random.normal([], 0.0, stddev)]))
    print(ds.element_spec)
    if 0*'Plot data':
        for b in ds.batch(ds.cardinality()):
            x,y = b[0],b[1]
            plt.plot(x,y)
            plt.show()

if 'Create neural model':
    n = 100
    params = {'activation':keras.layers.LeakyReLU(), 'kernel_initializer':'he_normal'}
    params = {'activation':'elu', 'kernel_initializer':'he_normal'}
    model = keras.models.Sequential([
        keras.layers.Flatten(input_shape=[1]),
        keras.layers.Dense(n, **params),
        keras.layers.Dense(n, **params),
        keras.layers.Dense(n, **params),
        keras.layers.Dense(1),
        ])
    model.compile(optimizer='adam', loss='mse')

if 'Train neural model':
    batch_size = 32
    my_ds = ds
    my_ds = my_ds.shuffle(sample_cnt)
    my_ds = my_ds.batch(batch_size, drop_remainder=True)
    model.fit(my_ds, epochs=20)

if 'Apply model to dataset':
    def my_map(*b):
        z = model(b[0])
        return b+(z,)
    ds = ds.batch(1).map(my_map).unbatch()
    for e in ds.element_spec:
        print(e)

if 1*'Plot predictions':
    for b in ds.batch(sample_cnt):
        x,y,p = b[0],b[1],b[2]
        plt.plot(x,y,'g')
        plt.plot(x,p,'r.')
        plt.show()

