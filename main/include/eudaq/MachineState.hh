#ifndef EUDAQ_INCLUDED_MachineState
#define EUDAQ_INCLUDED_MachineState

#include <string>
#include <vector>
#include <map>
#include "eudaq/Status.hh"
#include "eudaq/TransportBase.hh"
#include "eudaq/Platform.hh"


namespace eudaq
{
	class DLLEXPORT MachineState

	{
	public:

		MachineState();
		int GetState(); //Returns the state of the whole machine 
		int GetState(ConnectionInfo id); // Returns the state of a single connection
		//int GetState(int index); //Returns the state of connection at index
		void SetState(ConnectionInfo id, Status* state); // Sets the connection associated with id to state. 
														//if the connection does not exist, add it to the array

		bool HasRunning(); //Returns true if there are running connections
		void RemoveState(ConnectionInfo id); //Removes a connection when it is disconnected....

		void Print();
	private:
		
		std::map<ConnectionInfo, Status> connection_status_info;

	};





}

#endif // EUDAQ_INCLUDED_MachineState
