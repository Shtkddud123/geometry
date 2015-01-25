// Boost.Geometry (aka GGL, Generic Geometry Library)
// Unit Test

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2012 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2012 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2015.
// Modifications copyright (c) 2015 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>

#include <boost/concept_check.hpp>

#include <boost/geometry/strategies/geographic/distance_thomas.hpp>

#include <boost/geometry/core/srs.hpp>
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <test_common/test_point.hpp>

#ifdef HAVE_TTMATH
#  include <boost/geometry/extensions/contrib/ttmath_stub.hpp>
#endif



template <typename P1, typename P2>
void test_thomas(double lon1, double lat1, double lon2, double lat2, double expected_km)
{
    // Set radius type, but for integer coordinates we want to have floating point radius type
    typedef typename bg::promote_floating_point
        <
            typename bg::coordinate_type<P1>::type
        >::type rtype;

    typedef bg::srs::spheroid<rtype> stype;

    typedef bg::strategy::distance::thomas<stype> thomas_type;

    BOOST_CONCEPT_ASSERT
        ( 
            (bg::concept::PointDistanceStrategy<thomas_type, P1, P2>)
        );

    thomas_type thomas;
    typedef typename bg::strategy::distance
        ::services::return_type<thomas_type, P1, P2>::type return_type;


    P1 p1, p2;

    bg::assign_values(p1, lon1, lat1);
    bg::assign_values(p2, lon2, lat2);

    BOOST_CHECK_CLOSE(thomas.apply(p1, p2), return_type(1000.0 * expected_km), 0.001);
    BOOST_CHECK_CLOSE(bg::distance(p1, p2, thomas), return_type(1000.0 * expected_km), 0.001);
}

template <typename P1, typename P2>
void test_all()
{
    test_thomas<P1, P2>(0, 90, 1, 80, 1116.825795); // polar
    test_thomas<P1, P2>(0, -90, 1, -80, 1116.825795); // polar
    test_thomas<P1, P2>(4, 52, 4, 52, 0.0); // no point difference
    test_thomas<P1, P2>(4, 52, 3, 40, 1336.025365); // normal case
}

template <typename P>
void test_all()
{
    test_all<P, P>();
}

int test_main(int, char* [])
{
    //test_all<float[2]>();
    //test_all<double[2]>();
    test_all<bg::model::point<int, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<float, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<double, 2, bg::cs::geographic<bg::degree> > >();

#if defined(HAVE_TTMATH)
    test_all<bg::model::point<ttmath::Big<1,4>, 2, bg::cs::geographic<bg::degree> > >();
    test_all<bg::model::point<ttmath_big, 2, bg::cs::geographic<bg::degree> > >();
#endif

    return 0;
}
