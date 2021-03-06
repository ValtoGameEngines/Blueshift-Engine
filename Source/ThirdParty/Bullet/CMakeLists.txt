cmake_minimum_required(VERSION 2.8.12)

project(Bullet)

if (MSVC)
    option(USE_MSVC_FAST_FLOATINGPOINT "Use MSVC /fp:fast option"	ON)
    if (USE_MSVC_FAST_FLOATINGPOINT)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fp:fast")
    endif()
ENDIF()

if (WIN32)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
    ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
endif()

set(SRC_FILES
    src/btBulletCollisionCommon.h
    src/btBulletDynamicsCommon.h

    src/BulletCollision/BroadphaseCollision/btAxisSweep3.h
    src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
    src/BulletCollision/BroadphaseCollision/btAxisSweep3Internal.h
    src/BulletCollision/BroadphaseCollision/btBroadphaseInterface.h
    src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.h
    src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
    src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h
    src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
    src/BulletCollision/BroadphaseCollision/btDbvt.h
    src/BulletCollision/BroadphaseCollision/btDbvt.cpp
    src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.h
    src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
    src/BulletCollision/BroadphaseCollision/btDispatcher.h
    src/BulletCollision/BroadphaseCollision/btDispatcher.cpp
    src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.h
    src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
    src/BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h
    src/BulletCollision/BroadphaseCollision/btQuantizedBvh.h
    src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
    src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.h
    src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp

    src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btBoxBoxDetector.h
    src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
    src/BulletCollision/CollisionDispatch/btCollisionConfiguration.h
    src/BulletCollision/CollisionDispatch/btCollisionCreateFunc.h
    src/BulletCollision/CollisionDispatch/btCollisionDispatcher.h
    src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
    src/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h
    src/BulletCollision/CollisionDispatch/btCollisionDispatcherMt.cpp
    src/BulletCollision/CollisionDispatch/btCollisionObject.h
    src/BulletCollision/CollisionDispatch/btCollisionObject.cpp
    src/BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h
    src/BulletCollision/CollisionDispatch/btCollisionWorld.h
    src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
    src/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
    src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h
    src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.h
    src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h
    src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
    src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btGhostObject.h
    src/BulletCollision/CollisionDispatch/btGhostObject.cpp
    src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.h
    src/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
    src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.h
    src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
    src/BulletCollision/CollisionDispatch/btManifoldResult.h
    src/BulletCollision/CollisionDispatch/btManifoldResult.cpp
    src/BulletCollision/CollisionDispatch/btSimulationIslandManager.h
    src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
    src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h
    src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
    src/BulletCollision/CollisionDispatch/btUnionFind.h
    src/BulletCollision/CollisionDispatch/btUnionFind.cpp
    src/BulletCollision/CollisionDispatch/SphereTriangleDetector.h
    src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp

    src/BulletCollision/CollisionShapes/btBox2dShape.h
    src/BulletCollision/CollisionShapes/btBox2dShape.cpp
    src/BulletCollision/CollisionShapes/btBoxShape.h
    src/BulletCollision/CollisionShapes/btBoxShape.cpp
    src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h
    src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
    src/BulletCollision/CollisionShapes/btCapsuleShape.h
    src/BulletCollision/CollisionShapes/btCapsuleShape.cpp
    src/BulletCollision/CollisionShapes/btCollisionMargin.h
    src/BulletCollision/CollisionShapes/btCollisionShape.h
    src/BulletCollision/CollisionShapes/btCollisionShape.cpp
    src/BulletCollision/CollisionShapes/btCompoundShape.h
    src/BulletCollision/CollisionShapes/btCompoundShape.cpp
    src/BulletCollision/CollisionShapes/btConcaveShape.h
    src/BulletCollision/CollisionShapes/btConcaveShape.cpp
    src/BulletCollision/CollisionShapes/btConeShape.h
    src/BulletCollision/CollisionShapes/btConeShape.cpp
    src/BulletCollision/CollisionShapes/btConvex2dShape.h
    src/BulletCollision/CollisionShapes/btConvex2dShape.cpp
    src/BulletCollision/CollisionShapes/btConvexHullShape.h
    src/BulletCollision/CollisionShapes/btConvexHullShape.cpp
    src/BulletCollision/CollisionShapes/btConvexInternalShape.h
    src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
    src/BulletCollision/CollisionShapes/btConvexPointCloudShape.h
    src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
    src/BulletCollision/CollisionShapes/btConvexPolyhedron.h
    src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
    src/BulletCollision/CollisionShapes/btConvexShape.h
    src/BulletCollision/CollisionShapes/btConvexShape.cpp
    src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.h
    src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
    src/BulletCollision/CollisionShapes/btCylinderShape.h
    src/BulletCollision/CollisionShapes/btCylinderShape.cpp
    src/BulletCollision/CollisionShapes/btEmptyShape.h
    src/BulletCollision/CollisionShapes/btEmptyShape.cpp
    src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h
    src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
    src/BulletCollision/CollisionShapes/btMaterial.h
    src/BulletCollision/CollisionShapes/btMiniSDF.h
    src/BulletCollision/CollisionShapes/btMiniSDF.cpp
    src/BulletCollision/CollisionShapes/btMinkowskiSumShape.h
    src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
    src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h
    src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
    src/BulletCollision/CollisionShapes/btMultiSphereShape.h
    src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
    src/BulletCollision/CollisionShapes/btOptimizedBvh.h
    src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
    src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.h
    src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
    src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h
    src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
    src/BulletCollision/CollisionShapes/btSdfCollisionShape.h
    src/BulletCollision/CollisionShapes/btSdfCollisionShape.cpp
    src/BulletCollision/CollisionShapes/btShapeHull.h
    src/BulletCollision/CollisionShapes/btShapeHull.cpp
    src/BulletCollision/CollisionShapes/btSphereShape.h
    src/BulletCollision/CollisionShapes/btSphereShape.cpp
    src/BulletCollision/CollisionShapes/btStaticPlaneShape.h
    src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
    src/BulletCollision/CollisionShapes/btStridingMeshInterface.h
    src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
    src/BulletCollision/CollisionShapes/btTetrahedronShape.h
    src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
    src/BulletCollision/CollisionShapes/btTriangleBuffer.h
    src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
    src/BulletCollision/CollisionShapes/btTriangleCallback.h
    src/BulletCollision/CollisionShapes/btTriangleCallback.cpp
    src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h
    src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
    src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h
    src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
    src/BulletCollision/CollisionShapes/btTriangleInfoMap.h
    src/BulletCollision/CollisionShapes/btTriangleMesh.h
    src/BulletCollision/CollisionShapes/btTriangleMesh.cpp
    src/BulletCollision/CollisionShapes/btTriangleMeshShape.h
    src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
    src/BulletCollision/CollisionShapes/btTriangleShape.h
    src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
    src/BulletCollision/CollisionShapes/btUniformScalingShape.h

    src/BulletCollision/Gimpact/btBoxCollision.h
    src/BulletCollision/Gimpact/btClipPolygon.h
    src/BulletCollision/Gimpact/btCompoundFromGimpact.h
    src/BulletCollision/Gimpact/btContactProcessing.h
    src/BulletCollision/Gimpact/btContactProcessing.cpp
    src/BulletCollision/Gimpact/btContactProcessingStructs.h
    src/BulletCollision/Gimpact/btGenericPoolAllocator.h
    src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
    src/BulletCollision/Gimpact/btGeometryOperations.h
    src/BulletCollision/Gimpact/btGImpactBvh.h
    src/BulletCollision/Gimpact/btGImpactBvh.cpp
    src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h
    src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
    src/BulletCollision/Gimpact/btGImpactMassUtil.h
    src/BulletCollision/Gimpact/btGImpactQuantizedBvh.h
    src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
    src/BulletCollision/Gimpact/btGImpactQuantizedBvhStructs.h
    src/BulletCollision/Gimpact/btGImpactShape.h
    src/BulletCollision/Gimpact/btGImpactShape.cpp
    src/BulletCollision/Gimpact/btQuantization.h
    src/BulletCollision/Gimpact/btTriangleShapeEx.h
    src/BulletCollision/Gimpact/btTriangleShapeEx.cpp
    src/BulletCollision/Gimpact/gim_array.h
    src/BulletCollision/Gimpact/gim_basic_geometry_operations.h
    src/BulletCollision/Gimpact/gim_bitset.h
    src/BulletCollision/Gimpact/gim_box_collision.h
    src/BulletCollision/Gimpact/gim_box_set.h
    src/BulletCollision/Gimpact/gim_box_set.cpp
    src/BulletCollision/Gimpact/gim_clip_polygon.h
    src/BulletCollision/Gimpact/gim_contact.h
    src/BulletCollision/Gimpact/gim_contact.cpp
    src/BulletCollision/Gimpact/gim_geom_types.h
    src/BulletCollision/Gimpact/gim_geometry.h
    src/BulletCollision/Gimpact/gim_hash_table.h
    src/BulletCollision/Gimpact/gim_linear_math.h
    src/BulletCollision/Gimpact/gim_math.h
    src/BulletCollision/Gimpact/gim_memory.h
    src/BulletCollision/Gimpact/gim_memory.cpp
    src/BulletCollision/Gimpact/gim_radixsort.h
    src/BulletCollision/Gimpact/gim_tri_collision.h
    src/BulletCollision/Gimpact/gim_tri_collision.cpp

    src/BulletCollision/NarrowPhaseCollision/btComputeGjkEpaPenetration.h
    src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.h
    src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
    src/BulletCollision/NarrowPhaseCollision/btConvexCast.h
    src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
    src/BulletCollision/NarrowPhaseCollision/btConvexPenetrationDepthSolver.h
    src/BulletCollision/NarrowPhaseCollision/btDiscreteCollisionDetectorInterface.h
    src/BulletCollision/NarrowPhaseCollision/btGjkCollisionDescription.h
    src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.h
    src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
    src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.h
    src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
    src/BulletCollision/NarrowPhaseCollision/btGjkEpa3.h
    src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.h
    src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
    src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.h
    src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
    src/BulletCollision/NarrowPhaseCollision/btManifoldPoint.h
    src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h
    src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
    src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.h
    src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
    src/BulletCollision/NarrowPhaseCollision/btPointCollector.h
    src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.h
    src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
    src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.h
    src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
    src/BulletCollision/NarrowPhaseCollision/btSimplexSolverInterface.h
    src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.h
    src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
    src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.h
    src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp

    src/BulletDynamics/Character/btCharacterControllerInterface.h
    src/BulletDynamics/Character/btKinematicCharacterController.h
    src/BulletDynamics/Character/btKinematicCharacterController.cpp

    src/BulletDynamics/ConstraintSolver/btBatchedConstraints.h
    src/BulletDynamics/ConstraintSolver/btBatchedConstraints.cpp
    src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.h
    src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btConstraintSolver.h
    src/BulletDynamics/ConstraintSolver/btContactConstraint.h
    src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btContactSolverInfo.h
    src/BulletDynamics/ConstraintSolver/btFixedConstraint.h
    src/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btGearConstraint.h
    src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h
    src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.h
    src/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
    src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h
    src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btHinge2Constraint.h
    src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
    src/BulletDynamics/ConstraintSolver/btHingeConstraint.h
    src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btJacobianEntry.h
    src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.h
    src/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
    src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h
    src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h
    src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
    src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.h
    src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.cpp
    src/BulletDynamics/ConstraintSolver/btSliderConstraint.h
    src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h
    src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btSolverBody.h
    src/BulletDynamics/ConstraintSolver/btSolverConstraint.h
    src/BulletDynamics/ConstraintSolver/btTypedConstraint.h
    src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
    src/BulletDynamics/ConstraintSolver/btUniversalConstraint.h
    src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp

    src/BulletDynamics/Dynamics/btActionInterface.h
    src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h
    src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
    src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h
    src/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp
    src/BulletDynamics/Dynamics/btDynamicsWorld.h
    src/BulletDynamics/Dynamics/btRigidBody.h
    src/BulletDynamics/Dynamics/btRigidBody.cpp
    src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.h
    src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
    src/BulletDynamics/Dynamics/btSimulationIslandManagerMt.h
    src/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp
    
    src/BulletDynamics/Featherstone/btMultiBody.h
    src/BulletDynamics/Featherstone/btMultiBody.cpp
    src/BulletDynamics/Featherstone/btMultiBodyConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
    src/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h
    src/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
    src/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.h
    src/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
    src/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.cpp
    src/BulletDynamics/Featherstone/btMultiBodyGearConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodyGearConstraint.cpp
    src/BulletDynamics/Featherstone/btMultiBodyJointFeedback.h
    src/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
    src/BulletDynamics/Featherstone/btMultiBodyJointMotor.h
    src/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
    src/BulletDynamics/Featherstone/btMultiBodyLink.h
    src/BulletDynamics/Featherstone/btMultiBodyLinkCollider.h
    src/BulletDynamics/Featherstone/btMultiBodyMLCPConstraintSolver.h
    src/BulletDynamics/Featherstone/btMultiBodyMLCPConstraintSolver.cpp
    src/BulletDynamics/Featherstone/btMultiBodyPoint2Point.h
    src/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
    src/BulletDynamics/Featherstone/btMultiBodySliderConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodySliderConstraint.cpp
    src/BulletDynamics/Featherstone/btMultiBodySolverConstraint.h
    src/BulletDynamics/Featherstone/btMultiBodySphericalJointMotor.h
    src/BulletDynamics/Featherstone/btMultiBodySphericalJointMotor.cpp

    src/BulletDynamics/MLCPSolvers/btDantzigLCP.h
    src/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
    src/BulletDynamics/MLCPSolvers/btDantzigSolver.h
    src/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.h
    src/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
    src/BulletDynamics/MLCPSolvers/btLemkeSolver.h
    src/BulletDynamics/MLCPSolvers/btMLCPSolver.h
    src/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
    src/BulletDynamics/MLCPSolvers/btMLCPSolverInterface.h
    src/BulletDynamics/MLCPSolvers/btPATHSolver.h
    src/BulletDynamics/MLCPSolvers/btSolveProjectedGaussSeidel.h

    src/BulletDynamics/Vehicle/btRaycastVehicle.h
    src/BulletDynamics/Vehicle/btRaycastVehicle.cpp
    src/BulletDynamics/Vehicle/btVehicleRaycaster.h
    src/BulletDynamics/Vehicle/btWheelInfo.h    
    src/BulletDynamics/Vehicle/btWheelInfo.cpp

    src/BulletInverseDynamics/details/IDEigenInterface.hpp
    src/BulletInverseDynamics/details/IDLinearMathInterface.hpp
    src/BulletInverseDynamics/details/IDMatVec.hpp
    src/BulletInverseDynamics/details/MultiBodyTreeImpl.hpp
    src/BulletInverseDynamics/details/MultiBodyTreeImpl.cpp
    src/BulletInverseDynamics/details/MultiBodyTreeInitCache.hpp
    src/BulletInverseDynamics/details/MultiBodyTreeInitCache.cpp
    src/BulletInverseDynamics/IDConfig.hpp
    src/BulletInverseDynamics/IDConfigBuiltin.hpp
    src/BulletInverseDynamics/IDConfigEigen.hpp
    src/BulletInverseDynamics/IDErrorMessages.hpp
    src/BulletInverseDynamics/IDMath.hpp
    src/BulletInverseDynamics/IDMath.cpp
    src/BulletInverseDynamics/MultiBodyTree.hpp
    src/BulletInverseDynamics/MultiBodyTree.cpp

    src/BulletSoftBody/btDefaultSoftBodySolver.h
    src/BulletSoftBody/btDefaultSoftBodySolver.cpp
    src/BulletSoftBody/btSoftBody.h
    src/BulletSoftBody/btSoftBody.cpp
    src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.h
    src/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp
    src/BulletSoftBody/btSoftBodyData.h
    src/BulletSoftBody/btSoftBodyHelpers.h
    src/BulletSoftBody/btSoftBodyHelpers.cpp
    src/BulletSoftBody/btSoftBodyInternals.h
    src/BulletSoftBody/btSoftMultiBodyDynamicsWorld.h
    src/BulletSoftBody/btSoftMultiBodyDynamicsWorld.cpp
    src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h
    src/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp
    src/BulletSoftBody/btSoftBodySolvers.h
    src/BulletSoftBody/btSoftBodySolverVertexBuffer.h
    src/BulletSoftBody/btSoftRigidCollisionAlgorithm.h
    src/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp
    src/BulletSoftBody/btSoftRigidDynamicsWorld.h
    src/BulletSoftBody/btSoftRigidDynamicsWorld.cpp
    src/BulletSoftBody/btSoftSoftCollisionAlgorithm.h
    src/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp
    src/BulletSoftBody/btSparseSDF.h

    src/LinearMath/btAabbUtil2.h
    src/LinearMath/btAlignedAllocator.h
    src/LinearMath/btAlignedAllocator.cpp
    src/LinearMath/btAlignedObjectArray.h
    src/LinearMath/btConvexHull.h
    src/LinearMath/btConvexHull.cpp
    src/LinearMath/btConvexHullComputer.h
    src/LinearMath/btConvexHullComputer.cpp
    src/LinearMath/btCpuFeatureUtility.h
    src/LinearMath/btDefaultMotionState.h
    src/LinearMath/btGeometryUtil.h
    src/LinearMath/btGeometryUtil.cpp
    src/LinearMath/btGrahamScan2dConvexHull.h
    src/LinearMath/btHashMap.h
    src/LinearMath/btIDebugDraw.h
    src/LinearMath/btList.h
    src/LinearMath/btMatrix3x3.h
    src/LinearMath/btMatrixX.h
    src/LinearMath/btMinMax.h
    src/LinearMath/btMotionState.h
    src/LinearMath/btPolarDecomposition.h
    src/LinearMath/btPolarDecomposition.cpp
    src/LinearMath/btPoolAllocator.h
    src/LinearMath/btQuadWord.h
    src/LinearMath/btQuaternion.h
    src/LinearMath/btQuickprof.h
    src/LinearMath/btQuickprof.cpp
    src/LinearMath/btRandom.h
    src/LinearMath/btScalar.h
    src/LinearMath/btSerializer.h
    src/LinearMath/btSerializer.cpp
    src/LinearMath/btSerializer64.cpp
    src/LinearMath/btSpatialAlgebra.h
    src/LinearMath/btStackAlloc.h
    src/LinearMath/btTransform.h
    src/LinearMath/btTransformUtil.h
    src/LinearMath/btThreads.h
    src/LinearMath/btThreads.cpp
    src/LinearMath/btVector3.h
    src/LinearMath/btVector3.cpp

    Extras/ConvexDecomposition/bestfit.h
    Extras/ConvexDecomposition/bestfit.cpp
    Extras/ConvexDecomposition/bestfitobb.h
    Extras/ConvexDecomposition/bestfitobb.cpp
    Extras/ConvexDecomposition/cd_hull.h
    Extras/ConvexDecomposition/cd_hull.cpp
    Extras/ConvexDecomposition/cd_vector.h
    Extras/ConvexDecomposition/cd_wavefront.h
    Extras/ConvexDecomposition/cd_wavefront.cpp
    Extras/ConvexDecomposition/concavity.h
    Extras/ConvexDecomposition/concavity.cpp
    Extras/ConvexDecomposition/ConvexBuilder.h
    Extras/ConvexDecomposition/ConvexBuilder.cpp
    Extras/ConvexDecomposition/ConvexDecomposition.h
    Extras/ConvexDecomposition/ConvexDecomposition.cpp
    Extras/ConvexDecomposition/fitsphere.h
    Extras/ConvexDecomposition/fitsphere.cpp
    Extras/ConvexDecomposition/float_math.h
    Extras/ConvexDecomposition/float_math.cpp
    Extras/ConvexDecomposition/meshvolume.h
    Extras/ConvexDecomposition/meshvolume.cpp
    Extras/ConvexDecomposition/planetri.h
    Extras/ConvexDecomposition/planetri.cpp
    Extras/ConvexDecomposition/raytri.h
    Extras/ConvexDecomposition/raytri.cpp
    Extras/ConvexDecomposition/splitplane.h
    Extras/ConvexDecomposition/splitplane.cpp
    Extras/ConvexDecomposition/vlookup.h
    Extras/ConvexDecomposition/vlookup.cpp

    Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.h
    Extras/GIMPACTUtils/btGImpactConvexDecompositionShape.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

auto_source_group(${SRC_FILES})

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/Extras")
include_directories("${PROJECT_SOURCE_DIR}/Extras/ConvexDecomposition")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    FOLDER ThirdParty
    PREFIX ""
    #DEBUG_POSTFIX "_d"
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Library/${ENGINE_BUILD_PLATFORM_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Library/${ENGINE_BUILD_PLATFORM_DIR})
