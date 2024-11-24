#pragma once
#include "PCH.h"
#include "json.hpp"
#include "Resource.h"
#include "Nut.h"

namespace SQ {
    class ShelledNut :
        public Resource
    {
    public:
        static ShelledNut* Load(std::string path);

        std::unique_ptr<Nut> Instantiate();
        

    private:
        std::unique_ptr<Nut> Instantiate(nlohmann::json data);
        nlohmann::json jsonData;
    };
}