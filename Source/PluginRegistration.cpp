#include "PluginRegistration.h"
#include <Urho3D/Core/Context.h>
#include "Mesh_PluginDemo.h"
#include "ComponentRegistration.h"

using namespace Urho3D;

extern "C"
{

	DLLEXPORT void IogramPluginMain(Urho3D::Context* context)
	{
		RegisterIogramType<Mesh_PluginDemo>(context);

	}

}
