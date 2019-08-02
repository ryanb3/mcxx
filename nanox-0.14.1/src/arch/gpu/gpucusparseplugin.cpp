/*************************************************************************************/
/*      Copyright 2015 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include "plugin.hpp"
#include "system_decl.hpp"

#include <iostream>

#include <cuda_runtime.h>

#ifdef NANOS_GPU_USE_CUDA32
// Cannot include cublas.h, as the redeclaration of two CUBLAS functions makes the
// compiler crash because we are treating warnings as errors
//#include <cublas.h>
extern void cusparseInit();
#else
#include <cusparse.h>
#endif


namespace nanos {
namespace ext {

class GPUCusparsePlugin : public Plugin
{
   public:
      GPUCusparsePlugin() : Plugin( "cuSPARSE Warmup Plugin", 1 ) {}

      void config( Config& cfg ) {}

      void init()
      {
#ifdef NANOS_GPU_USE_CUDA32
         cusparseInit();
#endif
         cudaFree( NULL );
      }
};

}
}

DECLARE_PLUGIN("arch-gpucusparse",nanos::ext::GPUCusparsePlugin);
