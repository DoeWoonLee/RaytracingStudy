#pragma once


// 이 친구를 상속받는 친구들은
// 소멸자에서 메모리 해제를 다 해줘야함
// delete this가 부모에서 호출되기 때문에!
 class IBaseClass
{
public:
	IBaseClass() :m_Ref(0) {}
protected:
	virtual ~IBaseClass() {}
public:
	void AddRef(void) { m_Ref++; }
	bool Release(void) {
		if (m_Ref == 0)
		{
			delete this;
			return true;
		}
		else
		{
			m_Ref--;
		}
		return false;
	}
private:
	uint32_t m_Ref;
};