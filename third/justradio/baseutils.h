// --------------------------------------------------------------------------------------
// Copyright (c) 2000-2005 Zalsoft Inc
// Copyright: zalsoft Inc
// --------------------------------------------------------------------------------------

#ifndef __BASE_UTILS_H__
#define __BASE_UTILS_H__

#include <os.h>
#include <baselib.h>
#include <stdarg.h>
#include <QString>
#include <unistd.h>

#define HINSTANCE   int

//---------------------------------------------------------------------------------------
#ifdef _UNICODE
    #pragma message ("building unicode version of the libray")
#endif//

#ifdef _DEBUG
#   ifdef WINDOWS
		INLN void _cdecl TRACEX(const tchar lpszFormat, ...)
		{
			static char szBuffer[1024];

			va_list args;
			va_start(args, lpszFormat);
			uint32_t nBuf = ::_vsnprintf(szBuffer, sizeof(szBuffer) / sizeof(char), lpszFormat, args);
#       ifdef _CONSOLE
			printf(szBuffer);
#       else
			::OutputDebugStringA(szBuffer);
#   endif //
			va_end(args);
		}
#else
		#define TRACEX printf
#endif
    #else //
        #ifdef WIN32
            #define TRACEX
        #else
            #ifdef _CONSOLE
                #define TRACEX printf
            #else
                #define TRACEX
            #endif //
        #endif

    #endif //

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
#define DECLARE_TIME()      static LARGE_INTEGER _Start;\
                                static LARGE_INTEGER _Freq;\
                                static LARGE_INTEGER _End;\
                                QueryPerformanceFrequency(&_Freq);\
                                QueryPerformanceCounter(&_Start)
#define WAIT_HERE(msec)     QueryPerformanceCounter(&_End);\
							while( (_End.QuadPart - _Start.QuadPart)*1000 / _Freq.QuadPart < msec)\
							{\
								 QueryPerformanceCounter(&_End);\
								 Sleep(0);\
							}
#define TRACETIME(msec)     QueryPerformanceCounter(&_End);\
							TRACEX(_T("%s %d ms\r\n"),msec ,(_End.QuadPart - _Start.QuadPart)*1000 / _Freq.QuadPart)
#define READ_CLOCK()        QueryPerformanceCounter(&_Start)

