import numpy as np
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import LSTM
from keras.utils import np_utils
from keras.utils.io_utils import HDF5Matrix
import os

import h5py
from keras.preprocessing.image import ImageDataGenerator
''' structure

iccv09Data/
    images/
        number.jpg
    labels/
        number.layers.txt
        number.regions.txt
        number.surfaces.txt
    horizons.txt
        
        
'''

data_dir = 'iccv09Data/images'
validation_data_dir = 'iccv09Data/labels'

nb_train_samples = 400
nb_validation_samples = 300
nb_epoch = 50

img_width, img_height = 320, 240


''' Network '''

model = Sequential()
model.add(LSTM(1, input_shape=(1,img_width*img_height)))
model.add(Dense(1, activation='softmax'))
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])