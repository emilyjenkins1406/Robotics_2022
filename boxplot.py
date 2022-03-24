# Imports
import seaborn as sns
from matplotlib import pyplot as plt
import pandas as pd
import csv
from tqdm import tqdm

# Get data
with open("putty.log") as f:
    lines = f.readlines()
lines = lines[1:]

# Sort lines into lists of data
data_x = {}
data_y = {}
data_z = {}

for line in tqdm(lines):
    if line.startswith('Smoothing factor '):
        key = float(line[-4:])
        data_x[key] = []
        data_y[key] = []
        data_z[key] = []
    elif line.strip() == "X, Y, Z" or line.strip() == "DONE":
        pass
    else:
        for row in csv.reader([line]):
            data_x[key].append(float(row[0]))
            data_y[key].append(float(row[1]))
            data_z[key].append(float(row[2]))

x_err = pd.DataFrame(data_x)
y_err = pd.DataFrame(data_y)
z_err = pd.DataFrame(data_z)

# We can now plot both datasets at the same time.
plot_x_error = sns.boxplot(data=x_err)
plot_x_error.set(xlabel ="alpha $\\alpha$", ylabel = "Error", title ='Box Plot for X Values')
plt.savefig("Figures/boxplot_x.png")
plt.show()

plot_y_error = sns.boxplot(data=y_err)
plot_y_error.set(xlabel ="alpha $\\alpha$", ylabel = "Error", title ='Box Plot for Y Values')
plt.savefig("Figures/boxplot_y.png")
plt.show()

plot_z_error = sns.boxplot(data=z_err)
plot_z_error.set(xlabel ="alpha $\\alpha$", ylabel = "Error", title ='Box Plot for Z Values')
plt.savefig("Figures/boxplot_z.png")
plt.show()