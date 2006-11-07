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
$Log: not supported by cvs2svn $
Revision 1.11  2006/10/19 07:33:03  cignoni
Corrected Laplacian, Added selection to HCSmooth

Revision 1.10  2006/09/25 09:41:41  cignoni
Added new version of pasodoble smoothing

Revision 1.9  2006/02/06 10:45:47  cignoni
Added missing typenames

Revision 1.7  2006/01/24 13:23:22  pietroni
used template types instead of point3f and float inside function calls

Revision 1.6  2005/12/06 17:55:16  pietroni
1 bug corrected

Revision 1.5  2005/12/02 16:24:56  pietroni
corrected 1 bug in Cross Prod Gradient

Revision 1.4  2005/11/23 16:24:44  pietroni
corrected CrossProdGradient( )

Revision 1.3  2005/07/11 13:12:05  cignoni
small gcc-related compiling issues (typenames,ending cr, initialization order)

Revision 1.2  2005/03/16 16:14:12  spinelli
aggiunta funzione PasoDobleSmooth e relative:

- FitMesh
- FaceErrorGrad
- CrossProdGradient
- TriAreaGradient
- NormalSmooth

e le classi:

- PDVertInfo
- PDFaceInfo

necessarie per utilizzare SimpleTempData

Revision 1.1  2004/12/11 14:53:19  ganovelli
first partial porting: compiled gcc,intel and msvc


****************************************************************************/


#ifndef __VCGLIB__SMOOTH
#define __VCGLIB__SMOOTH

#include <vcg/space/point3.h>
#include <vcg/space/line3.h>
#include <vcg/container/simple_temporary_data.h>
#include <vcg/complex/trimesh/update/normal.h>

namespace vcg
{

template<class FLT> 
class ScaleLaplacianInfo 
{
public:
	Point3<FLT> PntSum;
	FLT LenSum;
};

// Scale dependent laplacian smoothing [fujimori 95]
// Nuova versione, l'idea e'quella di usare anche gli angoli delle facce per pesare lo spostamento.
// 
// in pratica si sposta solo lungo la componente che e' parallela alla normale al vertice 
// (che si suppone esserci!!)
 

// Non ha bisogno della topologia
// Non fa assunzioni sull'ordinamento delle facce, ma vuole che i border flag ci siano!
//
// 

template<class MESH_TYPE>
void ScaleLaplacianSmooth(MESH_TYPE &m, int step, typename  MESH_TYPE::ScalarType delta)
{
	SimpleTempData<typename MESH_TYPE::VertContainer, ScaleLaplacianInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
	ScaleLaplacianInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.PntSum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.LenSum=0;
	TD.Start(lpz);
	typename  MESH_TYPE::FaceIterator fi;
	for(int i=0;i<step;++i)
	{
		typename  MESH_TYPE::VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;
		typename MESH_TYPE::ScalarType a[3];
		for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
		{
			typename  MESH_TYPE::CoordType  mp=((*fi).V(0)->P() + (*fi).V(1)->P() + (*fi).V(2)->P())/3.0;
			typename  MESH_TYPE::CoordType  e0=((*fi).V(0)->P() - (*fi).V(1)->P()).Normalize(); 
			typename  MESH_TYPE::CoordType  e1=((*fi).V(1)->P() - (*fi).V(2)->P()).Normalize();
			typename  MESH_TYPE::CoordType  e2=((*fi).V(2)->P() - (*fi).V(0)->P()).Normalize();

			a[0]=AngleN(-e0,e2);
			a[1]=AngleN(-e1,e0);
			a[2]=AngleN(-e2,e1);
			//assert(fabs(M_PI -a[0] -a[1] -a[2])<0.0000001);

			for(int j=0;j<3;++j){
						typename  MESH_TYPE::CoordType dir= (mp-(*fi).V(j)->P()).Normalize();
						TD[(*fi).V(j)].PntSum+=dir*a[j];
						TD[(*fi).V(j)].LenSum+=a[j];
			}
		}		
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].LenSum>0 )
				 (*vi).P() = (*vi).P() +  (TD[*vi].PntSum/TD[*vi].LenSum ) * delta;
				
	}		 			
	TD.Stop();
};

