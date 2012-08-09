namespace CGAL {

/*!
\ingroup PkgGenerators

creates the first \f$ n\f$ points on the regular \f$ \lceil n^{1/3}\,\rceil\times\lceil n^{1/3}\,\rceil\times\lceil n^{1/3}\,\rceil\f$ 
grid within the cube
\f$ [-a,a]\times[-a,a]\times[-a, a]\f$. Returns the value of \f$ o\f$ after 
inserting the \f$ n\f$ points. 

The function `points_on_cube_grid_3` generates a given number of points on a cubic 
grid whose size is determined by the number of points to be generated. 

Requirements 
-------------- 

<UL> 
<LI>`Creator` must be a function object accepting three 
`double` values \f$ x\f$, \f$ y\f$, and \f$ z\f$ and returning an initialized 
point `(x,y,z)` of type `P`. Predefined implementations for 
these creators like the default can be found in 
Section \ref sectionCreatorFunctionObjects . 
<LI>The `OutputIterator` must accept values of type `P`. If the 
`OutputIterator` has a `value_type` the default 
initializer of the `creator` can be used. `P` is set to 
the `value_type` in this case. 
</UL> 

`CGAL::points_on_square_grid_2` 

`CGAL::random_selection` 

*/
template <class OutputIterator, Creator creator>
OutputIterator
points_on_cube_grid_3( double a, std::size_t n, OutputIterator o,
Creator creator = 
Creator_uniform_3<Kernel_traits<P>::Kernel::RT,P>);

} /* namespace CGAL */

namespace CGAL {

/*!
\ingroup PkgGenerators

The class `Random_points_in_cube_3` is an input iterator creating points uniformly 
distributed in a half-open cube. The default `Creator` is 
`Creator_uniform_3<Kernel_traits<P>::Kernel::RT,P>`. 

\models ::InputIterator 
\models ::PointGenerator 

\sa `CGAL::cpp0x::copy_n` 
\sa `CGAL::Counting_iterator` 
\sa `CGAL::Random_points_in_square_2<Point_2, Creator>` 
\sa `CGAL::Random_points_in_sphere_3<Point_3, Creator>` 
\sa `CGAL::Random_points_on_sphere_3<Point_3, Creator>` 
\sa `std::random_shuffle` 

*/
template< typename Point_3, typename Creator >
class Random_points_in_cube_3 {
public:

/// \name Types 
/// @{

/*! 

*/ 
typedef std::input_iterator_tag iterator_category; 

/*! 

*/ 
typedef Point_3 value_type; 

/*! 

*/ 
typedef std::ptrdiff_t difference_type; 

/*! 

*/ 
typedef const Point_3* pointer; 

/*! 

*/ 
typedef const Point_3& reference; 

/// @} 

/// \name Operations 
/// @{

/*! 
\f$ g\f$ is an input iterator creating points of type `Point_3` uniformly 
distributed in the half-open cube with side length \f$ 2 a\f$, centered 
at the origin, i.e. \f$ \forall p = *g: -a \le p.x(),p.y(),p.z() < a\f$ . 
Three random numbers are needed from `rnd` for each point. 

*/ 
Random_points_in_cube_3( double a, Random& rnd = 
default_random); 

/// @}

}; /* end Random_points_in_cube_3 */
} /* end namespace CGAL */

namespace CGAL {

/*!
\ingroup PkgGenerators

The class `Random_points_in_sphere_3` is an input iterator creating points uniformly 
distributed in an open sphere. The default `Creator` is 
`Creator_uniform_3<Kernel_traits<P>::Kernel::RT,P>`. 

\models ::InputIterator 
\models ::PointGenerator 

\sa `CGAL::cpp0x::copy_n` 
\sa `CGAL::Counting_iterator` 
\sa `CGAL::Random_points_in_disc_2<Point_2, Creator>` 
\sa `CGAL::Random_points_in_cube_3<Point_3, Creator>` 
\sa `CGAL::Random_points_on_sphere_3<Point_3, Creator>` 
\sa `std::random_shuffle` 

*/
template< typename Point_3, typename Creator >
class Random_points_in_sphere_3 {
public:

/// \name Types 
/// @{

/*! 

*/ 
typedef std::input_iterator_tag iterator_category; 

/*! 

*/ 
typedef Point_3 value_type; 

/*! 

*/ 
typedef std::ptrdiff_t difference_type; 

/*! 

*/ 
typedef const Point_3* pointer; 

/*! 

*/ 
typedef const Point_3& reference; 

/// @} 

/// \name Operations 
/// @{

/*! 
\f$ g\f$ is an input iterator creating points of type `Point_3` uniformly 
distributed in the open sphere with radius \f$ r\f$, 
i.e. \f$ |*g| < r\f$ . Three random numbers are needed from 
`rnd` for each point. 

*/ 
Random_points_in_sphere_3( double r, Random& rnd = 
default_random); 

/// @}

}; /* end Random_points_in_sphere_3 */
} /* end namespace CGAL */

namespace CGAL {

/*!
\ingroup PkgGenerators

The class `Random_points_on_sphere_3` is an input iterator creating points uniformly 
distributed on a sphere. The default `Creator` is 
`Creator_uniform_3<Kernel_traits<P>::Kernel::RT,P>`. 
The generated points are computed using floating point arithmetic, 
whatever the Kernel is, thus they are on the circle/sphere only up to 
rounding errors. 

\models ::InputIterator 
\models ::PointGenerator 

\sa `CGAL::cpp0x::copy_n` 
\sa `CGAL::Counting_iterator` 
\sa `CGAL::Random_points_on_circle_2<Point_2, Creator>` 
\sa `CGAL::Random_points_in_cube_3<Point_3, Creator>` 
\sa `CGAL::Random_points_in_sphere_3<Point_3, Creator>` 
\sa `std::random_shuffle` 

*/
template< typename Point_3, typename Creator >
class Random_points_on_sphere_3 {
public:

/// \name Types 
/// @{

/*! 

*/ 
typedef std::input_iterator_tag iterator_category; 

/*! 

*/ 
typedef Point_3 value_type; 

/*! 

*/ 
typedef std::ptrdiff_t difference_type; 

/*! 

*/ 
typedef const Point_3* pointer; 

/*! 

*/ 
typedef const Point_3& reference; 

/// @} 

/// \name Operations 
/// @{

/*! 
\f$ g\f$ is an input iterator creating points of type `Point_3` uniformly 
distributed on the boundary of a sphere with radius \f$ r\f$, 
i.e. \f$ |*g| == r\f$ . Two random numbers are needed from 
`rnd` for each point. 

*/ 
Random_points_on_sphere_3( double r, Random& rnd = 
default_random); 

/// @}

}; /* end Random_points_on_sphere_3 */
} /* end namespace CGAL */
