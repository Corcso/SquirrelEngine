#include "PCH.h"
#include "PhysicsJolt.h"
#include "Services.h"
#include "CollisionShapeJolt.h"

void SQ::PhysicsJolt::Init()
{
	// Contains Adapted Code From (Rouwe, no date b)
	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	JPH::RegisterDefaultAllocator();

	// Install trace and assert callbacks
	//Trace = TraceImpl;
	//JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	JPH::Factory::sInstance = new JPH::Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	JPH::RegisterTypes();

	// We need a temp allocator for temporary allocations during the physics update. We're
	// pre-allocating 10 MB to avoid having to do allocations during the physics update.
	// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
	// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
	// malloc / free.
	tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const UINT cMaxBodies = 1024;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const UINT cNumBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const UINT cMaxBodyPairs = 1024;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
	const UINT cMaxContactConstraints = 1024;

	

	// Now we can create the actual physics system.
	
	physicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	// A body activation listener gets notified when bodies activate and go to sleep
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	//MyBodyActivationListener body_activation_listener;
	//physics_system.SetBodyActivationListener(&body_activation_listener);

	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	//MyContactListener contact_listener;
	physicsSystem.SetContactListener(&contactListner);

	// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
	// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
	bodyInterface = &physicsSystem.GetBodyInterface();


	JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

	// Create the shape
	JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()


	// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
	JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(0.0, -1.0, 0.0), JPH::Quat::sIdentity(), JPH::EMotionType::Static, SQJOLT::Layers::NON_MOVING);

	// Create the actual rigid body
	JPH::Body* floor = bodyInterface->CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

	// Add it to the world
	bodyInterface->AddBody(floor->GetID(), JPH::EActivation::DontActivate);

}

void SQ::PhysicsJolt::RegisterBody(PhysicsNut* nut)
{
	// Default shape will be overwritten
	// Set new shape settings
	JPH::SphereShapeSettings tempShapeBeforeUpdate(1.0f);
	tempShapeBeforeUpdate.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

	// Create the body in the system setting relevant parameters.
	JPH::BodyCreationSettings newBodySettings(tempShapeBeforeUpdate.Create().Get(), JPH::RVec3(nut->GetPosition().X, nut->GetPosition().Y, nut->GetPosition().Z), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, SQJOLT::Layers::MOVING);
	JPH::BodyID newBodyID = bodyInterface->CreateAndAddBody(newBodySettings, JPH::EActivation::Activate);
	bodyInterface->SetRestitution(newBodyID, 0.78);

	nutsInSystem[newBodyID] = nut;
	nutsInSystemReversed[nut] = newBodyID;

	BodyShapeUpdated(nut);
}

void SQ::PhysicsJolt::Update()
{

	// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
	const int cCollisionSteps = 2;
	const float cDeltaTime = 1.0f / 120.0f;

	// Clear collision lists
	collisionsEnteredThisFrame.clear();
	collisionsExitedThisFrame.clear();

	// Step the world
	physicsSystem.Update(cDeltaTime, cCollisionSteps, tempAllocator, jobSystem);
	
	for (std::map<JPH::BodyID, PhysicsNut*>::iterator it = nutsInSystem.begin(); it != nutsInSystem.end(); ++it) {
		JPH::RVec3 pos = bodyInterface->GetCenterOfMassPosition(it->first);
		JPH::Quat rot = bodyInterface->GetRotation(it->first);

		it->second->SetPosition(V3(pos.GetX(), pos.GetY(), pos.GetZ()));
		it->second->SetRotation(Q(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW()));
	}

	// Collision Signalling & Updating
	// For new collisions
	for (int i = 0; i < collisionsEnteredThisFrame.size(); i++) {
		if (collisionsEnteredThisFrame[i].first.GetIndexAndSequenceNumber() == 16777216 || collisionsEnteredThisFrame[i].second.GetIndexAndSequenceNumber() == 16777216) continue;

		// Add them to the currently colliding list
		nutsInSystem[collisionsEnteredThisFrame[i].first]->currentlyColliding.insert(nutsInSystem[collisionsEnteredThisFrame[i].second]);
		nutsInSystem[collisionsEnteredThisFrame[i].second]->currentlyColliding.insert(nutsInSystem[collisionsEnteredThisFrame[i].first]);

		// Also run function for collision entered
		nutsInSystem[collisionsEnteredThisFrame[i].first]->OnCollisionStart(nutsInSystem[collisionsEnteredThisFrame[i].second]);
		nutsInSystem[collisionsEnteredThisFrame[i].second]->OnCollisionStart(nutsInSystem[collisionsEnteredThisFrame[i].first]);
		
	}

	// For ended collisions
	for (int i = 0; i < collisionsExitedThisFrame.size(); i++) {
		if (collisionsExitedThisFrame[i].first.GetIndexAndSequenceNumber() == 16777216 || collisionsExitedThisFrame[i].second.GetIndexAndSequenceNumber() == 16777216) continue;

		// Remove them from the currently colliding list
		nutsInSystem[collisionsExitedThisFrame[i].first]->currentlyColliding.erase(nutsInSystem[collisionsExitedThisFrame[i].second]);
		nutsInSystem[collisionsExitedThisFrame[i].second]->currentlyColliding.erase(nutsInSystem[collisionsExitedThisFrame[i].first]);

		// Also run function for collision exited
		nutsInSystem[collisionsExitedThisFrame[i].first]->OnCollisionEnd(nutsInSystem[collisionsExitedThisFrame[i].second]);
		nutsInSystem[collisionsExitedThisFrame[i].second]->OnCollisionEnd(nutsInSystem[collisionsExitedThisFrame[i].first]);
	}


}

