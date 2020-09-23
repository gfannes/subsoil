import os
import re
import urllib.request
import tarfile
import numpy as np
import pandas as pd
from pandas.plotting import scatter_matrix
import matplotlib.pyplot as plt
from sklearn import model_selection, impute, pipeline, compose
import sklearn

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
        tt = sklearn.model_selection.train_test_split(data, test_size=ratio, random_state=random_state)
    else:
        if 0:
            data[stratifier].hist()
            plt.show()
        sss = sklearn.model_selection.StratifiedShuffleSplit(n_splits=1, test_size=ratio, random_state=random_state)
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
cols_num, cols_cat = [], []
if 'local_scope':
    cat_cols = {'ocean_proximity'}
    for name in data.columns:
        cols = cols_cat if name in cat_cols else cols_num
        cols.append(name)

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
(train, test) = tt


#Pimp data
#  Clean data
train_num, train_cat = train[cols_num], train[cols_cat]
pipeline_num, pipeline_cat = [], []
if 'clean_numerical_data':
    pipeline_num.append(('imputer', sklearn.impute.SimpleImputer(strategy="median")))
#  Handle categorical data
#    Ordinal, 1hot or embedding
if 'handle_categorical_data':
    strategy = 'ordinal'
    # strategy = 'onehot'
    if strategy == 'ordinal':
        def print_value_counts(train_cat):
            for name in cols_cat:
                print(name, '\n', train_cat[name].value_counts())
        print_value_counts(train_cat)
        ordcoder = sklearn.preprocessing.OrdinalEncoder()
        pipeline_cat.append(('ordinal', ordcoder))
        # ordcoder.fit(train_cat)
        # train_cat = pd.DataFrame(ordcoder.transform(train_cat), index=train_cat.index, columns=train_cat.columns)
        # print_value_counts(train_cat)
    if strategy == 'onehot':
        new_train_cat = None
        for name in cols_cat:
            data = train_cat[[name]]
            onehotcoder = sklearn.preprocessing.OneHotEncoder()
            pipeline_cat.append(('onehot', onehotcoder))
            onehotcoder.fit(data)
            x = onehotcoder.transform(data)
            print(x.shape)
            data = pd.DataFrame(x.toarray(), index=data.index, columns=[f"{name}_{ix}" for ix in range(x.shape[1])])
            print(data.columns)
            if not new_train_cat:
                new_train_cat = data
            else:
                new_train_cat.merge(data)
        train_cat = new_train_cat
    print(train_cat.head())

#  Create derived columns
if 'create_derived_columns':
    class DerivedVarsAdder(sklearn.base.BaseEstimator, sklearn.base.TransformerMixin):
        def __init__(self, column_names):
            self.column_names = column_names.tolist()
            self.new_column_names = []
        def fit(self, data, y=None):
            pass
        def transform(self, data, y=None):
            new_cols = (data,)
            def append_ratio(avg_name, value_name, total_name):
                self.new_column_names.append(avg_name)
                value_ix, total_ix = (self.column_names.index(e) for e in (value_name, total_name))
                new_col = data[:,value_ix]/data[:,total_ix]
                new_col = new_col.reshape((len(new_col),1))
                nonlocal new_cols
                new_cols = new_cols+(new_col,)
            append_ratio('avg_rooms', 'total_rooms', 'households')
            append_ratio('avg_bedrooms', 'total_bedrooms', 'households')
            append_ratio('household_size', 'population', 'households')
            append_ratio('bedroom_ratio', 'total_bedrooms', 'total_rooms')
            append_ratio('bedrooms_pp', 'total_bedrooms', 'population')
            append_ratio('rooms_pp', 'total_rooms', 'population')
            data = np.hstack(new_cols)
            return data
    deriver = DerivedVarsAdder(train_num.columns)
    pipeline_num.append(('deriver', deriver))

pipeline_num = sklearn.pipeline.Pipeline(pipeline_num)
pipeline_cat = sklearn.pipeline.Pipeline(pipeline_cat)
pipeline = sklearn.compose.ColumnTransformer([
    ('num', pipeline_num, cols_num),
    ('cat', pipeline_cat, cols_cat)
    ])

pipeline.fit(train)

train_num = pd.DataFrame(pipeline_num.transform(train_num), index=train_num.index, columns=train_num.columns.tolist()+deriver.new_column_names)
#    Check correlation with target
corr_matrix = train_num.corr()
print(corr_matrix[target].sort_values(ascending=False))

print(f'train_num.describe()\n: {train_num.describe()}')
train_num.info()

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

