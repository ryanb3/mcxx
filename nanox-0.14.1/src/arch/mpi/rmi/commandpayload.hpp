
#ifndef COMMAND_PAYLOAD_HPP
#define COMMAND_PAYLOAD_HPP

#include "commandid.hpp"

#include <cstddef>
#include <mpi.h>

namespace nanos {
namespace mpi {
namespace command {

/*
 * Maybe we can turn the optional code into a template argument, 
 * so that this is not always needed.
 * However, we would need an additional dispatcher listening for
 * messages of strictly this type, as we need to allocate a buffer
 * for this purpose with the proper size.
 *
 * Since most of the commands only use the id and, for the task
 * execution, the command id and the task id, with two ints will
 * be enough for now.
 */
class CommandPayload {
	private:
		int _id; //!< Used to identify which command is being received
		int _code; //! < Optional. May include additional information.

		/**
		 * Custom MPI datatype for GenericCommand
		 * This is necessary since we must compute
		 * the displacement of the attributes
		 * with respect to the object's base address
		 */
		static MPI_Datatype _type;
	public:
		void initialize()
		{
			_id = OPID_INVALID;
			_code = 0;
		}

		void initialize( int id )
		{
			initialize();
			_id = id;
		}

		void initialize( int id, int code )
		{
			initialize(id);
			_code = code;
		}

		int getId() const
		{
			return _id;
		}

		int getCode() const
		{
			return _code;
		}

		static void initDataType()
		{
			int blocklen  = 2;
			MPI_Aint disp = offsetof(CommandPayload,_id);

			MPI_Type_create_hindexed(1, &blocklen, &disp, MPI_INT, &_type );
			MPI_Type_commit( &_type );
		}

		static void freeDataType()
		{
			MPI_Type_free( &_type );
		}

		static MPI_Datatype getDataType()
		{
			return _type;
		}
};

} // namespace command
} // namespace mpi
} // namespace nanos

#endif // COMMAND_PAYLOAD_HPP

