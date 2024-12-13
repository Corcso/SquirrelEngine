#include "PCH.h"
#include "Nut.h"
#include "Services.h"
namespace SQ {
	void Nut::SetParent(Nut* newParent, UniquePoolPtr<Nut> currentOwnership)
	{
		if (parent != nullptr) {
			for (std::vector<UniquePoolPtr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
				if (it->get() == this) {
					newParent->AddChild(std::move(*it));
					parent->children.erase(it);
					break;
				}
			}
		}
		else {
			newParent->AddChild(std::move(currentOwnership));
		}
		parent = newParent;
	}

	UniquePoolPtr<Nut> Nut::TakeOwnership()
	{
		UniquePoolPtr<Nut> tempOwner;
		if (parent != nullptr) {
			for (std::vector<UniquePoolPtr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
				if (it->get() == this) {
					tempOwner = std::move(*it);
					parent->children.erase(it);
					break;
				}
			}
		}
		return std::move(tempOwner);
	}

	void Nut::AddChild(UniquePoolPtr<Nut> newChild)
	{
		newChild->parent = this;
		children.push_back(std::move(newChild));
		NewChildAdded(true);
	}

	unsigned int Nut::GetChildCount()
	{
		return children.size();
	}

	void Nut::QueueDestroy()
	{
		isQueuedToDestroy = true;
	}

	bool Nut::IsQueuedForDestruction()
	{
		return isQueuedToDestroy;
	}

	UniquePoolPtr<Nut> Nut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		UniquePoolPtr<Nut> owner;
		if (deserializeInto == nullptr) { 
			owner = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Nut>(); 
			deserializeInto	= owner.get();
		}

		if (!serializedData["name"].is_null()) deserializeInto->name = serializedData["name"];
		else throw 17;

		return owner;
	}
	void Nut::NewChildAdded(bool myChild)
	{
		if(parent != nullptr) parent->NewChildAdded(false);
	}
}