/***************************************************//**
 * @file    SaturationEEPROMSlotFeatureBase.h
 * @date    March 2016
 * @author  Ocean Optics, Inc.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2016, Ocean Optics Inc
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

#ifndef SATURATIONEEPROMSLOTFEATUREBASE_H
#define SATURATIONEEPROMSLOTFEATUREBASE_H

#include "vendors/OceanOptics/features/spectrometer/ProgrammableSaturationFeatureBase.h"
#include "vendors/OceanOptics/features/eeprom_slots/EEPROMSlotFeatureBase.h"

namespace seabreeze {
    
    class SaturationEEPROMSlotFeatureBase
            : public EEPROMSlotFeatureBase, public ProgrammableSaturationFeatureBase {
    public:
        SaturationEEPROMSlotFeatureBase();
        virtual ~SaturationEEPROMSlotFeatureBase();
        
        /* Inherited from ProgrammableSaturationFeature */
        virtual unsigned int getSaturation() noexcept(false);
        
        /* Inherited from Feature */
        virtual bool initialize(const Protocol &protocol, const Bus &bus)
            noexcept(false);
        
        virtual FeatureFamily getFeatureFamily();
        
    protected:
        /* Derived classes must implement this in whatever way is appropriate
         * to get the saturation level for the device.
         */
        virtual unsigned int getSaturation(const Protocol &protocol,
                const Bus &bus) noexcept(false) = 0;
        
    private:
        unsigned int saturation;
        bool valid;
    };
    
} /* end namespace seabreeze */

#endif /* SATURATIONEEPROMSLOTFEATUREBASE_H */
