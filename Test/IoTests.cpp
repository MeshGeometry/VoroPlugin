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

#include "IoGraph.h"

using namespace Urho3D;

bool initialized = false;
Context* context_;
Engine* engine_;
Scene* scene_;


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



