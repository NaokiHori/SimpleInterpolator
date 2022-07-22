######
Others
######

********
Features
********

   * Easy extension

      An incompressible flow solver usually contains Poisson solver, which is (almost) directly applicable here.
      Although an FFT-based solver is adopted by default, one can replace it with the other option, and as a result non-uniform grid spacings can be used even in :math:`y` direction.

   * Arbitrary interpolating method for scalar field

      Solenoidal velocity field is inherently guaranteed.
      User can choose a scheme to interpolate the scalar potential without taking into account the continuity.

****
TODO
****

Extensions to

   * Three-dimensional fields

   * Non-uniform grids in :math:`y` (and :math:`z`) direction(s)

   * Cylindrical and spherical coordinates

