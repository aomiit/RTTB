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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <string>
#include <vector>

#include "rttbStructure.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbITKImageMaskAccessor.h"
#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{
            class ApplicationData;
            

            void processData(ApplicationData& appData);

			/**@brief Search the label with the position from index
			@return a label from the list as string
			*/
			void removeSpecialCharacters(std::string& label);

            /**@brief create a mask with _rtStructureSet and _doseAccessor object.
            @return a mask object
            */
            core::MaskAccessorInterface::Pointer createMask(
                core::DoseAccessorInterface::Pointer doseAccessorPtr,
              rttb::core::Structure::Pointer structurePtr,
                bool strict);

            /**@brief write the mask into the outputfile
            @param Outputfilename
            */
            void writeMaskToFile(std::vector<core::MaskAccessorInterface::Pointer> maskVector,
                const std::string& outputFileName, bool voxelization);

            io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer addMultipleStructsToImage(
                std::vector<core::MaskAccessorInterface::Pointer> maskVector);
            io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer applyThresholdFilter(
                io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer itkImage);

		}
	}
}