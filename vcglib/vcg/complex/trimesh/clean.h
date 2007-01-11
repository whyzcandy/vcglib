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
Revision 1.47  2006/12/01 21:26:14  cignoni
Corrected bug in the IsFFAdjacencyConsistent the Topology checking function.

Revision 1.46  2006/12/01 08:12:30  cignoni
Added a function for FF topology consistency check

Revision 1.45  2006/12/01 00:00:56  cignoni
Corrected IsOrientedMesh. After the templating of the swapedge it did not worked any more....
Added Texture management to the FlipMesh

Revision 1.44  2006/11/27 10:36:35  cignoni
Added IsSizeConsistent

Revision 1.43  2006/11/09 17:26:24  cignoni
Corrected RemoveNonManifoldFace

Revision 1.42  2006/10/15 07:31:22  cignoni
typenames and qualifiers for gcc compliance

Revision 1.41  2006/10/09 20:06:46  cignoni
Added Remove NonManifoldFace

Revision 1.40  2006/05/25 09:41:09  cignoni
missing std and other gcc detected syntax errors

Revision 1.39  2006/05/16 21:51:07  cignoni
Redesigned the function for the removal of faces according to their area and edge lenght

Revision 1.38  2006/05/03 21:40:27  cignoni
Changed HasMark to HasPerFaceMark(m) and commented some unused internal vars of the class

Revision 1.37  2006/04/18 07:01:22  zifnab1974
added a ; how could this ever compile?

Revision 1.36  2006/04/12 15:08:51  cignoni
Added ConnectedIterator (should be moved somewhere else)
Cleaned ConnectedComponents

Revision 1.35  2006/02/28 16:51:29  ponchio
Added typename

Revision 1.34  2006/02/01 15:27:00  cignoni
Added IsD() test in SelfIntersection

Revision 1.33  2006/01/27 09:55:25  corsini
fix signed/unsigned mismatch

Revision 1.32  2006/01/23 13:33:54  cignoni
Added a missing vcg::

Revision 1.31  2006/01/22 17:06:27  cignoni
vi/fi mismatch in ClipWithBox

Revision 1.30  2006/01/22 10:07:42  cignoni
Corrected use of Area with the unambiguous DoubleArea
Added ClipWithBox function

Revision 1.29  2006/01/11 15:40:14  cignoni
Added RemoveDegenerateFace and added its automatic invocation at the end of RemoveDuplicateVertex

Revision 1.28  2006/01/02 09:49:36  cignoni
Added some missing std::

Revision 1.27  2005/12/29 12:27:37  cignoni
Splitted IsComplexManifold in IsTwoManifoldFace and IsTwoManifoldVertex

Revision 1.26  2005/12/21 14:15:03  corsini
Remove printf

Revision 1.25  2005/12/21 13:09:03  corsini
Modify genus computation

Revision 1.24  2005/12/19 15:13:06  corsini
Fix IsOrientedMesh

Revision 1.23  2005/12/16 13:13:44  cignoni
Reimplemented SelfIntersection

Revision 1.22  2005/12/16 10:54:59  corsini
Reimplement isOrientedMesh

Revision 1.21  2005/12/16 10:53:39  corsini
Take account for deletion in isComplexManifold

Revision 1.20  2005/12/16 10:51:43  corsini
Take account for deletion in isRegularMesh

Revision 1.19  2005/12/15 13:53:13  corsini
Reimplement isComplexManifold
Reimplement isRegular

Revision 1.18  2005/12/14 14:04:35  corsini
Fix genus computation

Revision 1.17  2005/12/12 12:11:40  cignoni
Removed unuseful detectunreferenced

Revision 1.16  2005/12/04 00:25:00  cignoni
Changed DegeneratedFaces -> RemoveZeroAreaFaces

Revision 1.15  2005/12/03 22:34:25  cignoni
Added  missing include and sdt:: (tnx to Mario Latronico)

Revision 1.14  2005/12/02 00:14:43  cignoni
Removed some pointer vs iterator issues that prevented gcc compilation

Revision 1.13  2005/11/22 14:04:10  rita_borgo
Completed and tested self-intersection routine

