#include "PCH.h"
#include "PhysicsJolt.h"
#include "Services.h"

void SQ::PhysicsJolt::Init()
{
	// Contains Adapted Code From (Rouwe, no date b)
	// 
	// 
	// 
	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	JPH::RegisterDefaultAllocator();

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
	tempAllocator = std::move(std::unique_ptr<JPH::TempAllocatorImpl>(new JPH::TempAllocatorImpl(10 * 1024 * 1024)));

	// We need a job system that will execute physics jobs on multiple threads. Typically
	// you would implement the JobSystem interface yourself and let Jolt Physics run on top
	// of your own job scheduler. JobSystemThreadPool is an example implementation.
	jobSystem = std::move(std::unique_ptr<JPH::JobSystemThreadPool>(new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1)));

	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// For a real project use something in the order of 65536.
	const UINT cMaxBodies = 65536;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	// Ive set to 1
	const UINT cNumBodyMutexes = 1;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: For a real project use something in the order of 65536.
	const UINT cMaxBodyPairs = 65536;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: For a real project use something in the order of 10240.
	const UINT cMaxContactConstraints = 10240;

	// Now we can create the actual physics system.
	physicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);


	// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
	// Note that this is called from a job so whatever you do here needs to be thread safe.
	// Registering one is entirely optional.
	//MyContactListener contact_listener;
	physicsSystem.SetContactListener(&contactListner);

	// The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
	// variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
	bodyInterface = &physicsSystem.GetBodyInterface();
}

void SQ::PhysicsJolt::RegisterBody(PhysicsNut* nut)
{
	std::lock_guard<std::mutex> lock(mutex);

	// Default shape will be overwritten
	// Set new shape settings
	JPH::SphereShapeSettings tempShapeBeforeUpdate(1.0f);
	tempShapeBeforeUpdate.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

	// Get static/dynamic properties ready.
	JPH::EMotionType motionType = nut->IsStatic() ? JPH::EMotionType::Static : JPH::EMotionType::Dynamic;
	JPH::ObjectLayer layer = nut->IsStatic() ? SQJOLT::Layers::NON_MOVING : SQJOLT::Layers::MOVING;
	JPH::EActivation wake = nut->IsStatic() ? JPH::EActivation::DontActivate : JPH::EActivation::Activate;

	// Create the body in the system setting relevant parameters.
	JPH::BodyCreationSettings newBodySettings(tempShapeBeforeUpdate.Create().Get(), 
		JPH::RVec3(nut->GetGlobalPosition().X, nut->GetGlobalPosition().Y, nut->GetGlobalPosition().Z),
		JPH::Quat(nut->GetRotation().X, nut->GetRotation().Y, nut->GetRotation().Z, nut->GetRotation().W), motionType, layer);
	JPH::BodyID newBodyID = bodyInterface->CreateAndAddBody(newBodySettings, wake);
	bodyInterface->SetRestitution(newBodyID, 0.78);

	// Register nut in maps
	nutsInSystem[newBodyID] = nut;
	nutsInSystemReversed[nut] = newBodyID;

	// Add the nuts actual shape to its body
	BodyShapeUpdated(nut);
}

