from linreg import Linreg
import numpy as np
import matplotlib.pyplot as plt
import sklearn
from sklearn import linear_model, preprocessing

def create_X(x):
    degree = 20
    if degree==1:
        X = np.c_[np.ones(x.shape), x]
    else:
        X = sklearn.preprocessing.PolynomialFeatures(degree=degree, include_bias=True).fit_transform(np.c_[x])
    return X

def create_X_y():
    x = np.linspace(-5.0, 5.0, 20)

    a, b = 1.0, 2.0
    noise = np.random.standard_normal(x.shape)*0.5
    y = np.sin(a*x+b)+noise

    return create_X(x), y

X, y = create_X_y()

class Namespace(dict):
    def __getattr__(self, name):
        return dict[name]

if 0*'my':
    my = Namespace()
    my.lr = Linreg()
    my.lr.fit(X, y, strategy='gd')
    my.preds = my.lr.transform(X)

if 1*'sk':
    sk = Namespace()
    sk.lr = sklearn.linear_model.LinearRegression()
    sk.lr.fit(X, y)
    sk.preds = sk.lr.predict(X)

if 1*'plot':
    # for params in my.lr.all_params_:
    #     preds = my.lr.transform(X, params=params)
    #     plt.plot(X[:,1], preds, '-y')
    # plt.plot(X[:,1], my.preds, '-r', label='my')
    plt.plot(X[:,1], sk.preds, '*b', label='sk')

    x = np.linspace(-5.0, 5.0, 1000)
    XX = create_X(x)
    preds = sk.lr.predict(XX)
    plt.plot(x, preds, '-b')
    plt.ylim(-2, 2)

    plt.plot(X[:,1], y, '.g')
    plt.legend()
    plt.show()

