#include "PluginRegistration.h"
#include <Urho3D/Core/Context.h>
#include "Voro_Voronoi.h"
#include "Voro_PullToCentroid.h"
#include "ComponentRegistration.h"

using namespace Urho3D;

extern "C"
{

	DLLEXPORT void IogramPluginMain(Urho3D::Context* context)
	{
		RegisterIogramType<Voro_Voronoi>(context);
		RegisterIogramType<Voro_PullToCentroid>(context);
	}

}
