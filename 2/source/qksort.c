/*****************************************************************************
*                                                                            *
*  ------------------------------- qksort.c -------------------------------  *
*                                                                            *
*****************************************************************************/ 
    
    
#include <stdlib.h>
#include <string.h>
    
#include "sort.h"
    
/*****************************************************************************
*                                                                            *
*  ------------------------------ compare_int -----------------------------  *
*                                                                            *
*****************************************************************************/ 

{
	
/*****************************************************************************
*                                                                            *
*  Compare two integers (used during median-of-three partitioning).          *
*                                                                            *
*****************************************************************************/ 
	    
		
	
	else if (*(const int *)int1 < *(const int *)int2)
		
	
	else
		



/*****************************************************************************
*                                                                            *
*  ------------------------------- partition ------------------------------  *
*                                                                            *
*****************************************************************************/ 

		      (const void *key1, const void *key2))
{
	
	
	
	
/*****************************************************************************
*                                                                            *
*  Allocate storage for the partition value and swapping.                    *
*                                                                            *
*****************************************************************************/ 
	    
		
	
		
		
	
	
/*****************************************************************************
*                                                                            *
*  Use the median-of-three method to find the partition value.               *
*                                                                            *
*****************************************************************************/ 
	    
	
	
	
	
	
/*****************************************************************************
*                                                                            *
*  Create two partitions around the partition value.                         *
*                                                                            *
*****************************************************************************/ 
	    
	
	
		
   /**************************************************************************
   *                                                                         *
   *  Move left until an element is found in the wrong partition.            *
   *                                                                         *
   **************************************************************************/ 
		    
		do {
			
		
		
   /**************************************************************************
   *                                                                         *
   *  Move right until an element is found in the wrong partition.           *
   *                                                                         *
   **************************************************************************/ 
		    
		do {
			
		
		
			
      /***********************************************************************
      *                                                                      *
      *  Stop partitioning when the left and right counters cross.           *
      *                                                                      *
      ***********************************************************************/ 
			    
		
		
		else {
			
      /***********************************************************************
      *                                                                      *
      *  Swap the elements now under the left and right counters.            *
      *                                                                      *
      ***********************************************************************/ 
			    
			
			
		
	
	
/*****************************************************************************
*                                                                            *
*  Free the storage allocated for partitioning.                              *
*                                                                            *
*****************************************************************************/ 
	    
	
	
/*****************************************************************************
*                                                                            *
*  Return the position dividing the two partitions.                          *
*                                                                            *
*****************************************************************************/ 
	    



/*****************************************************************************
*                                                                            *
*  -------------------------------- qksort --------------------------------  *
*                                                                            *
*****************************************************************************/ 

	    (const void *key1, const void *key2))
{
	
	
/*****************************************************************************
*                                                                            *
*  Stop the recursion when it is not possible to partition further.          *
*                                                                            *
*****************************************************************************/ 
	    
		
   /**************************************************************************
   *                                                                         *
   *  Determine where to partition the elements.                             *
   *                                                                         *
   **************************************************************************/ 
		    
			
		
   /**************************************************************************
   *                                                                         *
   *  Recursively sort the left partition.                                   *
   *                                                                         *
   **************************************************************************/ 
		    
			
		
   /**************************************************************************
   *                                                                         *
   *  Recursively sort the right partition.                                  *
   *                                                                         *
   **************************************************************************/ 
		    
			
	
	


