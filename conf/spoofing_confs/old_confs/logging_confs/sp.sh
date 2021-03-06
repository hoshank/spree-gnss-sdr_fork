#!/bin/bash
 sed -i '/Spoofing.ap_detection = false/c\Spoofing.ap_detection = true' loc*
 sed -i '/Channels.in_acquisition= 5/c\Channels.in_acquisition= 12' loc*
 sed -i '/Channels_GPS.count = 5/c\Channels_GPS.count = 24' loc*
 sed -i '/Acquisition_GPS.implementation=GPS_L1_CA_PCPS_Acquisition/c\Acquisition_GPS.implementation=GPS_L1_CA_PCPS_SD_Acquisition' loc*
 sed -i '/PVT.implementation=GPS_L1_CA_PVT/c\PVT.implementation=GPS_L1_CA_SD_PVT' loc*
 sed -i '/TelemetryDecoder_GPS.implementation=GPS_L1_CA_Telemetry_Decoder/c\TelemetryDecoder_GPS.implementation=GPS_L1_CA_SD_Telemetry_Decoder' loc*
