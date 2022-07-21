import numpy as np


def main(dirname):
    xf = np.load("{}/xf.npy".format(dirname))
    yf = np.load("{}/yf.npy".format(dirname))
    ux = np.load("{}/ux.npy".format(dirname))
    uy = np.load("{}/uy.npy".format(dirname))
    # remove boundaries, add halo
    uy = uy[:, 1:-1]
    uy = np.vstack([uy, uy[0, :]])
    duxdx = np.diff(ux, axis=1)/np.diff(xf)
    duydy = np.diff(uy, axis=0).T/np.diff(yf)
    duydy = duydy.T
    div = duxdx+duydy
    print(div.shape)
    print("maximum divergence of {}: {: .1e}".format(dirname, np.max(np.abs(div))))

if __name__ == "__main__":
    main("bef")
    main("aft")
