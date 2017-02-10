from __future__ import print_function
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.layers import LSTM
from keras.optimizers import RMSprop
from keras.utils.data_utils import get_file
from keras.models import load_model

import numpy as np
import random
import sys
from PIL import Image
import PIL
import cv2







model = load_model("lstm_pic.h5")
cap = cv2.VideoCapture(0)
ret, frame = cap.read()
gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)



samp = []
for i in range(100,130):
	samp.append(gray[0][i])
samp = np.array(samp)
samp = samp.reshape(1,1,30)
samp = samp.astype('float32')
samp /= 255
pred = model.predict(samp, verbose=1)

np.array(50400)

print(pred)
"""

# you have to change your input shape (nb_samples, timesteps, input_dim)
X_train = X_train.reshape(len(X_train), 1, 1)
# and also the output shape (note that the output *shape* is 2 dimensional)
y_train = y_train.reshape(len(y_train), 1)

img = np.array(Image.open("sexbomb.jpg"))

basewidth = 300
img = Image.open('sexbomb.jpg')
wpercent = (basewidth / float(img.size[0]))
hsize = int((float(img.size[1]) * float(wpercent)))
img = img.resize((basewidth, hsize), PIL.Image.ANTIALIAS)
img = np.array(img)
print(img.shape)
#img.save('resized_image.jpg')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = gray.reshape(1, 168, 300)
print("Gray img shape: ", gray.shape)

imglist = gray.reshape(50400)

pix_indices = dict((c, i) for i, c in enumerate(imglist))
indices_pix = dict((i, c) for i, c in enumerate(imglist))



seqlength = 30
samp = []
for i in range(100,130):
	samp.append(gray2[0][i])
print("Sample pixels")
print (samp)
samp = np.array(samp)
x = np.zeros((1, seqlength, len(imglist)))
for t, pix in enumerate(samp):
	x[0, t, pix_indices[pix]] = 1
preds = model.predict(x, verbose=1)


preds = preds.reshape(168, 300)
preds *= 200000000
preds2 = preds.astype('uint8')
for i in preds:
	for j in i:
		print(j)
image = Image.fromarray(preds2)
image.show()
"""
#for i in range(400):
#x = np.zeros((1, 30, 50400))
#samp = samp.reshape(1, 30)
#print (samp)
#out = model.predict(samp, 30 ,verbose = 1)
#for t, pix in enumerate