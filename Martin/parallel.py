from __future__ import print_function
import numpy as np
np.random.seed(1337)  # for reproducibility

from keras.datasets import mnist
from keras.layers.core import TimeDistributedDense, Activation, Dropout  
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils
from keras.optimizers import SGD
from keras.utils.visualize_util import plot
import pandas as pd
from keras import backend as K
import cv2
from matplotlib import pyplot as plt
from keras.layers import LSTM
from keras.models import load_model
from keras.layers import Merge
print("Hello")
#import theano
#theano.config.openmp = True

(X_train, y_train), (X_test, y_test) = mnist.load_data()

X_train = X_train.reshape(X_train.shape[0], 1, 28, 28)
X_test = X_test.reshape(X_test.shape[0], 1, 28, 28)
X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255
nr_classes = 10

Y_train = y_train #np_utils.to_categorical(y_train, nr_classes)
Y_test = y_test#np_utils.to_categorical(y_test, nr_classes)

print("Y train shape: ", Y_train.shape)
print("X_train shape: ", X_train.shape)
print(Y_train[0])



"""
model = Sequential()
model2 = Sequential()

model.add(Convolution2D(32, 3, 3, activation='relu', input_shape=(1,28,28), dim_ordering='th'))
print (model.output_shape)
model.add(Convolution2D(32, 3, 3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2,2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(30, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(30, activation='relu'))
sgd = SGD(lr=0.01, clipnorm=1.)

model.summary()

model2.add(Dense(300, activation='relu', input_shape=(1,28,28)))

model2.add(Dense(30, activation='relu'))

merge = Sequential()

merge.add(Merge(model, model2), mole = 'mul')

model.compile(loss='mean_absolute_error', optimizer=sgd)
model2.compile(loss='mean_absolute_error', optimizer=sgd)
merge.compile(loss= 'mean_absolute_error', optimizer=sgd)
#plot(model, to_file='model.png', show_shapes = True)

merge.fit(X_train, Y_train, 
          batch_size=32, nb_epoch=1, verbose=1)
merge.save("parallel_model.h5")
"""

#score = model.evaluate(X_test, Y_test, verbose=1)