// Scale dependent laplacian smoothing [fujimori 95]
// Non ha bisogno della topologia
// Non fa assunzioni sull'ordinamento delle facce, ma vuole che i border flag ci siano!
//
// 
template<class MESH_TYPE>
void ScaleLaplacianSmoothOld(MESH_TYPE &m, int step, typename  MESH_TYPE::ScalarType delta)
{
	SimpleTempData<typename MESH_TYPE::VertContainer, ScaleLaplacianInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
	ScaleLaplacianInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.PntSum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.LenSum=0;
	TD.Start(lpz);
	typename  MESH_TYPE::FaceIterator fi;
	for(int i=0;i<step;++i)
	{
			typename  MESH_TYPE::VertexIterator vi;
			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				 TD[*vi]=lpz;

			for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) {
						typename  MESH_TYPE::CoordType edge= (*fi).V1(j)->P() -(*fi).V(j)->P();
						typename MESH_TYPE::ScalarType len=Norm(edge);
						edge/=len;
						TD[(*fi).V(j)].PntSum+=edge;
						TD[(*fi).V1(j)].PntSum-=edge;
						TD[(*fi).V(j)].LenSum+=len;
						TD[(*fi).V1(j)].LenSum+=len;
					}
				
			for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
				for(int j=0;j<3;++j)
					// se l'edge j e' di bordo si riazzera tutto e si riparte
					if((*fi).IsB(j)) {
							TD[(*fi).V(j)].PntSum=typename  MESH_TYPE::CoordType(0,0,0);
							TD[(*fi).V1(j)].PntSum=typename  MESH_TYPE::CoordType(0,0,0);
							TD[(*fi).V(j)].LenSum=0;
							TD[(*fi).V1(j)].LenSum=0;
					}
				

			for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
					for(int j=0;j<3;++j)
						if((*fi).IsB(j)) 
						{ 
							typename  MESH_TYPE::CoordType edge= (*fi).V1(j)->P() -(*fi).V(j)->P();
							typename MESH_TYPE::ScalarType len=Norm(edge);
							edge/=len;
							TD[(*fi).V(j)].PntSum+=edge;
							TD[(*fi).V1(j)].PntSum-=edge;
							TD[(*fi).V(j)].LenSum+=len;
							TD[(*fi).V1(j)].LenSum+=len;
						}

			for(vi=m.vert.begin();vi!=m.vert.end();++vi)
				if(!(*vi).IsD() && TD[*vi].LenSum>0 )
				 (*vi).P() = (*vi).P() + (TD[*vi].PntSum/TD[*vi].LenSum)*delta;
	}		 			
	TD.Stop();
};


template<class FLT> 
class LaplacianInfo 
{
public:
	Point3<FLT> sum;
	FLT cnt;
};

// Classical Laplacian Smoothing. Each vertex can be moved onto the average of the adjacent vertices.
// Can smooth only the selected vertices and weight the smoothing according to the quality 
// In the latter case 0 means that the vertex is not moved and 1 means that the vertex is moved onto the computed position.

template<class MESH_TYPE>
void LaplacianSmooth(MESH_TYPE &m, int step, bool SmoothSelected=false, float QualityWeight=0)
{
	SimpleTempData<typename MESH_TYPE::VertContainer,LaplacianInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
  LaplacianInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.sum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.cnt=1;
	TD.Start(lpz);
	for(int i=0;i<step;++i)
	{
		typename  MESH_TYPE::VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi].sum=(*vi).P();

		typename  MESH_TYPE::FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD()) 
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) 
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								//TD[(*fi).V(j)]=lpz;
								//TD[(*fi).V1(j)]=lpz;
								TD[(*fi).V0(j)].sum=(*fi).P0(j);
								TD[(*fi).V1(j)].sum=(*fi).P1(j);
                TD[(*fi).V0(j)].cnt=1;
                TD[(*fi).V1(j)].cnt=1;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j)) 
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}
	
  if(QualityWeight>0)
  { // quality weighted smoothing
    // We assume that weights are in the 0..1 range.
    assert(tri::HasPerVertexQuality(m));
    for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
      {
        float q=1.0-(*vi).Q();
				(*vi).P()=(*vi).P()*(1.0-q) + (TD[*vi].sum/TD[*vi].cnt)*q;
      }
  }

	else
    for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
					(*vi).P()=TD[*vi].sum/TD[*vi].cnt;
	}
	 	
	TD.Stop();
};

