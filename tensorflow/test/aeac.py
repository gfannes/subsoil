#!/usr/bin/python

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'  # or any {'0', '1', '2'}

import aeac.cli
import aeac.data
import aeac.model
import tensorflow as tf
from tensorflow import keras
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

print('tf.executing_eagerly()',tf.executing_eagerly())

options = aeac.cli.get_options()
print(options)

if options.mode=='test':
    do_log = 1
    time_blocks = aeac.data.Wav.load_time_blocks(options.inputs, options.block_size, block_cnt=None)
    block_cnt = len(time_blocks)

    def show_freqs(dataset, na, nb):
        for b in dataset.batch(block_cnt):
            def get(n):
                t = b[n]
                m,s = tf.math.reduce_mean(t,0),tf.math.reduce_std(t,0)
                print(n,'mean',m,'std',s)
                return t,m,s
            ta,ma,sa = get(na)
            tb,mb,sb = get(nb)
            plt.plot(sa,'-')
            plt.plot(sb,'.')
            plt.show()
            plt.plot(ta,'-')
            plt.plot(tb,'.')
            plt.show()

    preprocessor = aeac.data.Preprocessor(options.block_size)

    dataset = preprocessor.init_from_time_blocks(time_blocks)
    dataset = preprocessor.samples_to_errors(dataset, 'orig_corners', 'orig_samples', 'orig_errors')
    dataset = preprocessor.errors_to_freqs(dataset, 'orig_errors', 'orig_freqs')

    if 0:
        n = 100
        params = {'activation':keras.layers.LeakyReLU(), 'kernel_initializer':'he_normal'}
        params = {'activation':'relu', 'kernel_initializer':'he_normal'}
        model = keras.models.Sequential([
            keras.layers.Flatten(input_shape=[options.block_size-1]),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(4),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(n, **params),
            keras.layers.Dense(options.block_size-1),
            ])
        model.compile(optimizer='adam', loss='mse')

        input_name = 'orig_freqs'
        def create_io(d):
            inp = d[input_name]
            return inp, inp
        train_ds = dataset.map(create_io)
        train_ds = train_ds.shuffle(block_cnt)
        train_ds = train_ds.batch(32, drop_remainder=True)
        model.fit(train_ds, epochs=100)
        print(model.summary())

        if 'Apply model':
            def apply(d):
                d['reco_freqs'] = model(d[input_name])
                return d
            dataset = dataset.batch(32).map(apply).unbatch()
    else:
        n = 4
        def recode(d):
            orig_freqs = d['orig_freqs']
            reco_freqs = tf.concat([orig_freqs[:n],orig_freqs[n:]*0], 0)
            d['reco_freqs'] = reco_freqs
            return d
        dataset = dataset.map(recode)

    show_freqs(dataset, 'orig_freqs', 'reco_freqs')

    if options.output:
        dataset = preprocessor.freqs_to_errors(dataset, 'reco_freqs', 'reco_errors')
        dataset = preprocessor.errors_to_samples(dataset, 'reco_errors', 'orig_corners', 'reco_samples')
        column_name = 'reco_samples'
        ds = dataset.map(lambda d: d[column_name])
        aeac.data.save_to_wav(options.output, ds, 48000//options.block_size)

if options.mode=='create':
    dataset = aeac.data.load_from_wav(options.inputs, options.block_size, shuffle=False)
    print(len(dataset), dataset)
    for item in dataset.take(1):
        print('shape of dataset item: ', item.shape)

    def mute(spectrum):
        n = 32
        gains = [1]*n + [0]*(len(spectrum)-n)
        gains = tf.constant(gains, dtype=tf.float32)
        return tf.math.multiply(spectrum, gains)
    dataset = dataset.map(mute)

    if options.output:
        aeac.data.save_to_wav(options.output, dataset)
