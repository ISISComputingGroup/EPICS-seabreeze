/***************************************************//**
 * @file    LightSourceFeatureImpl.cpp
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

#include "common/globals.h"
#include "vendors/OceanOptics/features/light_source/LightSourceFeatureImpl.h"

using namespace seabreeze;
using namespace std;

LightSourceFeatureImpl::LightSourceFeatureImpl(vector<ProtocolHelper *> helpers,
            int lampModuleIndex, int lightSources)
                : LightSourceFeatureBase(helpers, lampModuleIndex) {

    this->lightSourceCount = lightSources;
}

LightSourceFeatureImpl::~LightSourceFeatureImpl() {
    /* Nothing to do */
}

int LightSourceFeatureImpl::getLightSourceCount(const Protocol &protocol,
            const Bus &bus) noexcept(false) {
    /* Note: this gets called frequently to provide bounds checking.  If this
     * gets replaced by a call down to the actual device, then the value should
     * be cached and reused to keep things efficient.
     */
    return this->lightSourceCount;
}
