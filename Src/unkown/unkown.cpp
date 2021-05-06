#include <stdio.h>
#include <map>

//class IUnkown
//{
//private:
//
//public:
//	IUnkown() { m_count = 0};
//	~IUnkown() {};
//	//virtual bool QueryInterface(const IID& iid, void** ppv) = 0;
//	virtual int addRef()
//	{
//		m_count++
//	};
//	virtual int release()
//	{
//		if (--m_count == 0)
//		{
//			delete this;
//		}
//	}
//private:
//	int m_count;
//};

template <typename T>
class IFactory
{
public:
	IFactory() {};
	~IFactory() {};

	T* getInterface(int iid);

	T* createInstance();
public:
	std::map<int, T*> m_map;
};

class Instance //: public IUnkown
{
public:
	Instance() {};
	~Instance() {};

	/*ddd*/
};
/*
class CInstanceA : public Instance
{

private:
	CInstanceA() {};
	~CInstanceA() {};
public:
	class Factory: public IFactory<Instance>
	{
	public:
		Factory() {};
		~Factory() {};

		static Instance* getInstance(int clsid)
		{
			
			return  
		}
	};

};



int main()
{

}
//#ifdef WIN32
//	this->refercounter = 0;
//#else
//	atomic_set(&this->refercouter, 0);
//#endif
*/