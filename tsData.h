/*
  libpuDatatypes - Diverse datatypes: Regions, coordinates and alike
  
  $Id$

  Copyright (C) 2006 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _tsData_h
#define _tsData_h

// template class to hold a timeserie of one parameter.
// JS/PU 9/99 

#include <map>
#include <set>
#include <vector>

#include <puTools/miTime.h>
#include <puTools/miString.h>


using namespace std;

namespace puData {
  
  template<class V> struct progVal {
  
    V value;
    int prog;
    
    progVal():   prog(0) {}
    progVal(V val): value(val), prog(0) {}  
    progVal(V val, int pr):  value(val), prog(pr) {} 
    friend progVal<V> operator+(const progVal<V>& lhs, const progVal<V>& rhs)
    { return progVal((rhs.value+lhs.value), rhs.prog);}
    friend progVal<V> operator-(const progVal<V>& lhs, const progVal<V>& rhs)
    { return progVal((rhs.value-lhs.value), rhs.prog);}
    friend progVal<V> operator*(const progVal<V>& lhs, const progVal<V>& rhs)
    { return progVal((rhs.value*lhs.value), rhs.prog);}
    friend progVal<V> operator/(const progVal<V>& lhs, const progVal<V>& rhs)
    { return progVal((rhs.value/lhs.value), rhs.prog);}
    bool is_older_than( const progVal& rhs ) 
    { return rhs.prog < prog; }
    void set(const V& val, const int& pr)
    {value = val; prog = pr;}
    
  };

 

  struct datasource {
    miString name;
    miString tag;
  };
  
  
  typedef set<miTime> timeline;

  template<class T> class tsData {
  private:
    //  parameter p;
    datasource src;
    map<miTime,progVal<T> > data;
  public: 
    tsData<T>() {}
    tsData<T>(const tsData<T>& tsIn ) 
    { copyMembers(tsIn) ; }
    tsData<T>(const int & p, const int& s) 
      : pindex(p), sindex(s) {}

    // standart stuff. Get the data in and out again...
  
    int pindex,sindex;
    bool empty() {return data.empty(); }
    void  clean(){data.clear();}
    void  push(const miTime t, const T val, const int& pr= 0)
    { data[t] = progVal<T>(val,pr); }
    
    void setTs( map<miTime,progVal<T> > ts )
    { data = ts ;}
    

    T  value(miTime tim) {return data[tim].value;}
    int prog(miTime tim) {return data[tim].prog;} 
    map<miTime,progVal<T> > series() const    { return data;}
    timeline times()    { return times(data);}
    
    tsData<T>& operator=(const tsData<T>& rhs )
    { copyMembers(rhs); return *this; }
    
    progVal<T> operator[]( miTime req ) 
    {
      progVal<T> a;
      return ( ( data.count(req) > 0 ) ? data[req] : a); 
    }
    
    void copyMembers( const tsData<T>& rhs ){
      src  = rhs.src;
      data = rhs.data;
      pindex = rhs.pindex;
      sindex = rhs.sindex;
      
      // p = rhs.p;
    }
    // just for the test fun:
  
    friend ostream& operator<<(ostream& out, const tsData<T>& tsd){
      map<miTime,progVal<T> > ts = tsd.series();
    
      typedef typename   map<miTime,progVal<T> >::iterator osTProgvalItr;

      osTProgvalItr itr = ts.begin();
      cout<<"valid time           prog   value"<<endl;
      cout<<"---------------------------------"<<endl;
      for ( ; itr != ts.end(); itr++)
	out<<itr->first<<"  "
	   <<itr->second.prog
	   <<(itr->second.prog >= 100 ? "    " :"     ")
	   <<itr->second.value
	   <<endl;
      return out;
    }
    
  };


};

#endif



