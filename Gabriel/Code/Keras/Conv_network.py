import numpy as np

from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout, Activation, Flatten, pooling
from keras.layers import Convolution2D, MaxPooling2D
from keras.utils import np_utils
from keras import backend as K
from keras.models import load_model


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


if K.image_dim_ordering() == 'th':
    X_train = X_train.reshape(X.train.shape[0], 1, img_rows, img_cols)
    X_test = X_test.reshape(X_test.shape[0],1, img_rows, img_cols)
    input_shape = (img_rows, img_cols, 1)
else:
    X_train = X_train.reshape(X_train.shape[0], img_rows, img_cols,1)
    X_test = X_test.reshape(X_test.shape[0], img_rows, img_cols,1)
    input_shape = (img_rows, img_cols, 1)
    
X_train = X_train.astype('float32')
X_test = X_test.astype('float32')
X_train /= 255
X_test /= 255
print('X_train shape:', X_train.shape)
print(X_train.shape[0],'train samples')
print(X_test.shape[0], 'test_samples')

''' Convert class vectors to binary class matrices '''

Y_train = np_utils.to_categorical(y_train, nb_classes)
Y_test = np_utils.to_categorical(y_test, nb_classes)

''' Setting model to a sequence '''
model = Sequential()

''' Adding a convolutional layer '''

model.add(Convolution2D(nb_filters, kernel_size[0], kernel_size[1],
                        border_mode='valid',
                        input_shape=input_shape))

''' Adding an Activation layer after conv layer, in this case a ReLU '''

model.add(Activation('relu'))

''' Adding a second convolutional filter, same size '''
model.add(Convolution2D(nb_filters, kernel_size[0], kernel_size[1]))

''' Another activation filter '''

model.add(Activation('relu'))

''' Adding a max-pooling layer, downsampling the feature maps '''

model.add(MaxPooling2D(pool_size=pool_size))

''' Adding a dropout layer to reduce overfitting, setting the amount of nodes to drop ''' 

model.add(Dropout(0.25))

''' Adding a Flatten layer
Why flatten? The last stage of a CNN is a classifier, its called a dense layer which is an
ordinary artifical neural network classifier. And an ANN needs individual features which
is a feature vector. The CNN part of the networks is multidimensional and must therefore
be flattened, converted, to a 1D-feature vector. Output from the convolutional part 
flattens creating a single long feature vector '''

model.add(Flatten())

''' Adding a Dense Layer
A dense layer is simply a layer wher each neuron is connected to each neuron in the next layer, ie 
a fully connected layer, number is number of neurons '''

model.add(Dense(128))

''' Adding another activation layer '''

model.add(Activation('relu'))

''' Adding another dropout to further reduce overfitting '''
 
model.add(Dropout(0.5))

''' Adding the final dense layer with the size of the outputs '''

model.add(Dense(nb_classes))

''' Adding a softmax activation layer as output layer. 
The softmax layer ensures that the sum of the probabilities of the output is equal to 1.'''

model.add(Activation('softmax'))
 
''' Training the model
use model.compile, choose loss function, an optimizer and metric
Then use model.fit and input train data, batch size, number of epochs and the validation data set
use model.evaluate to evaluate the model with a test set.
 '''

model.compile(loss='categorical_crossentropy',
              optimizer='adadelta',
              metrics=['accuracy'])

model.fit(X_train, Y_train, batch_size=batch_size, nb_epoch=nb_epoch,
          verbose=1, validation_data=(X_test, Y_test))

score = model.evaluate(X_test, Y_test, verbose=0)

print('Test score:' , score[0])
print('Test accuracy:', score[1])


''' To save a model '''

model.save('my_model.h5')


















