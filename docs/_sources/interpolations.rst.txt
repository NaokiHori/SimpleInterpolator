#########################################
Appendix - one-dimensional interpolations
#########################################

.. image:: one-dimensional-interpolations.pdf
   :width: 800

As being discussed before, one can use an arbitrary scheme to interpolate the scalar potential :math:`\psi`.
By default, cubic and spectral interpolations are adopted in :math:`x` and :math:`y` directions, respectively and used in `src/kernel/interpolate_scalar_potential.c <https://github.com/NaokiHori/SimpleInterpolator/blob/main/src/kernel/interpolate_scalar_potential.c>`_.
Here they are briefly reviewed for completeness.

**********************
Spectral interpolation
**********************

==========
Definition
==========

.. code-block:: c

   int simple_interpolation_fft(
       const int n1,
       const int n2,
       const double *y1s,
       double *y2s
   );

=========== ====== ===============================
Name        intent description
=========== ====== ===============================
n1          in     number of points of input data
n2          in     number of points of output data
y1s         in     input signal (length is n1)
y2s         out    output signal (length is n2)
=========== ====== ===============================

.. warning::
   For now only even numbers are accepted for ``n1`` and ``n2``.

===========
Description
===========

This function interpolates a periodic signal to a new (refined or coarsened) coordinate.
As a first step, the original discrete signal :math:`y_i`, :math:`i = 0, 1, \cdots, n1-1`, is projected to a spectral space using Fourier transform:

.. math::
   Y_j = \mathcal{F} \left[ y_i \right],

where :math:`Y_j` contains power spectra of the original dataset, which is `extended` by adding zeros to the higher frequency part.

For instance, we consider the following signal (``n1 = 8``):

.. math::
   \begin{bmatrix}
      Y_0 & Y_1 & Y_2 & Y_3 & \frac{Y_4+Y_4^*}{2} & Y_3^* & Y_2^* & Y_1^*,
   \end{bmatrix}

where asterisk denotes complex conjugate of the quantity.
We would like to interpolate this dataset to a refined coordinate (say ``n2 = 12``), which is achieved by

.. math::
   \begin{bmatrix}
      Y_0 & Y_1 & Y_2 & Y_3 & \frac{Y_4}{2} & 0 & 0 & 0 & \frac{Y_4^*}{2} & Y_3^* & Y_2^* & Y_1^*,
   \end{bmatrix}

where higher frequency part between :math:`\frac{Y_4}{2}` and :math:`\frac{Y_4^*}{2}` are padded by zero.

Note that, because of the redundancy, only half of the spectral data needs to be kept, i.e.,

.. math::
   \begin{bmatrix}
      Y_0 & Y_1 & Y_2 & Y_3 & \frac{Y_4+Y_4^*}{2}
   \end{bmatrix}

(whose length is :math:`n_1/2+1`) and

.. math::
   \begin{bmatrix}
      Y_0 & Y_1 & Y_2 & Y_3 & \frac{Y_4}{2} & 0 & 0
   \end{bmatrix}

(whose length is :math:`n_2/2+1`) are sufficient to describe the original data in physical space.

*******************
Cubic interpolation
*******************

==========
Definition
==========

.. code-block:: c

   int simple_interpolation_cubic(
       const int n1,
       const int n2,
       const double *x1s,
       const double *x2s,
       const double *y1s,
       double *y2s
   );

=========== ====== ==========================================
Name        intent description
=========== ====== ==========================================
n1          in     number of points of input data
n2          in     number of points of output data
x1s         in     positions where input signals are defined
x2s         in     positions where output signals are defined
y1s         in     input signal (length is n1)
y2s         out    output signal (length is n2)
=========== ====== ==========================================

.. warning::
   This function only accepts bounded signals, i.e., the first points of ``x1s`` and ``x2s`` should be matched (``x1s[0] = x2s[0]``).
   Same holds for the last points (``x1s[-1] = x2s[-1]``).

===========
Description
===========

This function interpolates a bounded signal to a new (refined or coarsened) locations by fitting piecewise third-order polynomials :math:`y = a_3 x^3 + a_2 x^2 + a_1 x^1 + a_0 x^0`.
Since a third-order polynomials have four degrees of freedom, four points are needed to determine all coefficients.
Thus, in the vicinity of the walls, quadratic functions are used instead.

