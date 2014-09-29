import matplotlib.pyplot as plt
import subprocess

x = 0
y = 0
xc = [x]
yc = [y]
for i in range(1, 64 * 64):
	args = ['./hnext', '-n', '6', str(x), str(y)]
	proc = subprocess.Popen(args, stdout = subprocess.PIPE)
	line = proc.stdout.readline()
	splits = line.split()
	x = int(splits[0])
	y = int(splits[1])
	xc.append(x)
	yc.append(y)
plt.plot(xc, yc)
plt.show()

x = 0
y = 0
xc = [x]
yc = [y]
for i in range(1, 64 * 64):
	args = ['./znext', '-n', '6', '-d', '2',  str(x), str(y)]
	proc = subprocess.Popen(args, stdout = subprocess.PIPE)
	line = proc.stdout.readline()
	splits = line.split()
	x = int(splits[0])
	y = int(splits[1])
	xc.append(x)
	yc.append(y)
plt.plot(xc, yc)
plt.show()
