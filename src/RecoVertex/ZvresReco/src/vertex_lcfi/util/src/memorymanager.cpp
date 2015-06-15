#include <util/inc/memorymanager.h>

namespace vertex_lcfi
{
	
	MetaMemoryManager::MetaMemoryManager()
	{}
	
	MetaMemoryManager* MetaMemoryManager::Event() 
	{
		static MetaMemoryManager eventInstance;
		return &eventInstance;
	}
  
	MetaMemoryManager* MetaMemoryManager::Run() 
	{
		static MetaMemoryManager runInstance;
		return &runInstance;
	}
  
	void MetaMemoryManager::delAllObjects()
	{
		for(std::vector<MemoryManagerType*>::iterator iMem = _Types.begin();iMem != _Types.end();++iMem)
			(*iMem)->delAll();
	}
	
	void MetaMemoryManager::registerType(MemoryManagerType* Type)
	{
		_Types.push_back(Type);
	}
	
}


