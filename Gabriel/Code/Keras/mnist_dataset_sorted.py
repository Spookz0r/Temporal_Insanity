import numpy as np

from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten, pooling
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils
from keras import backend as K
from keras.models import load_model

from temporal_insanity import *

'''Choose batch size, number of output classes and numbers of epochs
Batch is the number of test data you should take from the training set each epoch
'''

batch_size = 128
nb_classes = 10
nb_epoch = 1

''' Choose image dimension for input '''
img_rows = 28
img_cols = 28

''' Number of convolutional filters (not including maxpool and relu etc...)
A larger number of filters makes it possible to detect more features
'''

nb_filters = 12

''' Size of the pooling area for max pooling, almost always 2x2 '''

pool_size = (2,2)

''' Convolution kernel size, ie the size of the matrix sliding over the image 
detecting features '''

kernel_size = (3,3)

''' Split up the data in train and test sets '''
(X_train, y_train),(X_test, y_test) = mnist.load_data()

#zero_array = [ np.array(0), np.array(0)]
zero_images = []
one_images = []
two_images = []
three_images = []
four_images = []
five_images = []
six_images = []
seven_images = []
eight_images = []
nine_images = []

for i, item in enumerate(y_train):
    if item == 0:
        zero_images.append(X_train[i])
    elif item == 1:
        one_images.append(X_train[i])
    elif item == 2:
        two_images.append(X_train[i])
    elif item == 3:
        three_images.append(X_train[i])
    elif item == 4:
        four_images.append(X_train[i])
    elif item == 5:
        five_images.append(X_train[i])
    elif item == 6:
        six_images.append(X_train[i])    
    elif item == 7:
        seven_images.append(X_train[i])
    elif item == 8:
        eight_images.append(X_train[i])
    elif item == 9:
        nine_images.append(X_train[i])

print("Done")



      
