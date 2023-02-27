/*
CmdCCTSetPulseOnGib.cpp
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
#include "CmdCCTSetPulseOnGib.h"
#include <sstream>
#define SCLAPP_ INFO_LOG(CmdCCTSetPulseOnGib) << "[" << getDeviceID() << "] "
#define SCLDBG_ DBG_LOG(CmdCCTSetPulseOnGib) << "[" << getDeviceID() << "] "
#define SCLERR_ ERR_LOG(CmdCCTSetPulseOnGib) << "[" << getDeviceID() << "] "
namespace own = driver::ccaltcontroller;
namespace c_data =  chaos::common::data;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
BATCH_COMMAND_OPEN_DESCRIPTION_ALIAS(driver::ccaltcontroller::,CmdCCTSetPulseOnGib,CMD_CCT_SETPULSEONGIB_ALIAS,
	"Set the pulse state of the gib",
	"dbdacd67-8897-43da-ad89-37501e4feb5d")
BATCH_COMMAND_ADD_STRING_PARAM(CMD_CCT_SETPULSEONGIB_GIBUID,"the UID name of the GIB . ALL for all gibs",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_SETPULSEONGIB_CHANNEL,"the target channel for pulse",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_SETPULSEONGIB_AMPLITUDE,"the pulse amplitude to set",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_SETPULSEONGIB_WIDTH,"the pulse width to set (ns)",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_ADD_INT32_PARAM(CMD_CCT_SETPULSEONGIB_STATE,"0=OFF, 1= ON",chaos::common::batch_command::BatchCommandAndParameterDescriptionkey::BC_PARAMETER_FLAG_MANDATORY)
BATCH_COMMAND_CLOSE_DESCRIPTION()


// return the implemented handler
uint8_t own::CmdCCTSetPulseOnGib::implementedHandler(){
	return      AbstractCCALTControllerCommand::implementedHandler()|chaos_batch::HandlerType::HT_Acquisition;
}
// empty set handler
void own::CmdCCTSetPulseOnGib::setHandler(c_data::CDataWrapper *data) {
	AbstractCCALTControllerCommand::setHandler(data);
	SCLAPP_ << "Set Handler SetPulseOnGib "; 
	setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	setStateVariableSeverity(StateVariableTypeAlarmCU,"MissingSetPointSnapshot",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	if(!data || !data->hasKey(CMD_CCT_SETPULSEONGIB_GIBUID))
	{
		SCLERR_ << "gibUID not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"gibUID not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	std::string tmp_gibUID=data->getStringValue(CMD_CCT_SETPULSEONGIB_GIBUID);
	if(!data || !data->hasKey(CMD_CCT_SETPULSEONGIB_CHANNEL))
	{
		SCLERR_ << "channel not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"channel not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	int32_t tmp_channel=data->getInt32Value(CMD_CCT_SETPULSEONGIB_CHANNEL);
	if ((tmp_channel<0) || (tmp_channel > 23) )
	{
		SCLERR_ << "channel out of bounds";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"bad channel number in SetPulseOnGib" );
		setStateVariableSeverity(StateVariableTypeAlarmCU,"bad_command_parameter",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	if(!data || !data->hasKey(CMD_CCT_SETPULSEONGIB_AMPLITUDE))
	{
		SCLERR_ << "amplitude not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"amplitude not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	int32_t tmp_amplitude=data->getInt32Value(CMD_CCT_SETPULSEONGIB_AMPLITUDE);
	if(!data || !data->hasKey(CMD_CCT_SETPULSEONGIB_WIDTH))
	{
		SCLERR_ << "width not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"width not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	int32_t tmp_width=data->getInt32Value(CMD_CCT_SETPULSEONGIB_WIDTH);
	if(!data || !data->hasKey(CMD_CCT_SETPULSEONGIB_STATE))
	{
		SCLERR_ << "state not present in CDataWrapper";
		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError,"state not present in CDataWrapper" );
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	
	int32_t tmp_state=data->getInt32Value(CMD_CCT_SETPULSEONGIB_STATE);
	int err=0;
	driver::misc::ChaosController::command_t cmd;
	std::string lowercaseName=boost::algorithm::to_lower_copy(tmp_gibUID);
	if ((lowercaseName.find("gib1")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB1->prepareCommand(gibcontrol::CMD_GIB_SETPULSE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_AMPLITUDE,tmp_amplitude);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_WIDTH,tmp_width);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_STATE,tmp_state);
		err+=GIB1->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib2")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB2->prepareCommand(gibcontrol::CMD_GIB_SETPULSE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_AMPLITUDE,tmp_amplitude);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_WIDTH,tmp_width);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_STATE,tmp_state);
		err+=GIB2->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib3")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB3->prepareCommand(gibcontrol::CMD_GIB_SETPULSE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_AMPLITUDE,tmp_amplitude);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_WIDTH,tmp_width);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_STATE,tmp_state);
		err+=GIB3->executeCmd(cmd,false);
	}
	if ((lowercaseName.find("gib3")!= std::string::npos) || (lowercaseName.find("all") != std::string::npos) )
	{
		cmd=GIB4->prepareCommand(gibcontrol::CMD_GIB_SETPULSE_ALIAS);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_CHANNEL,tmp_channel);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_AMPLITUDE,tmp_amplitude);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_WIDTH,tmp_width);
		cmd->addParameter(gibcontrol::CMD_GIB_SETPULSE_STATE,tmp_state);
		err+=GIB4->executeCmd(cmd,false);
	}

	if (err != 0)
	{

		metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," command SetPulseOnGib not acknowledged");
		setStateVariableSeverity(StateVariableTypeAlarmCU,"driver_command_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
		setWorkState(false);
		BC_FAULT_RUNNING_PROPERTY
		return;
	}
	setWorkState(true);
	BC_NORMAL_RUNNING_PROPERTY
}
// empty acquire handler
void own::CmdCCTSetPulseOnGib::acquireHandler() {
	SCLDBG_ << "Acquire Handler SetPulseOnGib "; 
}
// empty correlation handler
void own::CmdCCTSetPulseOnGib::ccHandler() {
	BC_END_RUNNING_PROPERTY;
}
// empty timeout handler
bool own::CmdCCTSetPulseOnGib::timeoutHandler() {
	SCLDBG_ << "Timeout Handler SetPulseOnGib "; 
	return false;
}
