#include "Voro_FrameNmesh.h"
#include "NMesh.h"
#include "TriMesh.h"
#include <iostream>


String Voro_FrameNmesh::iconTexture = "";

Voro_FrameNmesh::Voro_FrameNmesh(Context* context) :IoComponentBase(context, 0, 0)
{
    SetName("OffsetToCentroid");
    SetFullName("Offsets a mesh toward the centroid");
    SetDescription("Offsets a mesh toward the centroid");
    
    
    AddInputSlot(
                 "NMesh",
                 "M",
                 "Mesh To Frame",
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

void Voro_FrameNmesh::SolveInstance(
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
    if (!NMesh_Verify(meshIn)) {
        URHO3D_LOGWARNING("M must be a valid NMesh.");
        outSolveInstance[0] = Variant();
        return;
    }
    float dist = inSolveInstance[1].GetFloat();
    
    // for each face, compute the centroid.
    // use this to define new verts.
    // construct the frame mesh
    VariantVector verts = NMesh_GetVertexList(meshIn);
    VariantVector faces = NMesh_GetFaceList(meshIn);
    
    VariantVector frame_verts = verts;
    VariantVector frame_faces;
    
    for (int i = 0; i < faces.Size(); ++i){
        
        VariantMap face = faces[i].GetVariantMap();
        VariantVector face_ids = face["face_vertices"].GetVariantVector();
        
        int N = face_ids.Size();
        
        std::cout << "Numb verts in this face: " << N << std::endl;
        
        // find the centroid
        Vector3 cen = Vector3::ZERO;
        for (int j = 0; j < N; ++j){
            int vert_id = face_ids[j].GetInt();
            cen += verts[vert_id].GetVector3();
        }
        cen = cen*(1.0f/(float)N);

        // create new vertices
        for (int j = 0; j < N; ++j){
            int vert_id = face_ids[j].GetInt();
            Vector3 oldVert = verts[vert_id].GetVector3();
            Vector3 newVert = oldVert + dist*(cen - oldVert);
            frame_verts.Push(Variant(newVert));
        }

        // create new faces
        for (int j = 0; j < N; ++j){
            int A_0 = face_ids[j].GetInt();
            int A_1 = face_ids[(j+1)%N].GetInt();
            int B_0 = frame_verts.Size() - (N - j);
            int B_1 = frame_verts.Size() - (N - (j+1)%N);

            frame_faces.Push(Variant(A_0));
            frame_faces.Push(Variant(A_1));
            frame_faces.Push(Variant(B_0));

            frame_faces.Push(Variant(A_1));
            frame_faces.Push(Variant(B_1));
            frame_faces.Push(Variant(B_0));


        }
        
      
    }
    
    Variant out_mesh = TriMesh_Make(frame_verts, frame_faces);
    
    outSolveInstance[0] = out_mesh;
    
    
}

