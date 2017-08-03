#include "cl/devices/cl_engines.h"


void engine_cleanup (engine t)
{
  clReleaseProgram (t.program);
  clReleaseKernel (t.kernel);
  clReleaseCommandQueue (t.commands);
  clReleaseContext (t.context);
}
