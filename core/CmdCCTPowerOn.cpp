/*
CmdCCTPowerOn.cpp
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
#include "CmdCCTPowerOn.h"

#include <cmath>
#include  <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#define SCLAPP_ INFO_LOG(CmdCCTPowerOn) << "[" << getDeviceID() << "] "
#define SCLDBG_ DBG_LOG(CmdCCTPowerOn) << "[" << getDeviceID() << "] "
#define SCLERR_ ERR_LOG(CmdCCTPowerOn) << "[" << getDeviceID() << "] "
namespace own = driver::ccaltcontroller;
namespace c_data =  chaos::common::data;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
BATCH_COMMAND_OPEN_DESCRIPTION_ALIAS(driver::ccaltcontroller::,CmdCCTPowerOn,CMD_CCT_POWERON_ALIAS,
	"Switch CCALT HV ",
	"f88e9ab0-b575-4179-a1d7-dd67056fb1b5")
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_POWERON_ON_STATE,"1=ON , 0=OFF",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_CLOSE_DESCRIPTION()


// return the implemented handler
uint8_t own::CmdCCTPowerOn::implementedHandler(){
	return      AbstractCCALTControllerCommand::implementedHandler()|chaos_batch::HandlerType::HT_Acquisition;
}
// empty set handler
void own::CmdCCTPowerOn::setHandler(c_data::CDataWrapper *data) {
	AbstractCCALTControllerCommand::setHandler(data);
	SCLAPP_ << "Set Handler PowerOn "; 
	setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	if(!data || !data->hasKey(CMD_CCT_POWERON_ON_STATE))
	{
		SCLERR_ << "on_state not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"on_state not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	int32_t tmp_on_state=data->getInt32Value(CMD_CCT_POWERON_ON_STATE);
	if ((tmp_on_state <0) || (tmp_on_state > 1))
	{
		setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"on_state value incorrect" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
	}
	int err=0;
	for (int i=1; i <=4; i++)
	{
		driver::misc::ChaosController::command_t cmd=GIB1->prepareCommand(gibcontrol::CMD_GIB_POWERON_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_POWERON_ON_STATE,tmp_on_state);
		err+=GIB1->executeCmd(cmd,false);
		cmd=GIB2->prepareCommand(gibcontrol::CMD_GIB_POWERON_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_POWERON_ON_STATE,tmp_on_state);
		err+=GIB2->executeCmd(cmd,false);
		cmd=GIB3->prepareCommand(gibcontrol::CMD_GIB_POWERON_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_POWERON_ON_STATE,tmp_on_state);
		err+=GIB3->executeCmd(cmd,false);
		cmd=GIB4->prepareCommand(gibcontrol::CMD_GIB_POWERON_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_POWERON_ON_STATE,tmp_on_state);
		err+=GIB4->executeCmd(cmd,false);
		setWorkState(true);
	}
	if (err>0)
	{
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," command Power On not acknowledged");
		setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	BC_NORMAL_RUNNING_PROPERTY
}
// empty acquire handler
void own::CmdCCTPowerOn::acquireHandler() {
	SCLDBG_ << "Acquire Handler PowerOn "; 
}
// empty correlation handler
void own::CmdCCTPowerOn::ccHandler() {
	BC_END_RUNNING_PROPERTY;
}
// empty timeout handler
bool own::CmdCCTPowerOn::timeoutHandler() {
	SCLDBG_ << "Timeout Handler PowerOn "; 
	return false;
}
