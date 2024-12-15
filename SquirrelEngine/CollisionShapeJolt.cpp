#include "PCH.h"
#include "CollisionShapeJolt.h"

void SQ::CollisionShapeJolt::LoadInJolt()
{
	// Clear old shape if any
	if (shape != nullptr) delete shape;

	switch (type) {
	case Type::BOX:
		shape = new JPH::BoxShape(JPH::RVec3(boxHalfDimentions.X, boxHalfDimentions.Y, boxHalfDimentions.Z));
		break;
	case Type::SPHERE:
		shape = new JPH::SphereShape(sphereRadius);
		break;
	case Type::CAPSULE:
		shape = new JPH::CapsuleShape(capsuleHalfHeight, capsuleRadius);
		break;
	}
}

JPH::Shape* SQ::CollisionShapeJolt::GetJoltShape()
{
	return shape;
}

void SQ::CollisionShapeJolt::Update()
{
	LoadInJolt();
}
