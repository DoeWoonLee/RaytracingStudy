#pragma once



#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&)=delete;				\
	CLASSNAME& operator = (const CLASSNAME&)=delete;		

#define DECLARE_SINGLETON(CLASSNAME)				 \
	NO_COPY(CLASSNAME)								 \
	private:										 \
	static CLASSNAME*	m_pInstance;				 \
	public:											 \
	static CLASSNAME*	GetInstance( void ) noexcept;\
	static void DestroyInstance( void );			 \
    private:

#define IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = nullptr;			\
	CLASSNAME*	CLASSNAME::GetInstance( void )noexcept	{	\
	if(nullptr == m_pInstance) {							\
	m_pInstance = new CLASSNAME;							\
	}														\
	return m_pInstance;										\
	}														\
	void CLASSNAME::DestroyInstance( void ) {				\
	if(nullptr != m_pInstance)	{							\
	delete m_pInstance;										\
	m_pInstance = nullptr;									\
	}														\
	}