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
#ifndef __MASK_ACCESSOR_GENERATOR_BASE_H
#define __MASK_ACCESSOR_GENERATOR_BASE_H


#include "rttbMaskAccessorGeneratorInterface.h"

namespace rttb
{
	namespace core
	{
		/*! @class MaskAccessorGeneratorBase
			@brief Abstract class for all DoseAccessor generating classes
		*/
		class MaskAccessorGeneratorBase: public MaskAccessorGeneratorInterface
		{
		public:
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;


		private:


		protected:
			/*! @brief Mask accessor which should be generated */
			MaskAccessorPointer _maskAccessor;


		public:

		};
	}
}

#endif
