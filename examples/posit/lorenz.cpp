/*
 * lorenz_mp.cpp
 *
 * This example demonstrates how odeint can be used with posit universal numbers.
 *
 * Copyright 2011-2012 Karsten Ahnert
 * Copyright 2013 Mario Mulansky
 * Copyright 2018 Theodore Omtzigt
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */



#include <iostream>

//[ posit_lorenz_defs
#include <universal/posit/posit>
#include <boost/numeric/odeint.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace std;
using namespace boost::numeric::odeint;

static constexpr size_t nbits = 64;
static constexpr size_t es = 3;
using Real = sw::unum::posit<nbits, es>;

using value_type = Real;
using state_type = boost::array< value_type , 3 > ;
//]

//[ mp_lorenz_rhs
struct lorenz
{
    void operator()( const state_type &x , state_type &dxdt , value_type t ) const
    {
        const value_type sigma( 10 );
        const value_type R( 28 );
        const value_type b( value_type( 8 ) / value_type( 3 ) );

        dxdt[0] = sigma * ( x[1] - x[0] );
        dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
        dxdt[2] = -b * x[2] + x[0] * x[1];
    }
};
//]




struct streaming_observer
{
    std::ostream& m_out;

    streaming_observer( std::ostream &out ) : m_out( out ) { }

    template< class State , class Time >
    void operator()( const State &x , Time t ) const
    {
        m_out << t;
        for( size_t i=0 ; i<x.size() ; ++i ) m_out << "\t" << x[i] ;
        m_out << "\n";
    }
};






int main( int argc , char **argv )
{
    //[ mp_lorenz_int
    state_type x = {{ value_type( 10.0 ) , value_type( 1.0 ) , value_type( 1.0 ) }};

    cout.precision( 5 );
    integrate_const( runge_kutta4< state_type , value_type >() ,
            lorenz() , x , value_type( 0.0 ) , value_type( 5.0 ) , value_type( 0.0625f ) ,
            streaming_observer( cout ) );
    //]

    return 0;
}
