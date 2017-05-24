// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#ifndef __DVH_TXT_FILE_WRITER_H
#define __DVH_TXT_FILE_WRITER_H


#include "rttbDVH.h"
#include "../rttbDVHWriterInterface.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			/*! @class DVHTxtFileWriter
			@brief Writes DVHs to simple text files.
            @deprecated Please use DVHXMLFileWriter.
			*/
			class DVHTxtFileWriter: public DVHWriterInterface
			{
			public:
				typedef core::DVH::DataDifferentialType DataDifferentialType;
				typedef core::DVH::DVHPointer DVHPointer;

			private:
				FileNameString _fileName;
				DVHType _dvhType;

			public:
				/*! @brief Constructor
				@param aFileName a .txt file name to write the DVH to aDVHType: DIFFERENTIAL or CUMULATIVE.
				*/
				DVHTxtFileWriter(FileNameString aFileName, DVHType aDVHType);

				void setFileName(FileNameString aFileName);
				FileNameString getFileName() const;

				void setDVHType(DVHType aDVHType);
				DVHType getDVHType() const;

				/*! @brief Write aDvh to txt file with the name: _fileName
				@exception NullPointerException Thrown if _aDvh is NULL
				@exception InvalidParameterException Thrown if _fileName invalid: could not open;
				@exception InvalidParameterException Thrown if normalized is true;
				or if _dvhType invalid: only DIFFERENTIAL or CUMULATIVE is accepted!
				@exception Exception thrown if dvh init error
				*/
				void writeDVH(DVHPointer aDvh, bool normalized = false);
			};
		}
	}
}
#endif
