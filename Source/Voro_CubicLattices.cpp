//
// Copyright (c) 2016 - 2017 Mesh Consultants Inc.
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//


#include "Voro_CubicLattices.h"

#include <assert.h>

#include <Urho3D/Core/Variant.h>

using namespace Urho3D;

namespace {
	VariantVector ComputeFaceCenteredCubicPoints(double sz, int n) {
		Vector3 origin = Vector3::ZERO;
		Vector3 x_basis = Vector3(sz, 0.0f, 0.0f);
		Vector3 y_basis = Vector3(0.0f,sz, 0.0f);
		Vector3 z_basis = Vector3(0.0f, 0.0f, sz);
		Vector3 xz_center = origin + Vector3(sz / 2, 0, sz / 2);
		Vector3 xy_center = origin + Vector3(sz / 2, sz / 2, 0);
		Vector3 yz_center = origin + Vector3(0, sz / 2, sz / 2);

		VariantVector fc_pts;

		// X-Z
		for (int x = 0; x < n; ++x) {
			for (int z = 0; z < n; ++z) {
				for (int y = 0; y < n + 1; ++y) {
					fc_pts.Push(Variant(xz_center + x*x_basis + y*y_basis + z*z_basis));
				}
			}
		}

		// X-Y
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < n; ++y) {
				for (int z = 0; z < n + 1; ++z) {
					fc_pts.Push(Variant(xy_center + x*x_basis + y*y_basis + z*z_basis));
				}
			}
		}

		// Y-Z
		for (int y = 0; y < n; ++y) {
			for (int z = 0; z < n; ++z) {
				for (int x = 0; x < n + 1; ++x) {
					fc_pts.Push(Variant(yz_center + x*x_basis + y*y_basis + z*z_basis));
				}
			}
		}

		return fc_pts;
	}
}

String Voro_CubicLattices::iconTexture = "";

Voro_CubicLattices::Voro_CubicLattices(Urho3D::Context* context) : IoComponentBase(context, 3, 3)
{
	SetName("CubicLattice");
	SetFullName("Cubic Lattice Generator");
	SetDescription("Generates a 3D rectangular array of cells from transform, number of cells and cell size");
	SetGroup(IoComponentGroup::MATHS);
	SetSubgroup("Operators");

	inputSlots_[0]->SetName("transform");
	inputSlots_[0]->SetVariableName("T");
	inputSlots_[0]->SetDescription("Transform for coordinate system");
	inputSlots_[0]->SetVariantType(VariantType::VAR_MATRIX3X4);
	inputSlots_[0]->SetDefaultValue(Matrix3x4::IDENTITY);
	inputSlots_[0]->DefaultSet();

	inputSlots_[1]->SetName("numberX");
	inputSlots_[1]->SetVariableName("NX");
	inputSlots_[1]->SetDescription("Number of cells in each direction");
	inputSlots_[1]->SetVariantType(VariantType::VAR_INT);
	inputSlots_[1]->SetDefaultValue(10);
	inputSlots_[1]->DefaultSet();

	inputSlots_[2]->SetName("sizeX");
	inputSlots_[2]->SetVariableName("x");
	inputSlots_[2]->SetDescription("Size of cells in each direction");
	inputSlots_[2]->SetVariantType(VariantType::VAR_FLOAT);
	inputSlots_[2]->SetDefaultValue(1.0f);
	inputSlots_[2]->DefaultSet();

	outputSlots_[0]->SetName("Primitive Cubic");
	outputSlots_[0]->SetVariableName("P");
	outputSlots_[0]->SetDescription("Vertices");
	outputSlots_[0]->SetVariantType(VariantType::VAR_VECTOR3);
	outputSlots_[0]->SetDataAccess(DataAccess::LIST);

	outputSlots_[1]->SetName("Body-centered Cubic");
	outputSlots_[1]->SetVariableName("B");
	outputSlots_[1]->SetDescription("Vertices");
	outputSlots_[1]->SetVariantType(VariantType::VAR_VECTOR3);
	outputSlots_[1]->SetDataAccess(DataAccess::LIST);

	outputSlots_[2]->SetName("Face-centered Cubic");
	outputSlots_[2]->SetVariableName("F");
	outputSlots_[2]->SetDescription("Vertices");
	outputSlots_[2]->SetVariantType(VariantType::VAR_VECTOR3);
	outputSlots_[2]->SetDataAccess(DataAccess::LIST);


}