/*
  Improved Laplacian Smoothing of Noisy Surface Meshes
  J. Vollmer, R. Mencl, and H. M�ller
  EUROGRAPHICS Volume 18 (1999), Number 3
*/

template<class FLT> 
class HCSmoothInfo 
{
public:
	Point3<FLT> dif;
	Point3<FLT> sum;
	int cnt;
};
template<class MESH_TYPE>
void HCSmooth(MESH_TYPE &m, int step, bool SmoothSelected=false )
{
	typename MESH_TYPE::ScalarType beta=0.5;
	SimpleTempData<typename MESH_TYPE::VertContainer,HCSmoothInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
  HCSmoothInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.sum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.dif=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.cnt=0;
	TD.Start(lpz);
	// First Loop compute the laplacian
	typename  MESH_TYPE::FaceIterator fi;
	for(fi=m.face.begin();fi!=m.face.end();++fi)if(!(*fi).IsD())
		{
			for(int j=0;j<3;++j)
			{
				TD[(*fi).V(j)].sum+=(*fi).V1(j)->P();
				TD[(*fi).V1(j)].sum+=(*fi).V(j)->P();
				++TD[(*fi).V(j)].cnt;
				++TD[(*fi).V1(j)].cnt;
				// se l'edge j e' di bordo si deve sommare due volte
				if((*fi).IsB(j)) 
				{ 
					TD[(*fi).V(j)].sum+=(*fi).V1(j)->P(); 
					TD[(*fi).V1(j)].sum+=(*fi).V(j)->P(); 
					++TD[(*fi).V(j)].cnt;
					++TD[(*fi).V1(j)].cnt;
				}
			}
		}
	typename  MESH_TYPE::VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi) if(!(*vi).IsD())
		 TD[*vi].sum/=(float)TD[*vi].cnt;
	
	// Second Loop compute average difference
	for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
		{
			for(int j=0;j<3;++j)
			{
				TD[(*fi).V(j)].dif +=TD[(*fi).V1(j)].sum-(*fi).V1(j)->P();
				TD[(*fi).V1(j)].dif+=TD[(*fi).V(j)].sum-(*fi).V(j)->P();
				// se l'edge j e' di bordo si deve sommare due volte
				if((*fi).IsB(j)) 
				{ 
					TD[(*fi).V(j)].dif +=TD[(*fi).V1(j)].sum-(*fi).V1(j)->P();
					TD[(*fi).V1(j)].dif+=TD[(*fi).V(j)].sum-(*fi).V(j)->P();
				}
			}
		}

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		{
		 TD[*vi].dif/=(float)TD[*vi].cnt;
		 if(!SmoothSelected || (*vi).IsS())
	      (*vi).P()= TD[*vi].sum - (TD[*vi].sum - (*vi).P())*beta  + (TD[*vi].dif)*(1.f-beta);
		}
		 	
	TD.Stop();
};


// Laplacian smooth of the quality. 

template<class FLT> 
class QualitySmoothInfo 
{
public:
	FLT sum;
	int cnt;
};

template<class MESH_TYPE>
void LaplacianSmoothQuality(MESH_TYPE &m, int step,bool SmoothSelected=false)
{ 
	SimpleTempData<typename MESH_TYPE::VertContainer,QualitySmoothInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
  QualitySmoothInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.sum=0;
	lpz.cnt=0;
	TD.Start(lpz);
	for(int i=0;i<step;++i)
	{
		typename  MESH_TYPE::VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		typename  MESH_TYPE::FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD()) 
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) 
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->Q();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->Q();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j)) 
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->Q();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->Q();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}

	//typename  MESH_TYPE::VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
					(*vi).Q()=TD[*vi].sum/TD[*vi].cnt;
	}
		 	
	TD.Stop();
};
template<class MESH_TYPE>
void LaplacianSmoothNormals(MESH_TYPE &m, int step,bool SmoothSelected=false)
{
	SimpleTempData<typename MESH_TYPE::VertContainer,LaplacianInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
  LaplacianInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.sum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.cnt=0;
	TD.Start(lpz);
	for(int i=0;i<step;++i)
	{
		typename  MESH_TYPE::VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		typename  MESH_TYPE::FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD()) 
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) 
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->N();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->N();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j)) 
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->N();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->N();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}

	//typename  MESH_TYPE::VertexIterator vi;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			if(!SmoothSelected || (*vi).IsS())
					(*vi).N()=TD[*vi].sum/TD[*vi].cnt;
	}
		 	
	TD.Stop();
};

