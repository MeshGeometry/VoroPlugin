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
//#include "StockGeometries.h"
#include "TriMesh.h"

#include "batch.h"
#include "meshio.h"
#include "adjacency.h"
#include "meshstats.h"
#include "dedge.h"
#include "subdivide.h"
#include "batch.h"
#include "hierarchy.h"
#include "field.h"
#include "normal.h"
#include "extract.h"
#include "bvh.h"


#include "Geomlib_TriMeshSaveOFF.h"

using namespace Urho3D;

bool initialized = false;
Urho3D::SharedPtr<Urho3D::Context> context;
Engine* engine_;
Scene* scene_;

Urho3D::Variant MakeSphere()
{
	float t = (1 + (float)sqrt(5)) / 2;
	Eigen::MatrixXf V(12, 3);
	V <<
		t, 1, 0, // 0
		1, 0, t, // 1, A
		0, t, 1, // 2, B
		t, -1, 0, // 3, E
		-1, 0, t, // 4, DD <-> A
		0, t, -1, // 5, C
		-t, 1, 0, // 6, EE <-> B
		1, 0, -t, // 7, D
		0, -t, 1, // 8, CC <-> E
		-t, -1, 0, // 9
		-1, 0, -t, // 10, AA <-> C
		0, -t, -1; // 11, BB <-> D

	Eigen::MatrixXi F(20, 3);
	F <<
		1, 2, 4,
		2, 5, 6,
		5, 7, 10,
		7, 3, 11,
		3, 1, 8, //
		10, 7, 11,
		11, 3, 8,
		8, 1, 4,
		4, 2, 6,
		6, 5, 10, //
		0, 2, 1,
		0, 5, 2,
		0, 7, 5,
		0, 3, 7,
		0, 1, 3, //
		9, 10, 11,
		9, 11, 8,
		9, 8, 4,
		9, 4, 6,
		9, 6, 10;

	//Eigen::MatrixXf V2, NV;
	//Eigen::MatrixXi F2, NF;
	//subdivide_icosahedron(V, F, V2, F2);
	//subdivide_icosahedron(V2, F2, NV, NF);

	return TriMesh_Make(V, F);
}

void Initialize()
{
	context = new Urho3D::Context();
	context->RegisterSubsystem(new FileSystem(context));
	context->RegisterSubsystem(new ResourceCache(context));
	context->RegisterSubsystem(new Graphics(context));
	context->RegisterSubsystem(new IoGraph(context));
	engine_ = new Engine(context);
	scene_ = new Scene(context);

	initialized = true;
}

TEST(Basic, TriRemesh)
{
	
	if (!initialized)
		Initialize();

	Variant sphere = MakeSphere();


	//basic init
	//super important: posy must be 3 for tri meshes
	bool extrinsic = true, dominant = false, align_to_boundaries = false;
	bool fullscreen = false, help = false, deterministic = false, compat = false;
	int rosy = 6, posy = 3, face_count = -1, vertex_count = -1;
	uint32_t knn_points = 10, smooth_iter = 2;
	Float crease_angle = -1, scale = -1;

	//main file in
	std::string filename = "../../../Test/Sphere.obj";

	//log output
	std::string batchOutput = "../../../Test/remeshed.obj";


	int nprocs = -1;
	tbb::task_scheduler_init init(nprocs == -1 ? tbb::task_scheduler_init::automatic : nprocs);

	//main call
	try {
		batch_process(filename, batchOutput, rosy, posy, scale, face_count,
			vertex_count, crease_angle, extrinsic,
			align_to_boundaries, smooth_iter, knn_points,
			!dominant, deterministic);
		return;
	}
	catch (const std::exception &e) {
		cerr << "Caught runtime error : " << e.what() << endl;
		return;
	}

}

