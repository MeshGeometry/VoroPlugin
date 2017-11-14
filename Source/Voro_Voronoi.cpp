#include "Voro_Voronoi.h"
#include "container.hh"
#include "VoroToIogram.h"

namespace {
	double rnd() { return double(rand()) / RAND_MAX; }
}

String Voro_Voronoi::iconTexture = "";

Voro_Voronoi::Voro_Voronoi(Context* context) :IoComponentBase(context, 0, 0)
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
		"Number",
		"N",
		"Number of random Points",
		VAR_INT,
		DataAccess::ITEM
	);

	AddInputSlot(
		"Box",
		"M",
		"Mesh",
		VAR_VARIANTMAP,
		DataAccess::ITEM
	);

	AddOutputSlot(
		"MeshOut",
		"R",
		"Mesh Out",
		VAR_VARIANTMAP,
		DataAccess::LIST
		);

}

void Voro_Voronoi::SolveInstance(
	const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
	Urho3D::Vector<Urho3D::Variant>& outSolveInstance
	)
{
	////////////////////////////////////////////////////////////
	// Verify input slot 0
	VariantType type0 = inSolveInstance[0].GetType();
	if (!(type0 == VariantType::VAR_FLOAT || type0 == VariantType::VAR_INT)) {
		URHO3D_LOGWARNING("Min must be a valid float or integer.");
		outSolveInstance[0] = Variant();
		return;
	}
	int p = inSolveInstance[0].GetInt();



	//for now just hard code a 2X2X2 box
	// Set up constants for the container geometry
	const double x_min = -1, x_max = 1;
	const double y_min = -1, y_max = 1;
	const double z_min = -1, z_max = 1;
	const double cvol = (x_max - x_min)*(y_max - y_min)*(x_max - x_min);
	
	// Set up the number of blocks that the container is divided into
	const int n_x = 6, n_y = 6, n_z = 6;

	int i;
	double x, y, z;
	int particles = p;
	
	 //Create a container with the geometry given above, and make it
	 //non-periodic in each of the three coordinates. Allocate space for
	 //eight particles within each computational block
	voro::container con(x_min, x_max, y_min, y_max, z_min, z_max, n_x, n_y, n_z,
	                       false, false, false, 8);

	
	
	// Randomly add particles into the container
	for (i = 0; i < particles; i++) {
	               x = x_min + rnd()*(x_max - x_min);
	               y = y_min + rnd()*(y_max - y_min);
	               z = z_min + rnd()*(z_max - z_min);
	               con.put(i, x, y, z);
	
	}

	//now output some meshes
	VariantVector cell_meshes = GetMeshesFromContainer(con);

	//con.draw_cells_gnuplot("random_points_v.gnu");
	//con.draw_cells_pov("random_points.pov");

	Variant out_var(cell_meshes);

	outSolveInstance[0] = out_var;
	int test = 0;

	
}