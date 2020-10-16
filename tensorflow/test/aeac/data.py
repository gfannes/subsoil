import ft.convertor
import soundfile
import numpy as np
import tensorflow as tf
from tensorflow import keras

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

def load_time_blocks(wav_fps, block_size):
    data = []
    for wav_fp in wav_fps:
        samples, samplerate = soundfile.read(wav_fp.path())
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

def create_spectra(time_blocks):
    block_size = time_blocks.shape[1]
    convertor = ft.convertor.Convertor(block_size)
    spectra = np.zeros(time_blocks.shape)
    for ix,time_block in enumerate(time_blocks):
        spectra[ix,:] = convertor.to_spectrum(time_block)
    return spectra

def save_to_wav(wav_fn, dataset, samplerate=48000):
    for item in dataset.take(1):
        block_size = len(item)
    convertor = ft.convertor.Convertor(block_size)
    with soundfile.SoundFile(wav_fn, mode='w', samplerate=samplerate, channels=1) as wav:
        for spectrum in dataset:
            time = convertor.from_spectrum(spectrum)
            wav.write(time)
