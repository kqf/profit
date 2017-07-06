#!/usr/bin/python

import numpy as np
import seaborn
import matplotlib.pyplot as plt

def read_data(filename):
    processors, times = np.loadtxt(filename).T
    processors = processors.astype(int)

    data = {}
    for p, t in zip(processors, times):
        if not p in data:
            data[p] = np.array([t])

        data[p] = np.append(data[p], [t])

    print data.keys()
    print data
    return data.keys(), [data[p].mean() for p in data], [data[p].std() for p in data]



def main():

    labels = ['intel', 'amd']
    for label in labels:
        x, y, err = read_data('execution_time.txt.{0}'.format(label))
        print x, y
        plt.ylabel('execution time, in seconds')
        plt.xlabel('N processes')
        plt.errorbar(x, y, yerr = err, fmt='o', label = label)
        
    plt.legend()
    plt.show()


if __name__ == '__main__':
    main()
