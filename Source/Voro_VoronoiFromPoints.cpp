#include "Voro_VoronoiFromPoints.h"
#include "container.hh"
#include "VoroToIogram.h"

namespace {
	double rnd() { return double(rand()) / RAND_MAX; }
}

String Voro_VoronoiFromPoints::iconTexture = "";

Voro_VoronoiFromPoints::Voro_VoronoiFromPoints(Context* context) :IoComponentBase(context, 0, 0)
{
	SetName("VoronoiFromPoints");
	SetFullName("Voronoi From Points");
	SetDescription("Voronoi diagram from points and a container");

	//AddInputSlot(
	//	"Points",
	//	"P",
	//	"Points",
	//	VAR_VECTOR3,
	//	DataAccess::LIST
	//	);

	AddInputSlot(
		"Input Points",
		"P",
		"Points",
		VAR_VARIANTVECTOR,
		DataAccess::LIST
	);

	AddInputSlot(
		"BoxDim",
		"D",
		"Dimension of Box",
		VAR_FLOAT,
		DataAccess::ITEM,
		2.0f
	);

	AddOutputSlot(
		"MeshOut",
		"R",
		"Mesh Out",
		VAR_VARIANTMAP,
		DataAccess::LIST
	);

}

void Voro_VoronoiFromPoints::SolveInstance(
	const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
	Urho3D::Vector<Urho3D::Variant>& outSolveInstance
)
{
	////////////////////////////////////////////////////////////
	
	VariantVector L = inSolveInstance[0].GetVariantVector();
	// make sure it is a list of points!!
	for (int i = 0; i < L.Size(); i++)
	{
		if (L[i].GetType() != VariantType::VAR_VECTOR3)
		{
			URHO3D_LOGERROR("Inputs must be Vector3's!");
			return;
		}
	}

	float dim = inSolveInstance[1].GetFloat();

	//for now just hard code a 2X2X2 box
	// Set up constants for the container geometry
	const double x_min = 0, x_max = dim;
	const double y_min = 0, y_max = dim;
	const double z_min = 0, z_max = dim;
	const double cvol = (x_max - x_min)*(y_max - y_min)*(x_max - x_min);

	// Set up the number of blocks that the container is divided into
	// don't know what this is about
	const int n_x = 6, n_y = 6, n_z = 6;

	int i;

	//Create a container with the geometry given above, and make it
	//non-periodic in each of the three coordinates. Allocate space for
	//eight particles within each computational block
	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, n_x, n_y, n_z,
		false, false, false, 8);



	// Add the points into the container
	// what happens if they are outside???!!!
	for (i = 0; i < L.Size(); i++) {
		Vector3 p = L[i].GetVector3();
		con.put(i, p.x_, p.y_, p.z_);
	}

	//now output some meshes
	VariantVector cell_meshes = GetMeshesFromContainer(con);

	//con.draw_cells_gnuplot("random_points_v.gnu");
	//con.draw_cells_pov("random_points.pov");

	Variant out_var(cell_meshes);

	outSolveInstance[0] = out_var;
	int test = 0;


}