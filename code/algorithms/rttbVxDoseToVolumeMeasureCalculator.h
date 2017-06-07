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
// @version $Revision: 1674 $ (last changed revision)
// @date    $Date: 2017-01-27 10:34:46 +0100 (Fr, 27 Jan 2017) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#ifndef __DV_DOSE_TO_VOLUME_MEASURE_CALCULATOR_H
#define __DV_DOSE_TO_VOLUME_MEASURE_CALCULATOR_H

#include "rttbDoseToVolumeMeasureCalculator.h"

namespace rttb
{

	namespace algorithms
	{
		class RTTBAlgorithms_EXPORT VxDoseToVolumeMeasureCalculator : public DoseToVolumeMeasureCalculator {

		private:
			DoseStatisticType _minimum;

		public:			
			VxDoseToVolumeMeasureCalculator(const std::vector<double>& precomputeVolumeValues,
				const DoseTypeGy& referenceDose, const core::DoseIteratorInterface::DoseIteratorPointer doseIterator, DoseToVolumeMeasure::complexStatistics name);

		private:
			void computeSpecificValue(double xAbsolute);			
		};
	}
}


#endif