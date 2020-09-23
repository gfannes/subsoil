import sklearn
from sklearn import preprocessing
import pandas as pd

data = pd.DataFrame({'cat': ['a','b','c','a'],})
print(data.head())

ordcoder = sklearn.preprocessing.OrdinalEncoder()
data = ordcoder.fit_transform(data)
print(data.head())

