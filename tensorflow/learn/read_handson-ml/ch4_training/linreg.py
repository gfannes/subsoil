import numpy as np

class Linreg:
    def __init__(self):
        self.all_params_ = []
        self.norms_ = []

    def fit(self, X, y, strategy = 'ne'):
        def case(what):
            return strategy == what
        if case('ne'):
            self.params = np.linalg.inv(X.T.dot(X)).dot(X.T).dot(y)
        elif case('gd'):
            self.params = np.ones(X.shape[1])*-1
            mu = 0.001
            for i in range(10000):
                gradient = (X.dot(self.params)-y).T.dot(X)*(2.0/X.shape[0])
                gradient_l2 = np.linalg.norm(gradient, ord=2)
                self.norms_.append(gradient_l2)
                if gradient_l2 > 100:
                    gradient = gradient*(100.0/gradient_l2)
                gradient_l2 = np.linalg.norm(gradient, ord=2)
                self.all_params_.append(self.params)
                self.params = self.params - mu*gradient
                print('params', self.params, 'gradient', gradient, 'l2', gradient_l2)
        else:
            self.params = np.zeros(X.shape[1])
        self.all_params_.append(self.params)

    def transform(self, X, params=None):
        params = self.params if params is None else params
        return np.dot(X, params)
