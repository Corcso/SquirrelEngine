#include "PCH.h"
#include "Nut.h"
namespace SQ {
	void Nut::SetParent(Nut* newParent)
	{
		if (parent != nullptr) {
			for (std::vector<std::unique_ptr<Nut>>::iterator it = parent->children.begin(); it != parent->children.end(); it++) {
				if (it->get() == this) {
					newParent->AddChild(std::move(*it));
					children.erase(it);
				}
			}
		}
		else {
			newParent->AddChild(std::move(std::unique_ptr<Nut>(this)));
		}
		parent = newParent;
	}

	void Nut::AddChild(std::unique_ptr<Nut> newChild)
	{
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

	Nut* Nut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		if (deserializeInto == nullptr) deserializeInto = new Nut();

		if (!serializedData["name"].is_null()) deserializeInto->name = serializedData["name"];
		else throw 17;

		return deserializeInto;
	}
	void Nut::NewChildAdded(bool myChild)
	{
		if(parent != nullptr) parent->NewChildAdded(false);
	}
}