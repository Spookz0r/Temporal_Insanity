from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets('MNIST_data', one_hot=True)

import tensorflow as tf

train = 1

sess = tf.InteractiveSession()

#CNN

'''
Cnn needs alot of weights and biases and therefor functions are created for them.
Should generally initialize weights with a small amount of noise. Using ReLU neurons

'''

def weight_variable(shape):
    initial = tf.truncated_normal(shape, stddev=0.1)
    return tf.Variable(initial)

def bias_variable(shape):
    initial = tf.constant(0.1, shape=shape)
    return tf.Variable(initial)

'''
Convolutions and pooling operations:
Boundaries, stride size etc... Choosing stride one, zero padded, so the output is the same size as input.
pooling is 2x2 blocks
'''

def conv2d(x,W):
    return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')

def max_pool_2x2(x):
    return tf.nn.max_pool(x, ksize=[1,2,2,1],
                           strides=[1,2,2,1], padding='SAME')
    




x = tf.placeholder(tf.float32, [None, 784], name="input")  #input
y_ = tf.placeholder(tf.float32, [None, 10])  #output

W = tf.Variable(tf.zeros([784, 10]))  #weight   784*10 becuase 784 input features (28x28) and 10 outputs
b = tf.Variable(tf.zeros([10]))  #bias  10 because of 10 classes



'''
Creating layers.
First layer: Convolution followed by max pooling. The conv will compute 32 features for each 5x5 patch. weight tensor will have a shape of [5,5,1,32].
The first two dimensions are patch size, the next is number of input channels, last is output channels. Will also have a bias vector with a 
component for each output channel.
'''
    
W_conv1 = weight_variable([5, 5, 1, 32])
b_conv1 = bias_variable([32])
'''
To apply layer, first reshape x to a 4d tensor, with second and third dimension being image width and height, final dimension corresponding to number of color channels
'''

x_image = tf.reshape(x,  [-1, 28, 28, 1])

'''Then convolve image with weight tensor, add bias, apply relu and finally max pool. Pooling will reduce image to 14x14'''

h_conv1 = tf.nn.relu(conv2d(x_image, W_conv1) + b_conv1)
h_pool1 = max_pool_2x2(h_conv1)

''' Second Layer '''

W_conv2 = weight_variable([5, 5, 32, 64])
b_conv2 = bias_variable([64])
h_conv2 = tf.nn.relu(conv2d(h_pool1, W_conv2) + b_conv2)
h_pool2 = max_pool_2x2(h_conv2)


''' Image is now 7x7, add a fully connected layer with 1024 neurons'''

W_fc1 = weight_variable([7 * 7 * 64, 1024])
b_fc1 = bias_variable([1024])
h_pool2_flat = tf.reshape(h_pool2, [-1, 7*7*64])
h_fc1 = tf.nn.relu(tf.matmul(h_pool2_flat, W_fc1) + b_fc1)

''' Dropout 
To reduce overfitting, dropout is applied before readout layer, by creating a placeholder for the probability
that a neuron is kept makes it possible to enable dropout during training and disable during testing'''

keep_prob = tf.placeholder(tf.float32, name="keep_prob")
h_fc1_drop = tf.nn.dropout(h_fc1, keep_prob)

''' Readout layer '''

W_fc2 = weight_variable([1024, 10])
b_fc2 = bias_variable([10])
y_conv = tf.matmul(h_fc1_drop, W_fc2, name="output") + b_fc2



''' Train and evaluate '''

sess.run(tf.global_variables_initializer())



cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(y_conv, y_))
train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y_,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
sess.run(tf.global_variables_initializer())

if train == 1:
    
    for i in range(200):
      batch = mnist.train.next_batch(50)
      if i%1 == 0:
        train_accuracy = accuracy.eval(feed_dict={
            x:batch[0], y_: batch[1], keep_prob: 1.0})
        print("step %d, training accuracy %g"%(i, train_accuracy))
        
        
      train_step.run(feed_dict={x: batch[0], y_: batch[1], keep_prob: 0.5})
    
    saver = tf.train.Saver()
    save_path = saver.save(sess, 'mnist_tut_model.ckpt')
    print("Model saved to %s" % save_path)
    print("Training done")


if train == 0:
    saver = tf.train.Saver()
    saver = tf.train.import_meta_graph('mnist_tut_model.meta')
    saver.restore(sess, tf.train.latest_checkpoint('./'))
    all_vars = tf.get_collection('vars')
    for v in all_vars:
        v_ = sess.run(v)
    print("model loaded")
    tf.get_default_graph().as_graph_def()

    
print("Start testing")
#print("test accuracy CNN: %g"%accuracy.eval(feed_dict={
 #   x: mnist.test.images, y_: mnist.test.labels, keep_prob: 1.0}))

''' Save variables '''







