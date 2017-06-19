#!/usr/bin/python2

import numpy as np
import seaborn
import matplotlib.pyplot as plt

def read_data(filename):
    processors, times = np.loadtxt(filename).T
    processors.astype(int)

    data = {}
    for p, t in zip(processors, times):
        if not p in data:
            data[p] = np.array([t])

        data[p] = data[p] + [t]


    for p in data:
        data[p] = data[p].mean()

    return data.keys(), [data[p].mean() for p in data]



def main():
    x, y = read_data('execution_time.txt')

    plt.ylabel('execution time')
    plt.xlabel('N processes')
    plt.plot(x, y, 'o')
    plt.show()


if __name__ == '__main__':
    main()
