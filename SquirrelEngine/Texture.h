#pragma once
#include "Image.h"
namespace SQ {
    /// <summary>
    /// Textures which can be used in materials
    /// </summary>
    class Texture :
        public virtual Image
    {
        static Texture* Load(std::string path);
    };
}