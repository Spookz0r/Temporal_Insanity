import numpy as np

from keras.models import Sequential
from keras.layers import Dense, LSTM
from keras.utils import np_utils




''' Define alphabet '''
alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

''' create mapping of characters to integers 0-25 and reverse'''
char_to_int = dict((c,i) for i,c in enumerate(alphabet))
int_to_char = dict((i,c) for i,c in enumerate(alphabet))

''' Prepare the dataset of input to output pairs encoded as integers '''

seq_length = 3
dataX = []
dataY = []
for i in range(0, len(alphabet) - seq_length, 1):
    seq_in = alphabet[i:i + seq_length]
    seq_out = alphabet[i + seq_length]
    dataX.append([char_to_int[char] for char in seq_in])
    dataY.append(char_to_int[seq_out])
    print (seq_in, '->', seq_out)
    
''' reshape data expected by LSTM network [samples, time steps, features] '''

X = np.reshape(dataX, (len(dataX), seq_length, 1))
#normalize
X = X / float(len(alphabet))

''' one hot encode the output variable '''
y = np_utils.to_categorical(dataY)


''' CREATING A NAIVE LSTM MODEL FOR LEARNING ONE-CHAR to ONE-CHAR MAPPING '''

''' Create and fit the model, 32 units and a single output neuron with softmax acitvation '''
model =  Sequential()
model.add(LSTM(32, input_shape=(X.shape[1], X.shape[2])))
model.add(Dense(y.shape[1], activation='softmax'))
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
model.fit(X, y, nb_epoch=500, batch_size=1, verbose=2)

# performace

scores = model.evaluate(X,y,verbose=0)
print("Model accuracy: %.2f%%" % (scores[1]*100))
model.save('my_model_recurrent.h5')


#demonstrate

for pattern in dataX:
    x = np.reshape(pattern, (1, len(pattern),1))
    x = x / float(len(alphabet))
    prediction = model.predict(x,verbose=0)
    index = np.argmax(prediction)
    result = int_to_char[index]
    seq_in = [int_to_char[value] for value in pattern]
    print(seq_in, "->" , result)
    





