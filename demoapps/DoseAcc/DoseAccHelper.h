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

#ifndef __DOSE_ACC_HELPER_H
#define __DOSE_ACC_HELPER_H

#include "DoseAccApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{
			/**loads the dose from a file. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDose(const std::string& fileName,
			        const rttb::apps::doseAcc::ApplicationData::LoadingStyleArgType& args);

			/**loads the dose from a file using the dicom dose generator. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadDicomDose(const std::string& fileName);
			/**loads the dose from a path using the helax io dose generator. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadHelaxDose(const std::string& path);
			/**loads the dose from a file stored in an ITK supported data format. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadITKDose(const std::string& fileName);
			/**loads the dose from a file stored in Virtuos data format. Throws exception if loading fails*/
			core::DoseAccessorInterface::DoseAccessorPointer loadVirtuosDose(const std::string& fileName,
			        const std::string& planFileName);

			ApplicationData::RegistrationType::Pointer loadRegistration(const std::string& fileName);

			/**Containes the business logic for the accumulation of the doses and the storing of the result.
			 Uses appData for the input data and the correct configuration.*/
			void processData(ApplicationData& appData);

		}
	}
}


#endif
