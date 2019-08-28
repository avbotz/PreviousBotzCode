#include "NiFpga_DunkFlips.h"
#include <stdio.h>
int main()
{
   NiFpga_Session session;
   /* must be called before any other calls */
   NiFpga_Status status = NiFpga_Initialize();
   if (NiFpga_IsNotError(status))
   {
      /* opens a session, downloads the bitstream, and runs the FPGA */
      NiFpga_MergeStatus(&status, NiFpga_Open(NiFpga_DunkFlips_Bitfile,
                                              NiFpga_DunkFlips_Signature,
                                              "RIO0",
                                              0,
                                              &session));
      printf("status %i\n", status);
      if (NiFpga_IsNotError(status))
      {
	 printf("status %i\n", status);
	  int32_t data[3];
	  size_t element;
         /* must close if we successfully opened */
		NiFpga_MergeStatus(&status,NiFpga_ReadFifoI32(session,
		                                 NiFpga_DunkFlips_TargetToHostFifoI32_Output,
		                                 data,
		                                 3,
		                                 -1,
		                                 &element));
	         NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
		 printf("data %i %i %i", data[0], data[1], data[2]);
      }
      /* must be called after all other calls */
      NiFpga_MergeStatus(&status, NiFpga_Finalize());
   }
   return status;
}
