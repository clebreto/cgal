// Copyright (c) 2009-2010 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Come Le Breton, Laurent Rineau
//
//******************************************************************************
// File Description :
//
//******************************************************************************

#pragma once

#include <CGAL/license/Mesh_3.h>


#include <CGAL/iterator.h>
#include <CGAL/enum.h>
#include <CGAL/number_utils.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/is_streamable.h>
#include <CGAL/Real_timer.h>

#include <vector>
#include <set>
#include <map>
#include <boost/next_prior.hpp> // for boost::prior and boost::next
#include <boost/variant.hpp>
#include <boost/foreach.hpp>

#include <dtkClippedNurbsCurve>
#include <dtkContinuousGeometryUtils>

namespace CGAL {


namespace internal {
namespace Mesh_3 {

template <typename Gt, typename MapIterator>
struct Mesh_domain_segment_of_curve_primitive{
  typedef typename std::iterator_traits<MapIterator>::value_type Map_value_type;
  typedef typename Map_value_type::first_type Curve_id;
  typedef typename Map_value_type::second_type Polyline;

  typedef std::pair<MapIterator,
                    typename Polyline::const_iterator> Id;

  typedef typename std::iterator_traits<
    typename Polyline::const_iterator>::value_type Point;

  typedef typename Gt::Segment_3 Datum;

  Id id_;

  Mesh_domain_segment_of_curve_primitive(Id id) : id_(id) {}

  const Id& id() const { return id_; }

  const Point& reference_point() const {
    return *(id_.second);
  }

  Datum datum() const {
    return Datum(*id_.second, *(id_.second+1));
  }
}; // end Mesh_domain_segment_of_curve_primitive

} // end of namespace CGAL::internal::Mesh_3
} // end of namespace CGAL::internal





/**
 * @class cgalMeshDomainWithNurbsFeatures
 *
 *
 */
template < typename MeshDomain >
class cgalMeshDomainWithNurbsFeatures : public MeshDomain
{
  typedef MeshDomain Base;

public:
  // Index types
  typedef typename Base::Index    Index;

  typedef typename Base::Surface_patch_index Surface_patch_index;

  typedef int                     Curve_segment_index;
  typedef int                     Corner_index;

  typedef typename Base::R         Gt;
  typedef Gt                       R;
  typedef typename Base::Point_3   Point_3;
  typedef typename Gt::FT          FT;

  typedef CGAL::Tag_true           Has_features;


#ifndef CGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES
  template <typename ... T>
  cgalMeshDomainWithNurbsFeatures(const T& ...t)
    : Base(t...)
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}
#else
  /// Constructors
  /// Call the base class constructor
  cgalMeshDomainWithNurbsFeatures()
    : Base()
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}

  template <typename T1>
  cgalMeshDomainWithNurbsFeatures(const T1& o1)
    : Base(o1)
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}

  template <typename T1, typename T2>
  cgalMeshDomainWithNurbsFeatures(const T1& o1, const T2& o2)
    : Base(o1, o2)
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}

  template <typename T1, typename T2, typename T3>
  cgalMeshDomainWithNurbsFeatures(const T1& o1, const T2& o2,
                                       const T3& o3)
    : Base(o1, o2, o3)
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}

  template <typename T1, typename T2, typename T3, typename T4>
  cgalMeshDomainWithNurbsFeatures(const T1& o1, const T2& o2,
                                       const T3& o3, const T4& o4)
    : Base(o1, o2, o3, o4)
    , current_corner_index_(1)
    , current_curve_index_(1)
    , curves_aabb_tree_is_built(false) {}
