#include "Mesh_PluginDemo.h"

String Mesh_PluginDemo::iconTexture = "";

Mesh_PluginDemo::Mesh_PluginDemo(Context* context) :IoComponentBase(context, 0, 0)
{
	SetName("PluginDemo");
	SetFullName("Plugin Demo");
	SetDescription("Plugin Demo");

	AddInputSlot(
		"Mesh",
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
		DataAccess::ITEM
		);

}

void Mesh_PluginDemo::SolveInstance(
	const Urho3D::Vector<Urho3D::Variant>& inSolveInstance,
	Urho3D::Vector<Urho3D::Variant>& outSolveInstance
	)
{
	//insert your own logic here.
	//check out the other components for examples.
	outSolveInstance = inSolveInstance;
}