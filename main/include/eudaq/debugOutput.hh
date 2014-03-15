#ifndef debugOutput_h__
#define debugOutput_h__
#include <string>
#include "Platform.hh"
#include <iosfwd>
#include <algorithm>

#define __FUNCIONSTART__ debugout::dbprint ____debug(__FUNCTION__,__LINE__)
#define DEBUGBEGIN(X) debugout::dbprint ____debug(X,__LINE__)
#define  __DEBUG_VARIABLE__(X) ____debug.print(__LINE__,#X,X)
#define __DEBUG_PRINT(X) ____debug.print(__LINE__,X)
#define  DEBUGEND ____debug.printEnd()

namespace debugout{
	void print(const char* output);
	void openfile(const char* fileName);
	void closeFile();

	class DLLEXPORT dbprint{
	public:
		dbprint(const char* functionName, int line);
		~dbprint();
		template <typename T>
		void print(int line , const char * variable_name,const T& t){
			std::stringstream ss;
			ss << t;
			
			std::string dummy=std::string(variable_name) +" = "+ ss.str();
			print(line,dummy.c_str());
		}
		void print(int line , const char * variable_name,const std::string&  t);
		template <typename T>
		void print(int line , const T& t){
			std::stringstream ss;
			ss << t;

			std::string dummy= ss.str();
			print(line,dummy.c_str());
			
		}
		void print(const char* output);
		void print(int line, const char * output);
		void printEnd();
	private:
		std::string m_funName,coutString;
		int m_threadID;
		bool printedEnd;
	};
	

}

#endif // debugOutput_h__
