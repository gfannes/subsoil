import ft.convertor
import soundfile
import numpy as np
import tensorflow as tf
from tensorflow import keras
import matplotlib as mpl
import matplotlib.pyplot as plt

class Wav:
    def load_time_blocks(wav_fps, block_size):
        data = []
        for wav_fp in wav_fps:
            samples, sample_rate = soundfile.read(wav_fp.path())
            sample_cnt = len(samples)
            block_cnt = sample_cnt//block_size
            if len(samples.shape)==1:
                samples = samples.reshape((sample_cnt,1))
            channel_cnt = samples.shape[1]
            for channel_ix in range(channel_cnt):
                blocks = np.zeros((block_cnt, block_size))
                for block_ix in range(block_cnt):
                    sample_ix = block_ix*block_size
                    blocks[block_ix,:] = samples[sample_ix:sample_ix+block_size, channel_ix]
                data.append(blocks)
        return np.vstack(data)

class DiscreteSineTransform:
    def to_time(block_size):
        k_offset = [(k,-1+block_size/2/k) for k in range(1,block_size)]
        return tf.constant([[np.cos(k*np.pi/block_size*(x-offset)) for k,offset in k_offset] for x in range(block_size-1)], shape=[block_size-1,block_size-1], dtype=tf.float32)
    def to_freq(block_size):
        return tf.linalg.inv(DiscreteSineTransform.to_time(block_size))

class Preprocessor:
    def __init__(self, block_size):
        self.block_size = block_size
        window = tf.constant(tf.linspace(1.0, 0.0, self.block_size+1), dtype=tf.float32)
        self.window_front = window[1:]
        self.window_back  = tf.reverse(window[:-1], [0])
        self.to_freq = DiscreteSineTransform.to_freq(self.block_size)
        self.to_time = DiscreteSineTransform.to_time(self.block_size)

    def check_block_size_(self, block_size):
        if block_size != self.block_size:
            raise SystemError(f'Block size mismatch')
        return self.block_size

    def interpolate_(self, corners):
        return self.window_front*corners[0] + self.window_back*corners[1]

    def init_from_time_blocks(self, time_blocks):
        block_size = self.check_block_size_(time_blocks.shape[1])
        def my_generator():
            seed = 0.0
            for block in time_blocks:
                target = block[-1]
                yield({'orig_corners':[seed, target], 'orig_samples':block})
                seed = target
        return tf.data.Dataset.from_generator(my_generator, {'orig_corners':tf.float32, 'orig_samples':tf.float32}, {'orig_corners':tf.TensorShape([2]), 'orig_samples':tf.TensorShape([self.block_size])})

    def samples_to_errors(self, dataset, corners_name, samples_name, errors_name):
        block_size = self.check_block_size_(dataset.element_spec[samples_name].shape[0])
        def map_fn(d):
            interpolated = self.interpolate_(d[corners_name])
            errors = (d[samples_name]-interpolated)[:-1]
            d[errors_name] = errors
            return d
        return dataset.map(map_fn)

    def errors_to_freqs(self, dataset, errors_name, freqs_name):
        block_size = self.check_block_size_(dataset.element_spec[errors_name].shape[0]+1)
        def map_fn(d):
            d[freqs_name] = tf.linalg.matvec(self.to_freq, d[errors_name])
            return d
        return dataset.map(map_fn)

    def freqs_to_errors(self, dataset, freqs_name, errors_name):
        block_size = self.check_block_size_(dataset.element_spec[freqs_name].shape[0]+1)
        def map_fn(d):
            d[errors_name] = tf.linalg.matvec(self.to_time, d[freqs_name])
            return d
        return dataset.map(map_fn)

    def errors_to_samples(self, dataset, errors_name, corners_name, samples_name):
        block_size = self.check_block_size_(dataset.element_spec[errors_name].shape[0]+1)
        def map_fn(d):
            interpolated = self.interpolate_(d[corners_name])
            d[samples_name] = interpolated+tf.concat([d[errors_name],[0]], 0)
            return d
        return dataset.map(map_fn)



def load_from_wav(wav_fps, block_size, shuffle=False):
    blocks = load_time_blocks(wav_fps, block_size)
    if 1*'print time':
        print(blocks.shape)
        print(blocks)
    spectra = create_spectra(blocks)
    if 1*'print spectra':
        print(spectra.shape)
        print(spectra,spectra.dtype)
    dataset = tf.data.Dataset.from_tensor_slices(tf.constant(spectra, dtype=tf.float32))
    if shuffle:
        dataset = dataset.shuffle(len(dataset))
    return dataset

def split_corners_errors(time_blocks):
    block_cnt = len(time_blocks)
    block_size = time_blocks.shape[1]
    corners = np.zeros(block_cnt)
    errors = np.zeros((block_cnt, block_size-1))
    prev_corner = 0.0
    for bix,block in enumerate(time_blocks):
        corners[bix] = block[-1]
    return corners,errors

def create_spectra(time_blocks):
    block_size = time_blocks.shape[1]
    convertor = ft.convertor.Convertor(block_size)
    spectra = np.zeros(time_blocks.shape)
    for ix,time_block in enumerate(time_blocks):
        spectra[ix,:] = convertor.to_spectrum(time_block)
    return spectra

# def save_to_wav(wav_fn, dataset, sample_rate=48000):
#     for item in dataset.take(1):
#         block_size = len(item)
#     convertor = ft.convertor.Convertor(block_size)
#     with soundfile.SoundFile(wav_fn, mode='w', samplerate=sample_rate, channels=1) as wav:
#         for spectrum in dataset:
#             time = convertor.from_spectrum(spectrum)
#             wav.write(time)

def save_to_wav(wav_fn, dataset, block_rate):
    el_size = dataset.element_spec.shape[0]
    with soundfile.SoundFile(wav_fn, mode='w', samplerate=block_rate*el_size, channels=1) as wav:
        for el in dataset:
            wav.write(el)
