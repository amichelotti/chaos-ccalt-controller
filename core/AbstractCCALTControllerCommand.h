/*
AbstractCCALTControllerCommand.h
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
#ifndef __AbstractCCALTControllerCommand__
#define __AbstractCCALTControllerCommand__
#include "CCALTControllerConstants.h"
#include <driver/CCALTController/core/ChaosCCALTControllerInterface.h>
#include <chaos/cu_toolkit/control_manager/slow_command/SlowCommand.h>
#include <driver/misc/core/ChaosController.h>
#include <driver/misc/core/ChaosDatasetAttribute.h>
#include <common/misc/GibControl/core/AbstractGibControl.h>
#include <driver/misc/models/GibControl/core/GibControlConstants.h>

namespace c_data = chaos::common::data;
namespace ccc_slow_command = chaos::cu::control_manager::slow_command;
namespace driver {
	namespace ccaltcontroller{
		class AbstractCCALTControllerCommand: public ccc_slow_command::SlowCommand {
		public:
			AbstractCCALTControllerCommand();
			~AbstractCCALTControllerCommand();
		protected: 
			//common members
			int32_t	*o_status_id;
			uint64_t	*o_alarms;
			char		*o_status_desc;
			//reference of the chaos abstraction of driver
			chaos::driver::ccaltcontroller::ChaosCCALTControllerInterface *ccaltcontroller_drv;
			//implemented handler
			uint8_t implementedHandler();
			void ccHandler();
			void setHandler(c_data::CDataWrapper *data);
			void setWorkState(bool working);
			::driver::misc::ChaosController *GIB1;
			::driver::misc::ChaosController *GIB2;
			::driver::misc::ChaosController *GIB3;
			::driver::misc::ChaosController *GIB4;
			std::string GIB1Name="CCALT/GIB/GIB1";
			std::string GIB2Name="CCALT/GIB/GIB2";
			std::string GIB3Name="CCALT/GIB/GIB3";
			std::string GIB4Name="CCALT/GIB/GIB4";
			char		*gib1SetPoint;
			char		*gib2SetPoint;
			char		*gib3SetPoint;
			char		*gib4SetPoint;
			bool CalculateState(chaos::common::data::CDWShrdPtr Gib1,chaos::common::data::CDWShrdPtr Gib2,
				chaos::common::data::CDWShrdPtr Gib3,chaos::common::data::CDWShrdPtr Gib4);
			
			std::pair<std::vector<int32_t>,std::vector<std::string>> checkHealthState() ;
			bool batchGoToSetPoint();
			const u_int32_t DEFAULT_ELAPSED_TIME_HEALTH = 10;

		};// AbstractCCALTControllerCommand
	}// ccaltcontroller
}
#endif
