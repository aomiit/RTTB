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

#include <math.h>
#include <algorithm>
#include <sstream>
#include <stdlib.h>

#include <boost/lexical_cast.hpp>

#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbStructure.h"

#include "rttbDicomIODStructureSetGenerator.h"
#include "rttbDcmrtException.h"


namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			DicomIODStructureSetGenerator::DicomIODStructureSetGenerator(DRTStrSetIODPtr aDRTStructureSetIOD)
			{
				_drtStrSetIOD = aDRTStructureSetIOD;
			}


			void DicomIODStructureSetGenerator::readStrSet()
			{
				OFString uid;
				_drtStrSetIOD->getSeriesInstanceUID(uid);
				_UID = uid.c_str();

				OFString uid2;
				_drtStrSetIOD->getPatientID(uid2);
				_patientUID = uid2.c_str();

				DRTStructureSetROISequence* rois = &_drtStrSetIOD->getStructureSetROISequence();

				/*A structure is a DRTROIContourSequence::Item. Each Item defines a roi. Each ROI contains a sequence
				of one or more contours, where a contour is either a single point (for a point ROI) or more than
				one point (representing an open or closed polygon).
				*/
				DRTROIContourSequence* rcs;
				rcs = &_drtStrSetIOD->getROIContourSequence();
				DRTROIContourSequence::Item* rcsItem;

				long numberOfStructures = rcs->getNumberOfItems();
				bool isEmpty = rcs->isEmpty();

				if (numberOfStructures == 0 || isEmpty)
				{
					throw core::InvalidParameterException("Empty Structure Set!") ;
				}

				int structureNo = 0;

				for (rcs->gotoFirstItem(); (rcs->getCurrentItem(rcsItem)).good(); rcs->gotoNextItem())
				{
					OFString refROINumber;
					rcsItem->getReferencedROINumber(refROINumber);
					DRTContourSequence* cs;
					cs = &rcsItem->getContourSequence();
					unsigned long no2 = cs->getNumberOfItems();

					PolygonSequenceType structureVector;

					for (unsigned long j = 0; j < no2; j++)
					{
						/*DRTContourSequence::Item represents a contour (either a single point (for a point ROI) or more than
						one point (representing an open or closed polygon))*/
						DRTContourSequence::Item* csItem;
						csItem = &cs->getItem(j);
						OFString contourData;
						OFString numberOfContourPoints;
						csItem->getNumberOfContourPoints(numberOfContourPoints);

						unsigned int numberOfContourPointsInt;
						std::stringstream is(numberOfContourPoints.c_str());
						is >> numberOfContourPointsInt;
						OFString countourNumber;
						csItem->getContourNumber(countourNumber);

						PolygonType contourVector;
						char* pEnd;

						for (unsigned int k = 0; k < numberOfContourPointsInt; k++)
						{
							WorldCoordinate3D point;

							for (unsigned int i = 0; i < 3; i++)
							{
								csItem->getContourData(contourData, k * 3 + i);

								WorldCoordinate value = strtod(contourData.c_str(), &pEnd);

								if (*pEnd != '\0')
								{
									throw core::InvalidParameterException("Contour data not readable!") ;
								}

								if (i == 0)
								{
									point(0) = value;
								}
								else if (i == 1)
								{
									point(1) = value;
								}
								else
								{
									point(2) = value;
								}
							}

							contourVector.push_back(point);
						}

						structureVector.push_back(contourVector);
					}

					boost::shared_ptr<core::Structure> spStruct = boost::make_shared<core::Structure>(structureVector);
					StructTypePointer str(spStruct);

					for (unsigned long i = 0; i < rois->getNumberOfItems(); i++)
					{
						DRTStructureSetROISequence::Item* roisItem;
						roisItem = &rois->getItem(i);

						OFString roiNumber;
						roisItem->getROINumber(roiNumber);

						if (roiNumber == refROINumber)
						{
							OFString roiName;
							roisItem->getROIName(roiName);
							str->setLabel(roiName.c_str());
							std::cout << roiName.c_str() << std::endl;
							break;
						}
					}

					std::stringstream sstr;
					sstr << structureNo;
					str->setUID(sstr.str());

					_strVector.push_back(str);
					structureNo++;
				}
			}

			DicomIODStructureSetGenerator::~DicomIODStructureSetGenerator()
			{
			}

			DicomIODStructureSetGenerator::StructureSetPointer
			DicomIODStructureSetGenerator::generateStructureSet()
			{
				this->readStrSet();
				return boost::make_shared<core::StructureSet>(_strVector, _patientUID, _UID);
			}
		}//end namespace dicom
	}//end namespace io
}//end namespace rttb
