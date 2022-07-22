######
Theory
######

.. include:: /references.txt

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

