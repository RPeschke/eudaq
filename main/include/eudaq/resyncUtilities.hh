#ifndef resyncUtilities_h__
#define resyncUtilities_h__

//////////////////////////////////////////////////////////////////////////
// Compare Time stamps
#define Event_IS_EARLY -1
#define Event_IS_LATE 1
#define Event_IS_Sync 0

namespace eudaq{

	  inline int compareTLU2DUT(unsigned TLU_Trigger_Number, unsigned DUT_Trigger_number){
	  if (DUT_Trigger_number==TLU_Trigger_Number)
	  {
		  return Event_IS_Sync;	
	  }else if (DUT_Trigger_number>TLU_Trigger_Number)
	  {
		  return Event_IS_EARLY;
	  }
	  return Event_IS_LATE;



  }
  template <typename T>
  inline int compareTLU2DUT(T TLU_Trigger_Number, T DUT_Trigger_number){
	  if (DUT_Trigger_number==TLU_Trigger_Number)
	  {
		  return Event_IS_Sync;	
	  }else if (DUT_Trigger_number>TLU_Trigger_Number)
	  {
		  return Event_IS_EARLY;
	  }
	  return Event_IS_LATE;



  }

  template <typename T>
  inline int hasTimeOVerlaping(T eventBegin, T EventEnd, T TLUStart,T TLUEnd){



	  if (eventBegin<=TLUEnd)
	  {
		  if (EventEnd>=TLUStart)
		  {

			  	  /*

	                    | event start  |event End
	  ----------------------------------------->
	                                           t

                 | tlu start  | tlu End
	  ------------------------------------------>
											   t

	  */
			  return Event_IS_Sync;
		  }

		  	  /*

	    | event start  |event End
	  ----------------------------------------->
	                                           t

                               | tlu start  | tlu End
	  ------------------------------------------>
											   t

	  */
		  return Event_IS_EARLY;
	  }

	 
	  /*

	                     | event start  |event End
	  ----------------------------------------->
	                                           t

        | tlu start  | tlu End
	  ------------------------------------------>
											   t

	  */

	  return  Event_IS_LATE;
  }
}
#endif // resyncUtilities_h__
