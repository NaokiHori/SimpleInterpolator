import numpy as np


# number of pressure nodes
itot = 1024
jtot = 2048

# domain size, which should be the same as the original domain
x = {"min": 0., "max": 1.}
y = {"min": 0., "max": 2.}

# cell-face locations
# arbitrary arrangement works in x as long as the first and the last points are identical to x["min"] and x["max"], respectively.
# only uniform spacing is allowed in y since we adopt FFT-based poisson solver
xf = np.linspace(x["min"], x["max"], itot+1)
yf = np.linspace(y["min"], y["max"], jtot+1)

# save results, which will be loaded
np.save("itot.npy", itot)
np.save("jtot.npy", jtot)
np.save("xf.npy", xf)
np.save("yf.npy", yf)