void Voro_CubicLattices::SolveInstance(
	const Vector<Variant>& inSolveInstance,
	Vector<Variant>& outSolveInstance
	)
{
	assert(inSolveInstance.Size() == inputSlots_.Size());

	//check that we have the right kind of data
	bool res = false;
	if (inSolveInstance[0].GetType() == VAR_MATRIX3X4 &&
		(inSolveInstance[1].GetType() == VAR_INT || inSolveInstance[1].GetType() == VAR_FLOAT) &&
		(inSolveInstance[2].GetType() == VAR_INT || inSolveInstance[2].GetType() == VAR_FLOAT)  )
	{
		res = true;
	}

	if (res)
	{

		// get the rectangular grid as in plane component
		Matrix3x4 trans = inSolveInstance[0].GetMatrix3x4();
		int x_numb = inSolveInstance[1].GetInt();
		float x_size = inSolveInstance[2].GetFloat();


		// Set up x-, y-, z- array "basis" vectors, incorporating transform information
		Vector3 origin = Vector3::ZERO;
		Vector3 x_basis = Vector3(x_size, 0.0f, 0.0f);
		Vector3 y_basis = Vector3(0.0f, x_size, 0.0f);
		Vector3 z_basis = Vector3(0.0f, 0.0f, x_size);

		Vector3 cell_center = origin + Vector3(x_size / 2, x_size / 2, x_size / 2);

		VariantVector gridList, bodyList, faceList;
		VariantVector arrayList;
		VariantVector arrayList_transformed, bodyList_transformed, faceList_transformed;

		// Set up basic array
		for (int i = 0; i < x_numb+1; ++i) {
			for (int j = 0; j < x_numb+1; ++j) {
				for (int k = 0; k < x_numb+1; ++k) {
					arrayList.Push(Variant(origin + i*x_basis + j*y_basis + k*z_basis));
				}
			}
		}

		// for body centered, add point in center of each cell. 
		for (int i = 0; i < x_numb; ++i) {
			for (int j = 0; j < x_numb; ++j) {
				for (int k = 0; k < x_numb; ++k) {
					bodyList.Push(Variant(cell_center + i*x_basis + j*y_basis + k*z_basis));
				}
			}
		}

		// for face-centered, add point in center of each face
		faceList = ComputeFaceCenteredCubicPoints(x_size, x_numb);

		// now transform everything
		// transform the coordinates of the cell corners
		for (int i = 0; i < arrayList.Size(); ++i)
		{
			Vector3 curCoord = arrayList[i].GetVector3();
			Vector3 newCoord = trans*curCoord;
			arrayList_transformed.Push(Variant(newCoord));
		}

		bodyList_transformed = arrayList_transformed;
		// add the body-centered points
		for (int i = 0; i < bodyList.Size(); ++i)
		{
			Vector3 curCoord = bodyList[i].GetVector3();
			Vector3 newCoord = trans*curCoord;
			bodyList_transformed.Push(Variant(newCoord));
		}

		faceList_transformed = arrayList_transformed;
		// add the face-centered points
		for (int i = 0; i < faceList.Size(); ++i)
		{
			Vector3 curCoord = faceList[i].GetVector3();
			Vector3 newCoord = trans*curCoord;
			faceList_transformed.Push(Variant(newCoord));
		}

		outSolveInstance[0] = arrayList_transformed;
		outSolveInstance[1] = bodyList_transformed;
		outSolveInstance[2] = faceList_transformed;


	}
	else
	{ // !res
		URHO3D_LOGWARNING("Bad inputs! Review input types.");
		outSolveInstance[0] = Variant();
	}

}
