/*!
 * \file gps_l1_ca_pcps_sd_acquisition.cc
 * \brief Adapts a PCPS acquisition block to an AcquisitionInterface for
 *  GPS L1 C/A signals
 * \authors <ul>
 *          <li> Javier Arribas, 2011. jarribas(at)cttc.es
 *          <li> Luis Esteve, 2012. luis(at)epsilon-formacion.com
 *          <li> Marc Molina, 2013. marc.molina.pena(at)gmail.com
 *          <li> Hildur Ólafsdóttir, 2014. ohildur(at)gmail.com
 *          </ul>
 *
 * -------------------------------------------------------------------------
 *
 * Copyright (C) 2010-2014  (see AUTHORS file for a list of contributors)
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

#include "gps_l1_ca_pcps_sd_acquisition.h"
#include <iostream>
#include <stdexcept>
#include <boost/math/distributions/exponential.hpp>
#include <glog/logging.h>
#include <gnuradio/msg_queue.h>
#include "gps_sdr_signal_processing.h"
#include "GPS_L1_CA.h"
#include "configuration_interface.h"


using google::LogMessage;

GpsL1CaPcpsSdAcquisition::GpsL1CaPcpsSdAcquisition(
        ConfigurationInterface* configuration, std::string role,
        unsigned int in_streams, unsigned int out_streams,
        gr::msg_queue::sptr queue) :
    role_(role), in_streams_(in_streams), out_streams_(out_streams), queue_(queue)
{
    configuration_ = configuration;
    std::string default_item_type = "gr_complex";
    std::string default_dump_filename = "./data/acquisition.dat";

    DLOG(INFO) << "role " << role;

    item_type_ = configuration_->property(role + ".item_type",
            default_item_type);

    fs_in_ = configuration_->property("GNSS-SDR.internal_fs_hz", 2048000);
    DLOG(INFO) << "###fs_in: " << fs_in_;
    if_ = configuration_->property(role + ".ifreq", 0);
    dump_ = configuration_->property(role + ".dump", false);
    shift_resolution_ = configuration_->property(role + ".doppler_max", 15);
    sampled_ms_ = configuration_->property(role + ".coherent_integration_time_ms", 1);
    DLOG(INFO) << "###sampled_ms: " << sampled_ms_;

    bit_transition_flag_ = configuration_->property(role + ".bit_transition_flag", false);

    if (!bit_transition_flag_)
        {
            max_dwells_ = configuration_->property(role + ".max_dwells", 1);
        }
    else
        {
            max_dwells_ = 2;
        }

    dump_filename_ = configuration_->property(role + ".dump_filename",
            default_dump_filename);

    //--- Find number of samples per spreading code -------------------------
    code_length_ = round(fs_in_
            / (GPS_L1_CA_CODE_RATE_HZ / GPS_L1_CA_CODE_LENGTH_CHIPS));

    vector_length_ = code_length_ * sampled_ms_;

    code_= new gr_complex[vector_length_];

    if (item_type_.compare("gr_complex") == 0)
    {
        item_size_ = sizeof(gr_complex);
        acquisition_cc_ = pcps_make_sd_acquisition_cc(sampled_ms_, max_dwells_,
                shift_resolution_, if_, fs_in_, code_length_, code_length_,
                bit_transition_flag_, queue_, dump_, dump_filename_);

        stream_to_vector_ = gr::blocks::stream_to_vector::make(item_size_, vector_length_);

        DLOG(INFO) << "stream_to_vector(" << stream_to_vector_->unique_id()
                << ")";
        DLOG(INFO) << "acquisition(" << acquisition_cc_->unique_id()
                << ")";
    }
    else
    {
        LOG(WARNING) << item_type_
                << " unknown acquisition item type";
    }
}


GpsL1CaPcpsSdAcquisition::~GpsL1CaPcpsSdAcquisition()
{
	delete[] code_;
}


void GpsL1CaPcpsSdAcquisition::set_channel(unsigned int channel)
{
    channel_ = channel;
    if (item_type_.compare("gr_complex") == 0)
    {
        acquisition_cc_->set_channel(channel_);
    }
}


void GpsL1CaPcpsSdAcquisition::set_threshold(float threshold)
{
	float pfa = configuration_->property(role_ + boost::lexical_cast<std::string>(channel_) + ".pfa", 0.0);

	if(pfa == 0.0)
        {
                 pfa = configuration_->property(role_+".pfa", 0.0);
        }
	if(pfa == 0.0)
		{
			threshold_ = threshold;
		}
	else
		{
			threshold_ = calculate_threshold(pfa);
		}

	DLOG(INFO) <<"Channel "<<channel_<<" Threshold = " << threshold_;

    if (item_type_.compare("gr_complex") == 0)
    {
        acquisition_cc_->set_threshold(threshold_);
    }
}


void GpsL1CaPcpsSdAcquisition::set_doppler_max(unsigned int doppler_max)
{
    doppler_max_ = doppler_max;
    if (item_type_.compare("gr_complex") == 0)
    {
        acquisition_cc_->set_doppler_max(doppler_max_);
    }
}


void GpsL1CaPcpsSdAcquisition::set_doppler_step(unsigned int doppler_step)
{
    doppler_step_ = doppler_step;
    if (item_type_.compare("gr_complex") == 0)
        {
            acquisition_cc_->set_doppler_step(doppler_step_);
        }

}


void GpsL1CaPcpsSdAcquisition::set_channel_queue(
        concurrent_queue<int> *channel_internal_queue)
{
    channel_internal_queue_ = channel_internal_queue;
    if (item_type_.compare("gr_complex") == 0)
        {
            acquisition_cc_->set_channel_queue(channel_internal_queue_);
        }
}


void GpsL1CaPcpsSdAcquisition::set_gnss_synchro(Gnss_Synchro* gnss_synchro)
{
    gnss_synchro_ = gnss_synchro;
    if (item_type_.compare("gr_complex") == 0)
        {
            acquisition_cc_->set_gnss_synchro(gnss_synchro_);
        }
}

void GpsL1CaPcpsSdAcquisition::set_peak(unsigned int peak)
{
    peak_ = peak;
    if (item_type_.compare("gr_complex") == 0)
        {
            acquisition_cc_->set_peak(peak_);
        }
}


signed int GpsL1CaPcpsSdAcquisition::mag()
{
    if (item_type_.compare("gr_complex") == 0)
        {
            return acquisition_cc_->mag();
        }
    else
        {
            return 0;
        }
}


void GpsL1CaPcpsSdAcquisition::init()
{
    acquisition_cc_->init();
    set_local_code();
}


void GpsL1CaPcpsSdAcquisition::set_local_code()
{
    if (item_type_.compare("gr_complex") == 0)
    {
        std::complex<float>* code = new std::complex<float>[code_length_];

        gps_l1_ca_code_gen_complex_sampled(code, gnss_synchro_->PRN, fs_in_, 0);

        for (unsigned int i = 0; i < sampled_ms_; i++)
            {
                memcpy(&(code_[i*code_length_]), code,
                       sizeof(gr_complex)*code_length_);
            }

        acquisition_cc_->set_local_code(code_);

        delete[] code;
    }
}


void GpsL1CaPcpsSdAcquisition::reset()
{
    if (item_type_.compare("gr_complex") == 0)
    {
        acquisition_cc_->set_active(true);
    }
}


float GpsL1CaPcpsSdAcquisition::calculate_threshold(float pfa)
{
    //Calculate the threshold
    unsigned int frequency_bins = 0;
    for (int doppler = (int)(-doppler_max_); doppler <= (int)doppler_max_; doppler += doppler_step_)
        {
            frequency_bins++;
        }
    DLOG(INFO) << "Channel " << channel_<< "  Pfa = " << pfa;
    unsigned int ncells = vector_length_*frequency_bins;
    double exponent = 1/(double)ncells;
    double val = pow(1.0 - pfa, exponent);
    double lambda = double(vector_length_);
    boost::math::exponential_distribution<double> mydist (lambda);
    float threshold = (float)quantile(mydist,val);

    return threshold;
}


void GpsL1CaPcpsSdAcquisition::connect(gr::top_block_sptr top_block)
{
    if (item_type_.compare("gr_complex") == 0)
        {
            top_block->connect(stream_to_vector_, 0, acquisition_cc_, 0);
        }

}


void GpsL1CaPcpsSdAcquisition::disconnect(gr::top_block_sptr top_block)
{
    if (item_type_.compare("gr_complex") == 0)
    {
        top_block->disconnect(stream_to_vector_, 0, acquisition_cc_, 0);
    }
}


gr::basic_block_sptr GpsL1CaPcpsSdAcquisition::get_left_block()
{
    return stream_to_vector_;
}


gr::basic_block_sptr GpsL1CaPcpsSdAcquisition::get_right_block()
{
    return acquisition_cc_;
}

