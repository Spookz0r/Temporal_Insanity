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
model = Sequential()
model.add(Convolution2D(32, 3, 3, activation='relu', input_shape=(1,28,28), dim_ordering='th'))
print (model.output_shape)
model.add(Convolution2D(32, 3, 3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2,2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(30, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(1, activation='relu'))
sgd = SGD(lr=0.01, clipnorm=1.)
model.compile(loss='mean_absolute_error',
              optimizer=sgd)
#plot(model, to_file='model.png', show_shapes = True)

model.fit(X_train, Y_train, 
          batch_size=32, nb_epoch=1, verbose=1)
model.save("muh_model2.h5")
#score = model.evaluate(X_test, Y_test, verbose=1)




"""



(X_train, y_train), (X_test, y_test) = mnist.load_data()
print((X_train[0]).shape)
print(X_train.shape)
X_train = X_train.reshape(X_train.shape[0], 1, 28, 28)
X_test = X_test.reshape(X_test.shape[0], 1, 28, 28)
X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255
#plt.imshow(X_train[0])
nr_classes = 10


cap = cv2.VideoCapture(0)
ret, frame = cap.read()
gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

print("Ytrain", y_train.shape)
Y_train = np_utils.to_categorical(y_train, nr_classes)
Y_test = np_utils.to_categorical(y_test, nr_classes)

print("Setting up model")
model = Sequential()
print(gray.shape)

model.add(Convolution2D(32,3,3, border_mode='same', input_shape=(1,480,640), dim_ordering='th'))
model.add(Activation('relu'))
model.add(Convolution2D(32, 3, 3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2,2)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(TimeDistributedDense(3))
model.add(LSTM(3))

#model.add(Dense(128, activation='relu'))
#model.add(Dropout(0.5))
model.add(Dense(1))
model.add(Activation('relu'))
print("FInished model")
plot(model, to_file='model.png', show_shapes = True)
model.compile(loss='mean_squared_error',
              optimizer='adam')

model.summary()
frames = np.array([gray, gray])
frames = frames.reshape(frames.shape[0], 1, 480, 640)
frames = frames.astype('float32')
frames /= 255

ans = np.array([1,1])
print("ans frames: " , ans.shape)
#ans = np_utils.to_categorical(ans)
print("ans frames: " , ans.shape)

print("shape X_train: " , X_train.shape)
print("shape ans: " , ans.shape)
print("shape Y_train: ",Y_train.shape)
print("Starting training")
model.fit(frames, ans, batch_size = 1, nb_epoch = 1, verbose = 1)
score = model.evaluate(frames, ans, verbose=1)
print(score)
"""

"""

model = Sequential()
model.add(Convolution2D(32, 3, 3, activation='relu', input_shape=(1,28,28), dim_ordering='th'))
print (model.output_shape)
model.add(Convolution2D(32, 3, 3, activation='relu'))
model.add(MaxPooling2D(pool_size=(2,2)))
model.add(Dropout(0.25))
model.add(Flatten())
model.add(Dense(128, activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(10, activation='softmax'))
model.compile(loss='categorical_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])
model.fit(X_train, Y_train, 
          batch_size=32, nb_epoch=10, verbose=1)


"""


"""
plt.imshow(X_train[0])
plt.show()

while(True):
    # Capture frame-by-frame

    # Our operations on the frame come here

    # Display the resulting frame
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()

"""

print("Goodbye")
