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

$Log: import_raw.h,v $
****************************************************************************/

#ifndef __VCGLIB_IMPORT_ASC
#define __VCGLIB_IMPORT_ASC

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vcg/complex/trimesh/allocate.h>
#include <vcg/complex/trimesh/create/platonic.h>
#include <vcg/complex/trimesh/clean.h>

namespace vcg {
namespace tri {
namespace io {

/** 
This class encapsulate a filter for importing raw format pointcloud.
there exists many raw formats. each one with a particular sintax even if they only contains 

*/
template <class MESH_TYPE>
class ImporterASC
{
public:

typedef typename MESH_TYPE::VertexPointer VertexPointer;
typedef typename MESH_TYPE::ScalarType ScalarType;
typedef typename MESH_TYPE::VertexType VertexType;
typedef typename MESH_TYPE::FaceType FaceType;
typedef typename MESH_TYPE::VertexIterator VertexIterator;
typedef typename MESH_TYPE::FaceIterator FaceIterator;

enum RAWError {
	E_NOERROR,				// 0
		// Error open
	E_CANTOPEN,				// 1
	E_UNESPECTEDEOF,        // 2
};

static const char *ErrorMsg(int error)
{
  static const char * raw_error_msg[] =
  {
	"No errors",
	"Can't open file",
	"Premature End of file",
	};

  if(error>2 || error<0) return "Unknown error";
  else return raw_error_msg[error];
};

/*!
*	Standard call for reading a mesh
*	\param m			the destination mesh
*	\param filename		the name of the file to read from
*	\param triangulate	if true, the mesh will be triangulated, otherwise only points will be stored
*	\param lineskip  	number of lines to be skipped at the begin of the file
*	\return				the operation result
*/
static int Open( MESH_TYPE &m, const char * filename, CallBackPos *cb=0,bool triangulate=false)
{
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
      return E_CANTOPEN;
    }
		long currentPos = ftell(fp);
		fseek(fp,0L,SEEK_END);
		long fileLen = ftell(fp);
		fseek(fp,currentPos,SEEK_SET);

    m.Clear();
  
    Point3f pp;
    int cnt=0;
		int ret;
    /* Read a single facet from an ASCII .STL file */
    while(!feof(fp))
    {
      if(cb && (++cnt)%1000) cb( (ftell(fp)*100)/fileLen, "ASC Mesh Loading");	
			if(feof(fp)) break;
      ret=fscanf(fp, "%f, %f, %f\n", &pp.X(), &pp.Y(), &pp.Z());
			if(ret!=3) 		return E_UNESPECTEDEOF;
      VertexIterator vi=Allocator<MESH_TYPE>::AddVertices(m,1);
			(*vi).P().Import(pp); 
    }
		
    fclose(fp);
		// now try to triangulate.
		// search for the first jump
		float baseY = m.vert[0].P().Y();
		int i;
		for(i=1;i<m.vert.size();++i)
				{
				 if(m.vert[i].P().Y()!= baseY)  break;
				}
		cnt=m.vert.size();
		qDebug("Grid is %i x %i = %i (%i) ",i,cnt/i,i* (cnt/i),cnt);
		tri::FaceGrid(m,i,cnt/i);
		tri::Clean<CMeshO>::FlipMesh(m);
    return E_NOERROR;
  }

}; // end class
} // end Namespace tri
} // end Namespace io
} // end Namespace vcg

#endif
