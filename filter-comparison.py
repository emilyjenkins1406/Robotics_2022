# %%
# Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# %%
# Plot the data
fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True, sharey=True)
plot_columns = ['X']
plot_from = 100

# Unfiltered
raw_data = pd.read_csv('putty-raw.log', skiprows=2)
raw_data[plot_columns][plot_from:].plot(ax=ax1, legend=False)
ax1.set_title('Unfiltered data')

# Filtered
filtered_data = pd.read_csv('putty-filtered-0.9.log', skiprows=2)
filtered_data[plot_columns][plot_from:].plot(ax=ax2, legend=False)
ax2.set_title('Filtered data')

fig.suptitle("Comparison of filtered and unfiltered accelerometer data\nfor the stationary robot")

plt.subplots_adjust(top=0.8)
fig.tight_layout()
ax2.set_xticks([])

plt.savefig("Figures/filter-comparison.png")
plt.savefig("Figures/filter-comparison.pdf")
plt.show()

# %%
