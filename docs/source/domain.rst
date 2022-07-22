######
Domain
######

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

