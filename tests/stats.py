import numpy as np
import matplotlib.pyplot as plt

# all metrics available
metrics = ['inst_count', 'loads', 'stores', 'allocas', 'binary_and_unary', 'time']
# which metrics to show : how it appears in legend
display = ['loads', 'stores', 'time', 'allocas']


data = eval(open('parsed.txt').read())
metrics_percent_change = {}
for m in metrics:
    metrics_percent_change[m] = []

for case, val in data.items():
    before, after = val
    for m in metrics:
        metrics_percent_change[m].append(after[m]/before[m])

# test case names
keys = [x.split('/')[-1][5:-2] for x in data.keys()]
X = np.arange(len(keys))
labels = [x.capitalize() for x in display]

fig = plt.figure()
ax = fig.add_subplot(111)
N = len(display) + 1
plt.xticks(X + (N - 2.0)/(N*2), tuple(keys))

i = -1
for k in display:
    i += 1
    arr = metrics_percent_change[k]
    w = 1.0/N 
    ax.bar(X + w*i, arr, width=w, linewidth=1, edgecolor='k', zorder=2)

# loads = metrics_percent_change['loads']
# stores = metrics_percent_change['stores']
# time = metrics_percent_change['time']


# vals = [loads, stores, time]

# ax.bar(X + 0.00, vals[0], width = 0.25, linewidth=1, edgecolor='k', zorder=2)
# ax.bar(X + 0.25, vals[1], width = 0.25, linewidth=1, edgecolor='k', zorder=2)
# ax.bar(X + 0.50, vals[2], width = 0.25, linewidth=1, edgecolor='k', zorder=2)
# ax.legend(labels=['Loads', 'Stores', 'Time'])
ax.legend(labels=labels)
ax.axhline(y=1,linewidth=1, color='0.7', linestyle='--', zorder = 1)
ax.set_title('Results as Percent of Original Value (Lower is Better)')
ax.set_ylabel('Percent of Original Value')



plt.show()
