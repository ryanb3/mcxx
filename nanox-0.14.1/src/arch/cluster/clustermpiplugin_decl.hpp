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

#ifndef CLUSTERMPIPLUGIN_DECL_H
#define CLUSTERMPIPLUGIN_DECL_H

#include "plugin.hpp"
#include "system_decl.hpp"
#include "clusternode_decl.hpp"
#include "gasnetapi_fwd.hpp"
#include "remoteworkdescriptor_fwd.hpp"

namespace nanos {
namespace ext {

class ClusterMPIPlugin : public ArchPlugin
{
      GASNetAPI *_gasnetApi;

      unsigned int _numPinnedSegments;
      void ** _pinnedSegmentAddrList;
      std::size_t * _pinnedSegmentLenList;
      unsigned int _extraPEsCount;
      std::string _conduit;
      std::size_t _nodeMem;
      bool _allocFit;
      bool _allowSharedThd;
      bool _unalignedNodeMem;
      int _gpuPresend;
      int _smpPresend;
      System::CachePolicyType _cachePolicy;
      std::vector<ext::ClusterNode *> *_nodes;
      ext::SMPProcessor *_cpu;
      ext::SMPMultiThread *_clusterThread;
      std::size_t _gasnetSegmentSize;

   public:
      ClusterMPIPlugin();
      virtual void config( Config& cfg );
      virtual void init();

      void prepare( Config& cfg );
      std::size_t getNodeMem() const;
      int getGpuPresend() const;
      int getSmpPresend() const;
      System::CachePolicyType getCachePolicy ( void ) const;
      RemoteWorkDescriptor * getRemoteWorkDescriptor( int archId );
      bool getAllocFit() const;
      bool unalignedNodeMemory() const;
      virtual int initNetwork(int *argc, char ***argv);


      virtual void startSupportThreads();
      virtual void startWorkerThreads( std::map<unsigned int, BaseThread *> &workers);
      virtual void finalize();

      virtual ProcessingElement * createPE( unsigned id, unsigned uid );
      virtual unsigned getNumThreads() const; 
      void addPEs( PEMap &pes ) const;
      virtual void addDevices( DeviceList &devices ) const {}
      virtual unsigned int getNumPEs() const;
      virtual unsigned int getMaxPEs() const;
      virtual unsigned int getNumWorkers() const;
      virtual unsigned int getMaxWorkers() const;
      virtual BaseThread *getClusterThread() const;
};

} // namespace ext
} // namespace nanos


#endif /* CLUSTERMPIPLUGIN_DECL_H */
