#include "lupch.h"
#include "LayerStack.h"

namespace Luna
{

	LayerStack::~LayerStack()
	{
		for(Layer* CurrentLayer : mLayers)
		{
			CurrentLayer->OnDetach();
			delete CurrentLayer;
		}
	}

	void LayerStack::PushLayer(Layer* LayerToPush)
	{
		mLayers.emplace(mLayers.begin() + mInsertIndex, LayerToPush);
		mInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* Overlay)
	{
		mLayers.emplace_back(Overlay);
	}

	void LayerStack::PopLayer(Layer* LayerToPop)
	{
		auto It = std::find(mLayers.begin(), mLayers.begin() + mInsertIndex, LayerToPop);
		if (It != mLayers.begin() + mInsertIndex)
		{
			LayerToPop->OnDetach();
			mLayers.erase(It);
			mInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* Overlay)
	{
		auto It = std::find(mLayers.begin() + mInsertIndex, mLayers.end(), Overlay);
		if (It != mLayers.end())
		{
			Overlay->OnDetach();
			mLayers.erase(It);
		}
	}

}