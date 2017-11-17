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
        int num_verts_in_cur_face = faces[i].GetInt();
        std::cout << "Numb verts in this face: " << num_verts_in_cur_face << std::endl;
        
        // find the centroid
        Vector3 cen = Vector3::ZERO;
        for (int j = 0; j < num_verts_in_cur_face; ++j){
            int i0 = i + j + 1;
            int vert_id = faces[i0].GetInt();
            cen += verts[vert_id].GetVector3();
        }
        cen = cen*(1.0f/(float)num_verts_in_cur_face);
        
        // create new vertices
        for (int j = 0; j < num_verts_in_cur_face; ++j){
            int i0 = i + j + 1;
            int vert_id = faces[i0].GetInt();
            Vector3 oldVert = verts[vert_id].GetVector3();
            Vector3 newVert = oldVert + dist*(cen - oldVert);
            frame_verts.Push(Variant(newVert));
        }
            
        // create new faces
        for (int j = 0; j < num_verts_in_cur_face; ++j){
            int i0 = i + j + 1;
            int next = i + (j%num_verts_in_cur_face) + 1;
            int A_0 = faces[i0].GetInt();
            int A_1 = faces[next].GetInt();
            int B_0 = faces.Size() - (num_verts_in_cur_face - j);
            int B_1 = faces.Size() - (num_verts_in_cur_face - (j+1)%num_verts_in_cur_face);
            
            frame_faces.Push(Variant(A_0));
            frame_faces.Push(Variant(A_1));
            frame_faces.Push(Variant(B_0));
            
            frame_faces.Push(Variant(A_1));
            frame_faces.Push(Variant(B_1));
            frame_faces.Push(Variant(B_0));
            
            
        }
        
        i += num_verts_in_cur_face;
    }
    
    Variant out_mesh = TriMesh_Make(frame_verts, frame_faces);
    
    outSolveInstance[0] = out_mesh;
    
    
}

