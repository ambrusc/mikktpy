/* SWIG interface for mikktpy.cpp */

%module mikktspace
%{
/* Put header files here or function declarations like below */
#include <vector>
extern std::vector<float> compute_tri_tangents(const std::vector<float> &v);
%}

%include "std_vector.i"
namespace std {
   %template(vectorf) vector<float>;
};

extern std::vector<float> compute_tri_tangents(const std::vector<float> &v);
