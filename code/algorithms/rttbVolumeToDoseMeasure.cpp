#include "rttbVolumeToDoseMeasure.h"
#include "rttbDataNotAvailableException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		VolumeToDoseMeasure::VolumeToDoseMeasure(complexStatistics name, VolumeToDoseFunctionType values, VolumeType volume) :
			name(name), values(values), _volume(volume) {}

		void VolumeToDoseMeasure::insertValue(std::pair<VolumeType, DoseTypeGy> value)
		{
			this->values.insert(value);
		}

		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getSpecificValue(xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy VolumeToDoseMeasure::getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			return getSpecificValue(xVolumeAbsolute, findNearestValue, nearestXVolume);
		}
		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				VolumeType dummy;
				return getSpecificValue(xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}
		DoseTypeGy VolumeToDoseMeasure::getValueRelative(VolumeType xVolumeRelative, bool findNearestValue, VolumeType & nearestXVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1) {
				DoseTypeGy xVolumeAbsolute = xVolumeRelative * _volume;
				return getSpecificValue(xVolumeAbsolute, findNearestValue, nearestXVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}
		VolumeToDoseMeasure::VolumeToDoseFunctionType VolumeToDoseMeasure::getAllValues() const
		{
			return this->values;
		}

		void VolumeToDoseMeasure::setVolume(VolumeType volume)
		{
			this->_volume = volume;
		}

		double VolumeToDoseMeasure::getSpecificValue(double key,
			bool findNearestValueInstead, double& storedKey) const
		{
			if (values.find(key) != std::end(values))
			{
				return values.find(key)->second;
			}
			else
			{
				//value not in map. We have to find the nearest value
				if (values.empty())
				{
					throw core::DataNotAvailableException("No Vx values are defined");
				}
				else
				{
					if (findNearestValueInstead)
					{
						auto iterator = findNearestKeyInMap(values, key);
						storedKey = iterator->first;
						return iterator->second;
					}
					else
					{
						throw core::DataNotAvailableException("No Vx value with required dose is defined");
					}
				}
			}
		}		
		
		bool operator==(const VolumeToDoseMeasure& volumeToDoseMesure,const VolumeToDoseMeasure& otherVolumeToDoseMesure)
		{
			if (volumeToDoseMesure.getName() == otherVolumeToDoseMesure.getName() && volumeToDoseMesure.getAllValues() == otherVolumeToDoseMesure.getAllValues()) {
				return true;
			}
			return false;
		}
	}
}