// Smooth solo lungo la direzione di vista
	// alpha e' compreso fra 0(no smoot) e 1 (tutto smoot)
  // Nota che se smootare il bordo puo far fare bandierine.
template<class MESH_TYPE>
void DepthSmooth(MESH_TYPE &m,
				 const typename  MESH_TYPE::CoordType & viewpoint,
				 const typename  MESH_TYPE::ScalarType alpha,
				 int step, bool SmoothBorder=false )
{
	typedef typename  MESH_TYPE::CoordType v_type;
	typedef typename MESH_TYPE::ScalarType    s_type;


	//const typename  MESH_TYPE::CoordType viewpoint;
	//const typename MESH_TYPE::ScalarType alpha;

	SimpleTempData<typename MESH_TYPE::VertContainer,LaplacianInfo<typename MESH_TYPE::ScalarType> > TD(m.vert);
	LaplacianInfo<typename MESH_TYPE::ScalarType> lpz;
	lpz.sum=typename  MESH_TYPE::CoordType(0,0,0);
	lpz.cnt=0;
	TD.Start(lpz);
	for(int i=0;i<step;++i)
	{
		typename  MESH_TYPE::VertexIterator vi;
		for(vi=m.vert.begin();vi!=m.vert.end();++vi)
			 TD[*vi]=lpz;

		typename  MESH_TYPE::FaceIterator fi;
		for(fi=m.face.begin();fi!=m.face.end();++fi)
			if(!(*fi).IsD()) 
				for(int j=0;j<3;++j)
					if(!(*fi).IsB(j)) 
						{
							TD[(*fi).V(j)].sum+=(*fi).V1(j)->Supervisor_P();
							TD[(*fi).V1(j)].sum+=(*fi).V(j)->Supervisor_P();
							++TD[(*fi).V(j)].cnt;
							++TD[(*fi).V1(j)].cnt;
					}

			// si azzaera i dati per i vertici di bordo
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j))
							{
								TD[(*fi).V(j)]=lpz;
								TD[(*fi).V1(j)]=lpz;
							}

			// se l'edge j e' di bordo si deve mediare solo con gli adiacenti
     if(SmoothBorder)
			for(fi=m.face.begin();fi!=m.face.end();++fi)
				if(!(*fi).IsD()) 
					for(int j=0;j<3;++j)
						if((*fi).IsB(j)) 
							{
								TD[(*fi).V(j)].sum+=(*fi).V1(j)->Supervisor_P();
								TD[(*fi).V1(j)].sum+=(*fi).V(j)->Supervisor_P();
								++TD[(*fi).V(j)].cnt;
								++TD[(*fi).V1(j)].cnt;
						}
	
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		if(!(*vi).IsD() && TD[*vi].cnt>0 )
			{
				v_type np = TD[*vi].sum/TD[*vi].cnt;
				v_type d = (*vi).Supervisor_P() - viewpoint; d.Normalize();
				s_type s = d * ( np - (*vi).Supervisor_P() );
				(*vi).Supervisor_P() += d * (s*alpha);
			}
	}
	 	
	TD.Stop();
}



/****************************************************************************************************************/
/****************************************************************************************************************/
// Paso Double Smoothing
//  The proposed
// approach is a two step method where  in the first step the face normals
// are adjusted and then, in a second phase, the vertex positions are updated.
/****************************************************************************************************************/
/****************************************************************************************************************/
// Classi di info
template<class FLT> 
class PDVertInfo 
{
public:
	Point3<FLT> np;
};