Revision 1.12  2005/11/17 00:41:07  cignoni
Removed Initialize use updateflags::Clear() instead.

Revision 1.11  2005/11/16 16:33:23  rita_borgo
Changed ComputeSelfintersection

Revision 1.10  2005/11/15 12:16:34  rita_borgo
Changed DegeneratedFaces, sets the D flags for each faces
that is found to be degenerated.
CounEdges and ConnectedComponents check now if a face IsD()
else for degenerated faces many asserts fail.

Revision 1.9  2005/11/14 09:28:18  cignoni
changed access to face functions (border, area)
removed some typecast warnings

Revision 1.8  2005/10/11 16:03:40  rita_borgo
Added new functions belonging to triMeshInfo
Started the Self-Intersection routine

Revision 1.7  2005/10/03 15:57:53  rita_borgo
Alligned with TriMeshInfo Code

Revision 1.6  2005/01/28 11:59:35  cignoni
Add std:: to stl containers

Revision 1.5  2004/09/20 08:37:57  cignoni
Better Doxygen docs

Revision 1.4  2004/08/25 15:15:26  ganovelli
minor changes to comply gcc compiler (typename's and stuff)

Revision 1.3  2004/07/18 06:55:37  cignoni
NewUserBit -> NewBitFlag

Revision 1.2  2004/07/09 15:48:37  tarini
Added an include (<algorithm>)

Revision 1.1  2004/06/24 08:03:59  cignoni
Initial Release


****************************************************************************/

#ifndef __VCGLIB_CLEAN
#define __VCGLIB_CLEAN

// Standard headers
#include <map>
#include <algorithm>
#include <stack>

// VCG headers
#include <vcg/simplex/face/pos.h>
#include <vcg/simplex/face/topology.h>
#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/closest.h>
#include <vcg/space/index/grid_static_ptr.h>
#include<vcg/complex/trimesh/allocate.h>


namespace vcg {
	namespace tri{
template <class ConnectedMeshType>
class ConnectedIterator
{
  	public:
			typedef ConnectedMeshType MeshType; 
			typedef typename MeshType::VertexType     VertexType;
			typedef typename MeshType::VertexPointer  VertexPointer;
			typedef typename MeshType::VertexIterator VertexIterator;
			typedef	typename MeshType::ScalarType			ScalarType;
			typedef typename MeshType::FaceType       FaceType;
			typedef typename MeshType::FacePointer    FacePointer;
			typedef typename MeshType::FaceIterator   FaceIterator;
			typedef typename MeshType::FaceContainer  FaceContainer;
      typedef typename vcg::Box3<ScalarType>  Box3Type;

public:
 void operator ++()
 {
	FacePointer fpt=sf.top();
  sf.pop();
	for(int j=0;j<3;++j)
		if( !face::IsBorder(*fpt,j) )
			{
				FacePointer l=fpt->FFp(j);
				if( !mp->IsMarked(l) )
				{
					mp->Mark(l);
					sf.push(l);
				}
			}
}

 void start(MeshType &m, FacePointer p)
 {
  mp=&m;
  while(!sf.empty()) sf.pop();
  mp->UnMarkAll();
  assert(p);
  assert(!p->IsD());
	mp->Mark(p);
	sf.push(p);
 }
 bool completed() {
   return sf.empty();
 }

 FacePointer operator *() 
 {
   return sf.top();
 }
private:
  std::stack<FacePointer> sf;
  MeshType *mp;
};


		/// 
		/** \addtogroup trimesh */
		/*@{*/
		/// Class of static functions to clean/correct/restore meshs. 
		template <class CleanMeshType>
		class Clean
		{

		public:
			typedef CleanMeshType MeshType; 
			typedef typename MeshType::VertexType     VertexType;
			typedef typename MeshType::VertexPointer  VertexPointer;
			typedef typename MeshType::VertexIterator VertexIterator;
			typedef	typename MeshType::ScalarType			ScalarType;
			typedef typename MeshType::FaceType       FaceType;
			typedef typename MeshType::FacePointer    FacePointer;
			typedef typename MeshType::FaceIterator   FaceIterator;
			typedef typename MeshType::FaceContainer  FaceContainer;
      typedef typename vcg::Box3<ScalarType>  Box3Type;

			typedef GridStaticPtr<FaceType, ScalarType > TriMeshGrid;
			typedef Point3<ScalarType> Point3x;

			//TriMeshGrid   gM;
			//FaceIterator fi;
			//FaceIterator gi;
			//vcg::face::Pos<FaceType> he;
			//vcg::face::Pos<FaceType> hei;

			/* classe di confronto per l'algoritmo di eliminazione vertici duplicati*/
			class RemoveDuplicateVert_Compare{
			public:
				inline bool operator()(VertexPointer const &a, VertexPointer const &b)
				{
					return (*a).cP() < (*b).cP();
				}
			};


			/** This function removes all duplicate vertices of the mesh by looking only at their spatial positions. 
			Note that it does not update any topology relation that could be affected by this like the VT or TT relation.
			the reason this function is usually performed BEFORE building any topology information.
			*/
			static int RemoveDuplicateVertex( MeshType & m, bool RemoveDegenerateFlag=true)    // V1.0
			{
				if(m.vert.size()==0 || m.vn==0) return 0;

				std::map<VertexPointer, VertexPointer> mp;
				int i,j;
				VertexIterator vi; 
				int deleted=0;
				int k=0;
				size_t num_vert = m.vert.size();
				std::vector<VertexPointer> perm(num_vert);
				for(vi=m.vert.begin(); vi!=m.vert.end(); ++vi, ++k)
					perm[k] = &(*vi);

				RemoveDuplicateVert_Compare c_obj;

				std::sort(perm.begin(),perm.end(),c_obj);

				j = 0;
				i = j;
				mp[perm[i]] = perm[j];
				++i;
				for(;i!=num_vert;)
				{
					if( (! (*perm[i]).IsD()) && 
						(! (*perm[j]).IsD()) && 
						(*perm[i]).P() == (*perm[j]).cP() )
					{
						VertexPointer t = perm[i];
						mp[perm[i]] = perm[j];
						++i;
						(*t).SetD();
						deleted++;
					}
					else
					{
						j = i;
						++i;
					}
				}
				FaceIterator fi;
				for(fi = m.face.begin(); fi!=m.face.end(); ++fi)
					for(k = 0; k < 3; ++k)
						if( !(*fi).IsD() )
							if( mp.find( (typename MeshType::VertexPointer)(*fi).V(k) ) != mp.end() )
							{
								(*fi).V(k) = &*mp[ (*fi).V(k) ];
							}
							m.vn -= deleted;
							return deleted;
			
        if(RemoveDegenerateFlag) RemoveDegenerateFace(m);  
      }


			/** This function removes that are not referenced by any face. The function updates the vn counter.
			@param m The mesh
			@return The number of removed vertices
			*/
			static int RemoveUnreferencedVertex( MeshType& m, bool DeleteVertexFlag=true)   // V1.0
			{
				FaceIterator fi;
				VertexIterator vi;
				int referredBit = VertexType::NewBitFlag();

				int j;
				int deleted = 0;

				for(vi=m.vert.begin();vi!=m.vert.end();++vi)
					(*vi).ClearUserBit(referredBit);

				for(fi=m.face.begin();fi!=m.face.end();++fi)
					if( !(*fi).IsD() )
						for(j=0;j<3;++j)
							(*fi).V(j)->SetUserBit(referredBit);

				for(vi=m.vert.begin();vi!=m.vert.end();++vi)
					if( (!(*vi).IsD()) && (!(*vi).IsUserBit(referredBit)))
					{
						if(DeleteVertexFlag) 
            {
              (*vi).SetD();
              --m.vn;
            }
						++deleted;
					}
					VertexType::DeleteBitFlag(referredBit);
					return deleted;
			}


      /*
      Degenerate faces are faces that are Topologically degenerate, 
      i.e. have two or more vertex reference that link the same vertex 
      (and not only two vertexes with the same coordinates).
      All Degenerate faces are zero area faces BUT not all zero area faces are degenerate.
      We do not take care of topology because when we have degenerate faces the 
      topology calculation functions crash.
      */ 
      static int RemoveDegenerateFace(MeshType& m)
      {
				FaceIterator fi;
				int count_fd = 0;

				for(fi=m.face.begin(); fi!=m.face.end();++fi)
					if((*fi).V(0) == (*fi).V(1) || 
             (*fi).V(0) == (*fi).V(2) ||
             (*fi).V(1) == (*fi).V(2) )
					{
						count_fd++;
						fi->SetD();
						m.fn--;
					}
				return count_fd;
			}
      static int RemoveNonManifoldFace(MeshType& m)
      {
				FaceIterator fi;
				int count_fd = 0;
				std::vector<FacePointer> ToDelVec;

				for(fi=m.face.begin(); fi!=m.face.end();++fi)
					if (!fi->IsD())
					{
						if ((!IsManifold(*fi,0))||
								(!IsManifold(*fi,1))||
								(!IsManifold(*fi,2)))
					                  ToDelVec.push_back(&*fi);
					}
          
          for(int i=0;i<ToDelVec.size();++i)
          {
            if(!ToDelVec[i]->IsD())
            {
            FaceType &ff= *ToDelVec[i];
              if ((!IsManifold(ff,0))||
							  	(!IsManifold(ff,1))||
								  (!IsManifold(ff,2)))
              {
                for(int j=0;j<3;++j)
                    if(!face::IsBorder<FaceType>(ff,j)) 
                      vcg::face::FFDetach<FaceType>(ff,j);
                  
                ff.SetD();
                count_fd++;
						    m.fn--;	
              }
            }
          }
				return count_fd;
			}
       
      /*
      The following functions remove faces that are geometrically "bad" according to edges and area criteria. 
      They remove the faces that are out of a given range of area or edges (e.g. faces too large or too small, or with edges too short or too long)
      but that could be topologically correct.
      These functions can optionally take into account only the selected faces.
      */
      template<bool Selected>  
		  static int RemoveFaceOutOfRangeAreaSel(MeshType& m, ScalarType MinAreaThr=0, ScalarType MaxAreaThr=std::numeric_limits<ScalarType>::max())
			{
				FaceIterator fi;
				int count_fd = 0;
        MinAreaThr*=2;
        MaxAreaThr*=2;
        for(fi=m.face.begin(); fi!=m.face.end();++fi)
     			if(!(*fi).IsD())
              if(!Selected || (*fi).IsS())
              {
                const ScalarType doubleArea=DoubleArea<FaceType>(*fi);
					      if((doubleArea<=MinAreaThr) || (doubleArea>=MaxAreaThr) )
					      {
						      count_fd++;
						      fi->SetD();
						      m.fn--;
					      }
              }
				return count_fd;
			}
      template<bool Selected>  
        static int RemoveFaceOutOfRangeEdgeSel( MeshType& m, ScalarType MinEdgeThr=0, ScalarType MaxEdgeThr=std::numeric_limits<ScalarType>::max())
      {
        FaceIterator fi;
        int count_fd = 0;
        MinEdgeThr=MinEdgeThr*MinEdgeThr;
        MaxEdgeThr=MaxEdgeThr*MaxEdgeThr;
        for(fi=m.face.begin(); fi!=m.face.end();++fi)
          if(!(*fi).IsD())
            if(!Selected || (*fi).IsS())
            {
              for(unsigned int i=0;i<3;++i)
              {
                const ScalarType squaredEdge=SquaredDistance((*fi).V0(i)->cP(),(*fi).V1(i)->cP());
                if((squaredEdge<=MinEdgeThr) || (squaredEdge>=MaxEdgeThr) )
                {
                  count_fd++;
                  fi->SetD();
                  m.fn--;
                  break; // skip the rest of the edges of the tri
                }
              }
            }
            return count_fd;
      }

      	// alias for the old style. Kept for backward compatibility
      static int RemoveZeroAreaFace(MeshType& m) { return RemoveFaceOutOfRangeArea(m);}
      
      // Aliases for the functions that do not look at selection
      static int RemoveFaceOutOfRangeArea(MeshType& m, ScalarType MinAreaThr=0, ScalarType MaxAreaThr=std::numeric_limits<ScalarType>::max())
      {
        return RemoveFaceOutOfRangeAreaSel<false>(m,MinAreaThr,MaxAreaThr);
      }
      static int RemoveFaceOutOfRangeEdge(MeshType& m, ScalarType MinEdgeThr=0, ScalarType MaxEdgeThr=std::numeric_limits<ScalarType>::max())
      {
        return RemoveFaceOutOfRangeEdgeSel<false>(m,MinEdgeThr,MaxEdgeThr);
      }





			static int ClipWithBox( MeshType & m, Box3Type &bb) 
      {
        FaceIterator fi;
        VertexIterator vi;

				for (vi = m.vert.begin(); vi != m.vert.end(); ++vi) if(!(*vi).IsD())
        {
          if(!bb.IsIn((*vi).P()) )
          {
            (*vi).SetD();
            --m.vn;
          }
        }
				for (fi = m.face.begin(); fi != m.face.end(); ++fi) if(!(*fi).IsD())
        {
          if( (*fi).V(0)->IsD() || 
              (*fi).V(1)->IsD() || 
              (*fi).V(2)->IsD() )
                {
                  (*fi).SetD();
                   --m.fn;
                }
        }
        return m.vn;
      }


			/**
			 * Check if the mesh is a manifold.
			 *
			 * First of all, for each face the FF condition is checked.
			 * Then, a second test is performed: for each vertex the 
			 * number of face found have to be the same of the number of 
			 * face found with the VF walk trough.
			 */
			static bool IsTwoManifoldFace( MeshType & m ) 
			{
				bool flagManifold = true;

				FaceIterator fi;

				// First Test
				assert(m.HasFFTopology());
				for (fi = m.face.begin(); fi != m.face.end(); ++fi)
				{
					if (!fi->IsD())
					{
						if ((!IsManifold(*fi,0))||
								(!IsManifold(*fi,1))||
								(!IsManifold(*fi,2)))
						{
							flagManifold = false;
							break;
						}
					}
				}
        return flagManifold;
      }

			static bool IsTwoManifoldVertex( MeshType & m ) 
			{
				VertexIterator vi;
				bool flagManifold = true;
				assert(m.HasVFTopology());

					face::VFIterator<FaceType> vfi;
					int starSizeFF;
					int starSizeVF;
					for (vi = m.vert.begin(); vi != m.vert.end(); ++vi)
					{
						if (!vi->IsD())
						{
							face::VFIterator<FaceType> vfi(&*vi);
							face::Pos<FaceType> pos((*vi).VFp(), &*vi);

							starSizeFF = pos.StarSize();

							starSizeVF = 0;
							while(!vfi.End())
							{
								++vfi;
								starSizeVF++;
							}

							if (starSizeFF != starSizeVF)
							{
								flagManifold = false;
								break;
							}
						}
					}

				return flagManifold;
			}

			static void CountEdges( MeshType & m, int &count_e, int &boundary_e ) 
			{
				FaceIterator fi;
				vcg::face::Pos<FaceType> he;
				vcg::face::Pos<FaceType> hei;
				bool counted =false;
				for(fi=m.face.begin();fi!=m.face.end();fi++)
				{
					if(!((*fi).IsD()))
					{
					(*fi).SetS();
					count_e +=3; //assume that we have to increase the number of edges with three
					for(int j=0; j<3; j++)
					{
            if (face::IsBorder(*fi,j)) //If this edge is a border edge
							boundary_e++; // then increase the number of boundary edges
						else if (IsManifold(*fi,j))//If this edge is manifold
						{
							if((*fi).FFp(j)->IsS()) //If the face on the other side of the edge is already selected
								count_e--; // we counted one edge twice
						}
						else//We have a non-manifold edge
						{
							hei.Set(&(*fi), j , fi->V(j));
							he=hei;
							he.NextF();
							while (he.f!=hei.f)// so we have to iterate all faces that are connected to this edge
							{
								if (he.f->IsS())// if one of the other faces was already visited than this edge was counted already.
								{
									counted=true;
									break;
								}
								else
								{
									he.NextF();
								}
							}
							if (counted)
							{
								count_e--;
								counted=false;
							}
						}
					}
					}
				}
			}


			static int CountHoles( MeshType & m)
			{
				int numholes=0;
				int numholev=0;
				int BEdges=0;
				FaceIterator fi;
				FaceIterator gi;
				vcg::face::Pos<FaceType> he;
				vcg::face::Pos<FaceType> hei;

        std::vector< std::vector<Point3x> > holes; //indices of vertices

				for(fi=m.face.begin();fi!=m.face.end();++fi)
					(*fi).ClearS();
				gi=m.face.begin(); fi=gi;

				for(fi=m.face.begin();fi!=m.face.end();fi++)//for all faces do
				{
					for(int j=0;j<3;j++)//for all edges
					{
						if(fi->V(j)->IsS()) continue;

            if(face::IsBorder(*fi,j))//found an unvisited border edge
						{
							he.Set(&(*fi),j,fi->V(j)); //set the face-face iterator to the current face, edge and vertex
              std::vector<Point3x> hole; //start of a new hole
							hole.push_back(fi->P(j)); // including the first vertex
							numholev++;
							he.v->SetS(); //set the current vertex as selected
							he.NextB(); //go to the next boundary edge


							while(fi->V(j) != he.v)//will we do not encounter the first boundary edge.
							{
								Point3x newpoint = he.v->P(); //select its vertex.
								if(he.v->IsS())//check if this vertex was selected already, because then we have an additional hole.
								{
									//cut and paste the additional hole.
                  std::vector<Point3x> hole2;
									int index = static_cast<int>(find(hole.begin(),hole.end(),newpoint) 
										- hole.begin());
									for(unsigned int i=index; i<hole.size(); i++)
										hole2.push_back(hole[i]);

									hole.resize(index);
									if(hole2.size()!=0) //annoying in degenerate cases
										holes.push_back(hole2);
								}
								hole.push_back(newpoint);
								numholev++;
								he.v->SetS(); //set the current vertex as selected
								he.NextB(); //go to the next boundary edge
							}
							holes.push_back(hole);
						}
					}
				}
				return static_cast<int>(holes.size());
			}

			static int BorderEdges( MeshType & m, int numholes)
			{
				int BEdges = 0;
				for(int i=0; i<numholes; i++)
				{
					if(i==numholes-1) 
						BEdges++;
					else 
						BEdges++;
				}
				return BEdges;

			}

 
      static int DeleteConnectedComponent(MeshType &m,FacePointer fp)
      {
        int deleted=0;
        return deleted;
      }
      
      /*
  Compute the set of connected components of a given mesh
  it fills a vector of pair < int , faceptr > with, for each connecteed component its size and a represnant
 */
			static int ConnectedComponents(MeshType &m)
      {
        std::vector< std::pair<int,FacePointer> > CCV;
        return ConnectedComponents(m,CCV);
      }
      static int ConnectedComponents(MeshType &m, std::vector< std::pair<int,FacePointer> > &CCV)
			{
				FaceIterator fi;
				FacePointer l;
		    CCV.clear();

				for(fi=m.face.begin();fi!=m.face.end();++fi)
					(*fi).ClearS();
		
        int Compindex=0;
        std::stack<FacePointer> sf;
        FacePointer fpt=&*(m.face.begin());
				for(fi=m.face.begin();fi!=m.face.end();++fi)
				{
					if(!((*fi).IsD()) && !(*fi).IsS())
					{
						(*fi).SetS();
						CCV.push_back(make_pair(0,&*fi));
						sf.push(&*fi);
						while (!sf.empty())
						{
							fpt=sf.top();
              ++CCV.back().first;
							sf.pop();
							for(int j=0;j<3;++j)
							{
								if( !face::IsBorder(*fpt,j) )
								{
									l=fpt->FFp(j);
									if( !(*l).IsS() )
									{
										(*l).SetS();
										sf.push(l);
									}
								}
							}
						}
						Compindex++;
					}
				}
        assert(CCV.size()==Compindex);
				return Compindex;
			}


			/**
			GENUS.
			
			A topologically invariant property of a surface defined as
			the largest number of non-intersecting simple closed curves that can be 
			drawn on the surface without separating it. 

      Roughly speaking, it is the number of holes in a surface. 
			The genus g of a closed surface, also called the geometric genus, is related to the 
			Euler characteristic by the relation $chi$ by $chi==2-2g$.
      
			The genus of a connected, orientable surface is an integer representing the maximum
			number of cuttings along closed simple curves without rendering the resultant 
			manifold disconnected. It is equal to the number of handles on it.

			For general polyhedra the <em>Euler Formula</em> is:

			      V + F - E = 2 - 2G - B

			where V is the number of vertices, F is the number of faces, E is the
			number of edges, G is the genus and B is the number of <em>boundary polygons</em>.

			The above formula is valid for a mesh with one single connected component. 
			By considering multiple connected components the formula becomes:

			      V + F - E = 2C - 2Gs - B

			where C is the number of connected components and Gs is the sum of
			the genus of all connected components.

			*/
			static int MeshGenus(MeshType &m, int numholes, int numcomponents, int count_e)
			{
				int V = m.vn;
				int F = m.fn;
				int E = count_e;
				return -((V + F - E + numholes - 2 * numcomponents) / 2);
			}

			/**
			 * Check if the given mesh is regular, semi-regular or irregular.
			 *
			 * Each vertex of a \em regular mesh has valence 6 except for border vertices
			 * which have valence 4.
			 *
			 * A \em semi-regular mesh is derived from an irregular one applying
			 * 1-to-4 subdivision recursively. (not checked for now)
			 *
			 * All other meshes are \em irregular.
			 */
			static void IsRegularMesh(MeshType &m, bool &Regular, bool &Semiregular)
			{
				// This algorithm requires Vertex-Face topology
				assert(m.HasVFTopology());

				Regular = true;
			
				VertexIterator vi;

				// for each vertex the number of edges are count
				for (vi = m.vert.begin(); vi != m.vert.end(); ++vi)
				{
					if (!vi->IsD())
					{
						face::Pos<FaceType> he((*vi).VFp(), &*vi);
						face::Pos<FaceType> ht = he;

						int n=0;
						bool border=false;
						do
						{
							++n;
							ht.NextE();
							if (ht.IsBorder()) 
								border=true;
						} 
						while (ht != he);

						if (border)
							n = n/2;

						if ((n != 6)&&(!border && n != 4))
						{
							Regular = false;
							break;
						}
					}
				}

				if (!Regular)
					Semiregular = false;
				else
				{
					// For now we do not account for semi-regularity
					Semiregular = false;
				}
			}

			static void IsOrientedMesh(MeshType &m, bool &Oriented, bool &Orientable)
			{
        assert(&Oriented != &Orientable);
				// This algorithms requires FF topology
				assert(m.HasFFTopology());

				Orientable = true;
				Oriented = true;

				// Ensure that each face is deselected
				FaceIterator fi;
				for (fi = m.face.begin(); fi != m.face.end(); ++fi)
					fi->ClearS();

				// initialize stack
        std::stack<FacePointer> faces;

				// for each face of the mesh
				FacePointer fp,fpaux;
				int iaux;
				for (fi = m.face.begin(); fi != m.face.end(); ++fi)
				{
					if (!fi->IsD() && !fi->IsS())
					{
						// each face put in the stack is selected (and oriented)
						fi->SetS();
						faces.push(&(*fi));

						// empty the stack
						while (!faces.empty())
						{
							fp = faces.top();
							faces.pop();
								
							// make consistently oriented the adjacent faces
							for (int j = 0; j < 3; j++)
							{
								// get one of the adjacent face
								fpaux = fp->FFp(j);
								iaux = fp->FFi(j);

								if (!fpaux->IsD() && fpaux != fp && face::IsManifold<FaceType>(*fp, j))
								{
									if (!CheckOrientation(*fpaux, iaux))
									{
                    Oriented = false;

										if (!fpaux->IsS())
										{
											face::SwapEdge<FaceType,true>(*fpaux, iaux);
											assert(CheckOrientation(*fpaux, iaux));
										}
										else
                    {
											Orientable = false;
                      break;
                    }
									}

									// put the oriented face into the stack

									if (!fpaux->IsS())
									{
										fpaux->SetS();
										faces.push(fpaux);
									}
								}
							}
						}
					}

					if (!Orientable)	break;
				}
			}
      /// Flip the orientation of the whole mesh flipping all the faces (by swapping the first two vertices)
			static void FlipMesh(MeshType &m)
      {
        for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi) if(!(*fi).IsD())
        {
			       face::SwapEdge<FaceType,false>((*fi), 0);
      			 if (HasPerWedgeTexture(m))
			        		swap((*fi).WT(0),(*fi).WT(1));
        }
      }

      static bool SelfIntersections(MeshType &m, std::vector<FaceType*> &ret)
			{
        //assert(FaceType::HasMark()); // Needed by the UG
        assert(HasPerFaceMark(m));// Needed by the UG
				Box3< ScalarType> bbox;
				TriMeshGrid   gM;
        ret.clear();
				FaceIterator fi;
	      int referredBit = FaceType::NewBitFlag();

				for(fi=m.face.begin();fi!=m.face.end();++fi)
					(*fi).ClearUserBit(referredBit);
				
				std::vector<FaceType*> inBox;
				gM.Set(m.face.begin(),m.face.end());
				
				for(fi=m.face.begin();fi!=m.face.end();++fi) if(!(*fi).IsD())
				{
          (*fi).SetUserBit(referredBit);
					(*fi).GetBBox(bbox);
					vcg::trimesh::GetInBoxFace(m, gM, bbox,inBox);
					bool Intersected=false;
          typename std::vector<FaceType*>::iterator fib;
          for(fib=inBox.begin();fib!=inBox.end();++fib)
          {
            if(!(*fib)->IsUserBit(referredBit) && (*fib != &*fi) )
              if(TestIntersection(&*fi,*fib)){
                ret.push_back(*fib);
                if(!Intersected) {
                  ret.push_back(&*fi);
                  Intersected=true;
                }
              }
          }
					inBox.clear();
				}	
				
        FaceType::DeleteBitFlag(referredBit);
				return (ret.size()>0);
			}

      /**
      This function simply test that the vn and fn counters be consistent with the size of the containers and the number of deleted simplexes.
      */
      static bool IsSizeConsistent(MeshType &m)
      {
        int DeletedVertexNum=0;
        for (VertexIterator vi = m.vert.begin(); vi != m.vert.end(); ++vi)
          if((*vi).IsD()) DeletedVertexNum++;

        int DeletedFaceNum=0;
        for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi)
          if((*fi).IsD()) DeletedFaceNum++;

        if(m.vn+DeletedVertexNum != m.vert.size()) return false;
        if(m.fn+DeletedFaceNum != m.face.size()) return false;

        return true;
      }

      /**
      This function simply test that all the faces have a consistent face-face topology relation.
      useful for checking that a topology modifying algorithm does not mess something.
      */
      static bool IsFFAdjacencyConsistent(MeshType &m)
      {
        if(!HasFFAdjacency(m)) return false;

        for (FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi)
          if(!(*fi).IsD()) 
          {
            for(int i=0;i<3;++i)
              if(!FFCorrectness(*fi, i)) return false;
          }
        return true;
      }

	//test real intersection between faces
static	bool TestIntersection(FaceType *f0,FaceType *f1)
	{
		assert((!f0->IsD())&&(!f1->IsD()));
		//no adiacent faces
		if ( (f0!=f1) && (!ShareEdge(f0,f1))
			&& (!ShareVertex(f0,f1)) )
			return (vcg::Intersection<FaceType>((*f0),(*f1)));
		return false;
	}

			//control if two faces share an edge
static	bool ShareEdge(FaceType *f0,FaceType *f1)
	{
		assert((!f0->IsD())&&(!f1->IsD()));
		for (int i=0;i<3;i++)
				if (f0->FFp(i)==f1)
					return (true);

		return(false);
	}

	//control if two faces share a vertex
static	bool ShareVertex(FaceType *f0,FaceType *f1)
	{
		assert((!f0->IsD())&&(!f1->IsD()));
		for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)
				if (f0->V(i)==f1->V(j))
					return (true);

		return(false);
	}


		}; // end class
		/*@}*/
	
	} //End Namespace Tri
} // End Namespace vcg
#endif
