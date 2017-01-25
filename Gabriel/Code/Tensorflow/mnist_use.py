import tensorflow as tf
import cv2
import matplotlib.pyplot as plt
import time

from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('MNIST_data', one_hot=True)



sess = tf.InteractiveSession()
''' Load model '''

saver = tf.train.import_meta_graph('mnist_tut_model.ckpt.meta')
saver.restore(sess, tf.train.latest_checkpoint('./'))
print("Model loaded")

tf.get_default_graph().as_graph_def()


''' Associate variables '''

x = sess.graph.get_tensor_by_name("input:0")
y_conv = sess.graph.get_tensor_by_name("output:0")
keep_prob = sess.graph.get_tensor_by_name("keep_prob:0")

''' You dont have to define any nodes inside the network, the graph handles it'''



''' Read picture '''

image_b = mnist.test.images[420]
plt.imshow(image_b.reshape([28,28]), cmap='Greys')

#plt.show()

print("image show")

''' Run picture in model '''

image_b = image_b.reshape([1, 784])
start_time = time.time()
result = sess.run(y_conv, feed_dict={x:image_b, keep_prob:1})
print("--- %s seconds ---" % (time.time() - start_time))
print(result)
print("I think it is a:")
print(sess.run(tf.argmax(result,1)))

plt.show()