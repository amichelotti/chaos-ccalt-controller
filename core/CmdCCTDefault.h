/*
CmdCCTDefault.h
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
#ifndef __CCALTController__CmdCCTDefault_h__
#define __CCALTController__CmdCCTDefault_h__
#include "AbstractCCALTControllerCommand.h"


#include <bitset>
namespace c_data = chaos::common::data;
namespace ccc_slow_command = chaos::cu::control_manager::slow_command;
namespace driver {
	namespace ccaltcontroller {
		DEFINE_BATCH_COMMAND_CLASS(CmdCCTDefault,AbstractCCALTControllerCommand) {
			//implemented handler
			uint8_t implementedHandler();
			//initial set handler
			void setHandler(c_data::CDataWrapper *data);
			//custom acquire handler
			void acquireHandler();
			//correlation and commit handler
			void ccHandler();
			//manage the timeout 
			bool timeoutHandler();

			bool UpdateVoltagesFromDataset(chaos::common::data::CDWShrdPtr fetched, double* destination);
			bool CheckGibsAlarms(chaos::common::data::CDWShrdPtr fetchedAlarm,uint64_t* alarmBitMask,uint8_t gibNum);
			void DecodeAlarmMaskAndRaiseAlarms();
			double* o_gib1_voltages;
			double* o_gib2_voltages;
			double* o_gib3_voltages;
			double* o_gib4_voltages;
			chaos::common::data::CDWShrdPtr  GIB1Dataset;
			chaos::common::data::CDWShrdPtr  GIB2Dataset;
			chaos::common::data::CDWShrdPtr  GIB3Dataset;
			chaos::common::data::CDWShrdPtr  GIB4Dataset;
		};
	}
}
#endif
