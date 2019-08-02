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

/* DESCRIPTION: Checking Allocator overlapping memory allocations in several threads
 * and memory deallocations in a different thread from this in which memory has been
 * allocated.
 */

/*<testinfo>
test_generator="gens/core-generator -a \"--gpus=0\""
</testinfo>*/

#include <iostream>
#include <string.h>
#include "config.hpp"
#include "smpprocessor.hpp"
#include "system.hpp"
#include "allocator.hpp"

using namespace std;
using namespace nanos;
using namespace nanos::ext;

#define CHECK_VALUE 3456
#define TIMES 1000

int sizes[] = { 7, 17, 33, 63, 123 };
bool check = true;

void allocate( void *args );
void deallocate ( void *ptr );

void deallocate ( void *ptr )
{
   //cerr << "      thread [" << getMyThreadSafe() << " ]deallocating: " << ptr << endl;
   Allocator::deallocate( ptr );
}

void allocate( void *args )
{
   int num_pes = sys.getSMPPlugin()->getNumWorkers();
   int id = *((int *) args);

    WD *wg = getMyThreadSafe()->getCurrentWD();
   //cerr << "   start of thread id: " << id << endl;

   Allocator allocator;

   for ( int  n = 1; n <= TIMES; n++ ) {
      for ( unsigned int i = 0; i < (sizeof( sizes )/sizeof(int)); i++ ) {

         int *ptr = (int *) allocator.allocate( sizes[i] * sizeof(int) );
         if ( ptr == NULL ) check = false;

         //cerr << "      thread id[" << getMyThreadSafe() << "] allocates: " << ptr << endl;

         for ( int j = 0; j < sizes[i]; j++ ) ptr[j]=CHECK_VALUE; // INI
         for ( int j = 0; j < sizes[i]; j++ ) ptr[j]++; // INC
         for ( int j = 0; j < sizes[i]; j++ ) ptr[j]--; // DEC

         // Check result
         for ( int j = 0; j < sizes[i]; j++ ) {
            if ( ptr[j] != CHECK_VALUE ) exit(-1);
         }

         // Creating a work descriptor to deallocate ptr in other thread
         ThreadTeam &team = *getMyThreadSafe()->getTeam();
         WD * wd = new WD( new SMPDD( deallocate ), sizeof( void * ), __alignof__( void * ), ptr  );
         wg->addWork( *wd );
         wd->tieTo(team[(id+1)%num_pes]);
         sys.submit( *wd );

         // Waiting decendants (to overlapping deallocations)
         if ( ((n*i)%7) == 0 ) wg->waitCompletion();
      }
   }

   wg->waitCompletion();
   //nanos_team_barrier();
   //cerr << "   end of thread id: " << getMyThreadSafe() << endl;

}

int main ( int argc, char **argv )
{
   int num_pes = sys.getSMPPlugin()->getNumWorkers();
   int id[num_pes];

   //cout << "Starting test... " << endl;

   // Work Group affiliation
   WD *wg = getMyThreadSafe()->getCurrentWD();

   ThreadTeam &team = *getMyThreadSafe()->getTeam();
   for ( int i = 0; i < num_pes; i++ ) {
      id[i] = i;
      WD * wd = new WD( new SMPDD( allocate ), sizeof( int ), __alignof__( int ), &id[i]  );
      wg->addWork( *wd );
      wd->tieTo(team[i]);
      sys.submit( *wd );
   }

   // barrier (kind of)
   wg->waitCompletion();

   //cout << "... test finish." << endl;

   if (check) { return 0; } else { return -1; }
}
