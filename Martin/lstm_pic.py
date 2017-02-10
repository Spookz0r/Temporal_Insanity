
from __future__ import print_function
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.layers import LSTM
from keras.optimizers import RMSprop
from keras.utils.data_utils import get_file
import numpy as np
import random
import sys
from PIL import Image
import PIL
import cv2
import math

img = np.array(Image.open("sexbomb.jpg"))

basewidth = 300
img = Image.open('sexbomb.jpg')
wpercent = (basewidth / float(img.size[0]))
hsize = int((float(img.size[1]) * float(wpercent)))
img = img.resize((basewidth, hsize), PIL.Image.ANTIALIAS)
img = np.array(img)
#img.save('resized_image.jpg')


gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = gray.reshape(1, 168, 300)

imglist = gray.reshape(50400)

seqlength = 30


"""
X[0] = img[0-29]
y[0] = img[30]
x[1] = img[1-30]
y[1] = img[31]
X[n] = img[50369-50399]
y[n] = img[50400]

n = len(img) - sequence_size
"""


X = np.zeros((50400, 30))
y = np.zeros(50400)
for i in range(0,50369):
    for j in range(0,30):
        X[i][j] = imglist[i+j]
    y[i] = imglist[i+30]



X = X.reshape(50400, 1, 30)
y = y.reshape(50400, 1)

X = X.astype('float32')
y = y.astype('float32')
X /= 255
y /= 255
#X = np.zeros((len(sequences), seqlength, len(imglist)), dtype=np.bool)
#y = np.zeros((len(sequences), len(imglist)), dtype=np.bool)
for i in y:
    print (i)
print('Build model...')
model = Sequential()
model.add(LSTM(128, input_shape= (1,seqlength), activation = 'linear'))
model.add(Dense(128))
model.add(Dense(1))
model.add(Activation('linear'))

optimizer = RMSprop(lr=0.001)
model.compile(loss='mean_absolute_error', optimizer=optimizer)
"""

for i in X:
    for j in i:
        for k in j:
            if(math.isnan(k)):
                print("420blazeit")
"""
model.fit(X, y, batch_size=128, nb_epoch=1, verbose=1)
model.save("lstm_pic.h5")




"""

path = get_file('nietzsche.txt', origin="https://s3.amazonaws.com/text-datasets/nietzsche.txt")
text = open(path).read().lower()
print('corpus length:', len(text))

chars = sorted(list(set(text)))
print('total chars:', len(chars))
char_indices = dict((c, i) for i, c in enumerate(chars))
indices_char = dict((i, c) for i, c in enumerate(chars))

# cut the text in semi-redundant sequences of maxlen characters
maxlen = 40
step = 3
sentences = []
next_chars = []
for i in range(0, len(text) - maxlen, step):
    sentences.append(text[i: i + maxlen])
    next_chars.append(text[i + maxlen])
print('nb sequences:', len(sentences))

print('Vectorization...')
X = np.zeros((len(sentences), maxlen, len(chars)), dtype=np.bool)
y = np.zeros((len(sentences), len(chars)), dtype=np.bool)
for i, sentence in enumerate(sentences):
    for t, char in enumerate(sentence):
        X[i, t, char_indices[char]] = 1
    y[i, char_indices[next_chars[i]]] = 1


# build the model: a single LSTM

print((maxlen, len(chars)))

print('Build model...')
model = Sequential()
model.add(LSTM(128, input_shape=(maxlen, len(chars))))
model.add(Dense(len(chars)))
model.add(Activation('softmax'))

optimizer = RMSprop(lr=0.01)
model.compile(loss='categorical_crossentropy', optimizer=optimizer)


def sample(preds, temperature=1.0):
    # helper function to sample an index from a probability array
    preds = np.asarray(preds).astype('float64')
    preds = np.log(preds) / temperature
    exp_preds = np.exp(preds)
    preds = exp_preds / np.sum(exp_preds)
    probas = np.random.multinomial(1, preds, 1)
    return np.argmax(probas)

# train the model, output generated text after each iteration
for iteration in range(1, 60):
    print()
    print('-' * 50)
    print('Iteration', iteration)
    model.fit(X, y, batch_size=128, nb_epoch=1)

    start_index = random.randint(0, len(text) - maxlen - 1)

    for diversity in [0.2, 0.5, 1.0, 1.2]:
        print()
        print('----- diversity:', diversity)

        generated = ''
        sentence = text[start_index: start_index + maxlen]
        generated += sentence
        print('----- Generating with seed: "' + sentence + '"')
        sys.stdout.write(generated)

        for i in range(400):
            x = np.zeros((1, maxlen, len(chars)))
            for t, char in enumerate(sentence):
                x[0, t, char_indices[char]] = 1.

            preds = model.predict(x, verbose=0)[0]
            next_index = sample(preds, diversity)
            next_char = indices_char[next_index]

            generated += next_char
            sentence = sentence[1:] + next_char

            sys.stdout.write(next_char)
            sys.stdout.flush()
print()
"""