void SQ::PhysicsJolt::Update()
{
	std::lock_guard<std::mutex> lock(mutex);
	// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
	const int cCollisionSteps = 2;
	const float cDeltaTime = 1.0f / 120.0f;

	// Clear collision lists
	collisionsEnteredThisFrame.clear();
	collisionsExitedThisFrame.clear();

	// Make sure physics world is same as game world
	for (std::map<JPH::BodyID, PhysicsNut*>::iterator it = nutsInSystem.begin(); it != nutsInSystem.end(); ++it) {
		// Get position and rotation from game world. 
		Vec3 worldPos = it->second->GetGlobalPosition();
		Quat worldRot = it->second->GetRotation();

		// Compare to the current position & rotation in the physics system, and change and wake up if different. 
		JPH::RVec3 physPos = bodyInterface->GetCenterOfMassPosition(it->first);
		JPH::Quat physRot = bodyInterface->GetRotation(it->first);
		
		if(ABS(worldPos.X - physPos.GetX()) > 0.0001 || ABS(worldPos.Y - physPos.GetY()) > 0.0001 || ABS(worldPos.Z - physPos.GetZ()) > 0.0001)
			bodyInterface->SetPosition(it->first, JPH::Vec3(worldPos.X, worldPos.Y, worldPos.Z), JPH::EActivation::Activate);
		if(DotQ(worldRot, Q(physRot.GetX(), physRot.GetY(), physRot.GetZ(), physRot.GetW())) > 0.001)
			bodyInterface->SetRotation(it->first, JPH::Quat(worldRot.X, worldRot.Y, worldRot.Z, worldRot.W), JPH::EActivation::Activate);
	}

	// Step the world
	physicsSystem.Update(cDeltaTime, cCollisionSteps, tempAllocator.get(), jobSystem.get());
	
	// Update rotation and position
	for (std::map<JPH::BodyID, PhysicsNut*>::iterator it = nutsInSystem.begin(); it != nutsInSystem.end(); ++it) {
		Vec3 worldPos = it->second->GetGlobalPosition();
		JPH::RVec3 pos = bodyInterface->GetCenterOfMassPosition(it->first);
		JPH::Quat rot = bodyInterface->GetRotation(it->first);

		it->second->SetGlobalPosition(V3(pos.GetX(), pos.GetY(), pos.GetZ()));
		Vec3 worldPosCheck = it->second->GetGlobalPosition();
		it->second->SetRotation(Q(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW()));
	}

	// Collision Signalling & Updating
	// For new collisions
	for (int i = 0; i < collisionsEnteredThisFrame.size(); i++) {
		// Make sure nuts exist for the collisions
		if (nutsInSystem.find(collisionsEnteredThisFrame[i].first) == nutsInSystem.end() ||
			nutsInSystem.find(collisionsEnteredThisFrame[i].second) == nutsInSystem.end()) continue;

		// Add them to the currently colliding list
		nutsInSystem[collisionsEnteredThisFrame[i].first]->currentlyColliding.insert(nutsInSystem[collisionsEnteredThisFrame[i].second]);
		nutsInSystem[collisionsEnteredThisFrame[i].second]->currentlyColliding.insert(nutsInSystem[collisionsEnteredThisFrame[i].first]);

		// Also run function for collision entered
		nutsInSystem[collisionsEnteredThisFrame[i].first]->OnCollisionStart(nutsInSystem[collisionsEnteredThisFrame[i].second]);
		nutsInSystem[collisionsEnteredThisFrame[i].second]->OnCollisionStart(nutsInSystem[collisionsEnteredThisFrame[i].first]);
		
	}

	// For ended collisions
	for (int i = 0; i < collisionsExitedThisFrame.size(); i++) {
		// Make sure nuts exist for the collisions
		// After deletion things can collision exit, make sure those are not processed. 
		if (nutsInSystem.find(collisionsExitedThisFrame[i].first) == nutsInSystem.end() ||
			nutsInSystem.find(collisionsExitedThisFrame[i].second) == nutsInSystem.end()) continue;

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
	std::lock_guard<std::mutex> lock(mutex);
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
		JPH::BoxShapeSettings newBoxShapeSettings(JPH::Vec3(JPH::RVec3(nut->GetShape()->GetBoxHalfDimensions().X, nut->GetShape()->GetBoxHalfDimensions().Y, nut->GetShape()->GetBoxHalfDimensions().Z)));
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

void SQ::PhysicsJolt::AddForce(PhysicsNut* nut, Vec3 force)
{
	bodyInterface->AddForce(nutsInSystemReversed[nut], JPH::Vec3(force.X, force.Y, force.Z));
}

void SQ::PhysicsJolt::AddImpulse(PhysicsNut* nut, Vec3 impulse)
{
	bodyInterface->AddImpulse(nutsInSystemReversed[nut], JPH::Vec3(impulse.X, impulse.Y, impulse.Z));
}

void SQ::PhysicsJolt::AddAngularImpulse(PhysicsNut* nut, Vec3 impulse)
{
	bodyInterface->AddAngularImpulse(nutsInSystemReversed[nut], JPH::Vec3(impulse.X, impulse.Y, impulse.Z));
}

void SQ::PhysicsJolt::AddTorque(PhysicsNut* nut, Vec3 torque)
{
	bodyInterface->AddTorque(nutsInSystemReversed[nut], JPH::Vec3(torque.X, torque.Y, torque.Z));
}

void SQ::PhysicsJolt::SetLinearVelocity(PhysicsNut* nut, Vec3 velocity)
{
	bodyInterface->SetLinearVelocity(nutsInSystemReversed[nut], JPH::Vec3(velocity.X, velocity.Y, velocity.Z));
}

void SQ::PhysicsJolt::SetAngularVelociry(PhysicsNut* nut, Vec3 velocity)
{
	bodyInterface->SetAngularVelocity(nutsInSystemReversed[nut], JPH::Vec3(velocity.X, velocity.Y, velocity.Z));
}

void SQJOLT::MyContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{
	dynamic_cast<SQ::PhysicsJolt*>(SQ::Services::GetPhysics())->collisionsEnteredThisFrame.push_back(std::pair<JPH::BodyID, JPH::BodyID>(inBody1.GetID(), inBody2.GetID()));
}

void SQJOLT::MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{
	dynamic_cast<SQ::PhysicsJolt*>(SQ::Services::GetPhysics())->collisionsExitedThisFrame.push_back(std::pair<JPH::BodyID, JPH::BodyID>(inSubShapePair.GetBody1ID(), inSubShapePair.GetBody2ID()));
}
