#include "PCH.h"
#include "PhysicsNut.h"
#include "Services.h"
namespace SQ {
    
    PhysicsNut::PhysicsNut(bool isStatic)
    {
        this->isStatic = isStatic;
    }

    UniquePoolPtr<Nut> SQ::PhysicsNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        PhysicsNut* toWorkOn = dynamic_cast<PhysicsNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        // We need to follow strict ownership with the pool ptr
        UniquePoolPtr<Nut> owner;
        if (toWorkOn == nullptr) {
            // Get the instance
            UniquePoolPtr<PhysicsNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<PhysicsNut>();
            // Set to work on to the instance
            toWorkOn = instance.get();
            // Transfer ownership into owner and static cast to nut base class
            owner = instance.StaticUniquePoolPtrCast<Nut>();
        }
        // Call parent deserialise, passing in our toWorkOn.
        WorldNut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 
        if (!serializedData["static"].is_null()) toWorkOn->isStatic = serializedData["static"];
        else toWorkOn->isStatic = false;
        if (!serializedData["elasticity"].is_null()) toWorkOn->elasticity = serializedData["elasticity"];
        if (!serializedData["density"].is_null()) toWorkOn->density = serializedData["density"];
        if (!serializedData["collisionShape"].is_null()) toWorkOn->shape = Services::GetResourceManager()->Retrieve<CollisionShape>(serializedData["collisionShape"]);
       
        toWorkOn->shape->RegisterNut(toWorkOn);

        // Return toWorkOn
        return owner;
    }

    void PhysicsNut::Ready()
    {
        // Call base ready and register body with physics engine
        WorldNut::Ready();
        Services::GetPhysics()->RegisterBody(this);
    }

    void PhysicsNut::SetShape(std::shared_ptr<CollisionShape> shape)
    {
        if (this->shape.get() != nullptr) this->shape->RemoveNut(this);
        this->shape = shape;
        this->shape->RegisterNut(this);
        if (HasReadyBeenCalled()) Services::GetPhysics()->BodyShapeUpdated(this);
    }
    std::shared_ptr<CollisionShape> PhysicsNut::GetShape()
    {
        return shape;
    }
    void PhysicsNut::SetElasticity(float elasticity)
    {
        this->elasticity = elasticity;
        if (HasReadyBeenCalled()) Services::GetPhysics()->BodyElasticityUpdated(this);
    }
    void PhysicsNut::SetDensity(float density)
    {
        this->density = density;
        if(HasReadyBeenCalled()) Services::GetPhysics()->BodyShapeUpdated(this);
    }
    float PhysicsNut::GetElasticity()
    {
        return elasticity;
    }
    float PhysicsNut::GetDensity()
    {
        return density;
    }
    void PhysicsNut::SetStatic(bool isStatic) {
        if (!HasReadyBeenCalled()) this->isStatic = isStatic;
    }

    bool PhysicsNut::IsStatic()
    {
        return isStatic;
    }
    void PhysicsNut::AddForce(Vec3 force)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->AddForce(this, force);
    }
    void PhysicsNut::AddImpulse(Vec3 impulse)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->AddImpulse(this, impulse);
    }
    void PhysicsNut::AddAngularImpulse(Vec3 impulse)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->AddAngularImpulse(this, impulse);
    }
    void PhysicsNut::AddTorque(Vec3 torque)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->AddTorque(this, torque);
    }
    void PhysicsNut::SetLinearVelocity(Vec3 velocity)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->SetLinearVelocity(this, velocity);
    }
    void PhysicsNut::SetAngularVelociry(Vec3 velocity)
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->SetAngularVelociry(this, velocity);
    }
    PhysicsNut::~PhysicsNut()
    {
        if (HasReadyBeenCalled()) Services::GetPhysics()->RemoveBody(this);
    }
}