from gubg.ft import DCT

class Convertor:
    def __init__(self, blocksize):
        self.blocksize = blocksize
        self.dct = DCT(self.blocksize, t='sin2')
    def to_spectrum(self, block):
        return self.dct.fwd(block)
    def from_spectrum(self, spectrum):
        return self.dct.inv(spectrum)
    def to_spectra(self, all_samples):
        nr_samples = all_samples.shape[0]
        nr_blocks = nr_samples//self.blocksize
        all_freqs = np.resize(all_samples, (nr_blocks, self.blocksize))
        for freqs in all_freqs:
            freqs[:] = self.to_spectrum(freqs)
        return all_freqs
    def from_spectra(self, all_freqs):
        all_samples = np.copy(all_freqs)
        for samples in all_samples:
            samples[:] = self.from_spectrum(samples)
        return all_samples.flatten()

