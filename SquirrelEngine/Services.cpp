#include "PCH.h"
#include "Services.h"
std::unique_ptr<SQ::Graphics> SQ::Services::graphicsService = std::unique_ptr<Graphics>(nullptr);
std::unique_ptr<SQ::Input> SQ::Services::inputService = std::unique_ptr<Input>(nullptr);
std::unique_ptr<SQ::ResourceManager> SQ::Services::resourceManagerService = std::unique_ptr<ResourceManager>(nullptr);
std::unique_ptr<SQ::Tree> SQ::Services::treeService = std::unique_ptr<Tree>(nullptr);
std::unique_ptr<SQ::PoolAllocationService> SQ::Services::poolAllocationService = std::unique_ptr<PoolAllocationService>(nullptr);
std::unique_ptr<SQ::Time> SQ::Services::timeService = std::unique_ptr<Time>(nullptr);
std::unique_ptr<SQ::Physics> SQ::Services::physicsService = std::unique_ptr<Physics>(nullptr);