#endif

  /// Destructor
  ~cgalMeshDomainWithNurbsFeatures() {}

  /// OutputIterator value type is std::pair<Corner_index, Point_3>
  template <typename OutputIterator>
  OutputIterator get_corners(OutputIterator out) const;

  /// OutputIterator value type is CGAL::cpp11::tuple<Curve_segment_index,
  /// std::pair<Point_3,Index>, std::pair<Point_3,Index> >
  template <typename OutputIterator>
  OutputIterator get_curve_segments(OutputIterator out) const;

  /// Returns a maximal error bound on the distance bewteen the cord linking C(p) to C(q), to the curve from C(p) to C(q)
  /// \c p : parameter of C(p) in C parameter space
  /// \c q : parameter of C(q) in C parameter space
  /// \c curve_index
  FT error_bound_cord_to_curve(double p, double q, const Curve_segment_index& curve_index) const;

  /// Construct a point on curve \c curve_index at parameter p
  /// of \c starting_point
  Point_3
  construct_point_on_curve_segment(double p,
                                   const Curve_segment_index& curve_index) const;

  /// Returns the sign of the orientation of p,q,r along curve segment
  /// of index \c index
  /// \c p : parameter of C(p) in C parameter space
  /// \c q : parameter of C(q) in C parameter space
  /// \c r : parameter of C(r) in C parameter space
  CGAL::Sign distance_sign_along_cycle(double p,
                                       double q,
                                       double r,
                                       const Curve_segment_index& index) const;

  /// Returns true if curve \c curve_index is a cycle
  bool is_cycle(const Curve_segment_index& index) const;

  /// Returns an Index from a Curve_segment_index
  Index index_from_curve_segment_index(const Curve_segment_index& index) const
  { return Index(index); }

  /// Returns an Curve_segment_index from an Index
  Curve_segment_index curve_segment_index(const Index& index) const
  { return boost::get<Curve_segment_index>(index); }

  /// Returns an Index from a Corner_index
  Index index_from_corner_index(const Corner_index& index) const
  { return Index(index); }

  /// Returns an Corner_index from an Index
  Corner_index corner_index(const Index& index) const
  { return boost::get<Corner_index>(index); }

  /// Insert a bunch of edges into domain
  ///   + InputIterator type should have begin() and end() function
  ///   + InputIterator::iterator value type must be Point_3
  //    + IndicesOutputIterator is an output iterator of value_type equal
  ///   to Curve_segment_index
  template <typename InputIterator, typename IndicesOutputIterator>
  IndicesOutputIterator
  add_features(InputIterator first, InputIterator last,
               IndicesOutputIterator out /*= CGAL::Emptyset_iterator()*/);

  template <typename InputIterator, typename IndicesOutputIterator>
  IndicesOutputIterator
  add_features_with_context(InputIterator first, InputIterator last,
                            IndicesOutputIterator out /*=
                                                        CGAL::Emptyset_iterator()*/);

  template <typename InputIterator>
  void
  add_features(InputIterator first, InputIterator last)
  { add_features(first, last, CGAL::Emptyset_iterator()); }

  template <typename InputIterator>
  void
  add_features_with_context(InputIterator first, InputIterator last)
  { add_features_with_context(first, last, CGAL::Emptyset_iterator()); }

  template <typename IndicesOutputIterator>
  IndicesOutputIterator
  get_incidences(Curve_segment_index id, IndicesOutputIterator out) const;

  template <typename IndicesOutputIterator>
  IndicesOutputIterator
  get_corner_incidences(Corner_index id, IndicesOutputIterator out) const;

  template <typename IndicesOutputIterator>
  IndicesOutputIterator
  get_corner_incident_curves(Corner_index id, IndicesOutputIterator out) const;

  typedef std::set<Surface_patch_index> Surface_patch_index_set;

  const Surface_patch_index_set&
  get_incidences(Curve_segment_index id) const;

  /// Insert one edge into domain
  /// InputIterator value type is Point_3
  template <typename InputIterator>
  Curve_segment_index insert_edge(InputIterator first, InputIterator last);

private:
  void register_corner(const Point_3& p, const Curve_segment_index& index);
  void compute_corners_incidences();

  /// Returns the sign of the geodesic distance between \c p and \c q
  /// Precondition: index is not a cycle
  CGAL::Sign distance_sign(double p, double q,
                           const Curve_segment_index& index) const;

  /// Returns Index associated to p (p must be the coordinates of a corner
  /// point)
  Index point_corner_index(const Point_3& p) const;

private:
  typedef std::map<Point_3,Corner_index> Corners;

  typedef std::map<Curve_segment_index, dtkClippedNurbsCurve> Edges;
  typedef std::map<Curve_segment_index, Surface_patch_index_set > Edges_incidences;
  typedef std::map<Corner_index, std::set<Curve_segment_index> > Corners_tmp_incidences;
  typedef std::map<Corner_index, Surface_patch_index_set > Corners_incidences;

  typedef internal::Mesh_3::Mesh_domain_segment_of_curve_primitive<
    Gt,
    typename Edges::const_iterator> Curves_primitives;

  typedef CGAL::AABB_traits<Gt,
                            Curves_primitives> AABB_curves_traits;

  Corners corners_;
  Corners_tmp_incidences corners_tmp_incidences_;
  Corner_index current_corner_index_;
  Corners_incidences corners_incidences_;

  Edges edges_;
  Curve_segment_index current_curve_index_;
  Edges_incidences edges_incidences_;

