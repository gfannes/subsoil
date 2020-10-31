import tensorflow as tf
import tensorflow_datasets as tfds
from tensorflow import keras

batch_size = 32
# batch_size = None

def _normalize_img(img, label):
  img = tf.cast(img, tf.float32) / 255.
  if batch_size:
      img = tf.reshape(img, [batch_size,28*28])
  else:
      img = tf.reshape(img, [28*28])
  tf.print(img.shape)
  return img, label

ds = tfds.load('mnist', split='train', as_supervised=True)
print(ds.element_spec)
if batch_size:
    ds = ds.batch(batch_size, drop_remainder=True)
ds = ds.map(_normalize_img)
print(ds.element_spec)

model = keras.models.Sequential([
    keras.layers.Flatten(input_shape=[28*28]),
    keras.layers.Dense(1),
    ])
model.compile(optimizer='adam', loss='mse')
model.fit(ds, epochs=5)

for batch in ds:
    for el in batch:
        p = model(el)
        print(p)
        quit()