void SQ::PhysicsJolt::RemoveBody(PhysicsNut* nut)
{
	// Remove and delete body
	bodyInterface->RemoveBody(nutsInSystemReversed[nut]);
	bodyInterface->DestroyBody(nutsInSystemReversed[nut]);

	// Remove from lists of nuts in system
	nutsInSystem.erase(nutsInSystemReversed[nut]);
	nutsInSystemReversed.erase(nut);
}

void SQ::PhysicsJolt::BodyShapeUpdated(PhysicsNut* nut)
{
	// Define shape result, accross all types
	JPH::ShapeSettings::ShapeResult newShapeResult;

	switch (nut->GetShape()->GetType()) {
	case CollisionShape::Type::BOX:
	{
		// Set new shape settings
		JPH::BoxShapeSettings newBoxShapeSettings(JPH::Vec3(JPH::RVec3(nut->GetShape()->GetBoxHalfDimentions().X, nut->GetShape()->GetBoxHalfDimentions().Y, nut->GetShape()->GetBoxHalfDimentions().Z)));
		newBoxShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.
		newBoxShapeSettings.SetDensity(nut->GetDensity());

		// Create the shape
		newShapeResult = newBoxShapeSettings.Create();
		// Set the body's shape, recalculate mass & inertia, wake up body too. 
		bodyInterface->SetShape(nutsInSystemReversed[nut], newShapeResult.Get(), true, JPH::EActivation::Activate);
	}
		break;
	case CollisionShape::Type::SPHERE:
	{
		// Set new shape settings
		JPH::SphereShapeSettings newSphereShapeSettings(nut->GetShape()->GetSphereRadius());
		newSphereShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.
		newSphereShapeSettings.SetDensity(nut->GetDensity());

		// Create the shape
		newShapeResult = newSphereShapeSettings.Create();
		// Set the body's shape, recalculate mass & inertia, wake up body too. 
		bodyInterface->SetShape(nutsInSystemReversed[nut], newShapeResult.Get(), true, JPH::EActivation::Activate);
	}
		break;
	case CollisionShape::Type::CAPSULE:
	{
		// Set new shape settings
		JPH::CapsuleShapeSettings newCapsuleShapeSettings(nut->GetShape()->GetCapsuleHalfHeight(), nut->GetShape()->GetCapsuleRadius());
		newCapsuleShapeSettings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.
		newCapsuleShapeSettings.SetDensity(nut->GetDensity());

		// Create the shape
		newShapeResult = newCapsuleShapeSettings.Create();
		// Set the body's shape, recalculate mass & inertia, wake up body too. 
		bodyInterface->SetShape(nutsInSystemReversed[nut], newShapeResult.Get(), true, JPH::EActivation::Activate);
	}
		break;
	}
}

void SQ::PhysicsJolt::BodyElasticityUpdated(PhysicsNut* nut)
{
	bodyInterface->SetRestitution(nutsInSystemReversed[nut], nut->GetElasticity());
}

void SQJOLT::MyContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	dynamic_cast<SQ::PhysicsJolt*>(SQ::Services::GetPhysics())->collisionsEnteredThisFrame.push_back(std::pair<JPH::BodyID, JPH::BodyID>(inBody1.GetID(), inBody2.GetID()));
}

void SQJOLT::MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	dynamic_cast<SQ::PhysicsJolt*>(SQ::Services::GetPhysics())->collisionsExitedThisFrame.push_back(std::pair<JPH::BodyID, JPH::BodyID>(inSubShapePair.GetBody1ID(), inSubShapePair.GetBody2ID()));
}
