#!/usr/bin/python

import aeac.cli
import aeac.data
import aeac.model
import tensorflow as tf
from tensorflow import keras

options = aeac.cli.get_options()
print(options)

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

if options.mode=='train':
    aeac.model.train(data)
