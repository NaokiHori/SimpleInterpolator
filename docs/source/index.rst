###################
Simple Interpolator
###################

Direct and divergence-free velocity interpolator

.. image:: images/snapshot/snapshot.png
   :width: 800

.. include:: /references.txt

******
Theory
******

.. seealso::

   The method is based on |BAO2017|, which is slightly modified to wall-bounded domains.

We consider a two-dimensional domain whose :math:`x` direction is wall-bounded whereas periodic boundary conditions are imposed in :math:`y` direction.

:math:`z` component of the vorticity vector :math:`\omega_z` leads

.. math::
   \omega_z
   =
   -\der{\uy}{x}
   +\der{\ux}{y},

which satisfies a Poisson equation

.. math::
   \der{}{x} \left( \der{\psi}{x} \right)
   +
   \der{}{y} \left( \der{\psi}{y} \right)
   =
   \omega_z,

where :math:`\psi` is a scalar potential (stream function for two-dimensional cases) whose derivatives give velocity

.. math::
   \ux &=  \der{\psi}{y}, \\
   \uy &= -\der{\psi}{x}.

Note that this relation indicates a proper boundary condition on the walls

.. math::
   \vat{\der{\psi}{x}}{walls}
   =
   - \vat{\uy}{x = 0, x = l_x},

i.e., Neumann boundary condition.

Since all operators are linear, conventional finite-difference approximations are directly applicable.
Also an extension to three-dimensional domain is straightforward.

****************
Numerical method
****************

We consider a domain with ``itot + 2`` and ``jtot`` pressure nodes :math:`\vat{p}{\pic, \pjc}` in :math:`x` and :math:`y` directions, respectively:

.. image:: images/grid/staggered1.pdf
   :width: 800

Here indices are also shown, which are used in the code.

Since :math:`x` direction is wall-bounded while periodic boundary conditions are assumed in :math:`y` direction, :math:`x` velocity :math:`\vat{\ux}{\xic, \xjc}` (defined at :math:`x` cell faces) and :math:`y` velocity :math:`\vat{\uy}{\yic, \yjc}` (defined at :math:`y` cell faces) have :math:`\left( itot+1 \right) \times jtot` and :math:`\left( itot + 2 \right) \times jtot` points to describe the whole velocity information (including the boundary conditions):

.. image:: images/grid/staggered2.pdf
   :width: 800

.. image:: images/grid/staggered3.pdf
   :width: 800

Vorticity and scalar potential are defined at cell corners:

.. image:: images/grid/staggered4.pdf
   :width: 800

.. note::

   Halo cells which are needed to evaluate discrete derivatives in the vicinity of the boundaries are also included in the above pictures.

The interpolation algorithm consists of 4 steps, which are implemented under ``src/kernel``.

#. Compute vorticity

   .. math::
      \omega_z
      =
      -\dder{\uy}{x}
      +\dder{\ux}{y},

   which are defined at all cell corners including points on the walls.

#. Compute scalar potential

   We solve a Poisson equation

   .. math::
      \dder{}{x} \left( \dder{\psi}{x} \right)
      +
      \dder{}{y} \left( \dder{\psi}{y} \right)
      =
      \omega_z,

   whose solutions are defined at all cell corners (same locations as :math:`\omega_z`).
   This equation is solved by an FFT-based solver which is adopted in `Simple Navier-Stokes Solver <https://naokihori.github.io/SimpleNavierStokesSolver/implementation/fluid/compute_potential.html>`_.

#. Interpolate scalar potential

   :math:`\psi` is interpolated to the new grid corners.
   User can choose an arbitrary scheme based on the accuracy, TVD property, etc.
   In the current example, cubic interpolations are used in :math:`x` direction, while information is interpolated in the spectral domain in :math:`y` direction thanks to the uniform grid spacing.

#. Compute velocity

   New velocity field on the interpolated grid is obtained by

   .. math::
      \ux &=  \dder{\psi}{y}, \\
      \uy &= -\dder{\psi}{x},

   which are of course defined at :math:`x` and :math:`y` cell faces, respectively.
   Conventional finite-difference schemes are used.

********
Features
********

   * Incompressible flow solvers usually contains Poisson solver, which is directly applicable here. Although an FFT-based solver is adopted by default, one can replace it with the other option and as a result non-uniform grid spacings can be used even in :math:`y` direction.

   * Solenoidal velocity field is inherently guaranteed. One can use an arbitrary scheme to interpolate the velocity field without taking into account the continuity.

****
TODO
****

Extensions to

   * Three-dimensional fields

   * Non-uniform grids in :math:`y` (and :math:`z`) direction(s)

   * Cylindrical and spherical coordinates

