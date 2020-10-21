#!/usr/bin/python

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'  # or any {'0', '1', '2'}

import aeac.cli
import aeac.data
import aeac.model
import tensorflow as tf
from tensorflow import keras
import numpy as np

print('tf.executing_eagerly()',tf.executing_eagerly())

options = aeac.cli.get_options()
print(options)

if options.mode=='test':
    do_log = 1
    time_blocks = aeac.data.Wav.load_time_blocks(options.inputs, options.block_size)

    preprocessor = aeac.data.Preprocessor(options.block_size)

    dataset = preprocessor.init_from_time_blocks(time_blocks)
    dataset = preprocessor.samples_to_errors(dataset, 'orig_corners', 'orig_samples', 'orig_errors')
    dataset = preprocessor.errors_to_freqs(dataset, 'orig_errors', 'orig_freqs')

    n = 15
    def encode(d):
        orig_errors = d['orig_freqs']
        repr_errors = tf.concat([orig_errors[:n],orig_errors[n:]*0], 0)
        d['repr_freqs'] = repr_errors
        return d
    dataset = dataset.map(encode)

    dataset = preprocessor.freqs_to_errors(dataset, 'repr_freqs', 'repr_errors')
    dataset = preprocessor.errors_to_samples(dataset, 'repr_errors', 'orig_corners', 'repr_samples')

    if options.output:
        column_name = 'repr_samples'
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
