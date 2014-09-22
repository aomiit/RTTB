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

#include "DoseAccApplicationData.h"
#include "DoseAccHelper.h"

rttb::apps::doseAcc::ApplicationData appData;

int main(int argc, char** argv)
{
  int result = 0;

  std::cout << "DoseAcc - RTTB demo app for simple dose accumulation." << std::endl;

  switch (rttb::apps::doseAcc::ParseArgumentsForAppData(argc, argv, appData))
  {
  case 1:
    {
      //showed version or help info. Done.
      return 1;
    }

  case 2:
    {
      std::cerr << "Missing Parameters. Use one of the following flags for more information:" <<
        std::endl;
      std::cerr << "-? or --help" << std::endl;
      return 2;
    }

  case 3:
    {
      //wrong option usage.
      return 3;
    }
  }

  if (appData._fileCount < 3)
  {
    std::cerr << "Missing Parameters. Use one of the following flags for more information:" <<
      std::endl;
    std::cerr << "-? or --help" << std::endl;
    return 1;
  }

  std::cout << std::endl << "*******************************************" << std::endl;
  std::cout << "Dose 1 file:        " << appData._dose1FileName << std::endl;
  if (!(appData._dose1VirtuosPlanFileName.empty()))
  {
    std::cout << "  as virtuos. Plan: " << appData._dose1VirtuosPlanFileName << std::endl;
  }
  std::cout << "Dose 2 file:        " << appData._dose2FileName << std::endl;
  if (!(appData._dose2VirtuosPlanFileName.empty()))
  {
    std::cout << "  as virtuos. Plan: " << appData._dose2VirtuosPlanFileName << std::endl;
  }
  std::cout << "Dose output file:   " << appData._outputFileName << std::endl;

  if (!(appData._regFileName.empty()))
  {
    std::cout << "Registration file: " << appData._regFileName << std::endl;
  }
  std::cout << "Dose 1 weight:      " << appData._weightDose1 << std::endl;
  std::cout << "Dose 2 weight:      " << appData._weightDose2 << std::endl;

  try
  {
    if (appData._dose1VirtuosPlanFileName.empty())
    {
      appData._Dose1 = rttb::apps::doseAcc::loadDose(appData._dose1FileName);
    }
    else
    {
      appData._Dose1 = rttb::apps::doseAcc::loadVirtuosDose(appData._dose1FileName, appData._dose1VirtuosPlanFileName);
    }
  }
  catch (::itk::ExceptionObject& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e << std::endl;
    return 4;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e.what() << std::endl;
    return 4;
  }
  catch (...)
  {
    std::cerr << "Error!!! unknown error while reading input image." << std::endl;
    return 4;
  }

  try
  {
    if (appData._dose2VirtuosPlanFileName.empty())
    {
      appData._Dose2 = rttb::apps::doseAcc::loadDose(appData._dose2FileName);
    }
    else
    {
      appData._Dose2 = rttb::apps::doseAcc::loadVirtuosDose(appData._dose2FileName, appData._dose2VirtuosPlanFileName);
    }
  }
  catch (::itk::ExceptionObject& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e << std::endl;
    return 4;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e.what() << std::endl;
    return 4;
  }
  catch (...)
  {
    std::cerr << "Error!!! unknown error while reading input image." << std::endl;
    return 4;
  }

  if (!(appData._regFileName.empty()))
  {
    try
    {
      appData._spReg = rttb::apps::doseAcc::loadRegistration(appData._regFileName);
    }
    catch (::itk::ExceptionObject& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e << std::endl;
      return 5;
    }
    catch (std::exception& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e.what() << std::endl;
      return 5;
    }
    catch (...)
    {
      std::cerr << "Error!!! unknown error while reading registration file." << std::endl;
      return 5;
    }
  }

  try
  {
    rttb::apps::doseAcc::processData(appData);
  }
  catch (::itk::ExceptionObject& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e << std::endl;
    return 9;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e.what() << std::endl;
    return 9;
  }
  catch (...)
  {
    std::cerr << "Error!!! unknown error while mapping and writing image." << std::endl;
    return 9;
  }

  std::cout << std::endl;

  return result;
}
