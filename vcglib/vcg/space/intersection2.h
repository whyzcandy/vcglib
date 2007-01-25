/****************************************************************************
 * VCGLib                                                            o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                _   O  _   *
 * Copyright(C) 2004                                                \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *   
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 *                                                                           *
 ****************************************************************************/
/****************************************************************************
 History

****************************************************************************/



#ifndef __VCGLIB_INTERSECTION_2
#define __VCGLIB_INTERSECTION_2
#include <vcg/space/line2.h>
#include <vcg/space/ray2.h>
#include <vcg/space/segment2.h>
#include <vcg/space/point2.h>
#include <vcg/space/triangle2.h>
#include <vcg/space/box2.h>




namespace vcg {
/** \addtogroup space */
/*@{*/
/** 
    Function computing the intersection between couple of geometric primitives in
    2 dimension
*/

/// return true if the algle is convex (right rotation)
template<class SCALAR_TYPE>
    inline bool Convex(const Point2<SCALAR_TYPE> & p0,const Point2<SCALAR_TYPE> & p1,const Point2<SCALAR_TYPE> & p2)
{
  const SCALAR_TYPE EPSILON= SCALAR_TYPE(1e-8);
  return (((p0-p1)^(p2-p1))<=EPSILON);
}

///return if exist the intersection point
///between 2 lines in a 2d plane
template<class SCALAR_TYPE>
inline bool LineLineIntersection(const vcg::Line2<SCALAR_TYPE> & l0,
								 const vcg::Line2<SCALAR_TYPE> & l1,
								 Point2<SCALAR_TYPE> &p)
{
	const SCALAR_TYPE EPSILON= SCALAR_TYPE(1e-8);
	///first line
	SCALAR_TYPE x1=l0.Origin().X();
	SCALAR_TYPE y1=l0.Origin().Y(); 
	SCALAR_TYPE x2=x1+l0.Direction().X();
	SCALAR_TYPE y2=y1+l0.Direction().Y(); 
	
	///second line
	SCALAR_TYPE x3=l1.Origin().X();
	SCALAR_TYPE y3=l1.Origin().Y(); 
	SCALAR_TYPE x4=x3+l1.Direction().X();
	SCALAR_TYPE y4=y3+l1.Direction().Y(); 

	///then  find intersection

	///denominator
	SCALAR_TYPE den=((x1-x2)*(y3-y4))-((y1-y2)*(x3-x4));
	if (fabs(den)<EPSILON)
		return false;

	SCALAR_TYPE d0=(x1*y2)-(y1*x2);
	SCALAR_TYPE d1=(x3*y4)-(y3*x4);
	SCALAR_TYPE numx=(d0*(x3-x4))-(d1*(x1-x2));
	SCALAR_TYPE numy=(d0*(y3-y4))-(d1*(y1-y2));

	p.X()=numx/den;
	p.Y()=numy/den;
	return true;
}

///return if exist the intersection point
///between 2 lines in a 2d plane
template<class SCALAR_TYPE>
inline bool RayLineIntersection(const vcg::Line2<SCALAR_TYPE> & l,
								const vcg::Ray2<SCALAR_TYPE> & r,
								 Point2<SCALAR_TYPE> &p)
{
	///construct line from ray
	vcg::Line2<SCALAR_TYPE> l_test;
	l_test.Set(r.Origin(),r.Direction());
	if (!LineLineIntersection(l,l_test,p))
		return false;
	Point2<SCALAR_TYPE> dir=p-r.Origin();
	dir.Normalize();
	return (dir*r.Direction()>0);
}


/// interseciton between point and triangle
template<class SCALAR_TYPE>
inline bool RaySegmentIntersection(const vcg::Ray2<SCALAR_TYPE> & r,
									const vcg::Segment2<SCALAR_TYPE> &seg,
									Point2<SCALAR_TYPE> &p_inters)
{
 ///first compute intersection between lines
 vcg::Line2<SCALAR_TYPE> line2;
 line2.SetOrigin(seg.P0());
 vcg::Point2<SCALAR_TYPE> dir=seg.P1()-seg.P0();
 dir.Normalize();
 line2.SetDirection(dir);
 if(!RayLineIntersection<SCALAR_TYPE>(line2,r,p_inters))
	return false;
 ///then test if intersection point is nearest 
 ///to both extremes then lenght of the segment 
 SCALAR_TYPE d0=(seg.P1()-p_inters).Norm();
 SCALAR_TYPE d1=(seg.P0()-p_inters).Norm();
 SCALAR_TYPE lenght=(seg.P0()-seg.P1()).Norm();
 return ((d0<lenght)&&(d1<lenght));
}

/// interseciton between point and triangle
template<class SCALAR_TYPE>
inline bool LineSegmentIntersection(const vcg::Line2<SCALAR_TYPE> & line,
									const vcg::Segment2<SCALAR_TYPE> &seg,
									Point2<SCALAR_TYPE> &p_inters)
{
 ///first compute intersection between lines
 vcg::Line2<SCALAR_TYPE> line2;
 line2.SetOrigin(seg.P0());
 vcg::Point2<SCALAR_TYPE> dir=seg.P1()-seg.P0();
 dir.Normalize();
 line2.SetDirection(dir);
 if(!LineLineIntersection(line,line2,p_inters))
	return false;
 ///then test if intersection point is nearest 
 ///to both extremes then lenght of the segment 
 SCALAR_TYPE d0=(seg.P1()-p_inters).Norm();
 SCALAR_TYPE d1=(seg.P0()-p_inters).Norm();
 SCALAR_TYPE lenght=(seg.P0()-seg.P1()).Norm();
 return ((d0<lenght)&&(d1<lenght));
}

/// interseciton between point and triangle
template<class SCALAR_TYPE>
inline bool SegmentSegmentIntersection(const vcg::Segment2<SCALAR_TYPE> &seg0,
									   const vcg::Segment2<SCALAR_TYPE> &seg1,
									   Point2<SCALAR_TYPE> &p_inters)
{
  ///test intersection of bbox
  vcg::Box2<SCALAR_TYPE> bb0,bb1;
  bb0.Add(seg0.P0());
  bb0.Add(seg0.P1());
  bb1.Add(seg1.P0());
  bb1.Add(seg1.P1());
  if (!bb0.Collide(bb1))
	  return false;
  else
  {
	///first compute intersection between lines
	vcg::Line2<SCALAR_TYPE> l0,l1;

	l0.SetOrigin(seg0.P0());
	vcg::Point2<SCALAR_TYPE> dir0=seg0.P1()-seg0.P0();
	dir0.Normalize();
	l0.SetDirection(dir0);
	l1.SetOrigin(seg1.P0());
	vcg::Point2<SCALAR_TYPE> dir1=seg1.P1()-seg1.P0();
	dir1.Normalize();
	l1.SetDirection(dir1);
	return ((LineSegmentIntersection<SCALAR_TYPE>(l0,seg1,p_inters))&&
			(LineSegmentIntersection<SCALAR_TYPE>(l1,seg0,p_inters)));
  }
}

/// interseciton between point and triangle
template<class SCALAR_TYPE>
    inline bool IsInsideTrianglePoint( const Triangle2<SCALAR_TYPE> & t,const Point2<SCALAR_TYPE> & p)
{
  Point2<SCALAR_TYPE> p0=t.P0(0);
  Point2<SCALAR_TYPE> p1=t.P0(1);
  Point2<SCALAR_TYPE> p2=t.P0(2);

  ///first test with bounding box
  vcg::Box2<SCALAR_TYPE> b2d;
  b2d.Add(p0);
  b2d.Add(p1);
  b2d.Add(p2);
  if (!b2d.IsIn(p))
	  return false;
  
  ///then text convex
  if (!Convex(p0,p1,p2))
    std::swap<Point2<SCALAR_TYPE> >(p1,p2);
  return((Convex(p,p0,p1))&&(Convex(p,p1,p2))&&(Convex(p,p2,p0)));
  //return((Convex(p,p0,p1))&&(Convex(p,p1,p2))&&(Convex(p,p2,p0)));
}
/*@}*/
} // end namespace
#endif