namespace baseg{

//---------------------------------------------------------------------------------------
INLN void StripEol(char* ln){
    while(ln[0] && (ln[strlen(ln)-1]=='\n' ||ln[strlen(ln)-1]=='\r'))
        ln[strlen(ln)-1]=0;
}

//---------------------------------------------------------------------------------------
template <class T> class InstCount
{
	static int32_t _lCount;
public:
	InstCount(){
		TRACEX(_T("%d\r\n"),_lCount);
		_lCount++;
	}
	~InstCount(){
		_lCount--;
		TRACEX(_T("%d\r\n"),_lCount);
	}

};
template <class T> int32_t              InstCount<T>::_lCount = 0;

//---------------------------------------------------------------------------------------
template <class T>u_bool32 _Noval(T& val)
{
    if(sizeof(val)==1)return (val == 0xFF);
    if(sizeof(val)==2)return (val == 0xFFFF);
    if(sizeof(val)==4)return (val == 0xFFFFFFFF);
    if(sizeof(val)==8)return (val == 0xFFFFFFFFFFFFFFFF);
    return 0;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
template <size_t SZ=256> class Tsstring
{
public:
    Tsstring(){_t[0]=0;}
    Tsstring(const char* o){strcpy(_t, o);}
    const char* c_str(){return _t;}

private:
    char   _t[SZ];
};

typedef Tsstring<16>    S16char;
typedef Tsstring<64>    S64char;
typedef Tsstring<128>   S128char;
typedef Tsstring<256>   S256char;

//---------------------------------------------------------------------------------------
class CBitArray
{
public:
	CBitArray(int32_t size){
		_size =     (size+ 7) / 8;
		_size = (_size * 3 + 3) & 0xFFFFFFFC;
		_storage = new uint8_t[_size];
		memset(_storage, 0, _size);
	}
	virtual ~CBitArray()
	{
		delete[] _storage;
	}

	void Clear(){memset(_storage, 0, _size);}

	u_bool32 operator[](int32_t i){
		return (_storage[ i>> 3 ] & (1 << ( i& 7))) != 0;
	}
	void Set(int32_t i){
		_storage[ i >> 3 ] |=  (1 << ( i & 7 ));
	}
	void Reset(int32_t i=0){
		if(0==i)
			::memset(_storage, 0, _size);
		else
			_storage[ i >> 3 ] &= ~(1 << ( i & 7 ));
	}
	int32_t Size(){return _size;}
	uint8_t* Buffer(int32_t& bytes){bytes = _size; return _storage;}
	void  Merge(CBitArray& pByArr){
		int32_t cnt;
		uint8_t* pDSrc = (uint8_t*)pByArr.Buffer(cnt);
		uint8_t* pDDst = Buffer(cnt);
		while(--cnt>=0){
			*pDDst++ |= *pDSrc++;
		}
	}
private:
    uint8_t*	_storage;
    int32_t		_size;
};

//------------------------------------------------------------------------------
template <class T> class Allocator
{
	T*	_p;
	public:
	Allocator(int32_t ins){
		_p = new T[ins];
		::memset(_p, 0, sizeof(T)*ins);
	}
	Allocator(){
		_p = new T;
	}
	operator T*(){return _p;}
};

//---------------------------------------------------------------------------------------
INLN int32_t BitsRoundByLong(int32_t nSetCount)
{
    int32_t rval =   (nSetCount + 7) / 8;    // bytes
    rval = (rval * 3 + 3) & 0xFFFFFFFC;  // round by mod4
    return rval;
}

//---------------------------------------------------------------------------------------
class Cfms
{
public:
	Cfms(const char* p,...){
		va_list args;
		va_start(args, p);
		_sc.vsprintf( p, args);
		va_end(args);
	}
	operator const char*(){return _sc.toUtf8();}
	static QString _sc;
};

#define	MKSTR	(const char*)Cfms

#define	MKQTR	(const QString&)Cfms

//---------------------------------------------------------------------------------------
class  FileWrap
{
public:
    FileWrap(FILE* pf=0):_pf(pf),_openhere(false),_store(false),_fpos(0){};
    virtual ~FileWrap(){
        if(_pf && _openhere)
            ::fclose(_pf);
        _pf=0;
    }

    u_bool32    Open(const char* file, const char* how){
        char loco[256];
        strcpy(loco, file);

#ifndef _CFG_GNU_LINUX
        // replace / with
        char* ps = loco;
        while(*ps)
        {
            if(*ps=='/')
                *ps='\\';
            ++ps;
        }
#endif //

        _pf = ::fopen(loco, how);
        _openhere =  _pf!=0;
        _store = (how[0]==('w'));
        return _openhere;
    }
    void Close(){
        if(_pf)
            ::fclose(_pf);
        _pf=0;
    }

    void Pos(int32_t pos){
        ::fseek(_pf,pos,SEEK_SET);
    }

    void ReStart(){
        ::fseek(_pf,0,SEEK_SET);
    }

    uint32_t GetPos(){
        return ::ftell(_pf);
    }

    u_bool32 IsEof(){
        return feof(_pf);
    }


    void test()
    {

        union SS
        {
            int16_t    s[64];
            char       c[256];
            int32_t    i[128];
        }s;




        int32_t pos = ftell(_pf);
        ::fread(&s,1,sizeof(s),_pf);
        ::fseek(_pf,pos,SEEK_SET);
    }


    uint32_t Getlength(){
        ::fseek(_pf,0,SEEK_END);
        uint32_t flength = ftell(_pf);
        ::fseek(_pf,0,SEEK_SET);
        return flength;
    }

    u_bool32 ReadLine(char* line, int32_t len){
        return ::fgets(line,len,_pf)!=0;
    }

    template <class T> u_bool32 Serialize(T& t){
        if(_store) return Write(t);
        return Read(t);
    }

    template <class T> u_bool32 Write(T& t){
        int32_t lenr = ::fwrite(&t,1,sizeof(T),_pf);
        return (lenr == sizeof(T));
    }

    template <class T> u_bool32 Read(T& t) {
        int32_t lenr = ::fread(&t,1,sizeof(T),_pf);
        _fpos=ftell(_pf);
        return (lenr == sizeof(T));
    }

    size_t Read(void* pv, size_t len) {
        size_t lenr= ::fread(pv,1,len,_pf);
        _fpos=ftell(_pf);
        return (lenr);
    }

    size_t Write(void* pb, size_t len) {
        return  ::fwrite(pb,1,len,_pf);
    }

    size_t Serialize(void* pv, int32_t len){
        if(_store)
            return Write(pv,len);
        return Read(pv,len);
    }

    size_t WriteTextAsIs(const char* t){
        size_t l = strlen(t);
        return fwrite(t, 1, l,  _pf);
    }


	size_t WriteStr(const char* t){
		size_t l = strlen(t);
		::fwrite(&l, 1, sizeof(int16_t), _pf);
		return fwrite(t, 1, l,  _pf);
	}

    size_t ReadStr(char* t){
        int16_t len;
        ::fread(&len, 1, sizeof(int16_t), _pf);
        size_t rd = ::fread((void*)t, 1, len,  _pf);
        _fpos=ftell(_pf);
        t[rd]=0;
        return rd=len;
    }
    FILE*   _pf;
    u_bool32    _openhere;
    u_bool32    _store;
    uint32_t _fpos;
};

//---------------------------------------------------------------------------------------


#endif
//-----------------------------------------------------------------------------------
class  CDirChange
{
public:
    CDirChange(const char * newDir){
        ::getcwd(_olddir,PATH_MAX);
        if(newDir)
        {
            strcpy(_curdir, newDir);
            if( _curdir[strlen(_curdir)-1] == ('\\') ||
                _curdir[strlen(_curdir)-1] == ('/'))
            {
                _curdir[strlen(_curdir)-1] = 0;
            }
            _rv = chdir(_curdir);
        }
        getcwd(_curdir,PATH_MAX);
    }
    ~CDirChange(){
        chdir(_olddir);
    }
    const char* CurDir(){return _curdir;};
    const char* OldDir(){return _olddir;};
    char* UseObj(){
        return _curdir;
    }
    int32_t InError(){return _rv;}
private:
    char _olddir[PATH_MAX];
    char _curdir[PATH_MAX];
    int32_t   _rv;
};

//----------------------||----------------------------------------------------------------


//----------------------||----------------------------------------------------------------
template <class T>class ResetExit
{
    T&  _ref;
public:
    ResetExit(T& ref):_ref(ref){}
    ~ResetExit(){_ref=0;}
};

//----------------------||----------------------------------------------------------------
template <class T>class DeleteExit
{
    T&  _ref;
public:
    DeleteExit(T& ref):_ref(ref){}
    ~DeleteExit(){delete _ref;}
};

//----------------------|functors|-----------------------------------------------------
template <class T>void Reset(T t)
{
    ::memset(&t,0,sizeof(T));
}

template <class T> void Copy(T&a , T&b)
{
    ::memcpy(&a,&b,sizeof(T));
}

INLN void SCopy(char* a , char* b, int32_t sza)
{
    ::strncpy(a, b, sza-1);
    a[sza-1]=0;
}

template <class T>
class ObjSizeDump
{
public:
    ObjSizeDump(const char* name){
        TRACEX("%s: %d \r\n",name, sizeof(T));
    }
    virtual ~ObjSizeDump(){}
};





#define OBJ_DMP(_x_)    ObjSizeDump<_x_>   _##_x_(#_x_)


#define ITOFX(x) ((x) << 8)			    // Integer to fixed point
#define FTOFX(x) ((x) * 256)			    // Float to fixed point
#define DTOFX(x) ((x) * 256)			    // Double to fixed point
#define FXTOI(x) ((x) >> 8)			    // Fixed point to integer
#define FXTOF(x) ((real_t) (x) / 256)		// Fixed point to real_t
#define MULFX(x,y) ( ((y) * (x)) >> 8 )	// Multiply a fixed by a fixed
#define DIVFX(x,y) ((y << 8) / (x))    	// Divide a fixed by a fixed


#ifdef _CFG_WIN32_WIN
struct Cms
{
// Attributes
	enum blockUsage
	{
		freeBlock,    // memory not used
		objectBlock,  // contains a CObject derived class object
		bitBlock,     // contains ::operator new data
		crtBlock,
		ignoredBlock,
		nBlockUseMax  // total number of usages
	};

	_CrtMemState m_memState;
	size_t m_lCounts[nBlockUseMax];
	size_t m_lSizes[nBlockUseMax];
	size_t m_lHighWaterCount;
	size_t m_lTotalCount;

	Cms()
	{
		::memset(this, 0, sizeof(*this));
	}


// Operations
	void Checkpoint()
	{
		_CrtMemCheckpoint(&m_memState);
		UpdateData();
	}

	u_bool32 Difference(const Cms& oldState,
					const Cms& newState)
	{

		int32_t nResult = _CrtMemDifference(&m_memState, &oldState.m_memState, &newState.m_memState);
		UpdateData();
		return nResult != 0;
	}

	void UpdateData()
	 {
		for(int32_t i = 0; i < nBlockUseMax; i++)
		{
			m_lCounts[i] = m_memState.lCounts[i];
			m_lSizes[i]  = m_memState.lSizes[i];
		}
		m_lHighWaterCount = m_memState.lHighWaterCount;
		m_lTotalCount = m_memState.lTotalCount;
	}


	// Output to afxDump
	void DumpStatistics() const
	{
		_CrtMemDumpStatistics(&m_memState);
	}
	void DumpAllObjectsSince() const
	{
		_CrtMemDumpAllObjectsSince(&m_memState);
	}
};

/*//////////////////////////////////////////////////////////////////////////////
	macros to count instances of a given object
	produces traces  indicating object count


   BEGIN_OBJCOUNT(CYourClass) // in cpp file add this at the top of file

   class CYourClass
   {
       CYourClass()
       {
            OBJPUT("My Object Name")    ; // do this for copy constructor too
       }
       ~CYourClass()
       {
           OBJGET("My Object Name")    ;
       }

       DECLARE_OBJCOUNT;    // add this line in your class
   };
*////////////////////////////////////////////////////////////////////////////////

#define DECLARE_OBJCOUNT\
                private:\
                static int32_t m_ObjCount;\
#define BEGIN_OBJCOUNT(_className_)\
                    int32_t _className_::m_ObjCount=0;\
#define OBJPUT(_obj_)	do{\
							::InterlockedIncrement(&m_ObjCount);\
							ALTTRACE(_obj_, m_ObjCount);\
						}while(0);\
#define OBJGET(_obj_)		do{\
							ALTTRACE(_obj_,  m_ObjCount);\
							::InterlockedDecrement(&m_ObjCount);\
						}while(0);\

