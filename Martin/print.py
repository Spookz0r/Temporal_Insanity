import theano
import theano.tensor as T
import theano.tensor.nnet as nnet
import numpy as np
import gzip
import pickle
import random

def getrand(invals, expvals):
    nums = random.sample(range(50000), 5)
    new_invals = []
    new_expvals = []
    
    for i in nums:

        new_invals.append(invals[i])
        new_expvals.append(expvals[i])
    return new_invals, new_expvals


def layer(x, w):
    b = np.array([1], dtype=theano.config.floatX)
    new_x = T.concatenate([x,b])
    m = T.dot(w.T, new_x)
    h = nnet.sigmoid(m)
    return h
def grad_desc(cost, theta):
    alpha = 0.1 #learning rate
    return theta - (alpha * T.grad(cost, wrt=theta))

print("Hello")
theano.config.optimizer='None'
theano.config.exception_verbosity='high'
f = gzip.open('mnist.pkl.gz', 'rb')
train, valid, test = pickle.load(f, encoding='latin-1')
f.close()
train_a, train_b = train
train_in = np.asarray(train_a)
train_exp = np.asarray(train_b)
print(train_in.shape)
print(train_exp.shape)



test_a, test_b = test
test_in = np.asarray(test_a)
test_exp = np.asarray(test_b)
x = T.dvector()
y = T.dscalar()

theta1 = theano.shared(np.array(np.random.rand(785,785), dtype=theano.config.floatX)) # randomly initialize
theta2 = theano.shared(np.array(np.random.rand(786,1), dtype=theano.config.floatX))

#theta1 = theano.shared(np.array(np.random.rand(3,3), dtype=theano.config.floatX)) # randomly initialize
#theta2 = theano.shared(np.array(np.random.rand(4,1), dtype=theano.config.floatX))


hid1 = layer(x, theta1)
out1 = T.sum(layer(hid1, theta2)) #output layer
fc = (out1 - y)**2 #cost expression
cost = theano.function(inputs=[x, y], outputs=fc, updates=[
        (theta1, grad_desc(fc, theta1)),
        (theta2, grad_desc(fc, theta2))])
run_forward = theano.function(inputs=[x], outputs=out1)


#inputs = np.array([[0,1],[1,0],[1,1],[0,0]]).reshape(4,2) #training data X
#exp_y = np.array([0, 0, 1, 0]) #training data Y
cur_cost = 0
for i in range(100):
    inputs, exp_y = getrand(train_in, train_exp)
    for k in range(len(inputs)):
        cur_cost = cost(inputs[k], exp_y[k]) #call our Theano-compiled cost function, it will auto update weights
        #print('Cost: %s' % (cur_cost,))
print(test_in[420])
print(run_forward(test_in[0]))    
print(test_exp[420])    

print("Goodbye")