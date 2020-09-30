from gubg.ft import DCT
import soundfile
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

class Convertor:
    def __init__(self, blocksize):
        self.blocksize = blocksize
        self.dct = DCT(self.blocksize, t='sin2')
    def to_freqs(self, all_samples):
        nr_samples = all_samples.shape[0]
        nr_blocks = nr_samples//self.blocksize
        all_freqs = np.resize(all_samples, (nr_blocks, self.blocksize))
        for freqs in all_freqs:
            freqs[:] = self.dct.fwd(freqs)
        return all_freqs
    def from_freqs(self, all_freqs):
        all_samples = np.copy(all_freqs)
        for samples in all_samples:
            samples[:] = self.dct.inv(samples)
        return all_samples.flatten()

all_samples, samplerate = soundfile.read('WelcomeToAuro3D.wav')
print(all_samples.shape)

blocksize = 16
convertor = Convertor(blocksize)
all_freqs = convertor.to_freqs(all_samples)

if 0*'show freqs':
    for ix in range(0,blocksize,4):
        plt.plot(all_freqs[:,ix:ix+4])
        plt.show()
for freqs in all_freqs:
    if 0*'add noise':
        noise = 0.01
        freqs += np.random.standard_normal(freqs.shape)*noise
    if 1*'zero some bands':
        nr_zero_bands = 14
        freqs[-nr_zero_bands:] *= 0

all_samples_recon = convertor.from_freqs(all_freqs)

soundfile.write('WelcomeToAuro3D_recon.wav', all_samples_recon, samplerate)
