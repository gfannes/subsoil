import os
import re
import urllib.request
import tarfile
import numpy as np
import pandas as pd
from pandas.plotting import scatter_matrix
import matplotlib.pyplot as plt
import sklearn.model_selection as skms

def load_data(name):
    root = "https://raw.githubusercontent.com/ageron/handson-ml2/master"
    path = f"datasets/{name}"
    url = os.path.join(root, path, f"{name}.tgz")

    def download_data(url, path):
        fn_tgz = os.path.join(path, f"{name}.tgz")
        if not os.path.isfile(fn_tgz):
            print(f"Downloading {fn_tgz} from {url}")
            os.makedirs(path, exist_ok=True)
            urllib.request.urlretrieve(url, fn_tgz)
        return fn_tgz
    fn_tgz = download_data(url, path)

    def extract_data(fn_tgz, path):
        with tarfile.open(fn_tgz) as tf:
            tf.extractall(path=path)
        fn_csv = re.sub(".tgz", ".csv", fn_tgz)
        if not os.path.isfile(fn_csv):
            raise UserWarning(f"Could not extract {fn_csv} from {fn_tgz}")
        return fn_csv
    fn_csv = extract_data(fn_tgz, path)

    data = pd.read_csv(fn_csv)
    return data

def split_tt(data, ratio):
    random_state = 43
    stratifier = 'income_cat'
    data[stratifier] = pd.cut(data['median_income'], bins=[0, 2.35, 3.14, 3.96, 5.1, np.inf], labels=np.linspace(1,5,num=5))
    if 0:
        tt = skms.train_test_split(data, test_size=ratio, random_state=random_state)
    else:
        if 0:
            data[stratifier].hist()
            plt.show()
        sss = skms.StratifiedShuffleSplit(n_splits=1, test_size=ratio, random_state=random_state)
        tt = None
        for train_ix, test_ix in sss.split(data, data[stratifier]):
            tt = (data.loc[train_ix], data.loc[test_ix])
    if 1:
        for t in tt:
            print(t[stratifier].value_counts()/len(t))
    for e in ((data,)+tt):
        e.drop(stratifier, axis=1, inplace=True)
    return tt

#Understand the business case and usage
target = 'median_house_value'
x, y = 'longitude', 'latitude'

#Load data
data = load_data("housing")

#Inspect data
#  Raw data
print(data.head())
#  Feature types
data.info()
print(data["ocean_proximity"].value_counts())
#  Feature ranges
print(data.describe())
for q in np.linspace(0.0, 1.0, num=6):
    print(data['median_income'].quantile(q))
#  Histograms
if 0:
    data.hist(bins=50)
    plt.show()
#  Correlation with target
corr_matrix = data.corr()
print(corr_matrix[target].sort_values(ascending=False))
#  Visualize data
#    Histograms
if 0:
    data.hist('median_income')
    plt.show()
#    Scatterplots
if 0:
    attrs = [target, 'median_income', 'total_rooms', 'housing_median_age']
    scatter_matrix(data[attrs])
    plt.show()
#    Geographical plots
if 0:
    data.plot(kind='scatter', x=x, y=y, label='population', alpha=0.1, s=data['population']/100, c=target, cmap=plt.get_cmap('jet'))
    plt.show()
#Create stable test set
tt = split_tt(data, 0.2)
for name, t in zip("Train Test".split(), tt):
    print(f"{name} {len(t)}")
    t.info()
#Pimp data
#  Clean data
#  Handle categorical data
#    Ordinal, 1hot or embedding
#  Create derived columns
#    Check correlation with target
#Search for good model structures
#  Create Scikit pipeline
#    Derivation of new columns
#    (Normalization and standardization)
#  Learn model
#Fine-tune model
#  Search hyperparamater
#    Grid
#    Random search
#  CrossValidation
#  Ensemble model
#Save model
#Test best model on test data
#Launch

