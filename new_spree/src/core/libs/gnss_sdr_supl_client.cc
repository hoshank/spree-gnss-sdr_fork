/*!
 * \file gnss_sdr_supl_client.c
 * \brief class that implements a C++ interface to external Secure User Location Protocol (SUPL) client library.
 * \author Javier Arribas, 2013. jarribas(at)cttc.es
 *
 * TODO: put here supl.c author info
 * class that implements a C++ interface to external Secure User Location Protocol (SUPL) client library.
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

#include "gnss_sdr_supl_client.h"
#include "GPS_L1_CA.h"
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <glog/logging.h>
#include <pugixml.hpp>
#include <cmath>      // for pow
#include <cstring>    // for strcpy
#include <exception>  // for exception
#include <iostream>   // for cerr
#include <utility>    // for pair

Gnss_Sdr_Supl_Client::Gnss_Sdr_Supl_Client()
{
    mcc = 0;
    mns = 0;
    lac = 0;
    ci = 0;
    supl_ctx_new(&ctx);
    assist = supl_assist_t();
    server_port = 0;
    request = 0;
}


Gnss_Sdr_Supl_Client::~Gnss_Sdr_Supl_Client() = default;


void Gnss_Sdr_Supl_Client::print_assistance()
{
    if (assist.set & SUPL_RRLP_ASSIST_REFTIME)
        {
            fprintf(stdout, "T %ld %ld %ld %ld\n", assist.time.gps_week, assist.time.gps_tow,
                assist.time.stamp.tv_sec, static_cast<long>(assist.time.stamp.tv_usec));
        }

    if (assist.set & SUPL_RRLP_ASSIST_UTC)
        {
            fprintf(stdout, "U %d %d %d %d %d %d %d %d\n",
                assist.utc.a0, assist.utc.a1, assist.utc.delta_tls,
                assist.utc.tot, assist.utc.wnt, assist.utc.wnlsf,
                assist.utc.dn, assist.utc.delta_tlsf);
        }

    if (assist.set & SUPL_RRLP_ASSIST_REFLOC)
        {
            fprintf(stdout, "L %f %f %d\n", assist.pos.lat, assist.pos.lon, assist.pos.uncertainty);
        }

    if (assist.set & SUPL_RRLP_ASSIST_IONO)
        {
            fprintf(stdout, "I %d %d %d %d %d %d %d %d\n",
                assist.iono.a0, assist.iono.a1, assist.iono.a2, assist.iono.a3,
                assist.iono.b0, assist.iono.b1, assist.iono.b2, assist.iono.b3);
        }

    if (assist.cnt_eph)
        {
            int i;

            fprintf(stdout, "E %d\n", assist.cnt_eph);

            for (i = 0; i < assist.cnt_eph; i++)
                {
                    struct supl_ephemeris_s* e = &assist.eph[i];

                    fprintf(stdout, "e %d %d %d %d %d %d %d %d %d %d",
                        e->prn, e->delta_n, e->M0, e->A_sqrt, e->OMEGA_0, e->i0, e->w, e->OMEGA_dot, e->i_dot, e->e);
                    fprintf(stdout, " %d %d %d %d %d %d",
                        e->Cuc, e->Cus, e->Crc, e->Crs, e->Cic, e->Cis);
                    fprintf(stdout, " %d %d %d %d %d %d",
                        e->toe, e->IODC, e->toc, e->AF0, e->AF1, e->AF2);
                    fprintf(stdout, " %d %d %d %d %d\n",
                        e->bits, e->ura, e->health, e->tgd, e->AODA);
                }
        }

    if (assist.cnt_alm)
        {
            int i;

            fprintf(stdout, "A %d\n", assist.cnt_alm);
            for (i = 0; i < assist.cnt_alm; i++)
                {
                    struct supl_almanac_s* a = &assist.alm[i];

                    fprintf(stdout, "a %d %d %d %d %d ",
                        a->prn, a->e, a->toa, a->Ksii, a->OMEGA_dot);
                    fprintf(stdout, "%d %d %d %d %d %d\n",
                        a->A_sqrt, a->OMEGA_0, a->w, a->M0, a->AF0, a->AF1);
                }
        }

    if (assist.cnt_acq)
        {
            int i;

            fprintf(stdout, "Q %d %d\n", assist.cnt_acq, assist.acq_time);
            for (i = 0; i < assist.cnt_acq; i++)
                {
                    struct supl_acquis_s* q = &assist.acq[i];

                    fprintf(stdout, "q %d %d %d ",
                        q->prn, q->parts, q->doppler0);
                    if (q->parts & SUPL_ACQUIS_DOPPLER)
                        {
                            fprintf(stdout, "%d %d ", q->doppler1, q->d_win);
                        }
                    else
                        {
                            fprintf(stdout, "0 0 ");
                        }
                    fprintf(stdout, "%d %d %d %d ",
                        q->code_ph, q->code_ph_int, q->bit_num, q->code_ph_win);
                    if (q->parts & SUPL_ACQUIS_ANGLE)
                        {
                            fprintf(stdout, "%d %d\n", q->az, q->el);
                        }
                    else
                        {
                            fprintf(stdout, "0 0\n");
                        }
                }
        }
}


int Gnss_Sdr_Supl_Client::get_assistance(int i_mcc, int i_mns, int i_lac, int i_ci)
{
    // SET SUPL CLIENT INFORMATION
    // GSM CELL PARAMETERS
    mcc = i_mcc;
    mns = i_mns;
    lac = i_lac;
    ci = i_ci;
    if (supl_ctx_new(&ctx))
        {
        }  // clean it before using
    supl_set_gsm_cell(&ctx, mcc, mns, lac, ci);

    // PERFORM SUPL COMMUNICATION
    char* cstr = new char[server_name.length() + 1];
    strcpy(cstr, server_name.c_str());

    int err;
    ctx.p.request = request;  // select assistance info request from a pre-defined set

    //std::cout<<"mcc="<<mcc<<"mns="<<mns<<"lac="<<lac<<"ci="<<ci<<std::endl;
    err = supl_get_assist(&ctx, cstr, &assist);
    if (err == 0)
        {
            read_supl_data();
            if (supl_ctx_free(&ctx))
                {
                }  // clean it up before leaving
        }
    else
        {
            /*
	   * If supl_get_assist() fails, the connection remains open
	   * and the memory/files are not released.
	   */
            supl_close(&ctx);
        }
    delete[] cstr;
    return err;
}


