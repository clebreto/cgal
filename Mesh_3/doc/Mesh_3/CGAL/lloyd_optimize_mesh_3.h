namespace CGAL {

/*!
\ingroup PkgMesh_3

The function `lloyd_optimize_mesh_3` is a mesh optimization process 
based on the minimization of a global energy function. 

In `lloyd_optimize_mesh_3`, the minimized global energy may be interpreted 
as the \f$ L^1\f$-norm of the error achieved 
when the function \f$ x^2\f$ is interpolated on the mesh domain 
using a piecewise linear function which is linear 
in each cell of the Voronoi diagram of the mesh vertices. 

The optimizer `lloyd_optimize_mesh_3` works in iterative steps. 
At each iteration, mesh vertices are moved into 
positions that bring to zero the energy gradient 
and the Delaunay triangulation is updated. 
Vertices on the mesh boundaries are handled 
in a special way so as to preserve an accurate 
representation of the domain boundaries. 

\pre `time_limit` \f$ \geq\f$ 0 and 0 \f$ \leq\f$ `convergence` \f$ \leq\f$ 1 and 0 \f$ \leq\f$ `freeze_bound` \f$ \leq\f$ 1 

Parameters 
-------------- 

Parameter `C3T3` is required to be a model of the concept 
`MeshComplex_3InTriangulation_3`. 
The argument `c3t3`, passed by 
reference, provides the initial mesh 
and is modified by the algorithm 
to represent the final optimized mesh. 

Parameter `MeshDomain_3` is required to be a model of the concept 
`MeshDomain_3`. The argument `domain` must be the `MeshDomain_3` 
object used to create the `c3t3` parameter. 

The function has four optional parameters which are named parameters (we use the Boost.Parameter library). 
Therefore, when calling the function, the parameters can be provided in any order 
provided that the names of the parameters are used 
(see example at the bottom of this page). 
<UL> 
<LI>Parameter `time_limit`, whose name is `parameters::time_limit`, 
is used to set up, in seconds, 
a CPU time limit after which the optimization process is stopped. This time is 
measured using `CGAL::Timer`. 
The default value is `0` and means that there is no time limit. 
<LI>Parameter `max_iteration_number`, whose name is 
`parameters::max_iteration_number`, sets a limit on the 
number of performed iterations. The default value of `0` means that there is 
no limit on the number of performed iterations. 
<LI>Parameter `convergence`, whose name is 
`parameters::convergence`, is a stopping criterion based on convergence: 
the optimization process is stopped, when at the last iteration, 
the displacement of any vertex is less than a given percentage of the 
length of the shortest edge incident to that vertex. 
The parameter `convergence` gives the threshold ratio. 
<LI>Parameter `freeze_bound`, whose name is `parameters::freeze_bound`, 
is designed to reduce running time of each optimization iteration. Any vertex 
that has 
a displacement less than a given percentage of the length of its shortest incident edge, is frozen (i.e. is 
not relocated). The parameter `freeze_bound` gives the threshold ratio. At each iteration, any vertex that 
moves, unfreezes all its incident vertices. 
</UL> 

Return Values 
-------------- 

The function `lloyd_optimize_mesh_3` returns a value of type `Mesh_optimization_return_code` 
which is: 
<UL> 
<LI>`TIME_LIMIT_REACHED` when the time limit is reached. 
<LI>`MAX_ITERATION_NUMBER_REACHED` when `lloyd_optimize_mesh_3` stops because it has performed `max_iteration_number` iterations. 
<LI>`CONVERGENCE_REACHED` when `lloyd_optimize_mesh_3` stops because the convergence criterion 
is achieved. 
</UL> 

Example 
-------------- 

\code{.cpp} 

// Lloyd-smoothing until convergence reaches 0.01, freezing vertices which 
// move less than 0.001*shortest_incident_edge_length 
lloyd_optimize_mesh_3(c3t3, domain, parameters::convergence=0.01, 
parameters::freeze_bound=0.001); 

\endcode 

\sa `CGAL::Mesh_optimization_return_code` 
\sa `CGAL::make_mesh_3` 
\sa `CGAL::refine_mesh_3` 
\sa `CGAL::exude_mesh_3` 
\sa `CGAL::perturb_mesh_3` 
\sa `CGAL::odt_optimize_mesh_3` 

*/

template<typename C3T3, typename MeshDomain_3>
Mesh_optimization_return_code
lloyd_optimize_mesh_3(C3T3& c3t3, MeshDomain_3 domain, 
double parameters::time_limit=0,
std::size_t parameters::max_iteration_number=0,
double parameters::convergence=0.02,
double parameters::freeze_bound = 0.01);

} /* namespace CGAL */
