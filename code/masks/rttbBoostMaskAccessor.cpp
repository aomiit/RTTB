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

#include "rttbBoostMaskAccessor.h"
#include "rttbBoostMask.h"

#include <boost/make_shared.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{

			BoostMaskAccessor::BoostMaskAccessor(StructTypePointer aStructurePointer,
			                                     const core::GeometricInfo& aGeometricInfo, bool strict)
				: _spStructure(aStructurePointer), _geoInfo(aGeometricInfo), _strict(strict)
			{
				_spRelevantVoxelVector = MaskVoxelListPointer();

				//generate new structure set uid
				::boost::uuids::uuid id;
				::boost::uuids::random_generator generator;
				id = generator();
				std::stringstream ss;
				ss << id;
				_maskUID = "BoostMask_" + ss.str();
			}


			BoostMaskAccessor::~BoostMaskAccessor()
			= default;

			void BoostMaskAccessor::updateMask()
			{
				MaskVoxelList newRelevantVoxelVector;

				if (_spRelevantVoxelVector)
				{
					return; // already calculated
				}

				BoostMask mask(::boost::make_shared<core::GeometricInfo>(_geoInfo),
				               _spStructure, _strict);

				_spRelevantVoxelVector = mask.getRelevantVoxelVector();
			}

			BoostMaskAccessor::MaskVoxelListPointer BoostMaskAccessor::getRelevantVoxelVector()
			{
				// if not already generated start voxelization here
				updateMask();
				return _spRelevantVoxelVector;
			}

			BoostMaskAccessor::MaskVoxelListPointer BoostMaskAccessor::getRelevantVoxelVector(
			    float lowerThreshold)
			{
        auto filteredVoxelVectorPointer = ::boost::make_shared<MaskVoxelList>();
				updateMask();
				// filter relevant voxels
				auto it = _spRelevantVoxelVector->begin();

				while (it != _spRelevantVoxelVector->end())
				{
					if ((*it).getRelevantVolumeFraction() > lowerThreshold)
					{
						filteredVoxelVectorPointer->push_back(*it);
					}

					++it;
				}

				// if mask calculation was not successful this is empty!
				return filteredVoxelVectorPointer;
			}

			bool BoostMaskAccessor::getMaskAt(VoxelGridID aID, core::MaskVoxel& voxel) const
			{
				//initialize return voxel
				voxel.setRelevantVolumeFraction(0);

				//check if ID is valid
				if (!_geoInfo.validID(aID))
				{
					return false;
				}

				//determine how a given voxel on the dose grid is masked
				if (_spRelevantVoxelVector)
				{
					auto it = _spRelevantVoxelVector->begin();

					while (it != _spRelevantVoxelVector->end())
					{
						if ((*it).getVoxelGridID() == aID)
						{
							voxel = (*it);
							return true;
						}

						++it;
					}

				}
				// returns false if mask was not calculated without triggering calculation (otherwise not const!)
				else
				{
					return false;
				}

				return false;

			}

			bool BoostMaskAccessor::getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const
			{
				//convert VoxelGridIndex3D to VoxelGridID
				VoxelGridID aVoxelGridID;

				if (_geoInfo.convert(aIndex, aVoxelGridID))
				{
					return getMaskAt(aVoxelGridID, voxel);
				}
				else
				{
					return false;
				}
			}

			const core::GeometricInfo& BoostMaskAccessor::getGeometricInfo() const
			{
				return _geoInfo;
			};
		}

	}
}