<!--
[proast](state:Done)(my_cost:4)
-->
# Setup environment

This section explains how I installed my `GeForce RTX 2060 GPU` on an up-to-date [Manjaro-linux OS](https://manjaro.org/).
You can find out your GPU model with a command like: `lspci | grep -i vga`.

## Install packages

```
yay -S hdf5 graphviz opencv hdfview
pip install --user numpy scipy matplotlib yaml h5py graphviz pydot-ng python-opencv tensorflow-gpu keras pandas sklearn
```

## Install GPU

* Install/enable the proprietary nvidia graphics driver. For Manjaro, you can use the `Manjaro Settings Manager -> Hardware Configuration`. Select `Auto Install Proprietary Driver` and reboot. You can find more details on nvidia driver version and hardware compatibility [here](https://www.nvidia.com/en-us/drivers/unix/). Find the most recent version of the nvidia drivers that mentions your actual hardware under `Supported Products`.
* Install CUDA and cuDNN. Depending on the version of TensorFlow, specific versions might be necessary. Run the test code hereunder and find-out what `cuda` and `cudnn` version you actually need. Following commands could be useful:
  * `yay -S cuda cudnn`
  * `yay -S cuda-10`
  * `yay -S cudnn7.0-cuda9.0`

## Test the setup

Run follow code to check if the GPU is available to TensorFlow:

```
import tensorflow as tf

if not tf.test.is_gpu_available():
    raise UserWarning("GPU is not available")
print(f"Your system has GPU support: {tf.test.gpu_device_name()}")
```

## Links

[Installing TensorFlow and CUDA on Manjaro](https://medium.com/@Soroush/installing-tensorflow-and-cuda-on-manjaro-linux-6cb64c5ece1e)
