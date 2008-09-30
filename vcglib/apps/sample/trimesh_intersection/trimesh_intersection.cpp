#include <vector>

using namespace std;

// VCG headers for triangular mesh processing
#include<vcg/simplex/edgeplus/base.h>
#include<vcg/simplex/edgeplus/component.h>
#include<vcg/simplex/vertexplus/base.h>
#include<vcg/simplex/faceplus/base.h>
#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/update/topology.h>
#include <vcg/complex/trimesh/update/edges.h>
#include <vcg/complex/trimesh/update/bounding.h>
#include <vcg/complex/trimesh/update/flag.h>
#include <vcg/complex/trimesh/clean.h>
#include <vcg/complex/intersection.h>
#include <vcg/space/index/grid_static_ptr.h>
//#include <vcg/simplex/edge/with/ae.h>
#include <vcg/complex/edgemesh/base.h>
#include <vcg/complex/edgemesh/allocate.h>
#include <vcg/complex/edgemesh/update/bounding.h>

// VCG File Format Importer/Exporter
#include <wrap/io_trimesh/import.h>
#include <wrap/io_edgemesh/export_svg.h>
#include <wrap/io_edgemesh/export_dxf.h>

// VCG Vertex

// VCG Faces

using namespace vcg;

class MyFace;
class MyEdge;
class MyVertex  : public VertexSimp2< MyVertex, MyEdge, MyFace, vert::Coord3f, vert::BitFlags, vert::Normal3f, vert::Mark>{};
class MyEdge    : public EdgeSimp2< MyVertex,MyEdge,  MyFace, edge::VertexRef, edge::EVAdj> {};
class MyFace    : public FaceSimp2  < MyVertex, MyEdge, MyFace, face::VertexRef,face::FFAdj, face::BitFlags, face::Normal3f> {};

class MyEdgeMesh: public vcg::edg::EdgeMesh< vector<MyVertex>, vector<MyEdge> > {};
class MyMesh : public tri::TriMesh< vector<MyVertex>, vector<MyFace > >{};


typedef vcg::GridStaticPtr<MyMesh::FaceType, MyMesh::ScalarType> TriMeshGrid;

int main(int argc,char ** argv)
{
	if (argc<6) 
	{
		printf("\n");
		printf("    Usage: trimesh_intersection <filename> <a> <b> <c> <d>\n\n");
		printf("       <filename>        Mesh model to intersect (PLY format).\n");
		printf("       <a> <b> <c> <d>   The coefficients that specifying a plane in the form:\n\n");
		printf("                             a*x + b*y + c*z + d = 0\n\n\n");

		printf("       Example: trimesh_intersection bunny.ply 0.0 1.0 0.0 0.0\n\n");

		return 0;
	}

	MyMesh m;

	// open a mesh
	int err = tri::io::Importer<MyMesh>::Open(m,argv[1]);
	if(err) 
	{
		printf("Error in reading %s: '%s'\n",argv[1],tri::io::Importer<MyMesh>::ErrorMsg(err));
		exit(-1);  
	}

	// some cleaning to get rid of bad file formats like stl that duplicate vertexes..
	int dup = tri::Clean<MyMesh>::RemoveDuplicateVertex(m);
	int unref =  tri::Clean<MyMesh>::RemoveUnreferencedVertex(m);

	if (dup > 0 || unref > 0)
		printf("Removed %i duplicate and %i unreferenced vertices from mesh %s\n",dup,unref,argv[1]);

	printf("");

	// Compute cross-intersection with the given plane
	/////////////////////////////////////////////////////////

	MyMesh::ScalarType a = static_cast<MyMesh::ScalarType>(atof(argv[2]));
	MyMesh::ScalarType b = static_cast<MyMesh::ScalarType>(atof(argv[3]));
	MyMesh::ScalarType c = static_cast<MyMesh::ScalarType>(atof(argv[4]));
	MyMesh::ScalarType d = static_cast<MyMesh::ScalarType>(atof(argv[5]));

	vcg::Point3<MyMesh::ScalarType> direction(a, b, c);
	MyMesh::ScalarType distance = -d / direction.Norm();
	direction.Normalize();

	vcg::Plane3<MyMesh::ScalarType> plane(distance, direction);

	double avg_length;     // average length of the edges
	MyEdgeMesh edge_mesh;  // returned EdgeMesh (i.e. the cross-section)

	// Create a static grid (for fast indexing) and fill it
	TriMeshGrid static_grid;
	static_grid.Set(m.face.begin(), m.face.end());
	std::vector<TriMeshGrid::Cell *> intersected_cells;

	vcg::Intersection<MyMesh, MyEdgeMesh, MyMesh::ScalarType, TriMeshGrid>(plane, 
		edge_mesh, avg_length, &static_grid, intersected_cells);

	// Compute bounding box
	vcg::edg::UpdateBounding<MyEdgeMesh>::Box(edge_mesh);

	// export the cross-section
	vcg::edg::io::SVGProperties pr;
	pr.setScale(500/(float)edge_mesh.bbox.Diag());
	pr.setDimension(500,500);

	if (vcg::edg::io::ExporterSVG<MyEdgeMesh>::Save(&edge_mesh, "out.svg",pr))
		printf("    The cross-intersection has been successfully saved (OUT.SVG).\n");
	else
		printf("    The cross-intersection cannot be saved.\n");


	return 0;
}

