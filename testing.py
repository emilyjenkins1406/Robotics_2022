# %%
# Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pathlib
import os
from tqdm import tqdm

def print_lines(file, n=10):
    with open(file) as myfile:
        head = [next(myfile) for _ in range(n)]
    print(*head)

# %%
# Load data
list_of_files = pathlib.Path("PuttyLogs").glob('*') # * means all if need specific format then *.csv
latest_file = max(list_of_files, key=os.path.getctime)

data = pd.read_csv(latest_file, skiprows=2)
data.dropna(inplace=True)
data.columns = [
    'time', # In seconds
    'acc_ax',
    'acc_vx',
    'acc_x',
    'wheel_ax',
    'wheel_vx',
    'wheel_x',
    'vx',
    'x',
    'weight',
    'count_l',
    'actual_x'
]

data = data.astype(float)
data[data.columns[np.logical_and(data.columns != 'weight', data.columns != 'count')]] /= 1000 # Convert from mm to meter

# %%
# Plot it
# data[['Acc_ax', 'Wheel_ax', 'vx', 'weight', 'actual_x']].plot()
data[['actual_x', 'count_l']].plot()

# Get idx of min value of data.Acc_ax and plot
min_idx = data['Acc_ax'].idxmin()
ymin, ymax = plt.ylim()
plt.vlines(x=min_idx, ymin=ymin, ymax=ymax, color='red')
plt.show()

# %%
# Python sim of code
time = data.time
old_time = time[0]
old_idx = 0

sample_rate = 50/1000 # s between measurements

sim_data = pd.DataFrame(0, index=data.index, columns=['acc', 'speed', 'pos'])

for i, t in enumerate(tqdm(time[1:]), start=1):
    if old_time + sample_rate < t:
        # Update values
        v = (data.count_l[i] - data.count_l[old_idx]) / (t - old_time)
        a = (v - sim_data.acc[old_idx]) / (t - old_time)

        # Save data
        sim_data.acc[old_idx] = a

    # Continue at same speed
    sim_data.speed[i] = sim_data.speed[i-1] + sim_data.acc[i] * (time[i-1] - t)
    sim_data.pos[i] = sim_data.pos[i-1] + sim_data.speed[i] * (time[i-1] - t)

# %%
# Plot
sim_data.plot()