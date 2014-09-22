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

#include "rttbInterpolationBase.h"
#include "rttbInvalidParameterException.h"
#include "rttbNullPointerException.h"
#include "rttbMappingOutsideOfImageException.h"

namespace rttb
{
	namespace interpolation
	{
		void InterpolationBase::setDoseAccessorPointer(const DoseAccessorPointer originalDose)
		{
			if (originalDose != NULL)
			{
				_spOriginalDose = originalDose;
			}
			else
			{
				throw core::NullPointerException("originalDose is NULL!");
			}
		};

		void InterpolationBase::getNeighborhoodVoxelValues(
		    const WorldCoordinate3D& aWorldCoordinate,
		    unsigned int neighborhood, boost::array<double, 3>& target,
		    boost::shared_array<DoseTypeGy> values) const
		{
			if (_spOriginalDose == NULL)
			{
				throw core::NullPointerException("originalDose is NULL!");
			}

			//Determine target (abs(desired worldCoordinate- corner pixel world coordinate/pixel spacing) and values of corner pixels (from originalDose)
			VoxelGridIndex3D aIndex;

			if (_spOriginalDose->getGeometricInfo().worldCoordinateToIndex(aWorldCoordinate, aIndex))
			{
				//determine just the nearest voxel to the world coordinate
				if (neighborhood == 0)
				{
					values[0] = _spOriginalDose->getDoseAt(aIndex);
				}
				//determine the 8 voxels around the world coordinate
				else if (neighborhood == 8)
				{
					std::vector<VoxelGridIndex3D> cornerPoints;

					WorldCoordinate3D theNextVoxel;
					_spOriginalDose->getGeometricInfo().indexToWorldCoordinate(aIndex, theNextVoxel);
					SpacingVectorType3D pixelSpacing = (_spOriginalDose->getGeometricInfo()).getSpacing();
					VoxelGridIndex3D leftTopFrontCoordinate;

					//find the voxel with the smallest coordinate values in each dimension. This defines the standard cube
					for (int i = 0; i < 3; i++)
					{
						if (aWorldCoordinate[i] < theNextVoxel[i])
						{
							if (aIndex[i] > 0)
							{
								leftTopFrontCoordinate[i] = aIndex[i] - 1;
								target[i] = (aWorldCoordinate[i] - (theNextVoxel[i] - pixelSpacing[i])) / pixelSpacing[i];
							}
							//@todo: this is a workaround, not a good solution
							else
							{
								leftTopFrontCoordinate[i] = aIndex[i];
								target[i] = (aWorldCoordinate[i] - (theNextVoxel[i] - pixelSpacing[i])) / pixelSpacing[i];
							}
						}
						else
						{
							leftTopFrontCoordinate[i] = aIndex[i];
							target[i] = (aWorldCoordinate[i] - theNextVoxel[i]) / pixelSpacing[i];
						}
					}

					for (int zIncr = 0; zIncr < 2; zIncr++)
					{
						for (int yIncr = 0; yIncr < 2; yIncr++)
						{
							for (int xIncr = 0; xIncr < 2; xIncr++)
							{
								cornerPoints.push_back(VoxelGridIndex3D(leftTopFrontCoordinate[0] + xIncr,
								                                        leftTopFrontCoordinate[1] + yIncr,
								                                        leftTopFrontCoordinate[2] + zIncr));
							}
						}
					}

					//target range has to be always [0,1]
					for (int i = 0; i < 3; i++)
					{
						assert(target[i] >= 0.0 && target[i] <= 1.0);
					}

					//now just get the values of all (dose) voxels and store them in values
					for (int i = 0; i < cornerPoints.size(); ++i)
					{
						if (_spOriginalDose->getGeometricInfo().isInside(cornerPoints.at(i)))
						{
							values[i] = _spOriginalDose->getDoseAt(cornerPoints.at(i));
						}
						else
						{
							//outside value! boundary treatment
							values[i] = getNearestInsideVoxelValue(cornerPoints.at(i));
						}

						assert(values[i] != -1);
					}
				}
				else
				{
					throw core::InvalidParameterException("neighborhoods other than 0 and 8 not yet supported in Interpolation");
				}
			}
			else
			{
				throw core::MappingOutsideOfImageException("Error in conversion from world coordinates to index");
			}
		}

		DoseTypeGy InterpolationBase::getNearestInsideVoxelValue(const VoxelGridIndex3D& currentVoxelIndex)
		const
		{
			VoxelGridIndex3D voxelChangedXYZ[] = {currentVoxelIndex, currentVoxelIndex, currentVoxelIndex, currentVoxelIndex, currentVoxelIndex, currentVoxelIndex, currentVoxelIndex};
			int runningIndex;

			//x,y,z
			for (runningIndex = 0; runningIndex < 3; ++runningIndex)
			{
				voxelChangedXYZ[runningIndex][runningIndex] -= 1;
			}

			//xy
			voxelChangedXYZ[runningIndex][0] -= 1;
			voxelChangedXYZ[runningIndex][1] -= 1;
			++runningIndex;
			//xz
			voxelChangedXYZ[runningIndex][0] -= 1;
			voxelChangedXYZ[runningIndex][2] -= 1;
			++runningIndex;
			//yz
			voxelChangedXYZ[runningIndex][1] -= 1;
			voxelChangedXYZ[runningIndex][2] -= 1;
			++runningIndex;
			//xyz
			voxelChangedXYZ[runningIndex][0] -= 1;
			voxelChangedXYZ[runningIndex][1] -= 1;
			voxelChangedXYZ[runningIndex][2] -= 1;
			++runningIndex;

			int replacementVoxelIndex = 0;

			while (replacementVoxelIndex < runningIndex)
			{
				if (_spOriginalDose->getGeometricInfo().validIndex(voxelChangedXYZ[replacementVoxelIndex]))
				{
					return _spOriginalDose->getDoseAt(voxelChangedXYZ[replacementVoxelIndex]);
				}

				++replacementVoxelIndex;
			}

			return -1;
		}


	}//end namespace core
}//end namespace rttb