void Gnss_Sdr_Supl_Client::read_supl_data()
{
    // READ REFERENCE LOCATION
    if (assist.set & SUPL_RRLP_ASSIST_REFLOC)
        {
            gps_ref_loc.lat = assist.pos.lat;
            gps_ref_loc.lon = assist.pos.lon;
            gps_ref_loc.uncertainty = assist.pos.uncertainty;
            gps_ref_loc.valid = true;
        }

    // READ REFERENCE TIME
    if (assist.set & SUPL_RRLP_ASSIST_REFTIME)
        {
            /* TS 44.031: GPSTOW, range 0-604799.92, resolution 0.08 sec, 23-bit presentation */
            gps_time.d_TOW = static_cast<double>(assist.time.gps_tow) * 0.08;
            gps_time.d_Week = static_cast<double>(assist.time.gps_week);
            gps_time.d_tv_sec = static_cast<double>(assist.time.stamp.tv_sec);
            gps_time.d_tv_usec = static_cast<double>(assist.time.stamp.tv_usec);
            gps_time.valid = true;
        }

    // READ UTC MODEL
    if (assist.set & SUPL_RRLP_ASSIST_UTC)
        {
            gps_utc.d_A0 = static_cast<double>(assist.utc.a0) * pow(2.0, -30);
            gps_utc.d_A1 = static_cast<double>(assist.utc.a1) * pow(2.0, -50);
            gps_utc.d_DeltaT_LS = static_cast<int32_t>(assist.utc.delta_tls);
            gps_utc.d_DeltaT_LSF = static_cast<int32_t>(assist.utc.delta_tlsf);
            gps_utc.d_t_OT = static_cast<int32_t>(assist.utc.tot) * pow(2.0, 12);
            gps_utc.i_DN = static_cast<int32_t>(assist.utc.dn);
            gps_utc.i_WN_T = static_cast<int32_t>(assist.utc.wnt);
            gps_utc.i_WN_LSF = static_cast<int32_t>(assist.utc.wnlsf);
            gps_utc.valid = true;
        }

    // READ IONOSPHERIC MODEL
    if (assist.set & SUPL_RRLP_ASSIST_IONO)
        {
            gps_iono.d_alpha0 = static_cast<double>(assist.iono.a0) * ALPHA_0_LSB;
            gps_iono.d_alpha1 = static_cast<double>(assist.iono.a1) * ALPHA_1_LSB;
            gps_iono.d_alpha2 = static_cast<double>(assist.iono.a2) * ALPHA_2_LSB;
            gps_iono.d_alpha3 = static_cast<double>(assist.iono.a3) * ALPHA_3_LSB;

            gps_iono.d_beta0 = static_cast<double>(assist.iono.b0) * BETA_0_LSB;
            gps_iono.d_beta1 = static_cast<double>(assist.iono.b1) * BETA_1_LSB;
            gps_iono.d_beta2 = static_cast<double>(assist.iono.b2) * BETA_2_LSB;
            gps_iono.d_beta3 = static_cast<double>(assist.iono.b3) * BETA_3_LSB;
            gps_iono.valid = true;
        }

    // READ SV ALMANAC
    if (assist.cnt_alm)
        {
            std::map<int, Gps_Almanac>::iterator gps_almanac_iterator;
            for (int i = 0; i < assist.cnt_alm; i++)
                {
                    struct supl_almanac_s* a = &assist.alm[i];
                    // Check if the SV is present in the map
                    gps_almanac_iterator = this->gps_almanac_map.find(a->prn);
                    // the SV is not present in the almanac data -> insert new SV register
                    if (gps_almanac_iterator == gps_almanac_map.end())
                        {
                            Gps_Almanac gps_almanac_entry;
                            gps_almanac_map.insert(std::pair<int, Gps_Almanac>(a->prn, gps_almanac_entry));
                            gps_almanac_iterator = this->gps_almanac_map.find(a->prn);
                        }
                    gps_almanac_iterator->second.i_satellite_PRN = a->prn;
                    gps_almanac_iterator->second.d_A_f0 = static_cast<double>(a->AF0) * pow(2.0, -20);
                    gps_almanac_iterator->second.d_A_f1 = static_cast<double>(a->AF1) * pow(2.0, -38);
                    gps_almanac_iterator->second.d_Delta_i = static_cast<double>(a->Ksii) * pow(2.0, -19);
                    gps_almanac_iterator->second.d_OMEGA = static_cast<double>(a->w) * pow(2.0, -23);
                    gps_almanac_iterator->second.d_OMEGA0 = static_cast<double>(a->OMEGA_0) * pow(2.0, -23);
                    gps_almanac_iterator->second.d_sqrt_A = static_cast<double>(a->A_sqrt) * pow(2.0, -11);
                    gps_almanac_iterator->second.d_OMEGA_DOT = static_cast<double>(a->OMEGA_dot) * pow(2.0, -38);
                    gps_almanac_iterator->second.i_Toa = static_cast<int32_t>(a->toa * pow(2.0, 12));
                    gps_almanac_iterator->second.d_e_eccentricity = static_cast<double>(a->e) * pow(2.0, -21);
                    gps_almanac_iterator->second.d_M_0 = static_cast<double>(a->M0) * pow(2.0, -23);
                }
        }

    // READ SV EPHEMERIS
    if (assist.cnt_eph)
        {
            std::map<int, Gps_Ephemeris>::iterator gps_eph_iterator;
            for (int i = 0; i < assist.cnt_eph; i++)
                {
                    struct supl_ephemeris_s* e = &assist.eph[i];
                    // Check if the SV is present in the map
                    gps_eph_iterator = this->gps_ephemeris_map.find(e->prn);
                    // the SV is not present in the assistance data -> insert new SV register
                    if (gps_eph_iterator == gps_ephemeris_map.end())
                        {
                            Gps_Ephemeris gps_eph;
                            gps_ephemeris_map.insert(std::pair<int, Gps_Ephemeris>(e->prn, gps_eph));
                            gps_eph_iterator = this->gps_ephemeris_map.find(e->prn);
                        }
                    if (gps_time.valid)
                        {
                            gps_eph_iterator->second.i_GPS_week = assist.time.gps_week;
                            /* TS 44.031: GPSTOW, range 0-604799.92, resolution 0.08 sec, 23-bit presentation */
                            gps_eph_iterator->second.d_TOW = static_cast<double>(assist.time.gps_tow) * 0.08;
                        }
                    else
                        {
                            gps_eph_iterator->second.i_GPS_week = 0;
                            gps_eph_iterator->second.d_TOW = 0;
                        }
                    gps_eph_iterator->second.i_satellite_PRN = e->prn;
                    // SV navigation model
                    gps_eph_iterator->second.i_code_on_L2 = e->bits;
                    gps_eph_iterator->second.i_SV_accuracy = e->ura;  //User Range Accuracy (URA)
                    gps_eph_iterator->second.i_SV_health = e->health;
                    gps_eph_iterator->second.d_IODC = static_cast<double>(e->IODC);
                    //miss P flag (1 bit)
                    //miss SF1 Reserved (87 bits)
                    gps_eph_iterator->second.d_TGD = static_cast<double>(e->tgd) * T_GD_LSB;
                    gps_eph_iterator->second.d_Toc = static_cast<double>(e->toc) * T_OC_LSB;
                    gps_eph_iterator->second.d_A_f0 = static_cast<double>(e->AF0) * A_F0_LSB;
                    gps_eph_iterator->second.d_A_f1 = static_cast<double>(e->AF1) * A_F1_LSB;
                    gps_eph_iterator->second.d_A_f2 = static_cast<double>(e->AF2) * A_F2_LSB;
                    gps_eph_iterator->second.d_Crc = static_cast<double>(e->Crc) * C_RC_LSB;
                    gps_eph_iterator->second.d_Delta_n = static_cast<double>(e->delta_n) * DELTA_N_LSB;
                    gps_eph_iterator->second.d_M_0 = static_cast<double>(e->M0) * M_0_LSB;
                    gps_eph_iterator->second.d_Cuc = static_cast<double>(e->Cuc) * C_UC_LSB;
                    gps_eph_iterator->second.d_e_eccentricity = static_cast<double>(e->e) * E_LSB;
                    gps_eph_iterator->second.d_Cus = static_cast<double>(e->Cus) * C_US_LSB;
                    gps_eph_iterator->second.d_sqrt_A = static_cast<double>(e->A_sqrt) * SQRT_A_LSB;
                    gps_eph_iterator->second.d_Toe = static_cast<double>(e->toe) * T_OE_LSB;
                    //miss fit interval flag (1 bit)
                    gps_eph_iterator->second.i_AODO = e->AODA * AODO_LSB;
                    gps_eph_iterator->second.d_Cic = static_cast<double>(e->Cic) * C_IC_LSB;
                    gps_eph_iterator->second.d_OMEGA0 = static_cast<double>(e->OMEGA_0) * OMEGA_0_LSB;
                    gps_eph_iterator->second.d_Cis = static_cast<double>(e->Cis) * C_IS_LSB;
                    gps_eph_iterator->second.d_i_0 = static_cast<double>(e->i0) * I_0_LSB;
                    gps_eph_iterator->second.d_Crs = static_cast<double>(e->Crs) * C_RS_LSB;
                    gps_eph_iterator->second.d_OMEGA = static_cast<double>(e->w) * OMEGA_LSB;
                    gps_eph_iterator->second.d_OMEGA_DOT = static_cast<double>(e->OMEGA_dot) * OMEGA_DOT_LSB;
                    gps_eph_iterator->second.d_IDOT = static_cast<double>(e->i_dot) * I_DOT_LSB;
                }
        }

    // READ SV ACQUISITION ASSISTANCE

    if (assist.cnt_acq)
        {
            std::map<int, Gps_Acq_Assist>::iterator gps_acq_iterator;
            for (int i = 0; i < assist.cnt_acq; i++)
                {
                    struct supl_acquis_s* e = &assist.acq[i];
                    // Check if the SV is present in the map
                    gps_acq_iterator = this->gps_acq_map.find(e->prn);
                    // the SV is not present in the assistance data -> insert new SV register
                    if (gps_acq_iterator == gps_acq_map.end())
                        {
                            Gps_Acq_Assist gps_acq_assist;
                            gps_acq_map.insert(std::pair<int, Gps_Acq_Assist>(e->prn, gps_acq_assist));
                            gps_acq_iterator = this->gps_acq_map.find(e->prn);
                        }
                    // fill the acquisition assistance structure
                    gps_acq_iterator->second.i_satellite_PRN = e->prn;
                    gps_acq_iterator->second.d_TOW = static_cast<double>(assist.acq_time);
                    gps_acq_iterator->second.d_Doppler0 = static_cast<double>(e->doppler0);
                    gps_acq_iterator->second.d_Doppler1 = static_cast<double>(e->doppler1);
                    gps_acq_iterator->second.dopplerUncertainty = static_cast<double>(e->d_win);
                    gps_acq_iterator->second.Code_Phase = static_cast<double>(e->code_ph);
                    gps_acq_iterator->second.Code_Phase_int = static_cast<double>(e->code_ph_int);
                    gps_acq_iterator->second.Code_Phase_window = static_cast<double>(e->code_ph_win);
                    gps_acq_iterator->second.Azimuth = static_cast<double>(e->az);
                    gps_acq_iterator->second.Elevation = static_cast<double>(e->el);
                    gps_acq_iterator->second.GPS_Bit_Number = static_cast<double>(e->bit_num);
                }
        }
}


