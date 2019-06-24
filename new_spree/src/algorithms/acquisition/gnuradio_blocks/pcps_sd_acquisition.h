/*!
 * \file pcps_sd_acquisition.h
 * \brief This class implements a Parallel Code Phase Search Acquisition
 *
 *  Acquisition strategy (Kay Borre book + CFAR threshold).
 *  <ol>
 *  <li> Compute the input signal power estimation
 *  <li> Doppler serial search loop
 *  <li> Perform the FFT-based circular convolution (parallel time search)
 *  <li> Record the maximum peak and the associated synchronization parameters
 *  <li> Compute the test statistics and compare to the threshold
 *  <li> Declare positive or negative acquisition using a message queue
 *  </ol>
 *
 * Kay Borre book: K.Borre, D.M.Akos, N.Bertelsen, P.Rinder, and S.H.Jensen,
 * "A Software-Defined GPS and Galileo Receiver. A Single-Frequency
 * Approach", Birkhauser, 2007. pp 81-84
 *
 * \authors <ul>
 *          <li> Javier Arribas, 2011. jarribas(at)cttc.es
 *          <li> Luis Esteve, 2012. luis(at)epsilon-formacion.com
 *          <li> Marc Molina, 2013. marc.molina.pena@gmail.com
 *          <li> Cillian O'Driscoll, 2017. cillian(at)ieee.org
 *          <li> Antonio Ramos, 2017. antonio.ramos@cttc.es
 *          </ul>
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2018  (see AUTHORS file for a list of contributors)
 *
 * GNSS-SDR is a software defined Global Navigation
 *          Satellite Systems receiver
 *
 * This file is part of GNSS-SDR.
 *
 * GNSS-SDR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GNSS-SDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNSS-SDR. If not, see <https://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#ifndef GNSS_SDR_PCPS_SD_ACQUISITION_H_
#define GNSS_SDR_PCPS_SD_ACQUISITION_H_

#include "acq_conf.h"
#include "channel_fsm.h"
#include <armadillo>
#include <gnuradio/block.h>
#include <gnuradio/fft/fft.h>
#include <gnuradio/gr_complex.h>     // for gr_complex
#include <gnuradio/thread/thread.h>  // for scoped_lock
#include <gnuradio/types.h>          // for gr_vector_const_void_star
#include <volk/volk_complex.h>       // for lv_16sc_t
#include <cstdint>
#include <string>
#include <utility>

class Gnss_Synchro;
class pcps_sd_acquisition;

using pcps_sd_acquisition_sptr = boost::shared_ptr<pcps_sd_acquisition>;


pcps_sd_acquisition_sptr
pcps_make_sd_acquisition(const Acq_Conf& conf_);

/*!
 * \brief This class implements a Parallel Code Phase Search Acquisition.
 *
 * Check \ref Navitec2012 "An Open Source Galileo E1 Software Receiver",
 * Algorithm 1, for a pseudocode description of this implementation.
 */
class pcps_sd_acquisition : public gr::block
{
private:
    friend pcps_sd_acquisition_sptr
    pcps_make_sd_acquisition(const Acq_Conf& conf_);

    pcps_sd_acquisition(const Acq_Conf& conf_);

    struct Peak{
        int code_phase;
        int doppler;
        float mag;
        float test_stats;
    };

    void update_local_carrier(gr_complex* carrier_vector, int32_t correlator_length_samples, float freq);
    void update_grid_doppler_wipeoffs();
    void update_grid_doppler_wipeoffs_step2();
    bool is_fdma();

    void acquisition_core(uint64_t samp_count);

    void send_negative_acquisition();

    void send_positive_acquisition();

    void dump_results(int32_t effective_fft_size);

    float first_vs_second_peak_statistic(uint32_t& indext, int32_t& doppler, uint32_t num_doppler_bins, int32_t doppler_max, int32_t doppler_step);
    std::map<float, Peak> max_to_input_power_statistic(uint32_t& indext, int32_t& doppler, float input_power, float spoofing_threshold, bool acquire_auxiliary_peaks, uint32_t num_doppler_bins, int32_t doppler_max, int32_t doppler_step);

    bool start();