public:
  typedef CGAL::AABB_tree<AABB_curves_traits> Curves_AABB_tree;
  typedef std::set<Surface_patch_index> Set_of_patch_ids;
  typedef std::map<Point_3, Set_of_patch_ids> Corners_incidence_map;

private:
  Corners_incidence_map corners_incidence_map_;
  mutable Curves_AABB_tree curves_aabb_tree_;
  mutable bool curves_aabb_tree_is_built;

public:
  const Corners_incidence_map& corners_incidences_map() const
  { return corners_incidence_map_; }

  const Curves_AABB_tree& curves_aabb_tree() const {
    if(!curves_aabb_tree_is_built) build_curves_aabb_tree();
    return curves_aabb_tree_;
  }
  Curve_segment_index maximal_curve_segment_index() const {
    if(edges_incidences_.empty()) return Curve_segment_index();
    return boost::prior(edges_incidences_.end())->first;
  }

//   void build_curves_aabb_tree() const {
// #if CGAL_MESH_3_VERBOSE
//     std::cerr << "Building curves AABB tree...";
//     CGAL::Real_timer timer;
//     timer.start();
// #endif
//     curves_aabb_tree_.clear();
//     for(typename Edges::const_iterator
//           edges_it = edges_.begin(),
//           edges_end = edges_.end();
//         edges_it != edges_end; ++edges_it)
//     {
//       const dtkClippedNurbsCurve& clipped_trim = edges_it->second;
//       for(typename dtkClippedNurbsCurve::const_iterator
//             ct_it = polyline.points_.begin(),
//             end = polyline.points_.end() - 1;
//           ct_it != end; ++ct_it)
//       {
//           curves_aabb_tree_.insert(std::make_pair(edges_it, ct_it));
//       }
//     }
//     curves_aabb_tree_.build();
//     curves_aabb_tree_is_built = true;
// #if CGAL_MESH_3_VERBOSE
//     timer.stop();
//     std::cerr << " done (" << timer.time() * 1000 << " ms)" << std::endl;
// #endif
//   } // end build_curves_aabb_tree()

private:
  // Disabled copy constructor & assignment operator
  typedef cgalMeshDomainWithNurbsFeatures Self;
  cgalMeshDomainWithNurbsFeatures(const Self& src);
  Self& operator=(const Self& src);

};  // end class cgalMeshDomainWithNurbsFeatures



template <class MD_>
template <typename OutputIterator>
OutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
get_corners(OutputIterator out) const
{
  for ( typename Corners::const_iterator
       cit = corners_.begin(), end = corners_.end() ; cit != end ; ++cit )
  {
    *out++ = std::make_pair(cit->second,cit->first);
  }

  return out;
}

template <class MD_>
template <typename OutputIterator>
OutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
get_curve_segments(OutputIterator out) const
{
    dtkContinuousGeometryPrimitives::Point_3 eval_point(0., 0., 0.);
    dtkContinuousGeometryPrimitives::Point_3 first_point(0., 0., 0.);
    dtkContinuousGeometryPrimitives::Point_3 last_point(0., 0., 0.);
  for ( typename Edges::const_iterator
       eit = edges_.begin(), end = edges_.end() ; eit != end ; ++eit )
  {
    CGAL_assertion( eit->second.is_valid() );

    // ///////////////////////////////////////////////////////////////////
    // Recovers the knots of the undelying nurbs curve
    // ///////////////////////////////////////////////////////////////////
    double p = eit->second.first_knot();
    double q = eit->second.last_knot();

    // ///////////////////////////////////////////////////////////////////
    // Checks that the last control points is not equal to the first one
    // ///////////////////////////////////////////////////////////////////
    eit->second.controlPoint(0, first_point.data());
    eit->second.controlPoint(eit->second.nbCps() - 1, last_point.data());

    Index p_index, q_index;
    //TODO Check distance instead...
    if ( ! first_point == last_point )
    {
      //Evaluate and compare to stored corners
      eit->second.evaluatePoint(p, eval_point.data());
      p_index = point_corner_index(Point_3(eval_point[0], eval_point[1], eval_point[2]));
      eit->second.evaluatePoint(q, eval_point.data());
      q_index = point_corner_index(Point_3(eval_point[0], eval_point[1], eval_point[2]));
    }
    else
    {
      p_index = index_from_curve_segment_index(eit->first);
      q_index = p_index;
    }

    *out++ = CGAL::cpp11::make_tuple(eit->first,
                                     std::make_pair(p, p_index),
                                     std::make_pair(q, q_index));
  }

  return out;
}


