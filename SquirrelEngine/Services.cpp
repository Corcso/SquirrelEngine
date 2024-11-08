#include "PCH.h"
#include "Services.h"
std::unique_ptr<SQ::Graphics> SQ::Services::graphicsService = std::unique_ptr<Graphics>(nullptr);
std::unique_ptr<SQ::Input> SQ::Services::inputService = std::unique_ptr<Input>(nullptr);