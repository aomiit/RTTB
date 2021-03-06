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


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include "rttbDicomFileDoseAccessorWriter.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseStatisticsCalculator.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			DicomFileDoseAccessorWriter::DicomFileDoseAccessorWriter()
			{
				_doseIOD = boost::make_shared<DRTDoseIOD>();
				_dataset = _fileformat.getDataset();
			}

			void DicomFileDoseAccessorWriter::setFileName(DICOMRTFileNameString aFileName)
			{
				_fileName = aFileName;
			}

			bool DicomFileDoseAccessorWriter::process()
			{
				OFCondition status;

				/* Prepare dcmtk */
				DcmItem* dcm_item = nullptr;

				//get geometric info
				rttb::core::GeometricInfo geometricInfo = _doseAccessor->getGeometricInfo();


				/* ----------------------------------------------------------------- */
				/*     Part 1  -- General header                                     */
				/* ----------------------------------------------------------------- */
				OFString CreationUID(_doseAccessor->getUID().c_str());
				_dataset->putAndInsertString(DCM_ImageType,
				                             R"(DERIVED\SECONDARY\REFORMATTED)");
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreationDate,
				                                    "");//Creation Date
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreationTime,
				                                    "");//Creation Time
				_dataset->putAndInsertOFStringArray(DCM_InstanceCreatorUID,
				                                    CreationUID);
				_dataset->putAndInsertString(DCM_SOPClassUID, UID_RTDoseStorage);
				_dataset->putAndInsertString(DCM_SOPInstanceUID,
				                             _doseAccessor->getUID().c_str());
				_dataset->putAndInsertOFStringArray(DCM_StudyDate,
				                                    "");
				_dataset->putAndInsertOFStringArray(DCM_StudyTime,
				                                    "");
				_dataset->putAndInsertOFStringArray(DCM_AccessionNumber, "");
				_dataset->putAndInsertOFStringArray(DCM_Modality, "RTDOSE");
				_dataset->putAndInsertString(DCM_Manufacturer, "RTToolbox");
				_dataset->putAndInsertString(DCM_InstitutionName, "");
				_dataset->putAndInsertString(DCM_ReferringPhysicianName, "");
				_dataset->putAndInsertString(DCM_StationName, "");
				_dataset->putAndInsertString(DCM_ManufacturerModelName, "RTToolbox");


				/* (0008,1140) DCM_ReferencedImageSequence -- MIM likes this */
				dcm_item = nullptr;
				_dataset->findOrCreateSequenceItem(
				    DCM_ReferencedImageSequence, dcm_item, -2);
				dcm_item->putAndInsertString(DCM_ReferencedSOPClassUID,
				                             UID_CTImageStorage);
				dcm_item->putAndInsertString(DCM_ReferencedSOPInstanceUID,
				                             "");

				_dataset->putAndInsertString(DCM_PatientName, "");
				_dataset->putAndInsertString(DCM_PatientID, "");
				_dataset->putAndInsertString(DCM_PatientBirthDate, "");
				_dataset->putAndInsertString(DCM_PatientSex, "O");
				_dataset->putAndInsertString(DCM_SliceThickness,
				                             boost::lexical_cast<std::string>(geometricInfo.getSpacing()(2)).c_str());
				_dataset->putAndInsertString(DCM_SoftwareVersions,
				                             "");
				_dataset->putAndInsertString(DCM_StudyInstanceUID,
				                             "");
				_dataset->putAndInsertString(DCM_SeriesInstanceUID,
				                             "");
				_dataset->putAndInsertString(DCM_StudyID, "10001");
				_dataset->putAndInsertString(DCM_SeriesNumber, "");
				_dataset->putAndInsertString(DCM_InstanceNumber, "1");


				/* GCS FIX: PatientOrientation */
				std::ostringstream sstr;
				sstr << geometricInfo.getImagePositionPatient().x() << R"(\)" <<
				     geometricInfo.getImagePositionPatient().y()
				     << R"(\)" << geometricInfo.getImagePositionPatient().z();
				_dataset->putAndInsertString(DCM_PatientOrientation, "L/P");
				_dataset->putAndInsertString(DCM_ImagePositionPatient, sstr.str().c_str());

        auto orientationMatrix = geometricInfo.getOrientationMatrix();

				sstr.str("");
				sstr << orientationMatrix(0,0) << R"(\)"
				     << orientationMatrix(1,0) << R"(\)"
				     << orientationMatrix(2,0) << R"(\)"
				     << orientationMatrix(0,1) << R"(\)"
				     << orientationMatrix(1,1) << R"(\)"
				     << orientationMatrix(2,1);

				_dataset->putAndInsertString(DCM_ImageOrientationPatient, sstr.str().c_str());
				_dataset->putAndInsertString(DCM_FrameOfReferenceUID,
				                             "");

				_dataset->putAndInsertString(DCM_SamplesPerPixel, "1");
				_dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
				sstr.str("");
				sstr << geometricInfo.getNumSlices();
				_dataset->putAndInsertString(DCM_NumberOfFrames, sstr.str().c_str());

				/* GCS FIX: Add FrameIncrementPointer */
				_dataset->putAndInsertString(DCM_FrameIncrementPointer,
				                             "(3004,000c)");

				_dataset->putAndInsertUint16(DCM_Rows, static_cast<Uint16>(geometricInfo.getNumRows()));
				_dataset->putAndInsertUint16(DCM_Columns, static_cast<Uint16>(geometricInfo.getNumColumns()));
				sstr.str("");
				sstr << geometricInfo.getSpacing()(1) << R"(\)" << geometricInfo.getSpacing()(0);
				_dataset->putAndInsertString(DCM_PixelSpacing, sstr.str().c_str());

				_dataset->putAndInsertString(DCM_BitsAllocated, "32");
				_dataset->putAndInsertString(DCM_BitsStored, "32");
				_dataset->putAndInsertString(DCM_HighBit, "31");

				_dataset->putAndInsertString(DCM_DoseUnits, "GY");

				_dataset->putAndInsertString(DCM_DoseSummationType, "PLAN");

				sstr.str("0");

				for (unsigned int i = 1; i < geometricInfo.getNumSlices(); i++)
				{
					sstr << R"(\)" <<  i* geometricInfo.getSpacing()(2);
				}

				_dataset->putAndInsertString(DCM_GridFrameOffsetVector, sstr.str().c_str());


				/* We need to convert image to uint16_t, but first we need to
				scale it so that the maximum dose fits in a 16-bit unsigned
				integer.  Compute an appropriate scaling factor based on the
				maximum dose. */

				/* Find the maximum value in the image */
				boost::shared_ptr<rttb::core::GenericDoseIterator> spTestDoseIterator =
				    boost::make_shared<rttb::core::GenericDoseIterator>(_doseAccessor);
				rttb::core::GenericDoseIterator::DoseIteratorPointer spDoseIterator(spTestDoseIterator);
				rttb::algorithms::DoseStatisticsCalculator myDoseStatsCalculator(spDoseIterator);
				auto doseStatistics =
				    myDoseStatsCalculator.calculateDoseStatistics();

				double maxDose = doseStatistics->getMaximum();

				/* Find scale factor */
				double dose_scale;
				dose_scale = maxDose / PixelDataMaxValue;

				/* Scale the image and add scale factor to _dataset */
				sstr.str("");
				sstr << dose_scale;
				_dataset->putAndInsertString(DCM_DoseGridScaling, sstr.str().c_str());

				/* (300c,0002) ReferencedRTPlanSequence -- for future expansion */
				dcm_item = nullptr;
				_dataset->findOrCreateSequenceItem(
				    DCM_ReferencedRTPlanSequence, dcm_item, -2);
				dcm_item->putAndInsertString(DCM_ReferencedSOPClassUID,
				                             UID_RTPlanStorage);
				dcm_item->putAndInsertString(DCM_ReferencedSOPInstanceUID,
				                             "");

				/* (300c,0060) DCM_ReferencedStructureSetSequence -- MIM likes this */
				dcm_item = nullptr;
				_dataset->findOrCreateSequenceItem(
				    DCM_ReferencedStructureSetSequence, dcm_item, -2);
				dcm_item->putAndInsertString(DCM_ReferencedSOPClassUID,
				                             UID_RTStructureSetStorage);
				dcm_item->putAndInsertString(DCM_ReferencedSOPInstanceUID,
				                             "");

				/* Convert image bytes to integer, then add to _dataset */
				Uint16* pixelData;
				auto pixelCount = static_cast<unsigned int>(geometricInfo.getNumRows() * geometricInfo.getNumColumns() *
				                          geometricInfo.getNumSlices());
				pixelData = new Uint16[pixelCount];

				for (unsigned int i = 0; i < pixelCount; ++i)
				{
					double doseValue = _doseAccessor->getValueAt(i);
					double pixelValue = doseValue / dose_scale;

					if (pixelValue > PixelDataMaxValue)
					{
						pixelValue = PixelDataMaxValue;
					}

					pixelData[i] = boost::numeric_cast<Uint16>(pixelValue);
				}

				status = _dataset->putAndInsertUint16Array(DCM_PixelData,
				         pixelData, pixelCount);

				if (!status.good())
				{
					throw core::InvalidDoseException("Error: put and insert pixel data failed!");
				}

				//Write dose to file
				status = _fileformat.saveFile(_fileName.c_str(), EXS_LittleEndianExplicit);

				if (status.bad())
				{
					std::cerr << "Error: cannot write DICOM RTDOSE!" << std::endl;
				}

				return true;
			}


		}//end namespace itk
	}//end namespace io
}//end namespace rttb

