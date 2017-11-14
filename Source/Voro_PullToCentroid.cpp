#include "Voro_PullToCentroid.h"
#include "container.hh"
#include "VoroToIogram.h"
#include "centroid.h"
#include "TriMesh.h"



namespace {
	double rnd() { return double(rand()) / RAND_MAX; }
}

String Voro_PullToCentroid::iconTexture = "";

Voro_PullToCentroid::Voro_PullToCentroid(Context* context) :IoComponentBase(context, 0, 0)
{
	SetName("OffsetToCentroid");
	SetFullName("Offsets a mesh toward the centroid");
	SetDescription("Offsets a mesh toward the centroid");

	//AddInputSlot(
	//	"Points",
	//	"P",
	//	"Points",
	//	VAR_VECTOR3,
	//	DataAccess::LIST
	//	);

	AddInputSlot(
		"Mesh",
		"M",
		"Mesh To Offset",
		VAR_VARIANTMAP,
		DataAccess::ITEM
	);

	AddInputSlot(
		"Dist",
		"D",
		"OffsetAmount",
		VAR_FLOAT,
		DataAccess::ITEM,
		-0.1f
	);

	AddOutputSlot(
		"MeshOut",
		"R",
		"Mesh Out",
		VAR_VARIANTMAP,
		DataAccess::ITEM
	);

}

void Voro_PullToCentroid::SolveInstance(
	const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
	Urho3D::Vector<Urho3D::Variant>& outSolveInstance
)
{
	///////////////////
	// VERIFY & EXTRACT

	if (!IsAllInputValid(inSolveInstance)) {
		SetAllOutputsNull(outSolveInstance);
	}

	// Still need to verify input slot 0
	Variant meshIn = inSolveInstance[0];
	if (!TriMesh_Verify(meshIn)) {
		URHO3D_LOGWARNING("M must be a valid TriMesh.");
		outSolveInstance[0] = Variant();
		return;
	}
	float dist = inSolveInstance[1].GetFloat();

	// get the centroid
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	TriMeshToDoubleMatrices(meshIn, V, F);

	Eigen::Vector3d c;
	igl::centroid(V, F, c);
	Vector3 cen(c.x(), c.y(), c.z());

	// iterate over the verts to find vector connecting the vert to the centroid
	VariantVector verts = TriMesh_GetVertexList(meshIn);
	VariantVector faces = TriMesh_GetFaceList(meshIn);
	VariantVector offVerts; 
	for (int i = 0; i < verts.Size(); ++i) {
		Vector3 disp = dist*(verts[i].GetVector3() - cen);
		offVerts.Push(Variant(verts[i].GetVector3() - disp));
	}

	Variant out_mesh = TriMesh_Make(offVerts, faces);

	outSolveInstance[0] = out_mesh;


}