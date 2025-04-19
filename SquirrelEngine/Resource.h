#pragma once
namespace SQ {
	/// <summary>
	/// Base resource class. 
	/// </summary>
	class Resource
	{
	public:
		virtual ~Resource() {}

		virtual void ImGuiRenderMyInspector() {}
	};
}

