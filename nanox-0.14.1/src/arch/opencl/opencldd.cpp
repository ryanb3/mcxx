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

#include "opencldd.hpp"
#include "opencldevice.hpp"
#include "openclconfig.hpp"

using namespace nanos;
using namespace nanos::ext;

OpenCLDevice nanos::ext::OpenCLDev( "OPENCL" );

OpenCLDD * OpenCLDD::copyTo ( void *toAddr )
{
   OpenCLDD *dd = new ( toAddr ) OpenCLDD( *this );
   return dd;
}

void OpenCLDD::setOpenclStreamIdx(int streamIdx){
    _oclStreamIdx=streamIdx;
}

int OpenCLDD::getOpenCLStreamIdx(){
    return _oclStreamIdx;
}