bool Gnss_Sdr_Supl_Client::load_ephemeris_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gps_ephemeris_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_ephemeris_map", this->gps_ephemeris_map);
            LOG(INFO) << "Loaded Ephemeris map data with " << this->gps_ephemeris_map.size() << " satellites";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_ephemeris_map_xml(const std::string& file_name, std::map<int, Gps_Ephemeris> eph_map)
{
    if (eph_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_ephemeris_map", eph_map);
                    LOG(INFO) << "Saved Ephemeris map data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Ephemeris, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gal_ephemeris_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gal_ephemeris_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_gal_ephemeris_map", this->gal_ephemeris_map);
            LOG(INFO) << "Loaded Ephemeris map data with " << this->gal_ephemeris_map.size() << " satellites";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool save_gal_ephemeris_map_xml(const std::string& file_name, std::map<int, Galileo_Ephemeris> eph_map)
{
    if (eph_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gal_ephemeris_map", eph_map);
                    LOG(INFO) << "Saved Galileo ephemeris map data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Galileo ephemeris, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_cnav_ephemeris_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gps_cnav_ephemeris_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_cnav_ephemeris_map", this->gps_cnav_ephemeris_map);
            LOG(INFO) << "Loaded Ephemeris map data with " << this->gps_cnav_ephemeris_map.size() << " satellites";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool save_cnav_ephemeris_map_xml(const std::string& file_name, std::map<int, Gps_CNAV_Ephemeris> eph_map)
{
    if (eph_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_cnav_ephemeris_map", eph_map);
                    LOG(INFO) << "Saved GPS CNAV ephemeris map data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save GPS CNAV ephemeris, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gnav_ephemeris_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gps_cnav_ephemeris_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_gnav_ephemeris_map", this->glonass_gnav_ephemeris_map);
            LOG(INFO) << "Loaded GLONASS ephemeris map data with " << this->gps_cnav_ephemeris_map.size() << " satellites";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool save_gnav_ephemeris_map_xml(const std::string& file_name, std::map<int, Glonass_Gnav_Ephemeris> eph_map)
{
    if (eph_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gnav_ephemeris_map", eph_map);
                    LOG(INFO) << "Saved GLONASS GNAV ephemeris map data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save GLONASS GNAV ephemeris, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_utc_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_utc_model", this->gps_utc);
            LOG(INFO) << "Loaded UTC model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_utc_xml(const std::string& file_name, Gps_Utc_Model& utc)
{
    if (utc.valid)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_utc_model", utc);
                    LOG(INFO) << "Saved GPS UTC Model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save GPS UTC model, no valid data";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_cnav_utc_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_cnav_utc_model", this->gps_cnav_utc);
            LOG(INFO) << "Loaded CNAV UTC model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_cnav_utc_xml(const std::string& file_name, Gps_CNAV_Utc_Model& utc)
{
    if (utc.valid)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_cnav_utc_model", utc);
                    LOG(INFO) << "Saved GPS CNAV UTC model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save GPS CNAV UTC model, no valid data";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gal_utc_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_gal_utc_model", this->gal_utc);
            LOG(INFO) << "Loaded Galileo UTC model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_gal_utc_xml(const std::string& file_name, Galileo_Utc_Model& utc)
{
    if (utc.flag_utc_model)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gal_utc_model", utc);
                    LOG(INFO) << "Saved Galileo UTC Model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Galileo UTC model, no valid data";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_iono_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_iono_model", this->gps_iono);
            LOG(INFO) << "Loaded IONO model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_iono_xml(const std::string& file_name, Gps_Iono &iono)
{
    if (iono.valid)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_iono_model", iono);
                    LOG(INFO) << "Saved IONO Model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save IONO model, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gal_iono_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_gal_iono_model", this->gal_iono);
            LOG(INFO) << "Loaded Galileo IONO model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_gal_iono_xml(const std::string& file_name, Galileo_Iono& iono)
{
    if (iono.ai0_5 != 0.0)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gal_iono_model", iono);
                    LOG(INFO) << "Saved Galileo IONO Model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Galileo IONO model, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gps_almanac_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gps_almanac_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_gps_almanac_map", this->gps_almanac_map);
            LOG(INFO) << "Loaded GPS almanac map data with " << this->gps_almanac_map.size() << " satellites";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_gps_almanac_xml(const std::string& file_name, std::map<int, Gps_Almanac> gps_almanac_map)
{
    if (gps_almanac_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gps_almanac_map", gps_almanac_map);
                    LOG(INFO) << "Saved GPS almanac data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save GPS almanac, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_gal_almanac_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            gal_almanac_map.clear();
            xml >> boost::serialization::make_nvp("GNSS-SDR_gal_almanac_map", this->gal_almanac_map);
        }
    catch (std::exception& e)
        {
            // Maybe the file is from https://www.gsc-europa.eu/system-status/almanac-data ?
            return this->read_gal_almanac_from_gsa(file_name);
        }
    LOG(INFO) << "Loaded Galileo almanac map data with " << this->gal_almanac_map.size() << " satellites";
    return true;
}


bool Gnss_Sdr_Supl_Client::read_gal_almanac_from_gsa(const std::string& file_name)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name.c_str());
    if (!result)
        {
            LOG(WARNING) << "Error loading file " << file_name << ":" << result.description();
            return false;
        }
    for (pugi::xml_node almanac : doc.child("signalData")
                                      .child("body")
                                      .child("Almanacs")
                                      .children("svAlmanac"))
        {
            Galileo_Almanac gal_alm;
            try
                {
                    uint32_t prn = static_cast<uint32_t>(std::stoi(almanac.child_value("SVID")));
                    gal_alm.i_satellite_PRN = prn;
                    gal_alm.i_Toa = std::stoi(almanac.child("almanac").child_value("t0a"));
                    gal_alm.i_WNa = std::stoi(almanac.child("almanac").child_value("wna"));
                    gal_alm.i_IODa = std::stoi(almanac.child("almanac").child_value("iod"));
                    gal_alm.d_Delta_i = std::stod(almanac.child("almanac").child_value("deltai"));
                    gal_alm.d_M_0 = std::stod(almanac.child("almanac").child_value("m0"));
                    gal_alm.d_e_eccentricity = std::stod(almanac.child("almanac").child_value("ecc"));
                    gal_alm.d_Delta_sqrt_A = std::stod(almanac.child("almanac").child_value("aSqRoot"));
                    gal_alm.d_OMEGA0 = std::stod(almanac.child("almanac").child_value("omega0"));
                    gal_alm.d_OMEGA = std::stod(almanac.child("almanac").child_value("w"));
                    gal_alm.d_OMEGA_DOT = std::stod(almanac.child("almanac").child_value("omegaDot"));
                    gal_alm.d_A_f0 = std::stod(almanac.child("almanac").child_value("af0"));
                    gal_alm.d_A_f1 = std::stod(almanac.child("almanac").child_value("af1"));
                    gal_alm.E5b_HS = std::stoi(almanac.child("svINavSignalStatus").child_value("statusE5b"));
                    gal_alm.E1B_HS = std::stoi(almanac.child("svINavSignalStatus").child_value("statusE1B"));
                    gal_alm.E5a_HS = std::stoi(almanac.child("svFNavSignalStatus").child_value("statusE5a"));

                    this->gal_almanac_map[static_cast<int>(prn)] = gal_alm;
                }
            catch (const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                }
        }
    if (this->gal_almanac_map.empty())
        {
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_gal_almanac_xml(const std::string& file_name, std::map<int, Galileo_Almanac> gal_almanac_map)
{
    if (gal_almanac_map.empty() == false)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_gal_almanac_map", gal_almanac_map);
                    LOG(INFO) << "Saved Galileo almanac data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Galileo almanac, map is empty";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_glo_utc_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_glo_utc_model", this->glo_gnav_utc);
            LOG(INFO) << "Loaded UTC model data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_glo_utc_xml(const std::string& file_name, Glonass_Gnav_Utc_Model& utc)
{
    if (utc.valid)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_glo_utc_model", utc);
                    LOG(INFO) << "Saved Glonass UTC Model data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Glonass UTC model, no valid data";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_ref_time_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_ref_time", this->gps_time);
            LOG(INFO) << "Loaded Ref Time data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_ref_time_xml(const std::string& file_name, Agnss_Ref_Time& ref_time)
{
    if (ref_time.valid)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_ref_time", ref_time);
                    LOG(INFO) << "Saved Ref Time data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Ref Time";
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::load_ref_location_xml(const std::string& file_name)
{
    std::ifstream ifs;
    try
        {
            ifs.open(file_name.c_str(), std::ifstream::binary | std::ifstream::in);
            boost::archive::xml_iarchive xml(ifs);
            xml >> boost::serialization::make_nvp("GNSS-SDR_ref_location", this->gps_ref_loc);
            LOG(INFO) << "Loaded Ref Location data";
        }
    catch (std::exception& e)
        {
            LOG(WARNING) << e.what() << "File: " << file_name;
            return false;
        }
    return true;
}


bool Gnss_Sdr_Supl_Client::save_ref_location_xml(const std::string& file_name, Agnss_Ref_Location& ref_location)
{
    if (ref_location.valid == true)
        {
            std::ofstream ofs;
            try
                {
                    ofs.open(file_name.c_str(), std::ofstream::trunc | std::ofstream::out);
                    boost::archive::xml_oarchive xml(ofs);
                    xml << boost::serialization::make_nvp("GNSS-SDR_ref_location", ref_location);
                    LOG(INFO) << "Saved Ref Location data";
                }
            catch (std::exception& e)
                {
                    LOG(WARNING) << e.what();
                    return false;
                }
        }
    else
        {
            LOG(WARNING) << "Failed to save Ref Location";
            return false;
        }
    return true;
}
