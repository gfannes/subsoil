import numpy as np
import sklearn
from sklearn import linear_model, model_selection, base, metrics, svm
import functools
import matplotlib
import matplotlib.pyplot as plt
import math

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

def cache(ftor):
    cached_value = None
    def helper():
        nonlocal cached_value
        if cached_value is None:
            cached_value = ftor()
        return cached_value
    return helper

@cache
def compute_cv_scores():
    print("Computing")
    return sklearn.model_selection.cross_val_predict(clas, train.inputs, train.labels01, cv=3, method='decision_function')

if 0*'CV precision/recall curve':
    cv_scores = compute_cv_scores()
    def each_precision_recall():
        thresholds = np.unique(cv_scores)
        for ix in range(len(thresholds)-1):
            threshold = thresholds[ix]
            precision = sklearn.metrics.precision_score(train.labels01, (cv_scores > threshold))
            recall = sklearn.metrics.recall_score(train.labels01, (cv_scores > threshold))
            yield threshold, precision, recall
    tpr = np.array(list(each_precision_recall()))
    # plt.plot(tpr[:,0], tpr[:,1], label='precision')
    plt.plot(tpr[:,1], tpr[:,2], label='recall')
    plt.legend()
    plt.show()
if 0*'CV ROC':
    cv_scores = compute_cv_scores()
    print( f'AUC: {sklearn.metrics.roc_auc_score(train.labels01, cv_scores)}' )
    fpr, tpr, _ = sklearn.metrics.roc_curve(train.labels01, cv_scores)
    plt.plot(fpr, tpr, '-')
    plt.show()

if 'SVM':
    svm =sklearn.svm.SVC()
    cv_scores = sklearn.model_selection.cross_val_score(svm, train.inputs, train.labels, cv=3, scoring='accuracy')
    print(cv_scores)
    cv_predictions = sklearn.model_selection.cross_val_predict(svm, train.inputs, train.labels, cv=3)
    confusion_matrix = sklearn.metrics.confusion_matrix(train.labels, cv_predictions).astype('float64')
    print(confusion_matrix)
    sums = confusion_matrix.sum(axis=1)
    print(sums)
    confusion_matrix = confusion_matrix / sums
    # confusion_matrix = confusion_matrix * (np.ones(confusion_matrix.shape)-np.eye(confusion_matrix.shape[0]))
    np.fill_diagonal(confusion_matrix, 0)
    print(confusion_matrix)
    if 0*'plot':
        plt.matshow(confusion_matrix)
        plt.show()

    def plot_digits(instances, images_per_row=10, **options):
        size = 28
        images_per_row = min(len(instances), images_per_row)
        images = [instance.reshape(size,size) for instance in instances]
        n_rows = (len(instances) - 1) // images_per_row + 1
        row_images = []
        n_empty = n_rows * images_per_row - len(instances)
        images.append(np.zeros((size, size * n_empty)))
        for row in range(n_rows):
            rimages = images[row * images_per_row : (row + 1) * images_per_row]
            row_images.append(np.concatenate(rimages, axis=1))
        image = np.concatenate(row_images, axis=0)
        plt.imshow(image, cmap = matplotlib.cm.binary, **options)
        plt.axis("off")
    def my_plot_digits(images, nr_per_row=5):
        nr_images = len(images)
        if not nr_images:
            return
        shape = images[0].shape
        nn = nr_per_row*nr_per_row
        if nr_images < nn:
            images = np.concatenate([images, np.zeros((nn-nr_images,)+shape)], axis=0)
        cols = [np.vstack(images[ix:ix+nr_per_row]) for ix in range(0, nn, nr_per_row)]
        image = np.hstack(cols)
        plt.imshow(image)

    cla, clb = 4, 9
    ta = train.images[(train.labels==cla) & (cv_predictions==cla)]
    tb = train.images[(train.labels==clb) & (cv_predictions==clb)]
    fa = train.images[(train.labels==clb) & (cv_predictions==cla)]
    fb = train.images[(train.labels==cla) & (cv_predictions==clb)]
    plt.subplot(221); my_plot_digits(ta, nr_per_row=5)
    plt.subplot(222); my_plot_digits(fb, nr_per_row=5)
    plt.subplot(223); my_plot_digits(fa, nr_per_row=5)
    plt.subplot(224); my_plot_digits(tb, nr_per_row=5)
    plt.show()
