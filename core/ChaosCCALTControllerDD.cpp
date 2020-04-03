/*
ChaosCCALTControllerDD.cpp
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
#include "ChaosCCALTControllerDD.h"
#include <string>
#include <chaos/cu_toolkit/driver_manager/driver/AbstractDriverPlugin.h>
// including interface
#include "driver/CCALTController/core/ChaosCCALTControllerInterface.h"
#define ACLAPP	LAPP_ << "[ChaosCCALTControllerDD] "
#define ACDBG	LDBG_ << "[ChaosCCALTControllerDD] "
#define ACERR	LERR_ << "[ChaosCCALTControllerDD] "
using namespace chaos::driver::ccaltcontroller;
//default constructor definition
DEFAULT_CU_DRIVER_PLUGIN_CONSTRUCTOR_WITH_NS(chaos::driver::ccaltcontroller, ChaosCCALTControllerDD) {
	devicedriver = NULL;
}
ChaosCCALTControllerDD::~ChaosCCALTControllerDD() {
}
void ChaosCCALTControllerDD::driverDeinit() {
	 if(devicedriver) {
		delete devicedriver;
	}
	devicedriver = NULL;
}
cu_driver::MsgManagmentResultType::MsgManagmentResult ChaosCCALTControllerDD::execOpcode(cu_driver::DrvMsgPtr cmd){
	 cu_driver::MsgManagmentResultType::MsgManagmentResult result = cu_driver::MsgManagmentResultType::MMR_EXECUTED;
	ccaltcontroller_iparams_t *in = (ccaltcontroller_iparams_t *)cmd->inputData;
	ccaltcontroller_oparams_t *out = (ccaltcontroller_oparams_t *)cmd->resultData;
	switch(cmd->opcode){
		case OP_POWERON: {
			out->result=devicedriver->PowerOn(in->int32_t1);
			ACDBG << "Sent to driver command PowerOn result is " << out->result;
			} break;
		case OP_SETVOLTAGESONGIB: {
			out->result=devicedriver->SetVoltagesOnGib(in->string1,in->int32_t1,in->double1);
			ACDBG << "Sent to driver command SetVoltagesOnGib result is " << out->result;
			} break;
		case OP_SETPULSEONGIB: {
			out->result=devicedriver->SetPulseOnGib(in->string1,in->int32_t1,in->int32_t1,in->int32_t1,in->int32_t1);
			ACDBG << "Sent to driver command SetPulseOnGib result is " << out->result;
			} break;
		case OP_GOTOSETPOINT: {
			out->result=devicedriver->GoToSetPoint();
			ACDBG << "Sent to driver command GoToSetPoint result is " << out->result;
			} break;
	}
	return result;
}
