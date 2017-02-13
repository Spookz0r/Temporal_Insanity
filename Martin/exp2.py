from __future__ import print_function
import numpy as np
#np.random.seed(1337)  # for reproducibility


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
import copy
import time
print("Hello")


(X_train, y_train), (X_test, y_test) = mnist.load_data()

nr = np.random.randint(0,9999)
print("Sample nr ",  nr)

pic = copy.deepcopy(X_test[nr])
img = X_test[nr]
img = img.reshape(1,1,28,28)
img = img.astype('float32')
img /= 255



model = load_model("muh_model.h5")
start = time.time()
x = model.predict(img, batch_size = 1, verbose = 1)
end = time.time()
print("Prediction is: ", x[0][0])
print("Answer is: ", y_test[nr])
print("It took: ", end-start, " seconds")
plt.imshow(pic)
plt.show()