template <class MD_>
typename cgalMeshDomainWithNurbsFeatures<MD_>::Index
cgalMeshDomainWithNurbsFeatures<MD_>::
point_corner_index(const Point_3& p) const
{
  typename Corners::const_iterator p_index_it = corners_.find(p);
  if ( p_index_it == corners_.end() )
  {
    CGAL_assertion(false);
    return Index();
  }

  return p_index_it->second;
}

template <class MD_>
typename cgalMeshDomainWithNurbsFeatures<MD_>::Point_3
cgalMeshDomainWithNurbsFeatures<MD_>::
construct_point_on_curve_segment(double p, const Curve_segment_index& curve_index) const
{
  // Get corresponding edge
  typename Edges::const_iterator eit = edges_.find(curve_index);
  CGAL_assertion(eit != edges_.end());
  dtkContinuousGeometryPrimitives::Point_3 eval_point(0., 0., 0.);
  eit->second.evaluatePoint(p, eval_point.data());
  return Point_3(eval_point[0], eval_point[1], eval_point[2]);
}

cgalMeshDomainWithNurbsFeatures<MD_>::FT cgalMeshDomainWithNurbsFeatures<MD_>::error_bound_cord_to_curve(double p, double q, const Curve_segment_index& curve_index) const
{
    // ///////////////////////////////////////////////////////////////////
    // Recovers the edge
    // ///////////////////////////////////////////////////////////////////
    // Get corresponding polyline
    typename Edges::const_iterator eit = edges_.find(curve_index);
    CGAL_assertion(eit != edges_.end());

    // ///////////////////////////////////////////////////////////////////
    // Recovers the bezier curve on which p lies
    // ///////////////////////////////////////////////////////////////////

    // ///////////////////////////////////////////////////////////////////
    // Recovers the bezier curve on which p lies
    // ///////////////////////////////////////////////////////////////////


    // ///////////////////////////////////////////////////////////////////
    // If both belong the same bezier curve
    // Clip the curve at p and q, recovers the middle bezier curve and compute
    // ///////////////////////////////////////////////////////////////////


    // ///////////////////////////////////////////////////////////////////
    // If they do not belong to the same bezier curve
    // C
    // ///////////////////////////////////////////////////////////////////

}

template <class MD_>
template <typename InputIterator, typename IndicesOutputIterator>
IndicesOutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
add_features(InputIterator first, InputIterator last,
             IndicesOutputIterator indices_out)
{
  // Insert one edge for each element
  while ( first != last )
  {
    *indices_out++ = insert_edge(first->begin(), first->end());
    ++first;
  }
  compute_corners_incidences();
  return indices_out;
}

template <class MD_>
template <typename InputIterator, typename IndicesOutputIterator>
IndicesOutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
add_features_with_context(InputIterator first, InputIterator last,
                          IndicesOutputIterator indices_out)
{
  // Insert one edge for each element
  for( ; first != last ; ++first )
  {
    const typename Gt::Point_3& p1 = first->polyline_content.front();
    const typename Gt::Point_3& p2 = first->polyline_content.back();
    Set_of_patch_ids& ids_p1 = corners_incidence_map_[p1];
    std::copy(first->context.adjacent_patches_ids.begin(),
              first->context.adjacent_patches_ids.end(),
              std::inserter(ids_p1, ids_p1.begin()));
    Set_of_patch_ids& ids_p2 = corners_incidence_map_[p2];
    std::copy(first->context.adjacent_patches_ids.begin(),
              first->context.adjacent_patches_ids.end(),
              std::inserter(ids_p2, ids_p2.begin()));
    Curve_segment_index curve_id =
      insert_edge(first->polyline_content.begin(), first->polyline_content.end());
    edges_incidences_[curve_id] = first->context.adjacent_patches_ids;
    *indices_out++ = curve_id;
  }
  compute_corners_incidences();
  return indices_out;
}

template <class MD_>
template <typename IndicesOutputIterator>
IndicesOutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
get_incidences(Curve_segment_index id,
               IndicesOutputIterator indices_out) const
{
  typename Edges_incidences::const_iterator it = edges_incidences_.find(id);

  if(it == edges_incidences_.end()) return indices_out;

  const Surface_patch_index_set& incidences = it->second;

  return std::copy(incidences.begin(), incidences.end(), indices_out);
}

template <class MD_>
template <typename IndicesOutputIterator>
IndicesOutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
get_corner_incidences(Corner_index id,
                      IndicesOutputIterator indices_out) const
{
  typename Corners_incidences::const_iterator it = corners_incidences_.find(id);
  const Surface_patch_index_set& incidences = it->second;
  return std::copy(incidences.begin(), incidences.end(), indices_out);
}

