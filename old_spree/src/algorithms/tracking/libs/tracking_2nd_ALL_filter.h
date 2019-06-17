/*!
 * \file tracking_2nd_ALL_filter.h
 * \brief Interface of a 2nd order ALL filter for code tracking loop.
 * \author Javier Arribas, 2011. jarribas(at)cttc.es
 *
 * Class that implements a 2nd order PLL filter for code tracking loop.
 * The algorithm is described in:
 * K.Borre, D.M.Akos, N.Bertelsen, P.Rinder, and S. H. Jensen,
 * A Software-Defined GPS and Galileo Receiver. A Single-Frequency Approach,
 * Birkhauser, 2007, Applied and Numerical Harmonic Analysis.
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2012  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_TRACKING_2ND_ALL_FILTER_H_
#define GNSS_SDR_TRACKING_2ND_ALL_FILTER_H_

/*!
 * \brief This class implements a 2nd order ALL filter for code tracking loop.
 *
 * The algorithm is described in:
 * K.Borre, D.M.Akos, N.Bertelsen, P.Rinder, and S. H. Jensen, A Software-Defined GPS
 * and Galileo Receiver. A Single-Frequency Approach,
 * Birkhauser, 2007, Applied and Numerical Harmonic Analysis.
 */
class Tracking_2nd_ALL_filter
{
private:
    // PLL filter parameters
    float d_tau1_amplitude;
    float d_tau2_amplitude;
    float d_pdi_amplitude;
    float d_allnoisebandwidth;
    float d_alldampingratio;
    float d_old_amplitude_error;
    float d_old_amplitude_nco;
    float old_result;
    float old_discriminator;
    float x;
    float x1;
    float x2;
    float y1;
    float y2;
    void calculate_lopp_coef(float* tau1,float* tau2, float lbw, float zeta, float k);

public:
    void set_ALL_BW(float all_bw_hz);                //! Set DLL filter bandwidth [Hz]
    void set_pdi(float pdi_amplitude); //! Set Summation interval for code [s]
    void initialize(); //! Start tracking with acquisition information
    float get_amplitude_nco(float ALL_discriminator);     //! Numerically controlled oscillator
    Tracking_2nd_ALL_filter(float pdi_code);
    Tracking_2nd_ALL_filter();
    ~Tracking_2nd_ALL_filter();
};

#endif
