import numpy as np
import sklearn
from sklearn import linear_model, model_selection, base, metrics

if False:
    from sklearn import datasets
    import sklearn

    #This loads very slow: 12 seconds
    mnist = sklearn.datasets.fetch_openml('mnist_784', version=1)

    print( f'{type(mnist)}' )
    print( f'{mnist.keys()}' )
    print( f'{mnist.DESCR}' )

    inp = mnist.data
    print( inp.shape )
    tgt = mnist.target
    print( tgt.shape )

import mnist
import matplotlib.pyplot as plt

class Data:
    __slots__ = ['n','images','labels','inputs','labels01']
    def __init__(self, images, labels):
        if 1*'small data':
            n = 1000
            images = images[0:n]
            labels = labels[0:n]
        self.images = images
        self.labels = labels

        self.n = self.images.shape[0]

        s = self.images.shape
        self.inputs = self.images.reshape(s[0], s[1]*s[2])
        self.labels01 = (labels == 5)

train = Data(mnist.train_images(), mnist.train_labels())
test = Data(mnist.test_images(), mnist.test_labels())
for e in (train, test):
    print( f'Images shape: {e.images.shape}')
    print( f'Inputs shape: {e.inputs.shape}')

if 'inspect data':
    if 0*'show images':
        for ix in range(10):
            print( f'{ix}: {train.labels[ix]}' )
            plt.imshow(train.images[ix], cmap='binary')
            plt.show()
    if 0*'show digit histogram':
        plt.hist(train.labels)
        plt.show()

if 0*'show digit histogram':
    @np.vectorize
    def convert_label(label):
        return 1 if label else 0
    plt.hist(convert_label(train.labels01))
    plt.show()

clas = sklearn.linear_model.SGDClassifier()
clas.fit(train.inputs, train.labels01)

if 0*'one by one prediction':
    oktot = (0,0)
    for ix in range(train.n):
        input,actual = train.inputs[ix], train.labels01[ix]
        pred = clas.predict([input])
        oktot = (oktot[0]+(1 if pred==actual else 0), oktot[1]+1)
        if pred != actual:
            print( f'{ix}: pred {pred} actual {actual}' )
    print( f'oktot: {oktot}' )
predictions = clas.predict(train.inputs)
print( f'nr correct: {sum(predictions==train.labels01)}' )

class Never5Classifier(sklearn.base.BaseEstimator):
    def fit(self, X, y=None):
        pass
    def predict(self, X):
        return np.zeros((X.shape[0],1), dtype=bool)
if 0*'always predict not-5':
    clas = Never5Classifier()

accuracies = sklearn.model_selection.cross_val_score(clas, train.inputs, train.labels01, cv=3, scoring='accuracy')
print( f'CV accuracies: {accuracies}' )

if 'CV predictions':
    cv_predictions = sklearn.model_selection.cross_val_predict(clas, train.inputs, train.labels01, cv=3)
    print( f'{cv_predictions}' )

    print( f'confusion_matrix: {sklearn.metrics.confusion_matrix(train.labels01, cv_predictions)}' )
    print( f'precision: {sklearn.metrics.precision_score(train.labels01, cv_predictions)}' )
    print( f'recall: {sklearn.metrics.recall_score(train.labels01, cv_predictions)}' )
    print( f'harmonic mean of precision and recall: {sklearn.metrics.f1_score(train.labels01, cv_predictions)}' )

if 'CV precision/recall curve':
    cv_scores = sklearn.model_selection.cross_val_predict(clas, train.inputs, train.labels01, cv=3, method='decision_function')
    thresholds = np.unique(cv_scores)
    precisions = [sklearn.metrics.precision_score(train.labels01, (cv_scores > threshold)) for threshold in thresholds]
    recalls    = [sklearn.metrics.recall_score   (train.labels01, (cv_scores > threshold)) for threshold in thresholds]
    plt.plot(thresholds, precisions, label='precision')
    plt.plot(thresholds, recalls, label='recall')
    plt.legend()
    plt.show()