template <class MD_>
template <typename IndicesOutputIterator>
IndicesOutputIterator
cgalMeshDomainWithNurbsFeatures<MD_>::
get_corner_incident_curves(Corner_index id,
                           IndicesOutputIterator indices_out) const
{
  typename Corners_tmp_incidences::const_iterator it =
    corners_tmp_incidences_.find(id);
  const std::set<Curve_segment_index>& incidences = it->second;
  return std::copy(incidences.begin(), incidences.end(), indices_out);
}

namespace internal { namespace Mesh_3 {

template <class MD_>
const typename cgalMeshDomainWithNurbsFeatures<MD_>::Surface_patch_index_set&
cgalMeshDomainWithNurbsFeatures<MD_>::
get_incidences(Curve_segment_index id) const
{
  typename Edges_incidences::const_iterator it = edges_incidences_.find(id);
  return it->second;
}

template <class MD_>
void
cgalMeshDomainWithNurbsFeatures<MD_>::
register_corner(const Point_3& p, const Curve_segment_index& curve_index)
{

  typename Corners::iterator cit = corners_.lower_bound(p);

  // If the corner already exists, returns...
  if(cit != corners_.end() && !(corners_.key_comp()(p, cit->first))) {
    corners_tmp_incidences_[cit->second].insert(curve_index);
    return;
  }

  // ...else insert it!

  const Corner_index index = current_corner_index_;
  ++current_corner_index_;

  corners_.insert(cit, std::make_pair(p, index));
  corners_tmp_incidences_[index].insert(curve_index);
}

template <class MD_>
CGAL::Sign
cgalMeshDomainWithNurbsFeatures<MD_>::
distance_sign(double p, double q,
              const Curve_segment_index& index) const
{
  typename Edges::const_iterator eit = edges_.find(index);
  CGAL_assertion(eit != edges_.end());

  CGAL_precondition( ! this->is_cycle(index));

  if ( p == q )
    return CGAL::ZERO;
  else if ( p < q )
    return CGAL::POSITIVE;
  else
    return CGAL::NEGATIVE;
}

template <class MD_>
CGAL::Sign
cgalMeshDomainWithNurbsFeatures<MD_>::
distance_sign_along_cycle(double p,
                          double q,
                          double r,
                          const Curve_segment_index& index) const
{
  // Find edge
  typename Edges::const_iterator eit = edges_.find(index);
  CGAL_assertion(eit != edges_.end());

  // If eit is not a cycle, then the orientation corresponds to the sign
  // of the distance
  if ( !this->is_cycle(index) )
  {
    return distance_sign(p, r, index);
  }

  // If p and r are the same point, it correspond to a complete loop on a cycle
  if ( p == r ) { return CGAL::POSITIVE; }

  // We are on a cycle without any clue (p==q). Return the shortest path as
  // orientation.
  //HOW TO DO THAT ? ...
  if ( p == q )
  {
    // FT pr = eit->second.geodesic_distance(p,r);
    // FT rp = eit->second.geodesic_distance(r,p);
    // if ( pr < rp ) { return CGAL::POSITIVE; }
    // else {
        return CGAL::NEGATIVE; // }
  }

  // If pq or pr is negative, edge is not a cycle, thus geodesic_distance
  // gives the answer.
  FT pq = eit->second.geodesic_distance(p,q);
  FT pr = eit->second.geodesic_distance(p,r);
  CGAL_assertion(pq > FT(0));
  CGAL_assertion(pr > FT(0));

  // Compare pq and pr
  if ( pq <= pr ) { return CGAL::POSITIVE; }
  else { return CGAL::NEGATIVE; }
}

template <class MD_>
bool
cgalMeshDomainWithNurbsFeatures<MD_>::
is_cycle(const Curve_segment_index& index) const
{
  // Find edge
  typename Edges::const_iterator eit = edges_.find(index);
  CGAL_assertion(eit != edges_.end());
  dtkContinuousGeometryPrimitives::Point_3 first_point(0., 0., 0.);
  dtkContinuousGeometryPrimitives::Point_3 last_point(0., 0., 0.);

  // ///////////////////////////////////////////////////////////////////
  // Checks that the last control point is not equal to the first one
  // ///////////////////////////////////////////////////////////////////
  eit->second.controlPoint(0, first_point);
  eit->second.controlPoint(eit->second.nbCps() - 1, last_point);
  return (first_point == last_point);
}


} //namespace CGAL