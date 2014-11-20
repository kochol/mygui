/*!
	@file
	@author		Ali Akbar Mohammadi
	@date		11/2014
*/

#ifndef __MYGUI_KGE_DATA_MANAGER_H__
#define __MYGUI_KGE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class KgeDataManager :
		public DataManager
	{
	public:
		KgeDataManager();

		void initialise();
		void shutdown();

		static KgeDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static KgeDataManager* getInstancePtr()
		{
			return static_cast<KgeDataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::freeData */
		virtual void freeData(IDataStream* _data);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_KGE_DATA_MANAGER_H__
