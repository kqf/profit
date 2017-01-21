#!/usr/bin/python


import sys
import matplotlib.pyplot as plt
import seaborn as sns
import re
import numpy as np


def update_dict(d, key, value):
	if not key in d: d[key] = []
	d[key].append(value)

class ParametersEvolution(object):
	def __init__(self, filename):
		super(ParametersEvolution, self).__init__()
		self.parameters, self.chi2 = self.read(filename)

	def read(self, filename):
		with open(filename, 'r') as f:
			useful = (l for l in f if self.is_parameter(l))
			data = [ self.parameter(l.split()) for l in useful]

		with open(filename, 'r') as f:
			fcn = (l for l in f if 'FCN=' in l)
			chi2 = [float(re.findall("\d*\.*\d+", l)[0]) for l in fcn]

		parameters = {}
		map(lambda x, y: update_dict(parameters, x, y), *zip(*data))
		return parameters, chi2

	def parameter(self, data):
		par = [k for k in data if self.is_parameter(k)][0]
		value = data[data.index(par) + 1]
		return par, float(value)
		
	def is_parameter(self, s):
		return "'" in s

	def draw_parameter(self, name, values, chi2):
		plt.plot(values, label = name)
		plt.ylabel(name)
		plt.xlabel('iteration')

		# Now plot FCN-chi2 value
		chi = np.array(chi2, dtype=float) * max(values) / max(chi2)
		plt.plot(chi, label = 'scaled FCN')

		plt.legend()
		# plt.yscale('log')
		plt.show()

	def draw_all_parameters(self, a = 0, b = -1):
		for k in self.parameters:
			self.draw_parameter(k, self.parameters[k][a:b], self.chi2[a:b]) 

def main():
	assert len(sys.argv) == 2, "No input found.\nUsage:\n\t./check someoutput "
	parameters = ParametersEvolution(sys.argv[1])
	parameters.draw_all_parameters(3000)

if __name__ == '__main__':
	main()