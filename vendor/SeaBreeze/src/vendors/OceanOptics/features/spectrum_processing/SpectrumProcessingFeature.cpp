/***************************************************//**
 * @file    SpectrumProcessingFeature.cpp
 * @date    February 2015
 * @author  Kirk Clendinning, Heliospectra
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2015, Heliospetra AB
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
#include "vendors/OceanOptics/features/spectrum_processing/SpectrumProcessingFeature.h"
#include "vendors/OceanOptics/protocols/interfaces/SpectrumProcessingProtocolInterface.h"
#include "vendors/OceanOptics/protocols/obp/impls/OBPSpectrumProcessingProtocol.h"
#include "common/exceptions/FeatureProtocolNotFoundException.h"
#include "common/exceptions/FeatureControlException.h"
#include "api/seabreezeapi/FeatureFamilies.h"

using namespace seabreeze;
using namespace seabreeze::oceanBinaryProtocol;
using namespace seabreeze::api;
using namespace std;

SpectrumProcessingFeature::SpectrumProcessingFeature(vector<ProtocolHelper *> helpers) {
    vector<ProtocolHelper *>::iterator iter;

    for(iter = helpers.begin(); iter != helpers.end(); iter++) {
        this->protocols.push_back(*iter);
    }
}

SpectrumProcessingFeature::~SpectrumProcessingFeature() {

}

#ifdef _WINDOWS
#pragma warning (disable: 4101) // unreferenced local variable
#endif

unsigned short int SpectrumProcessingFeature::readSpectrumProcessingScansToAverage(
		const Protocol &protocol, const Bus &bus) noexcept(false) {

    SpectrumProcessingProtocolInterface *processingPI = NULL;
	unsigned short int scansToAverage;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        processingPI = static_cast<SpectrumProcessingProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to get SpectrumProcessing.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        scansToAverage = processingPI->readSpectrumProcessingScansToAverage(bus);
        return scansToAverage;
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

void SpectrumProcessingFeature::writeSpectrumProcessingScansToAverage(const Protocol &protocol,
        const Bus &bus, unsigned short int scansToAverage) noexcept(false) {

    SpectrumProcessingProtocolInterface *processingPI = NULL;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        processingPI = static_cast<SpectrumProcessingProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException fpnfe) {
        string error(
                "Could not find matching protocol implementation to set spectrum processing state.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        processingPI->writeSpectrumProcessingScansToAverage(bus, scansToAverage);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

unsigned char SpectrumProcessingFeature::readSpectrumProcessingBoxcarWidth(
		const Protocol &protocol, const Bus &bus) noexcept(false) {

    SpectrumProcessingProtocolInterface *processingPI = NULL;
	unsigned char boxcarWidth;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        processingPI = static_cast<SpectrumProcessingProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException &e) {
        string error(
                "Could not find matching protocol implementation to get SpectrumProcessing.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        boxcarWidth = processingPI->readSpectrumProcessingBoxcarWidth(bus);
        return boxcarWidth;
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

void SpectrumProcessingFeature::writeSpectrumProcessingBoxcarWidth(const Protocol &protocol,
        const Bus &bus, unsigned char boxcarWidth) noexcept(false) {

    SpectrumProcessingProtocolInterface *processingPI = NULL;
    ProtocolHelper *proto = NULL;

    try {
        proto = lookupProtocolImpl(protocol);
        processingPI = static_cast<SpectrumProcessingProtocolInterface *>(proto);
    } catch (FeatureProtocolNotFoundException fpnfe) {
        string error(
                "Could not find matching protocol implementation to set spectrum processing state.");
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureProtocolNotFoundException(error);
    }

    try {
        processingPI->writeSpectrumProcessingBoxcarWidth(bus, boxcarWidth);
    } catch (ProtocolException &pe) {
        string error("Caught protocol exception: ");
        error += pe.what();
        /* FIXME: previous exception should probably be bundled up into the new exception */
        throw FeatureControlException(error);
    }
}

FeatureFamily SpectrumProcessingFeature::getFeatureFamily() {
    FeatureFamilies families;

    return families.SPECTRUM_PROCESSING;
}