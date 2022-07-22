##############
Implementation
##############

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

