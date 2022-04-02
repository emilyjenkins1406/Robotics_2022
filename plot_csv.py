# %%
# Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# %%
# 
data = pd.read_csv("PuttyLogs/putty-164348.log", skiprows=1, header=None)
data.dropna(inplace=True)
data.columns = ["Acc", "Wheel"]
data.plot(
    xlabel="Time (Arbitrary Units)",
    ylabel="Acceleratin in $mm/s^2$",
    title="Acceleration vs. Time",
)

plt.savefig("Figures/accelerometer-crash.png")
plt.savefig("Figures/accelerometer-crash.pdf")
plt.show()

data.min().round(0)