template<class FLT> 
class PDFaceInfo 
{
public:
	Point3<FLT> m;
};
/***************************************************************************/
// Paso Doble Step 1 compute the smoothed normals
/***************************************************************************/
// Requirements:
// VF Topology
// Normalized Face Normals
// 
// This is the Normal Smoothing approach of Shen and Berner
// Fuzzy Vector Median-Based Surface Smoothing TVCG 2004

template<class MESH_TYPE>
void NormalSmoothSB(MESH_TYPE &m, 	
				  SimpleTempData<typename MESH_TYPE::FaceContainer,PDFaceInfo< typename MESH_TYPE::ScalarType > > &TD,
				  typename MESH_TYPE::ScalarType sigma)
{
	int i;
	
	typedef typename MESH_TYPE::CoordType CoordType;
	typedef typename MESH_TYPE::ScalarType ScalarType;
	typename MESH_TYPE::FaceIterator fi;

	for(fi=m.face.begin();fi!=m.face.end();++fi)
	{
		CoordType bc=(*fi).Barycenter();
    // 1) Clear all the selected flag of faces that are vertex-adjacent to fi
		for(i=0;i<3;++i)
		{
    	vcg::face::VFIterator<typename MESH_TYPE::FaceType> ep(&*fi,i);
		  while (!ep.End())
			{
				ep.f->ClearS();
				++ep;
			}
		}

    // 1) Effectively average the normals weighting them with 
    (*fi).SetS();
		CoordType mm=CoordType(0,0,0);
		for(i=0;i<3;++i)
		{
		  vcg::face::VFIterator<typename MESH_TYPE::FaceType> ep(&*fi,i);
		  while (!ep.End())
			{
				if(! (*ep.f).IsS() )
				{
					if(sigma>0) 
					{
						ScalarType dd=SquaredDistance(ep.f->Barycenter(),bc);
						ScalarType ang=AngleN(ep.f->N(),(*fi).N());
						mm+=ep.f->N()*exp((-sigma)*ang*ang/dd);
					}
					else mm+=ep.f->N();
					(*ep.f).SetS();
				}
				++ep;
			}
		}
		mm.Normalize();
		TD[*fi].m=mm;
	}
}

/***************************************************************************/
// Paso Doble Step 1 compute the smoothed normals
/***************************************************************************/
// Requirements:
// VF Topology
// Normalized Face Normals
// 
// This is the Normal Smoothing approach bsased on a angle thresholded weighting
// sigma is in the 0 .. 1 range
template<class MESH_TYPE>
void NormalSmooth(MESH_TYPE &m, 	
				  SimpleTempData<typename MESH_TYPE::FaceContainer,PDFaceInfo< typename MESH_TYPE::ScalarType > > &TD,
				  typename MESH_TYPE::ScalarType sigma)
{
	int i;
	
	typedef typename MESH_TYPE::CoordType CoordType;
	typedef typename MESH_TYPE::ScalarType ScalarType;
	typedef typename vcg::face::VFIterator<typename MESH_TYPE::FaceType> VFLocalIterator;
  typename MESH_TYPE::FaceIterator fi;
	
	for(fi=m.face.begin();fi!=m.face.end();++fi)
	{
		CoordType bc=Barycenter<MESH_TYPE::FaceType>(*fi);
    // 1) Clear all the selected flag of faces that are vertex-adjacent to fi
		for(i=0;i<3;++i)
		{
    	VFLocalIterator ep(&*fi,i);
		  for (;!ep.End();++ep)
				ep.f->ClearS();
		}

    // 1) Effectively average the normals weighting them with 
    //(*fi).SetS();
		CoordType mm=CoordType(0,0,0);
		//CoordType mm=(*fi).N();
		for(i=0;i<3;++i)
		{
    	VFLocalIterator ep(&*fi,i);
		  for (;!ep.End();++ep)
			{
				if(! (*ep.f).IsS() )
				{ 
          ScalarType cosang=ep.f->N()*(*fi).N();
          if(cosang >= sigma)
          {
            ScalarType w = cosang-sigma;
						mm += ep.f->N()*(w*w);
          }
					(*ep.f).SetS();
				}
			}
		}
		mm.Normalize();
		TD[*fi].m=mm;
	}
  
  for(fi=m.face.begin();fi!=m.face.end();++fi) 
    (*fi).N()=TD[*fi].m;
}

