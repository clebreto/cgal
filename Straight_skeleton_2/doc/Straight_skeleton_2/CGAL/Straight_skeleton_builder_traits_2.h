namespace CGAL {

/*!
\ingroup PkgStraightSkeleton2

The class `Straight_skeleton_builder_traits_2` provides a model for the 
`StraightSkeletonBuilderTraits_2` concept which is the traits class 
required by the `Straight_skeleton_builder_2` algorithm class. The class 
`Straight_skeleton_builder_traits_2` has one template argument: a 2D \cgal Kernel. This parameter must be a model for the `Kernel` concept, such as the `Exact_predicates_inexact_constructions_kernel`, which is the recommended one. 

It is unspecified which subset of the kernel is used into the output sequence and the returned iterator will be equal to `out`. 

For any given input polygon, it in this traits class (and by extension in the builder class). This is to avoid restricting the choices in the implementation. 

\models ::StraightSkeletonBuilderTraits_2 
\models ::DefaultConstructible 
\models ::CopyConstructible 

\sa `CGAL::Straight_skeleton_builder_2<Gt,Ssds>` 

*/
template< typename Kernel >
class Straight_skeleton_builder_traits_2 {
public:

/// @}

}; /* end Straight_skeleton_builder_traits_2 */
} /* end namespace CGAL */
