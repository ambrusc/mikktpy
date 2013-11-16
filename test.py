#
# Copyright (c) 2013 Ambrus Csaszar
#
# Tests the mikktspace python extension using known data.
# Currently the extension only supports un-indexed triangle data passed as a
# flat tuple or list (see format hints below).
#

import mikktspace as mk

test_tris = (
  0, 0, 0,                            # position x,y,z
  0, -0.70710678118, 0.70710678118,   # normal x,y,z  (normalized)
  0, 0,                               # texcoord u,v

  1, 0, 0,
  0.70710678118, 0, 0.70710678118,
  1, 1,

  0, 1, 0,
  0, 0.70710678118, 0.70710678118,
  0, 1,

  0, 1, 0,
  0, 0.70710678118, 0.70710678118,
  0, 1,

  1, 0, 0,
  0.70710678118, 0, 0.70710678118,
  1, 1,

  1, 1, 0,
  -0.70710678118, 0, 0.70710678118,
  1, 2,
)

expected_result = (
  0.8164966702461243, -0.40824833512306213, -0.40824827551841736, # tangent (normalized)
  0.0, 0.7071068286895752, 0.7071067690849304,                    # bi-tangent (normalized)
  1.4142135381698608, 1.0,                                        # mag-tan, mag-bitan

  0.40824833512306213, -0.8164965510368347, -0.40824827551841736, 
  0.0, 0.9999999403953552, 0.0, 
  1.4142135381698608, 1.0, 

  0.8164965510368347, -0.40824833512306213, 0.40824827551841736, 
  0.0, 0.7071067690849304, -0.7071067094802856,
  1.4142135381698608, 1.0,

  0.8164965510368347, -0.40824833512306213, 0.40824827551841736,
  0.0, 0.7071067690849304, -0.7071067094802856,
  1.4142135381698608, 1.0,

  0.40824833512306213, -0.8164965510368347, -0.40824827551841736,
  0.0, 0.9999999403953552, 0.0,
  1.4142135381698608, 1.0,

  0.40824833512306213, -0.8164966702461243, 0.40824827551841736,
  0.0, 1.0, 0.0,
  1.4142135381698608, 1.0
)

# Compute
ret = mk.compute_tri_tangents(test_tris)

# Validate
if len(ret) != len(expected_result):
  print "ERROR: length mismatch"
if ret == expected_result:
  print "All good. Exact result obtained."
else:
  err = 0
  for i in xrange(len(ret)):
    err = max(err, abs(expected_result[i] - ret[i]))
  print "Inexact result: max error is %s" % err
