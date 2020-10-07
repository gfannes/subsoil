import soundfile
import numpy as np

def create_data(wav_fns, blocksize):
    data = []
    for wav_fn in wav_fns:
        samples, samplerate = soundfile.read(wav_fn)
        nr_blocks = len(samples)//blocksize
        blocks = np.resize(samples, (nr_blocks, blocksize))
        data.append(blocks)
    return np.vstack(data)

def save_wav(wav_fn, data, samplerate):
    soundfile.write(wav_fn, data.flatten(), samplerate)
