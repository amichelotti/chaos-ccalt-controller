/*
CCALTControllerConstants.h
!CHAOS
Created by CUGenerator

Copyright 2013 INFN, National Institute of Nuclear Physics
Licensed under the Apache License, Version 2.0 (the "License")
you may not use this file except in compliance with the License.
      You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef CCALTController_CCALTControllerConstants_h
#define CCALTController_CCALTControllerConstants_h
namespace driver {
	namespace ccaltcontroller {
		#define TROW_ERROR(n,m,d) throw chaos::CException(n, m, d);
		#define LOG_TAIL(n) "[" << #n << "] - " << getDeviceID() << " - [" << getUID() << "] - " 
		const char* const CMD_CCT_DEFAULT_ALIAS = "Default";
		const char* const CMD_CCT_POWERON_ALIAS = "PowerOn";
		const char* const CMD_CCT_POWERON_ON_STATE = "on_state";
		const char* const CMD_CCT_SETVOLTAGESONGIB_ALIAS = "SetVoltagesOnGib";
		const char* const CMD_CCT_SETVOLTAGESONGIB_GIBUID = "gibUID";
		const char* const CMD_CCT_SETVOLTAGESONGIB_CHANNEL = "channel";
		const char* const CMD_CCT_SETVOLTAGESONGIB_VOLTAGE = "voltage";
		const char* const CMD_CCT_SETPULSEONGIB_ALIAS = "SetPulseOnGib";
		const char* const CMD_CCT_SETPULSEONGIB_GIBUID = "gibUID";
		const char* const CMD_CCT_SETPULSEONGIB_CHANNEL = "channel";
		const char* const CMD_CCT_SETPULSEONGIB_AMPLITUDE = "amplitude";
		const char* const CMD_CCT_SETPULSEONGIB_WIDTH = "width";
		const char* const CMD_CCT_SETPULSEONGIB_STATE = "state";
		const char* const CMD_CCT_GOTOSETPOINT_ALIAS = "GoToSetPoint";
		#define DEFAULT_COMMAND_TIMEOUT_MS   10000
	}
}
#endif