/****************************************************************************************************************/
// Restituisce il gradiente dell'area del triangolo nel punto p.
// Nota che dovrebbe essere sempre un vettore che giace nel piano del triangolo e perpendicolare al lato opposto al vertice p.
// Ottimizzato con Maple e poi pesantemente a mano.
template <class FLT>
Point3<FLT> TriAreaGradient(Point3<FLT> &p,Point3<FLT> &p0,Point3<FLT> &p1)
{
	Point3<FLT> dd = p1-p0;
	Point3<FLT> d0 = p-p0;
	Point3<FLT> d1 = p-p1;
	Point3<FLT> grad;

	FLT t16 =  d0[1]* d1[2] - d0[2]* d1[1];
	FLT t5  = -d0[2]* d1[0] + d0[0]* d1[2];
	FLT t4  = -d0[0]* d1[1] + d0[1]* d1[0];

	FLT delta= sqrtf(t4*t4 + t5*t5 +t16*t16);

	grad[0]= (t5  * (-dd[2]) + t4 * ( dd[1]))/delta;
	grad[1]= (t16 * (-dd[2]) + t4 * (-dd[0]))/delta;
	grad[2]= (t16 * ( dd[1]) + t5 * ( dd[0]))/delta;

	return grad;
}

template <class FLT>
Point3<FLT> CrossProdGradient(Point3<FLT> &p, Point3<FLT> &p0, Point3<FLT> &p1, Point3<FLT> &m)
{
	Point3<FLT> grad;
	Point3<FLT> p00=p0-p;
	Point3<FLT> p01=p1-p;
	grad[0] = (-p00[2] + p01[2])*m[1] + (-p01[1] + p00[1])*m[2];
	grad[1] = (-p01[2] + p00[2])*m[0] + (-p00[0] + p01[0])*m[2];
	grad[2] = (-p00[1] + p01[1])*m[0] + (-p01[0] + p00[0])*m[1];

	return grad;
}

/*
Deve Calcolare il gradiente di 
E(p) = A(p,p0,p1) (n - m)^2 =
A(...) (2-2nm)   = 
(p0-p)^(p1-p) 
2A - 2A * ------------- m  =
2A

2A  -  2 (p0-p)^(p1-p) * m
*/
template <class FLT>
Point3<FLT> FaceErrorGrad(Point3<FLT> &p,Point3<FLT> &p0,Point3<FLT> &p1, Point3<FLT> &m)
{
	return     TriAreaGradient(p,p0,p1) *2.0f
		- CrossProdGradient(p,p0,p1,m) *2.0f ;
}
/***************************************************************************/
// Paso Doble Step 2 Fitta la mesh a un dato insieme di normali
/***************************************************************************/

template<class MESH_TYPE>
void FitMesh(MESH_TYPE &m, 
			 SimpleTempData<typename MESH_TYPE::VertContainer, PDVertInfo<typename MESH_TYPE::ScalarType> > &TDV,
			 SimpleTempData<typename MESH_TYPE::FaceContainer, PDFaceInfo<typename MESH_TYPE::ScalarType> > &TDF,
			 float lambda)
{
	//vcg::face::Pos<typename MESH_TYPE::FaceType> ep;
	vcg::face::VFIterator<typename MESH_TYPE::FaceType> ep;
	typename MESH_TYPE::VertexIterator vi;
	typedef typename MESH_TYPE::ScalarType ScalarType;
	typedef typename MESH_TYPE::CoordType CoordType;
	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
	{
		CoordType ErrGrad=CoordType(0,0,0);

		ep.f=(*vi).VFp();
		ep.z=(*vi).VFi();
		while (!ep.End())
		{
			ErrGrad+=FaceErrorGrad(ep.f->V(ep.z)->P(),ep.f->V1(ep.z)->P(),ep.f->V2(ep.z)->P(),TDF[ep.f].m);
			++ep;
		}
		TDV[*vi].np=(*vi).P()-ErrGrad*(ScalarType)lambda;					
	}

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		(*vi).P()=TDV[*vi].np;

}
/****************************************************************************************************************/


