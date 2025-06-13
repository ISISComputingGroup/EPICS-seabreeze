/***************************************************//**
 * @file    OBPLightSourceProtocol.h
 * @date    April 2013
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#ifndef SEABREEZE_OBPLIGHTSOURCEPROTOCOL_H
#define SEABREEZE_OBPLIGHTSOURCEPROTOCOL_H

#include "vendors/OceanOptics/protocols/interfaces/LightSourceProtocolInterface.h"

namespace seabreeze {
    namespace oceanBinaryProtocol {

        class OBPLightSourceProtocol : public LightSourceProtocolInterface {

        public:
            OBPLightSourceProtocol();
            virtual ~OBPLightSourceProtocol();

            /* Required by LightSourceProtocolInterface */
            virtual bool isLightSourceEnabled(const Bus &bus, int moduleIndex,
                int lightSourceIndex) noexcept(false);

            virtual void setLightSourceEnable(const Bus &bus, int moduleIndex,
                int lightSourceIndex, bool enable) noexcept(false);

        protected:
            virtual int getModuleCount() = 0;
            virtual int getLightSourceCount(int moduleIndex) = 0;

        };

        class OBPLightSourceProtocol_NormalizedIntensity : public OBPLightSourceProtocol,
                public LightSourceProtocolInterface_NormalizedIntensity {
        public:
            OBPLightSourceProtocol_NormalizedIntensity();
            virtual ~OBPLightSourceProtocol_NormalizedIntensity();

            virtual double getIntensity(const Bus &bus, int moduleIndex,
                int lightSourceIndex) noexcept(false);

            virtual void setIntensity(const Bus &bus, int moduleIndex,
                int lightSourceIndex, double intensity) noexcept(false);
        };

        class OBPLightSourceProtocol_Counts : public OBPLightSourceProtocol,
                public LightSourceProtocolInterface_Counts {
        public:
            OBPLightSourceProtocol_Counts();
            virtual ~OBPLightSourceProtocol_Counts();

            virtual int getIntensityCounts(const Bus &bus, int moduleIndex,
                int lightSourceIndex) noexcept(false);

            virtual void setIntensityCounts(const Bus &bus, int moduleIndex,
                int lightSourceIndex, int counts) noexcept(false);
        };

    }
}

#endif
