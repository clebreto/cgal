namespace CGAL {

/*!
\ingroup PkgPointSet2

computes a handle to a vertex \f$ w\f$ of \f$ delau\f$ that is closest to \f$ v\f$.
If \f$ v\f$ is the only vertex in \f$ delau\f$ , \f$ NULL\f$ is returned. 

Requirements 
-------------- 

`Dt` is a \cgal Delaunay triangulation and contains the following subset of types from the concept `PointSetTraits` and from 
the Delaunay triangulation data type: 
<UL> 
<LI>`Dt::Geom_traits` 
<LI>`Dt::Point` 
<LI>`Dt::Vertex_circulator` 
<LI>`Dt::Vertex_handle` 
<LI>`Dt::Geom_traits::Compare_distance_2` 
</UL> 

*/
template<class Dt>
Dt::Vertex_handle nearest_neighbor(const Dt& delau, Dt::Vertex_handle v);

} /* namespace CGAL */

namespace CGAL {

/*!
\ingroup PkgPointSet2

computes the \f$ k\f$ nearest neighbors of \f$ p\f$ in \f$ delau\f$, and places the
handles to the corresponding vertices as a sequence of objects of type
Vertex_handle in a container of value type of \f$ res\f$
which points to the first object in the sequence. The function
returns an output iterator pointing to the position beyond the end
of the sequence. 

The function `nearest_neighbors` is the function template version of the k nearest 
neighbors search on Delaunay triangulations. There are two versions of 
this function, one taking a point of the Delaunay triangulation and the 
other taking a vertex handle. 

Requirements 
-------------- 

`Dt` is a \cgal Delaunay triangulation and contains the following subset of types from the concept `PointSetTraits` and from 
the Delaunay triangulation data type: 
<UL> 
<LI>`Dt::Geom_traits` 
<LI>`Dt::Vertex_handle` 
<LI>`Dt::Vertex_iterator` 
<LI>`Dt::Vertex_circulator` 
<LI>`Dt::Vertex` 
<LI>`Dt::Face` 
<LI>`Dt::Face_handle` 
<LI>`Dt::Locate_type` 
<LI>`Dt::Point` 
<LI>`Dt::Geom_traits::FT` 
<LI>`Dt::Geom_traits::Compute_squared_distance_2` 
</UL> 

*/
template<class Dt, class OutputIterator>
OutputIterator nearest_neighbors(Dt& delau, const Dt::Point& p, Dt::size_type k, OutputIterator res);

/*!
\ingroup PkgPointSet2

computes the \f$ k\f$ nearest neighbors of \f$ v\f$ (including \f$ v\f$) in \f$ delau\f$, and places them as a sequence of objects of type
Vertex_handle in a container of value type of \f$ res\f$
which points to the first object in the sequence. The function
returns an output iterator pointing to the position beyond the end
of the sequence. 

The function `nearest_neighbors` is the function template version of the k nearest 
neighbors search on Delaunay triangulations. There are two versions of 
this function, one taking a point of the Delaunay triangulation and the 
other taking a vertex handle. 

Requirements 
-------------- 

`Dt` is a \cgal Delaunay triangulation and contains the following subset of types from the concept `PointSetTraits` and from 
the Delaunay triangulation data type: 
<UL> 
<LI>`Dt::Geom_traits` 
<LI>`Dt::Vertex_handle` 
<LI>`Dt::Vertex_iterator` 
<LI>`Dt::Vertex_circulator` 
<LI>`Dt::Vertex` 
<LI>`Dt::Point` 
<LI>`Dt::Geom_traits::FT` 
<LI>`Dt::Geom_traits::Compute_squared_distance_2` 
</UL> 

*/
template<class Dt, class OutputIterator>
OutputIterator nearest_neighbors(Dt& delau, Dt::Vertex_handle v, Dt::size_type k, OutputIterator res);

} /* namespace CGAL */