template<class MESH_TYPE>
void FastFitMesh(MESH_TYPE &m, 
			 SimpleTempData<typename MESH_TYPE::VertContainer, PDVertInfo<typename MESH_TYPE::ScalarType> > &TDV,
			 SimpleTempData<typename MESH_TYPE::FaceContainer, PDFaceInfo<typename MESH_TYPE::ScalarType> > &TDF)
{
	//vcg::face::Pos<typename MESH_TYPE::FaceType> ep;
	vcg::face::VFIterator<typename MESH_TYPE::FaceType> ep;
	typename MESH_TYPE::VertexIterator vi;
	typedef typename MESH_TYPE::ScalarType ScalarType;
	typedef typename MESH_TYPE::CoordType CoordType;
	typedef typename vcg::face::VFIterator<typename MESH_TYPE::FaceType> VFLocalIterator;

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
	{
   CoordType Sum(0,0,0);
   ScalarType cnt=0;
   VFLocalIterator ep(&*vi);
	 for (;!ep.End();++ep)
		{
      CoordType bc=Barycenter<MESH_TYPE::FaceType>(*ep.F());
      Sum += ep.F()->N()*(ep.F()->N()*(bc - (*vi).P()));
      ++cnt;
		}
		TDV[*vi].np=(*vi).P()+ Sum*(1.0/cnt);					
	}

	for(vi=m.vert.begin();vi!=m.vert.end();++vi)
		(*vi).P()=TDV[*vi].np;

}






template<class MeshType>
void PasoDobleSmooth(MeshType &m, int step, typename MeshType::ScalarType Sigma=0, int FitStep=10, typename MeshType::ScalarType FitLambda=0.05)
{
  typedef typename MeshType::ScalarType     ScalarType;
  typedef typename MeshType::CoordType     CoordType;


	SimpleTempData< typename MeshType::VertContainer, PDVertInfo<ScalarType> > TDV(m.vert);
	SimpleTempData< typename MeshType::FaceContainer, PDFaceInfo<ScalarType> > TDF(m.face);
	PDVertInfo<ScalarType> lpzv;
	lpzv.np=CoordType(0,0,0);
	PDFaceInfo<ScalarType> lpzf;
	lpzf.m=CoordType(0,0,0);

	assert(m.HasVFTopology());
	m.HasVFTopology();
	TDV.Start(lpzv);
	TDF.Start(lpzf);
	for(int j=0;j<step;++j)
	{

		vcg::tri::UpdateNormals<MeshType>::PerFace(m);
		NormalSmooth<MeshType>(m,TDF,Sigma);
		for(int k=0;k<FitStep;k++)
			FitMesh<MeshType>(m,TDV,TDF,FitLambda);
	}

	TDF.Stop();
	TDV.Stop();

}
template<class MeshType>
void PasoDobleSmoothFast(MeshType &m, int step, typename MeshType::ScalarType Sigma=0, int FitStep=50, bool SmoothSelected =false)
{
  typedef typename MeshType::ScalarType     ScalarType;
  typedef typename MeshType::CoordType     CoordType;


	SimpleTempData< typename MeshType::VertContainer, PDVertInfo<ScalarType> > TDV(m.vert);
	SimpleTempData< typename MeshType::FaceContainer, PDFaceInfo<ScalarType> > TDF(m.face);
	PDVertInfo<ScalarType> lpzv;
	lpzv.np=CoordType(0,0,0);
	PDFaceInfo<ScalarType> lpzf;
	lpzf.m=CoordType(0,0,0);

	assert(m.HasVFTopology());
	m.HasVFTopology();
	TDV.Start(lpzv);
	TDF.Start(lpzf);
	
  for(int j=0;j<step;++j)
	   NormalSmooth<MeshType>(m,TDF,Sigma);
		
  for(int j=0;j<FitStep;++j)
	  FastFitMesh<MeshType>(m,TDV,TDF);

	

	TDF.Stop();
	TDV.Stop();

}



}		// End namespace vcg

#endif //  VCG_SMOOTH
