#pragma once
#include "Resource.h"
namespace SQ {
    class Image :
        public Resource
    {
    public:
        static Image* Load(std::string path);

        virtual ~Image() override {};
    };
}