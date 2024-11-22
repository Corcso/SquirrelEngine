#include "PCH.h"
#include "Services.h"
std::unique_ptr<SQ::Graphics> SQ::Services::graphicsService = std::unique_ptr<Graphics>(nullptr);
std::unique_ptr<SQ::Input> SQ::Services::inputService = std::unique_ptr<Input>(nullptr);
std::unique_ptr<SQ::ResourceManager> SQ::Services::resourceManagerService = std::unique_ptr<ResourceManager>(nullptr);
std::unique_ptr<SQ::Tree> SQ::Services::treeService = std::unique_ptr<Tree>(nullptr);