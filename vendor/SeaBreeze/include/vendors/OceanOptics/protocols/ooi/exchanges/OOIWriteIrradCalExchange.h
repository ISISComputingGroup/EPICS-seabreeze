/***************************************************//**
 * @file    OOIWriteIrradCalExchange.h
 * @date    February 2012
 * @author  Ocean Optics, Inc.
 *
 * This is a protocol exchange that can be used for the
 * USB2000+ and some others.
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

#ifndef SEABREEZE_OOIWRITEIRRADCALEXCHANGE_H
#define SEABREEZE_OOIWRITEIRRADCALEXCHANGE_H

#include "vendors/OceanOptics/protocols/ooi/exchanges/OOIIrradCalExchange.h"
#include "common/Data.h"

namespace seabreeze {
  namespace ooiProtocol {
    class OOIWriteIrradCalExchange : public OOIIrradCalExchange {
    public:
        OOIWriteIrradCalExchange(int numberOfPixels);
        virtual ~OOIWriteIrradCalExchange();
        int setCalibration(const std::vector<float> &calibration);

        /* Inherited */
        virtual Data *transfer(TransferHelper* helper) noexcept(false);
    };
  }
}

#endif /* SEABREEZE_OOIWRITEIRRADCALEXCHANGE_H */