    Acq_Conf acq_parameters;
    bool d_active;
    bool d_worker_active;
    bool d_cshort;
    bool d_step_two;
    bool d_use_CFAR_algorithm_flag;
    int32_t d_positive_acq;
    float d_threshold;
    float d_mag;
    float d_input_power;
    float d_test_statistics;
    float* d_magnitude;
    float** d_magnitude_grid;
    float* d_tmp_buffer;
    gr_complex* d_input_signal;
    uint32_t d_samplesPerChip;
    int64_t d_old_freq;
    int32_t d_state;
    uint32_t d_channel;
    std::weak_ptr<ChannelFsm> d_channel_fsm;
    uint32_t d_doppler_step;
    float d_doppler_center_step_two;
    uint32_t d_num_noncoherent_integrations_counter;
    uint32_t d_fft_size;
    uint32_t d_consumed_samples;
    uint32_t d_num_doppler_bins;
    uint64_t d_sample_counter;
    gr_complex** d_grid_doppler_wipeoffs;
    gr_complex** d_grid_doppler_wipeoffs_step_two;
    gr_complex* d_fft_codes;
    gr_complex* d_data_buffer;
    lv_16sc_t* d_data_buffer_sc;
    gr::fft::fft_complex* d_fft_if;
    gr::fft::fft_complex* d_ifft;
    Gnss_Synchro* d_gnss_synchro;
    arma::fmat grid_;
    arma::fmat narrow_grid_;
    uint32_t d_num_doppler_bins_step2;
    int64_t d_dump_number;
    uint32_t d_dump_channel;
    uint32_t d_buffer_count;
    bool d_dump;
    std::string d_dump_filename;
    unsigned int d_peak;

public:
    ~pcps_sd_acquisition();

    /*!
     * \brief Set acquisition/tracking common Gnss_Synchro object pointer
     * to exchange synchronization data between acquisition and tracking blocks.
     * \param p_gnss_synchro Satellite information shared by the processing blocks.
     */
    inline void set_gnss_synchro(Gnss_Synchro* p_gnss_synchro)
    {
        gr::thread::scoped_lock lock(d_setlock);  // require mutex with work function called by the scheduler
        d_gnss_synchro = p_gnss_synchro;
    }

    /*!
     * \brief Returns the maximum peak of grid search.
     */
    inline uint32_t mag() const
    {
        return d_mag;
    }

    /*!
     * \brief Initializes acquisition algorithm and reserves memory.
     */
    void init();

    /*!
     * \brief Sets local code for PCPS acquisition algorithm.
     * \param code - Pointer to the PRN code.
     */
    void set_local_code(std::complex<float>* code);

    /*!
     * \brief Starts acquisition algorithm, turning from standby mode to
     * active mode
     * \param active - bool that activates/deactivates the block.
     */
    inline void set_active(bool active)
    {
        gr::thread::scoped_lock lock(d_setlock);  // require mutex with work function called by the scheduler
        d_active = active;
    }

    /*!
     * \brief If set to 1, ensures that acquisition starts at the
     * first available sample.
     * \param state - int=1 forces start of acquisition
     */
    void set_state(int32_t state);

    /*!
     * \brief Set acquisition channel unique ID
     * \param channel - receiver channel.
     */
    inline void set_channel(uint32_t channel)
    {
        d_channel = channel;
    }

    /*!
     * \brief Set channel fsm associated to this acquisition instance
     */
    inline void set_channel_fsm(std::weak_ptr<ChannelFsm> channel_fsm)
    {
        d_channel_fsm = std::move(channel_fsm);
    }

    /*!
     * \brief Set statistics threshold of PCPS algorithm.
     * \param threshold - Threshold for signal detection (check \ref Navitec2012,
     * Algorithm 1, for a definition of this threshold).
     */
    inline void set_threshold(float threshold)
    {
        gr::thread::scoped_lock lock(d_setlock);  // require mutex with work function called by the scheduler
        d_threshold = threshold;
    }

    /*!
     * \brief Set maximum Doppler grid search
     * \param doppler_max - Maximum Doppler shift considered in the grid search [Hz].
     */
    inline void set_doppler_max(uint32_t doppler_max)
    {
        gr::thread::scoped_lock lock(d_setlock);  // require mutex with work function called by the scheduler
        acq_parameters.doppler_max = doppler_max;
    }

    /*!
     * \brief Set Doppler steps for the grid search
     * \param doppler_step - Frequency bin of the search grid [Hz].
     */
    inline void set_doppler_step(uint32_t doppler_step)
    {
        gr::thread::scoped_lock lock(d_setlock);  // require mutex with work function called by the scheduler
        d_doppler_step = doppler_step;
    }


    void set_resampler_latency(uint32_t latency_samples);

    /*!
     * \brief Parallel Code Phase Search Acquisition signal processing.
     */
    int general_work(int noutput_items, gr_vector_int& ninput_items,
        gr_vector_const_void_star& input_items,
        gr_vector_void_star& output_items);
};

#endif /* GNSS_SDR_pcps_sd_acquisition_H_*/
