#pragma once


// �� ģ���� ��ӹ޴� ģ������
// �Ҹ��ڿ��� �޸� ������ �� �������
// delete this�� �θ𿡼� ȣ��Ǳ� ������!
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