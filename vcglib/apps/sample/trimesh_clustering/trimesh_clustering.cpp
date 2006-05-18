	
// mesh definition 
//#include <vcg/simplex/vertex/with/vn.h>
//#include <vcg/simplex/face/with/af.h>
//#include <vcg/complex/trimesh/base.h>

#include<vcg/simplex/vertexplus/base.h>
#include<vcg/simplex/faceplus/base.h>
#include<vcg/simplex/face/topology.h>

#include<vcg/complex/trimesh/base.h>

#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/update/topology.h>
#include <vcg/complex/trimesh/update/normal.h>
#include <vcg/complex/trimesh/update/flag.h>
#include <vcg/complex/trimesh/clustering.h>

// input output
#include <wrap/io_trimesh/import_ply.h>
#include <wrap/io_trimesh/export_ply.h>

// std
#include <vector>
#include <time.h>

using namespace vcg;
using namespace std;

class MyEdge;    // dummy prototype never used
class MyFace;
class MyVertex;

class MyVertex  : public VertexSimp2< MyVertex, MyEdge, MyFace, vert::Coord3f, vert::Normal3f, vert::BitFlags  >{};
class MyFace    : public FaceSimp2  < MyVertex, MyEdge, MyFace, face::VertexRef, face::Normal3f, face::BitFlags > {};
class MyMesh    : public vcg::tri::TriMesh< vector<MyVertex>, vector<MyFace> > {};

int  main(int argc, char **argv)
{
 if(argc<3)
	{
		printf(
		"\n                  trimesh_clustering ("__DATE__")\n"
			"						Visual Computing Group I.S.T.I. C.N.R.\n"
      "Usage: PlyRefine filein.ply fileout.ply [opt] \n"
      "options: \n"
      "-k cellnum     approx number of cluster that should be defined; (default 10e5)\n"
			"-s size        in absolute units the size of the clustering cell\n"
			);
		exit(0);
	}

  int i=3;  int CellNum=100000; float CellSize=0;

	while(i<argc)
		{
			if(argv[i][0]!='-')
				{printf("Error unable to parse option '%s'\n",argv[i]); exit(0);}
			switch(argv[i][1])
			{				
				case 'k' :	CellNum=atoi(argv[i+1]); ++i; printf("Using %i clustering cells\n",CellNum); break;
				case 's' :	CellSize=atof(argv[i+1]); ++i; printf("Using %5f as clustering cell size\n",CellSize); break;
				default : {printf("Error unable to parse option '%s'\n",argv[i]); exit(0);}
			}
			++i;
		}

	MyMesh m;

	if(vcg::tri::io::ImporterPLY<MyMesh>::Open(m,argv[1])!=0)
		{
      printf("Error reading file  %s\n",argv[1]);
			exit(0);
		}
  vcg::tri::UpdateBounding<MyMesh>::Box(m);
  //vcg::tri::UpdateNormals<MyMesh>::PerVertexNormalized(m);
  vcg::tri::UpdateNormals<MyMesh>::PerFace(m);
  printf("Input mesh  vn:%i fn:%i\n",m.vn,m.fn);
  vcg::tri::Clustering<MyMesh, vcg::tri::AverageCell<MyMesh> > Grid;
  Grid.Init(m.bbox,CellNum,CellSize);
  
  printf("Clustering to %i cells\n",Grid.Grid.siz[0]*Grid.Grid.siz[1]*Grid.Grid.siz[2] );
  printf("Grid of %i x %i x %i cells\n",Grid.Grid.siz[0],Grid.Grid.siz[1],Grid.Grid.siz[2]);
  printf("with cells size of %.2f x %.2f x %.2f units\n",Grid.Grid.voxel[0],Grid.Grid.voxel[1],Grid.Grid.voxel[2]);
  
  int t0=clock();
  Grid.Add(m);
  int t1=clock();
  Grid.Extract(m);
  int t2=clock();
  printf("Output mesh vn:%i fn:%i\n",m.vn,m.fn);
  printf("Simplified in :%i msec (%i+%i)\n",t2-t0,t1-t0,t2-t1);
	
  vcg::tri::io::PlyInfo pi;
	vcg::tri::io::ExporterPLY<MyMesh>::Save(m,argv[2],pi.mask);
	return 0;
	}