TEST(Basic, FromVariant)
{
	if (!initialized)
		Initialize();

	Variant sphere = MakeSphere();



	//basic init
	//super important: posy must be 3 for tri meshes
	bool extrinsic = true, pure_quad = false, align_to_boundaries = false;
	bool fullscreen = false, help = false, deterministic = false, compat = false;
	int rosy = 6, posy = 3, face_count = -1, vertex_count = -1;
	uint32_t knn_points = 10, smooth_iter = 2;
	Float creaseAngle = -1, scale = -1;


	Vector<int> faces = TriMesh_GetFacesAsInts(sphere);
	Vector<float> verts = TriMesh_GetVerticesAsFloats(sphere);


	////////GET MESH FROM VARIANT/////////////////
	MatrixXu F;
	MatrixXf V, N;
	VectorXf A;
	std::set<uint32_t> crease_in, crease_out;
	BVH *bvh = nullptr;
	AdjacencyMatrix adj = nullptr;

	F.resize(3, faces.Size() / 3);
	//memcpy(TF.data(), faces.Buffer(), sizeof(uint32_t)*faces.Size());
	for (int i = 0; i < faces.Size() / 3; ++i)
	{
		uint32_t a = faces[3 * i + 0];
		uint32_t b = faces[3 * i + 1];
		uint32_t c = faces[3 * i + 2];
		F(0,i) = a;
		F(1,i) = b;
		F(2,i) = c;
	}

	V.resize(3, verts.Size()/3);
	for (int i = 0; i < verts.Size() / 3; ++i)
	{
		float x = verts[3 * i + 0];
		float y = verts[3 * i + 1];
		float z = verts[3 * i + 2];
		V(0, i) = x;
		V(1, i) = y;
		V(2, i) = z;
	}

	//////////////////////////////////////

	bool pointcloud = F.size() == 0;

	IM_Timer<> timer;
	MeshStats stats = compute_mesh_stats(F, V, deterministic);

	if (pointcloud) {
		bvh = new BVH(&F, &V, &N, stats.mAABB);
		bvh->build();
		adj = generate_adjacency_matrix_pointcloud(V, N, bvh, stats, knn_points, deterministic);
		A.resize(V.cols());
		A.setConstant(1.0f);
	}

	if (scale < 0 && vertex_count < 0 && face_count < 0) {
		cout << "No target vertex count/face count/scale argument provided. "
			"Setting to the default of 1/16 * input vertex count." << endl;
		vertex_count = V.cols() / 16;
	}

	if (scale > 0) {
		Float face_area = posy == 4 ? (scale*scale) : (std::sqrt(3.f) / 4.f*scale*scale);
		face_count = stats.mSurfaceArea / face_area;
		vertex_count = posy == 4 ? face_count : (face_count / 2);
	}
	else if (face_count > 0) {
		Float face_area = stats.mSurfaceArea / face_count;
		vertex_count = posy == 4 ? face_count : (face_count / 2);
		scale = posy == 4 ? std::sqrt(face_area) : (2 * std::sqrt(face_area * std::sqrt(1.f / 3.f)));
	}
	else if (vertex_count > 0) {
		face_count = posy == 4 ? vertex_count : (vertex_count * 2);
		Float face_area = stats.mSurfaceArea / face_count;
		scale = posy == 4 ? std::sqrt(face_area) : (2 * std::sqrt(face_area * std::sqrt(1.f / 3.f)));
	}

	cout << "Output mesh goals (approximate)" << endl;
	cout << "   Vertex count           = " << vertex_count << endl;
	cout << "   Face count             = " << face_count << endl;
	cout << "   Edge length            = " << scale << endl;

	MultiResolutionHierarchy mRes;

	if (!pointcloud) {
		/* Subdivide the mesh if necessary */
		VectorXu V2E, E2E;
		VectorXb boundary, nonManifold;
		if (stats.mMaximumEdgeLength * 2 > scale || stats.mMaximumEdgeLength > stats.mAverageEdgeLength * 2) {
			cout << "Input mesh is too coarse for the desired output edge length "
				"(max input mesh edge length=" << stats.mMaximumEdgeLength
				<< "), subdividing .." << endl;
			build_dedge(F, V, V2E, E2E, boundary, nonManifold);
			subdivide(F, V, V2E, E2E, boundary, nonManifold, std::min(scale / 2, (Float)stats.mAverageEdgeLength * 2), deterministic);
		}

		/* Compute a directed edge data structure */
		build_dedge(F, V, V2E, E2E, boundary, nonManifold);

		/* Compute adjacency matrix */
		adj = generate_adjacency_matrix_uniform(F, V2E, E2E, nonManifold);

		/* Compute vertex/crease normals */
		if (creaseAngle >= 0)
			generate_crease_normals(F, V, V2E, E2E, boundary, nonManifold, creaseAngle, N, crease_in);
		else
			generate_smooth_normals(F, V, V2E, E2E, nonManifold, N);

		/* Compute dual vertex areas */
		compute_dual_vertex_areas(F, V, V2E, E2E, nonManifold, A);

		mRes.setE2E(std::move(E2E));
	}

	/* Build multi-resolution hierarrchy */
	mRes.setAdj(std::move(adj));
	mRes.setF(std::move(F));
	mRes.setV(std::move(V));
	mRes.setA(std::move(A));
	mRes.setN(std::move(N));
	mRes.setScale(scale);
	mRes.build(deterministic);
	mRes.resetSolution();

	if (align_to_boundaries && !pointcloud) {
		mRes.clearConstraints();
		for (uint32_t i = 0; i<3 * mRes.F().cols(); ++i) {
			if (mRes.E2E()[i] == INVALID) {
				uint32_t i0 = mRes.F()(i % 3, i / 3);
				uint32_t i1 = mRes.F()((i + 1) % 3, i / 3);
				Vector3f p0 = mRes.V().col(i0), p1 = mRes.V().col(i1);
				Vector3f edge = p1 - p0;
				if (edge.squaredNorm() > 0) {
					edge.normalize();
					mRes.CO().col(i0) = p0;
					mRes.CO().col(i1) = p1;
					mRes.CQ().col(i0) = mRes.CQ().col(i1) = edge;
					mRes.CQw()[i0] = mRes.CQw()[i1] = mRes.COw()[i0] =
						mRes.COw()[i1] = 1.0f;
				}
			}
		}
		mRes.propagateConstraints(rosy, posy);
	}

	if (bvh) {
		bvh->setData(&mRes.F(), &mRes.V(), &mRes.N());
	}
	else if (smooth_iter > 0) {
		bvh = new BVH(&mRes.F(), &mRes.V(), &mRes.N(), stats.mAABB);
		bvh->build();
	}

	cout << "Preprocessing is done. (total time excluding file I/O: "
		<< timeString(timer.reset()) << ")" << endl;

	Optimizer optimizer(mRes, false);
	optimizer.setRoSy(rosy);
	optimizer.setPoSy(posy);
	optimizer.setExtrinsic(extrinsic);

	cout << "Optimizing orientation field .. ";
	cout.flush();
	optimizer.optimizeOrientations(-1);
	optimizer.notify();
	optimizer.wait();
	cout << "done. (took " << timeString(timer.reset()) << ")" << endl;

	std::map<uint32_t, uint32_t> sing;
	compute_orientation_singularities(mRes, sing, extrinsic, rosy);
	cout << "Orientation field has " << sing.size() << " singularities." << endl;
	timer.reset();

	cout << "Optimizing position field .. ";
	cout.flush();
	optimizer.optimizePositions(-1);
	optimizer.notify();
	optimizer.wait();
	cout << "done. (took " << timeString(timer.reset()) << ")" << endl;

	//std::map<uint32_t, Vector2i> pos_sing;
	//compute_position_singularities(mRes, sing, pos_sing, extrinsic, rosy, posy);
	//cout << "Position field has " << pos_sing.size() << " singularities." << endl;
	//timer.reset();

	optimizer.shutdown();

	MatrixXf O_extr, N_extr, Nf_extr;
	std::vector<std::vector<TaggedLink>> adj_extr;
	extract_graph(mRes, extrinsic, rosy, posy, adj_extr, O_extr, N_extr,
		crease_in, crease_out, deterministic);

	MatrixXu F_extr;
	extract_faces(adj_extr, O_extr, N_extr, Nf_extr, F_extr, posy,
		mRes.scale(), crease_out, true, pure_quad, bvh, smooth_iter);
	cout << "Extraction is done. (total time: " << timeString(timer.reset()) << ")" << endl;

	
	////////////TEST///////////////////////
	//write_mesh(batchOutput, F_extr, O_extr, MatrixXf(), Nf_extr);
	/////////////////////////////////////////

	//translate back to variant
	VariantVector newVerts;
	VariantVector newFaces;

	for (int i = 0; i < F_extr.cols(); i++)
	{
		newFaces.Push(F_extr(0, i));
		newFaces.Push(F_extr(1, i));
		newFaces.Push(F_extr(2, i));
	}

	for (int i = 0; i < O_extr.cols(); i++)
	{
		Vector3 currVert(
				O_extr(0, i),
				O_extr(1, i),
				O_extr(2, i)
			);
		
		newVerts.Push(currVert);
	}

	Variant newMesh = TriMesh_Make(newVerts, newFaces);

	//test it
	File* f = new File(context, "../../../Test/remeshed_test.off", FileMode::FILE_WRITE);
	Geomlib::TriMeshWriteOFF(newMesh, *f);
	f->Close();

	if (bvh)
		delete bvh;

}