/*/////////////////////////////////////////////////////////////////////////////////
	MCCROS To CHECK MEMORY LEAKS FOR A CLASS OR PORTION OF A CODE: uses as explained


	class CYourClass
	{
		CYourClass()
		{
			MINIT0(somename);
		}


		~CYourClass()
		{

		}

		 void Foo();

		MDECLARE(somename);

	};


	void CYourClass::Foo()
	{
		MSTART(somename , "Checking meory in funtion ...");        // the same name as in MDECLARE

		  .. lot's of code

		MEND(somename); /// produces memeory dump if is any


		// you repeat this sequence

	}


  // FOR CODE OUT OF CLASS"

  SomeFunction()
  {
      MSTART(xyz,"Checking point line xxx");

    .. lot's of code


      MEND(xyz); /// produces memeory dump if is any
  }
*////////////////////////////////////////////////////////////////////////////////////////////////

class  MEMCHECK
{
public:
	MEMCHECK(char* pLine)
	{
		sprintf(m_line,"\r\n---------------Memory Dump For %s -----------\r\n",pLine);
		oMem.Checkpoint();
	}

	~MEMCHECK()
	{
		TRACEX(m_line);
		newMem.Checkpoint();
		if(diffMem.Difference(oMem,newMem))
		{

			TRACEX("\r\n");
			diffMem.DumpStatistics();
			TRACEX("\r\n________________________________________\r\n");
			TRACEX("\r\n Dumping now all objects\r\n");
			oMem.DumpAllObjectsSince();
			TRACEX("\r\n________________________________________\r\n");
		}
	}
	Cms   oMem,newMem,diffMem;
	char m_line[200];
};
#define MEM_START(xyz,_l_)MEMCHECK* xyz = new MEMCHECK(_l_)
#define MEM_END(xyz)      if(xyz!=NULL) delete xyz;xyz=NULL;
#define MDECLARE(xyz)     MEMCHECK* xyz
#define MINIT0(xyz)       xyz = NULL
#define MINIT(xyz)        MEMCHECK* xyz = NULL
#define MSTART(xyz,_l_)   if(xyz == NULL)xyz = new MEMCHECK(_l_)
#define MEND(xyz)         if(xyz!=NULL) delete xyz;xyz=NULL;
#else
#define MEM_START(xyz,_l_)
#define MEM_END(xyz)
#define MDECLARE(xyz)
#define MINIT0(xyz)
#define MINIT(xyz)
#define MSTART(xyz,_l_)
#define MEND(xyz)

#endif //


};//baseg

#ifdef _USE_BASEG
namespace baseg{
    INLN int32_t nmsp_dummy(){return 0;};
};
    using namespace baseg;
#endif //

