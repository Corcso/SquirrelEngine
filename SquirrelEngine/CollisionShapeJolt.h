#pragma once
#include "CollisionShape.h"
#include "SquirrelEnginePI.h"
namespace SQ {
    class CollisionShapeJolt :
        public CollisionShape
    {
    public:
        void LoadInJolt();

        JPH::Shape* GetJoltShape();

        virtual void Update() override;
    private:
        JPH::Shape* shape;
    };
}