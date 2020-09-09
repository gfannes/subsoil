import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sklearn.linear_model

dir = "../handson-ml/datasets/lifesat"

bli = pd.read_csv(f"{dir}/oecd_bli_2015.csv", thousands=',')
bli.info()

gdp = pd.read_csv(f"{dir}/gdp_per_capita.csv", delimiter='\t', encoding='latin1', thousands=',', na_values="n/a")
gdp.info()
