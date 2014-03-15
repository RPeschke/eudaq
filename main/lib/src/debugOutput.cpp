#include "eudaq/debugOutput.hh"
#include <fstream>
#include <mutex>
#include <iostream>
#include "windows.h"
#include <map>
std::ofstream *out=nullptr;
std::mutex debug_mutex;
std::map<int,int> threadList;
int __currentThreadID=0;
using namespace std;

debugout::dbprint::dbprint( const char* functionName, int line ):printedEnd(false),m_threadID(0)
{

	m_funName=functionName;
	int threadid=GetCurrentThreadId();
	if (threadList.find(threadid)==threadList.end())
	{
		m_threadID=__currentThreadID;
		threadList[threadid]=__currentThreadID++;

	}else
	{
		m_threadID=threadList.find(threadid)->second;
	}
	
	std::string dummy;
	dummy="<"+m_funName + " line = "+std::to_string(line)+" thread id = "+ to_string(m_threadID) +">\n";

	print(dummy.c_str());

}

void debugout::dbprint::print(const char * output )
{
	//if (m_threadID==1)
	{
		debugout::print((string(m_threadID*10,' ')+ output).c_str());
	}
	


}

void debugout::dbprint::print( int line, const char * output )
{
	std::string dummy =std::string("<") + output +"  "+ std::to_string(line) +"/>\n";
	print(dummy.c_str());
}

void debugout::dbprint::print( int line , const char * variable_name,const std::string& t )
{
	std::string dummy=std::string(variable_name) +" = "+ t;
	for(int i=0;i<32;++i){
		std::replace(std::begin(dummy),std::end(dummy),char(i),'*');
	}

	print(line,dummy.c_str());
}

debugout::dbprint::~dbprint()
{
	if (!printedEnd)
	{
		printEnd();
	}
}

void debugout::dbprint::printEnd()
{

	std::string dummy ="</ "+m_funName+">\n\n\n";
	print(dummy.c_str());

	debugout::print(coutString.c_str());
	printedEnd=true;
}



void debugout::print( const char* output )
{
	std::unique_lock<std::mutex> m(debug_mutex);
	if (out==nullptr)
	{
		std::cout<<output;
	}else
	{
		*out<<output;
	}
	
	
}

void debugout::openfile( const char* fileName )
{
	if (out==nullptr)
	{
		out=new std::ofstream(fileName);
	}

}

void debugout::closeFile()
{
	if (out!=nullptr)
	{
		out->close();
		delete out;
		out=nullptr;
	}
}
