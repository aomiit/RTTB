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
#ifndef __GENERIC_MASKED_DOSE_ITERATOR_NEW_H
#define __GENERIC_MASKED_DOSE_ITERATOR_NEW_H

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbMaskedDoseIteratorInterface.h"
#include "rttbMaskVoxel.h"

#include "RTTBCoreExports.h"


namespace rttb
{
	namespace core
	{
		/*! @class GenericMaskedDoseIterator
			@brief This is a templated class representing a generic masked dose iterator for a VoxelizationPolicy.
			@see testing/GenericMaskedDoseIteratorTest.cpp for more information.
		*/
        class RTTBCore_EXPORT GenericMaskedDoseIterator : public MaskedDoseIteratorInterface
		{
		public:
			typedef MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;
			typedef MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef MaskedDoseIteratorInterface::MaskAccessorPointer MaskAccessorPointer;
			typedef MaskedDoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;

		private:

			typedef MaskVoxelList::const_iterator MaskVoxelListIterator;

			/*! The current index position of the vector _maskVoxelVec*/
			MaskVoxelListIterator _currentMaskPos;

			/*! vector of MaskVoxel, as defined in the voxelization*/
			MaskVoxelListPointer _maskVoxelVec;

			/*! the volume in cm^3 of the current dose voxel*/
			DoseVoxelVolumeType _currentVoxelVolume;


		public:

			GenericMaskedDoseIterator(MaskAccessorPointer aSpMask, DoseAccessorPointer aDoseAccessor)
				: MaskedDoseIteratorInterface(aSpMask, aDoseAccessor) {};

			/*! @brief Set the position on the first index. Use also as initialization.
			*/
			bool reset();

			/*! move to next mask position. The validity of the position is not checked here.
			*/
			void next();

			/*! @brief Volume of one voxel (in cm3)
				@exception InvalidParameterException  if a inhomogeneous grid is defined in the dose accessors, because
				these grids are currently not supported.
			*/
			DoseVoxelVolumeType getCurrentVoxelVolume() const;

			FractionType getCurrentRelevantVolumeFraction() const;

			inline MaskVoxelListPointer getMaskVoxelVec() const
			{
				return _maskVoxelVec;
			};

			/*! Check first if the position inside the maskedVoxelVector is valid. If so, check if the gridID at the
				current position in the MaskedVoxelVector is valid in the dose and mask grid.
			*/
			bool isPositionValid() const;

			/*! @brief get current VoxelGridID (on dose voxel grid)*/
			VoxelGridID getCurrentVoxelGridID() const;

			/*! @return current dose value multiplied by current volume fraction*/
			DoseTypeGy getCurrentMaskedDoseValue() const;

			/*! @return current dose value without masking*/
			DoseTypeGy getCurrentDoseValue() const;
		};
	}
}

#endif
