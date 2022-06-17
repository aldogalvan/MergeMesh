// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2016 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "ismember_rows.h"
#include "sortrows.h"
#include "unique.h"
#include "unique_rows.h"
#include <iostream>

template <
  typename DerivedA,
  typename DerivedB,
  typename DerivedIA,
  typename DerivedLOCB>
IGL_INLINE void igl::ismember_rows(
  const Eigen::MatrixBase<DerivedA> & A,
  const Eigen::MatrixBase<DerivedB> & B,
  Eigen::PlainObjectBase<DerivedIA> & IA,
  Eigen::PlainObjectBase<DerivedLOCB> & LOCB)
{
  using namespace Eigen;
  using namespace std;
  assert(A.cols() == B.cols() && "number of columns must match");
  IA.resize(A.rows(),1);
  IA.setConstant(false);
  LOCB.resize(A.rows(),1);
  LOCB.setConstant(-1);
  // boring base cases
  if(A.size() == 0)
  {
    return;
  }
  if(B.size() == 0)
  {
    return;
  }

  // Get rid of any duplicates
  typedef Eigen::Matrix<typename DerivedA::Scalar,DerivedA::RowsAtCompileTime,DerivedA::RowsAtCompileTime> MatrixA;
  typedef Eigen::Matrix<typename DerivedB::Scalar,DerivedB::RowsAtCompileTime,DerivedB::RowsAtCompileTime> MatrixB;
  MatrixA uA;
  MatrixB uB;
  Eigen::Matrix<typename DerivedA::Index,Dynamic,1> uIA,uIuA,uIB,uIuB;
  unique_rows(A,uA,uIA,uIuA);
  unique_rows(B,uB,uIB,uIuB);
  // Sort both
  MatrixA sA;
  MatrixB sB;
  Eigen::Matrix<typename DerivedA::Index,Dynamic,1> sIA,sIB;
  sortrows(uA,true,sA,sIA);
  sortrows(uB,true,sB,sIB);

  Eigen::Matrix<bool,Eigen::Dynamic,1> uF = 
    Eigen::Matrix<bool,Eigen::Dynamic,1>::Zero(sA.size(),1);
  Eigen::Matrix<typename DerivedLOCB::Scalar, Eigen::Dynamic,1> uLOCB =
    Eigen::Matrix<typename DerivedLOCB::Scalar,Eigen::Dynamic,1>::
    Constant(sA.size(),1,-1);
  const auto & row_greater_than = [&sA,&sB](const int a, const int b)
  {
    for(int c = 0;c<sA.cols();c++)
    {
      if(sA(a,c) > sB(b,c)) return true;
      if(sA(a,c) < sB(b,c)) return false;
    }
    return false;
  };
  {
    int bi = 0;
    // loop over sA
    bool past = false;
    for(int a = 0;a<sA.rows();a++)
    {
      assert(bi < sB.rows());
      while(!past && row_greater_than(a,bi))
      {
        bi++;
        past = bi>=sB.rows();
      }
      if(!past && (sA.row(a).array()==sB.row(bi).array()).all() )
      {
        uF(sIA(a)) = true;
        uLOCB(sIA(a)) = uIB(sIB(bi));
      }
    }
  }

  for(int a = 0;a<A.rows();a++)
  {
    IA(a) = uF(uIuA(a));
    LOCB(a) = uLOCB(uIuA(a));
  }
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
template void igl::ismember_rows<Eigen::Matrix<int, -1, 2, 0, -1, 2>, Eigen::Matrix<int, -1, 2, 0, -1, 2>, Eigen::Array<bool, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> > const&, Eigen::PlainObjectBase<Eigen::Array<bool, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
template void igl::ismember_rows<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<bool, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<bool, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
template void igl::ismember_rows<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<bool, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<bool, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
template void igl::ismember_rows<Eigen::Matrix<int, -1, 4, 0, -1, 4>, Eigen::Matrix<int, 12, 4, 0, 12, 4>, Eigen::Array<bool, -1, 1, 0, -1, 1>, Eigen::Matrix<int, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<int, -1, 4, 0, -1, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<int, 12, 4, 0, 12, 4> > const&, Eigen::PlainObjectBase<Eigen::Array<bool, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> >&);
#endif

