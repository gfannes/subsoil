import tensorflow as tf
from tensorflow import keras

dataset = tf.data.Dataset.from_tensor_slices([[1],[2],[3]])

for item in dataset:
    print(item)
