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
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/RTDVHTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests


#include "litCheckMacros.h"

#include "rttbDVH.h"
#include "rttbDVHTxtFileReader.h"
#include "rttbBaseType.h"
#include "rttbDvhBasedModels.h"

namespace rttb{

    namespace testing{

		/*! @brief RTDVHTest.  Max, min, mean, modal, median, Vx, Dx, EUD, BED, LQED2 are tested.
		Test if calculation in new architecture returns similar results to the 
		original implementation.

		Comparison of actual DVH values is performed in DVHCalculatorComparisonTest.cpp.

		WARNING: The values for comparison need to be adjusted if the input files are changed!		
		*/
        int RTDVHTest(int argc, char* argv[] )
        {
            
			PREPARE_DEFAULT_TEST_REPORTING;

		    //ARGUMENTS: 1: dvh file name

		    std::string RTDVH_FILENAME_PTV;

		    if (argc>1)
			{
				RTDVH_FILENAME_PTV = argv[1];
			}

			typedef core::DVH::DVHPointer DVHPointer;

			/*test RT dvh*/
			rttb::io::other::DVHTxtFileReader dvhReader=rttb::io::other::DVHTxtFileReader(RTDVH_FILENAME_PTV);

			const DoseCalcType expectedValue = 0.01305;

			//dvhReader
			DVHPointer dvh=dvhReader.generateDVH();
			
			CHECK_CLOSE(expectedValue,models::getEUD(dvh,10),errorConstant);
			std::cout << models::getEUD(dvh,10)<<std::endl;
			std::map<rttb::DoseTypeGy,rttb::DoseCalcType> bedMap = models::calcBEDDVH(dvh,15,15);
			std::map<rttb::DoseTypeGy,rttb::DoseCalcType> LqedMap = models::calcLQED2DVH(dvh,15,10);
			CHECK_NO_THROW(models::calcBEDDVH(dvh,15,15));
			CHECK_NO_THROW(models::calcLQED2DVH(dvh,15,10));

			CHECK_NO_THROW(dvh->getDataDifferential(true));
			CHECK_EQUAL(1,dvh->calcCumulativeDVH(true).at(0));
			CHECK_NO_THROW(models::calcBEDDVH(dvh,15,15,true)); 
			CHECK_NO_THROW(models::calcLQED2DVH(dvh,15,10,true));
			

			//test statistics (relative comulative data)
			CHECK_CLOSE(expectedValue,dvh->getMaximum(),errorConstant);
			CHECK_CLOSE(expectedValue,dvh->getMinimum(),errorConstant);
			CHECK_CLOSE(expectedValue,dvh->getMean(),errorConstant);
			CHECK_CLOSE(expectedValue,dvh->getMedian(),errorConstant);
			CHECK_CLOSE(expectedValue,dvh->getModal(),errorConstant);
			CHECK_EQUAL(0,dvh->getVx(0.014));
			CHECK_EQUAL(0.125,dvh->getVx(0.01));
			rttb::DoseTypeGy dTest = dvh->getDx(100);
			CHECK_CLOSE(0.0131,dvh->getDx(100),errorConstant+errorConstant*10);
			CHECK_CLOSE(0.013,dvh->getDx(249),errorConstant);
			dTest = dvh->getDx(249);
			CHECK_EQUAL(0,dvh->getDx(251));
			dTest = dvh->getDx(251);

			//test statistics (absolute comulative data)
			CHECK_EQUAL(2000,dvh->calcCumulativeDVH(false).at(0));
			CHECK_EQUAL(0,dvh->getVx(0.014));
			CHECK_EQUAL(250,dvh->getVx(0.01));
			dTest = dvh->getDx(100);
			CHECK_CLOSE(0.0131,dvh->getDx(100),errorConstant+errorConstant*10); 
			CHECK_CLOSE(0.013,dvh->getDx(249),errorConstant);
			dTest = dvh->getDx(249);
			CHECK_EQUAL(0,dvh->getDx(251));
			dTest = dvh->getDx(251);
			
			RETURN_AND_REPORT_TEST_SUCCESS;        
        }
  
  }//testing
}//rttb

