// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosCubeinfoDoseAccessorGenerator.h"
#include "rttbVirtuosDoseFileDoseAccessorGenerator.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "../rttbDoseAccessorTester.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief VirtuosDoseIOTest - test the IO for virtuos data
				1) test getPrescribedDose() and getNormalizationDose()
				2) test virtuos dose import if geometric info was set correctly
				3) test virtuos dose import accessing dose data and converting

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int VirtuosDoseIOTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//           1: plan  file name (virtuos)		.../testing/data/Virtuos/prostate_ac/prostate_ac101.pln
			//           2: dose1 file name (virtuos)		.../testing/data/Virtuos/prostate_ac/prostate_ac101.dos.gz
			//           3: dose2 file name (trip):		.../testing/data/Virtuos/prostate_ac/prostate_ac101.dos
			// WARNING: Test will fail if dose2 does not contain the same dose as dose1!

			std::string RTPLAN_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTPLAN_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}

			//1) test getPrescribedDose() and getNormalizationDose()

			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGeneratorTest3(
			    RTDOSE_FILENAME.c_str(), 885.0, 76.0);
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> virtuosTest3 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGeneratorTest3.generateDoseAccessor());
			CHECK_EQUAL(76.0, virtuosTest3->getPrescribedDose());
			CHECK_EQUAL(885.0, virtuosTest3->getNormalizationDose());

			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGeneratorTest4(
			    RTDOSE_FILENAME.c_str(), 0, 0);
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> virtuosTest4 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGeneratorTest4.generateDoseAccessor());
			CHECK_EQUAL(1, virtuosTest4->getPrescribedDose());
			CHECK_EQUAL(1, virtuosTest4->getNormalizationDose());

			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str(),
			        RTPLAN_FILENAME.c_str());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor1 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator1.generateDoseAccessor());

			CHECK_EQUAL(76.0, doseAccessor1->getPrescribedDose());
			CHECK_EQUAL(885.0, doseAccessor1->getNormalizationDose());

			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(256, geoInfo.getNumRows());
			CHECK_EQUAL(256, geoInfo.getNumColumns());
			CHECK_EQUAL(144, geoInfo.getNumSlices());
			CHECK_EQUAL(256 * 256 * 144, doseAccessor1->getGridSize());
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween;
			VoxelGridIndex3D end3D, inbetween3D;

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(0, doseAccessor1->getDoseAt(start));
			CHECK_EQUAL(0, doseAccessor1-> getDoseAt(start3D));

			inbetween = int(doseAccessor1->getGridSize() / 2.0);
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(0, doseAccessor1->getDoseAt(inbetween));
			CHECK_EQUAL(0, doseAccessor1-> getDoseAt(inbetween3D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(0, doseAccessor1->getDoseAt(end));
			CHECK_EQUAL(0, doseAccessor1-> getDoseAt(end3D));

			/* Dose without plan */
			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str(),
			        doseAccessor1->getNormalizationDose(),
			        doseAccessor1->getPrescribedDose());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor2 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator2.generateDoseAccessor());

			CHECK_EQUAL(doseAccessor1->getPrescribedDose(), doseAccessor2->getPrescribedDose());
			CHECK_EQUAL(doseAccessor1->getNormalizationDose(), doseAccessor2->getNormalizationDose());


			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo2 = doseAccessor2->getGeometricInfo();
			CHECK_EQUAL(geoInfo.getNumRows(), geoInfo2.getNumRows());
			CHECK_EQUAL(geoInfo.getNumColumns(), geoInfo2.getNumColumns());
			CHECK_EQUAL(geoInfo.getNumSlices(), geoInfo2.getNumSlices());
			CHECK_EQUAL(doseAccessor1->getGridSize(), doseAccessor2->getGridSize());
			CHECK_EQUAL(geoInfo.getOrientationMatrix(), geoInfo2.getOrientationMatrix());

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(doseAccessor1->getDoseAt(start), doseAccessor2->getDoseAt(start));
			CHECK_EQUAL(doseAccessor1-> getDoseAt(start3D), doseAccessor2-> getDoseAt(start3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(start), doseAccessor2-> getDoseAt(start3D));

			inbetween = int(doseAccessor2->getGridSize() / 2.0);
			doseAccessor2->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween), doseAccessor2->getDoseAt(inbetween));
			CHECK_EQUAL(doseAccessor1-> getDoseAt(inbetween3D), doseAccessor2-> getDoseAt(inbetween3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(inbetween), doseAccessor2-> getDoseAt(inbetween3D));

			end = doseAccessor2->getGridSize() - 1;
			doseAccessor2->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(doseAccessor1->getDoseAt(end), doseAccessor2->getDoseAt(end));
			CHECK_EQUAL(doseAccessor1-> getDoseAt(end3D), doseAccessor2-> getDoseAt(end3D));
			CHECK_EQUAL(doseAccessor2->getDoseAt(end), doseAccessor2-> getDoseAt(end3D));

			/* Import Trip dose */
			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE2_FILENAME.c_str(),
			        doseAccessor1->getNormalizationDose(),
			        doseAccessor1->getPrescribedDose());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor3 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator3.generateDoseAccessor());


			CHECK_EQUAL(doseAccessor1->getPrescribedDose(), doseAccessor3->getPrescribedDose());
			CHECK_EQUAL(doseAccessor1->getNormalizationDose(), doseAccessor3->getNormalizationDose());


			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo3 = doseAccessor3->getGeometricInfo();
			CHECK_EQUAL(geoInfo.getNumRows(), geoInfo3.getNumRows());
			CHECK_EQUAL(geoInfo.getNumColumns(), geoInfo3.getNumColumns());
			CHECK_EQUAL(geoInfo.getNumSlices(), geoInfo3.getNumSlices());
			CHECK_EQUAL(doseAccessor1->getGridSize(), doseAccessor3->getGridSize());
			CHECK_EQUAL(geoInfo.getOrientationMatrix(), geoInfo3.getOrientationMatrix());

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(doseAccessor1->getDoseAt(start), doseAccessor3->getDoseAt(start));
			CHECK_EQUAL(doseAccessor1-> getDoseAt(start3D), doseAccessor3-> getDoseAt(start3D));
			CHECK_EQUAL(doseAccessor3->getDoseAt(start), doseAccessor3-> getDoseAt(start3D));

			inbetween = int(doseAccessor3->getGridSize() / 2.0);
			doseAccessor3->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween), doseAccessor3->getDoseAt(inbetween));
			CHECK_EQUAL(doseAccessor1-> getDoseAt(inbetween3D), doseAccessor3-> getDoseAt(inbetween3D));
			CHECK_EQUAL(doseAccessor3->getDoseAt(inbetween), doseAccessor3-> getDoseAt(inbetween3D));

			DoseAccessorTester doseCompare(doseAccessor1, doseAccessor3);
			CHECK_TESTER(doseCompare);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

