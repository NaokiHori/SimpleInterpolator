###################
Simple Interpolator
###################

|License|_ |CI|_

.. |License| image:: https://img.shields.io/github/license/NaokiHori/SimpleInterpolator
.. _License: https://opensource.org/licenses/MIT

.. |CI| image:: https://github.com/NaokiHori/SimpleInterpolator/actions/workflows/ci.yml/badge.svg
.. _CI: https://github.com/NaokiHori/SimpleInterpolator/actions/workflows/ci.yml

.. image:: https://raw.githubusercontent.com/NaokiHori/SimpleInterpolator/gh-pages/docs/_images/snapshot.png

********
Overview
********

This library interpolates a velocity field of wall-bounded flows to the other grid while keeping its original divergence.
Both velocity fields are assumed to be defined on staggered grids.

This might be useful to create an initial velocity field which must be solenoidal (e.g., multi-phase simulations with a phase indicator) from a flow field obtained by a coarser grid.

******
Method
******

Please refer to the documentation.

**********
Dependency
**********

Although the library uses FFTW3 just for simplicity (the author is interested in a very simple configuration), it is only needed by the Poisson solver and the interpolation itself is independent of it.
One can get rid of it by modifying the Poisson solver and scalar interpolating methods (see documentation).

For quick start to see what is going on, use `Docker <https://www.docker.com>`_.

***********
Quick start
***********

#. Create working directory

   .. code-block:: console

      $ mkdir /path/to/your/working/directory
      $ cd    /path/to/your/working/directory

#. Fetch source

   .. code-block:: console

      $ git clone https://github.com/NaokiHori/SimpleInterpolator
      $ cd SimpleInterpolator

#. Build

   .. code-block:: console

      $ docker build -t simpleinterpolator:latest .
      $ docker run -it --rm --workdir /project -v ${PWD}:/project simpleinterpolator:latest
      $ make all

   Notice that the original grid (number of points ``itot`` and ``jtot``, cell-face positions ``xf`` and ``yf``) and velocity (``ux`` and ``uy``) exist under ``bef`` directory:

   .. code-block:: text

      bef
      ├── itot.npy
      ├── jtot.npy
      ├── ux.npy
      ├── uy.npy
      ├── xf.npy
      └── yf.npy

   while the new grid (number of points ``itot`` and ``jtot``, cell-face positions ``xf`` and ``yf``) are stored in ``aft`` directory:

   .. code-block:: text

      aft
      ├── itot.npy
      ├── jtot.npy
      ├── xf.npy
      └── yf.npy

#. Run

   .. code-block:: console

      $ ./a.out

The results can be found as ``aft/ux.npy`` and ``aft/uy.npy``.
Maximum divergence of the original and interpolated velocity fields can be checked by

.. code-block:: console

   $ python check_divergence.py

giving e.g.

.. code-block:: text

   maximum divergence of bef:  2.7e-14
   maximum divergence of aft:  2.2e-12

