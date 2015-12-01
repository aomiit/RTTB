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

#ifndef __MASKED_DOSE_ITERATOR_INTERFACE_NEW_H
#define __MASKED_DOSE_ITERATOR_INTERFACE_NEW_H


#include <string>
#include <vector>
#include <exception>

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbMaskAccessorInterface.h"

namespace rttb
{
	namespace core
	{

		/*! @class MaskedDoseIteratorInterface
			@brief Give access to masked dose data.
		*/
		class MaskedDoseIteratorInterface: public DoseIteratorInterface
		{
		public:
			typedef boost::shared_ptr<MaskAccessorInterface> MaskAccessorPointer;
			typedef DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
			typedef boost::shared_ptr<MaskedDoseIteratorInterface> MaskedDoseIteratorPointer;

		private:
			MaskedDoseIteratorInterface(const MaskedDoseIteratorInterface&);
			MaskedDoseIteratorInterface& operator=(const MaskedDoseIteratorInterface&);

		protected:
			/*! @brief Mask that is to be applied to currently loaded dose*/
			MaskAccessorPointer _spMask;

		public:
			/* Constructor
			@pre core::GeometricInfo represents the same geometric space for both mask and dose,
			i.e. both live on the same data grid. Both accessors need to be valid.
			*/
			MaskedDoseIteratorInterface(MaskAccessorPointer aMaskAccessor, DoseAccessorPointer aDoseAccessor);

			virtual ~MaskedDoseIteratorInterface() {};

			inline MaskAccessorPointer getMaskAccessor() const
			{
				return _spMask;
			};

			/* Return doseValue*voxelFraction for the current position
			*/
			virtual DoseTypeGy getCurrentMaskedDoseValue() const = 0;
		};
	}
}
#endif