/*
ChaosCCALTControllerInterface.h
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
#ifndef __ChaosCCALTControllerInterface__
#define __ChaosCCALTControllerInterface__
#include <iostream>
#include <chaos/cu_toolkit/driver_manager/driver/DriverTypes.h>
#include <chaos/cu_toolkit/driver_manager/driver/DriverAccessor.h>
#include <common/debug/core/debug.h>
#include <stdint.h>
#include "AbstractCCALTController.h"
namespace chaos_driver=::chaos::cu::driver_manager::driver;
namespace chaos {
	namespace driver {
		#define MAX_STR_SIZE 256
		namespace ccaltcontroller {
			typedef enum {
				OP_POWERON,
				OP_SETVOLTAGESONGIB,
				OP_SETPULSEONGIB,
				OP_GOTOSETPOINT
			} ChaosCCALTControllerOpcode;
			typedef struct {
				uint32_t timeout;
				int32_t int32_t1;
				int32_t int32_t2;
				int32_t int32_t3;
				int32_t int32_t4;
				std::string string1;
				double double1;
			} ccaltcontroller_iparams_t;
			typedef struct {
				int64_t int64_t1;
				int32_t result;
			} ccaltcontroller_oparams_t;
			//wrapper interface
			class ChaosCCALTControllerInterface:public ::common::ccaltcontroller::AbstractCCALTController {
				protected:
				chaos_driver::DrvMsg message;
				public: 
				ChaosCCALTControllerInterface(chaos_driver::DriverAccessor*_accessor):accessor(_accessor){};
				chaos_driver::DriverAccessor* accessor;
				int PowerOn(int32_t on_state);
				int SetVoltagesOnGib(std::string gibUID,int32_t channel,double voltage);
				int SetPulseOnGib(std::string gibUID,int32_t channel,int32_t amplitude,int32_t width,int32_t state);
				int GoToSetPoint();
			};
		}
	}//driver
}//chaos
namespace chaos_ccaltcontroller_dd = chaos::driver::ccaltcontroller;
#endif
