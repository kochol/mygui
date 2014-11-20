/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#ifndef __MYGUI_KGE_PLATFORM_H__
#define __MYGUI_KGE_PLATFORM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_KgeRenderManager.h"
#include "MyGUI_KgeDataManager.h"
#include "MyGUI_KgeTexture.h"
#include "MyGUI_KgeVertexBuffer.h"
#include "MyGUI_KgeDiagnostic.h"
#include "MyGUI_LogManager.h"

namespace MyGUI
{

	class KgePlatform
	{
	public:
		KgePlatform() :
			mIsInitialise(false)
		{
			mLogManager = new LogManager();
			mRenderManager = new KgeRenderManager();
			mDataManager = new KgeDataManager();
		}

		~KgePlatform()
		{
			assert(!mIsInitialise);
			delete mRenderManager;
			delete mDataManager;
			delete mLogManager;
		}

		void initialise(IDirect3DDevice9* _device, const std::string& _logName = MYGUI_PLATFORM_LOG_FILENAME)
		{
			assert(!mIsInitialise);
			mIsInitialise = true;

			if (!_logName.empty())
				LogManager::getInstance().createDefaultSource(_logName);

			mRenderManager->initialise(_device);
			mDataManager->initialise();
		}

		void shutdown()
		{
			assert(mIsInitialise);
			mIsInitialise = false;

			mRenderManager->shutdown();
			mDataManager->shutdown();
		}

		KgeRenderManager* getRenderManagerPtr()
		{
			assert(mIsInitialise);
			return mRenderManager;
		}

		KgeDataManager* getDataManagerPtr()
		{
			assert(mIsInitialise);
			return mDataManager;
		}

	private:
		bool mIsInitialise;
		KgeRenderManager* mRenderManager;
		KgeDataManager* mDataManager;
		LogManager* mLogManager;

	};

} // namespace MyGUI

#endif // __MYGUI_KGE_PLATFORM_H__
