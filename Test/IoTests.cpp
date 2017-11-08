#include <stdio.h>
#include "gtest/gtest.h"

#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Graphics.h>

#include "VoroToIogram.h"
#include "voro++.hh"

#include "IoGraph.h"

using namespace Urho3D;

bool initialized = false;
Context* context_;
Engine* engine_;
Scene* scene_;

double rnd() { return double(rand()) / RAND_MAX; }


void Initialize()
{
	context_ = new Urho3D::Context();
	context_->RegisterSubsystem(new FileSystem(context_));
	context_->RegisterSubsystem(new ResourceCache(context_));
	context_->RegisterSubsystem(new Graphics(context_));
	context_->RegisterSubsystem(new IoGraph(context_));
	engine_ = new Engine(context_);
	scene_ = new Scene(context_);

	initialized = true;
}

TEST(Basic, FirstTest)
{
	if(!initialized)
		Initialize();
}

TEST(Basic, CellsToMeshes)
{
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
	int particles = 20;

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
	//VariantVector cell_meshes = GetMeshesFromContainer(con);
}


