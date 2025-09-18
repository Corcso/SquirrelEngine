#pragma once
#include "Resource.h"
namespace SQ {
    class Image :
        public Resource
    {
        static Image* Load(std::string path);
    };
}