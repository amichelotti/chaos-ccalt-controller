/*
CmdCCTDefault.cpp
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
#include "CmdCCTDefault.h"

#include <cmath>
#include  <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#define SCLAPP_ INFO_LOG(CmdCCTDefault) << "[" << getDeviceID() << "] "
#define SCLDBG_ DBG_LOG(CmdCCTDefault) << "[" << getDeviceID() << "] "
#define SCLERR_ ERR_LOG(CmdCCTDefault) << "[" << getDeviceID() << "] "
namespace own = driver::ccaltcontroller;
namespace c_data =  chaos::common::data;
namespace chaos_batch = chaos::common::batch_command;
using namespace chaos::cu::control_manager;
BATCH_COMMAND_OPEN_DESCRIPTION(driver::ccaltcontroller::,CmdCCTDefault,
	"Default command executed when no other commands in queue",
	"ddd645cf-0db6-4393-a3da-1a454e1aacdd")
BATCH_COMMAND_CLOSE_DESCRIPTION()


// return the implemented handler
uint8_t own::CmdCCTDefault::implementedHandler(){
	return      AbstractCCALTControllerCommand::implementedHandler()|chaos_batch::HandlerType::HT_Acquisition;
}
// set handler
void own::CmdCCTDefault::setHandler(c_data::CDataWrapper *data) {
	AbstractCCALTControllerCommand::setHandler(data);
	o_gib1_voltages=getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "GIB1_Voltages");
	o_gib2_voltages=getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "GIB2_Voltages");
	o_gib3_voltages=getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "GIB3_Voltages");
	o_gib4_voltages=getAttributeCache()->getRWPtr<double>(DOMAIN_OUTPUT, "GIB4_Voltages");
	i_setPointBehaviour=getAttributeCache()->getRWPtr<int32_t>(DOMAIN_INPUT,"registered_setpoint_behaviour");

	if ((i_setPointBehaviour!= NULL) && (*i_setPointBehaviour != 0))
	{
		char* gib1SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB1");
		char* gib2SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB2");
		char* gib3SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB3");
		char* gib4SetPoint = getAttributeCache()->getRWPtr<char>(DOMAIN_INPUT, "setpointNameGIB4");
		if ((gib1SetPoint!=NULL)&&(gib2SetPoint!=NULL)&&(gib3SetPoint!=NULL)&&(gib4SetPoint!=NULL) )
		{
			std::map<u_int64_t,std::string> retMap;
			GIB1->getSnapshotsofCU(GIB1Name,retMap);
			bool found=false;
			for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
			{
				if ((*Iter).second == gib1SetPoint)
				{
					found=true;
					
					SCLDBG_ << "retrieving snapshot "<< gib1SetPoint << " from " << GIB1Name;
					this->snap1=GIB1->getSnapshotDataset(gib1SetPoint,GIB1Name);
					
				}
			}
			if (!found)
			{
				std::string errStr=" No snapshot of name" + std::string(gib1SetPoint);
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelWarning,errStr);
				*i_setPointBehaviour=0;
			}
			GIB2->getSnapshotsofCU(GIB2Name,retMap);
			found=false;
			for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
			{
				if ((*Iter).second == gib2SetPoint)
				{
					found=true;
					this->snap2=GIB1->getSnapshotDataset(gib2SetPoint,GIB2Name);
				}
			}
			if (!found)
			{
				std::string errStr=" No snapshot of name" + std::string(gib2SetPoint);
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelWarning,errStr);
				*i_setPointBehaviour=0;
			}
			GIB3->getSnapshotsofCU(GIB3Name,retMap);
			found=false;
			for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
			{
				if ((*Iter).second == gib3SetPoint)
				{
					found=true;
					this->snap3=GIB1->getSnapshotDataset(gib3SetPoint,GIB3Name);
				}
			}
			if (!found)
			{
				std::string errStr=" No snapshot of name" + std::string(gib3SetPoint);
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelWarning,errStr);
				*i_setPointBehaviour=0;
			}
			GIB4->getSnapshotsofCU(GIB4Name,retMap);
			found=false;
			for (std::map<u_int64_t,std::string>::iterator Iter=retMap.begin(); Iter != retMap.end(); Iter++)
			{
				if ((*Iter).second == gib4SetPoint)
				{
					found=true;
					this->snap4=GIB1->getSnapshotDataset(gib4SetPoint,GIB4Name);
				}
			}
			if (!found)
			{
				std::string errStr=" No snapshot of name" + std::string(gib4SetPoint);
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelWarning,errStr);
				*i_setPointBehaviour=0;
			}
		}
		else
		{
			std::string errStr=" Not enough setpoint configured. Control over setpoint cannot be activated" + std::string(gib4SetPoint);
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelWarning,errStr);
			*i_setPointBehaviour=0;
		}
	}





	clearFeatures(chaos_batch::features::FeaturesFlagTypes::FF_SET_COMMAND_TIMEOUT);
	setBusyFlag(false);
	
	SCLAPP_ << "Set Handler Default " ;
	BC_NORMAL_RUNNING_PROPERTY
}
// empty acquire handler
void own::CmdCCTDefault::acquireHandler() {

	try
	{
		//dpck_mds_ats o dpck_hr_ats o dpck_ats
		//std::string inJson =GIB1->fetchJson(0);
		std::pair<std::vector<int32_t>,std::vector<std::string>> retCuState=this->checkHealthState();
		bool raisedDeadAlarm=false;
		bool raiseNotStartAlarm=false;
		for (int i=0; i < retCuState.first.size();++i)
		{
			if (retCuState.first[i] > DEFAULT_ELAPSED_TIME_HEALTH)
			{
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," GIB Control Unit Dead");
				setStateVariableSeverity(StateVariableTypeAlarmDEV,"GIBCU_DEAD",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
				raisedDeadAlarm=true;
			}
			if (retCuState.second[i] != "Start")
			{
				metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," GIB Control Unit Dead");
				setStateVariableSeverity(StateVariableTypeAlarmDEV,"GIB_CU_NOT_STARTED",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
				raiseNotStartAlarm=true;
			}
		}
		if (raisedDeadAlarm==false) {setStateVariableSeverity(StateVariableTypeAlarmDEV,"GIBCU_DEAD",chaos::common::alarm::MultiSeverityAlarmLevelClear);}
		if (raiseNotStartAlarm==false) {setStateVariableSeverity(StateVariableTypeAlarmDEV,"GIB_CU_NOT_STARTED",chaos::common::alarm::MultiSeverityAlarmLevelClear);}
		GIB1Dataset=GIB1->getLiveChannel(GIB1Name,0);
		GIB2Dataset=GIB2->getLiveChannel(GIB2Name,0);
		GIB3Dataset=GIB3->getLiveChannel(GIB3Name,0);
		GIB4Dataset=GIB4->getLiveChannel(GIB4Name,0);
		bool failed=false;
		if (GIB1Dataset == NULL)
		{
			SCLERR_ << "ALEDEBUGG GIB1Dataset null";
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve dataset GIB1");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			UpdateVoltagesFromDataset(GIB1Dataset,o_gib1_voltages);
		}
		if (GIB2Dataset == NULL)
		{
			SCLERR_ << "ALEDEBUGG GIB2Dataset null";
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve dataset GIB2");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			UpdateVoltagesFromDataset(GIB2Dataset,o_gib2_voltages);
		}
		if (GIB3Dataset == NULL)
		{
			SCLERR_ << "ALEDEBUGG GIB3Dataset null";
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve dataset GIB3");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			UpdateVoltagesFromDataset(GIB3Dataset,o_gib3_voltages);
		}
		if (GIB4Dataset == NULL)
		{
			SCLERR_ << "ALEDEBUGG GIB4Dataset null";
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve dataset GIB4");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			UpdateVoltagesFromDataset(GIB4Dataset,o_gib4_voltages);
		}
		if (!failed)
			this->CalculateState(GIB1Dataset,GIB2Dataset,GIB3Dataset,GIB4Dataset);
		
		failed=false;
		chaos::common::data::CDWShrdPtr  CUAlarmsDataset;
		CUAlarmsDataset=GIB1->getLiveChannel(GIB1Name,6);
		if (CUAlarmsDataset == NULL)
		{
			SCLERR_ << "Alarm dataset GIB1 not retrieved" ;
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB1");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			this->CheckGibsAlarms(CUAlarmsDataset,o_alarms,1);
		}
		CUAlarmsDataset=GIB2->getLiveChannel(GIB2Name,6);
		if (CUAlarmsDataset == NULL)
		{
			SCLERR_ << "Alarm dataset GIB2 not retrieved" ;
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB2");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			this->CheckGibsAlarms(CUAlarmsDataset,o_alarms,2);
		}
		CUAlarmsDataset=GIB3->getLiveChannel(GIB3Name,6);
		if (CUAlarmsDataset == NULL)
		{
			SCLERR_ << "Alarm dataset GIB3 not retrieved" ;
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB3");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			this->CheckGibsAlarms(CUAlarmsDataset,o_alarms,3);
		}
		CUAlarmsDataset=GIB4->getLiveChannel(GIB4Name,6);
		if (CUAlarmsDataset == NULL)
		{
			SCLERR_ << "Alarm dataset GIB4 not retrieved" ;
			metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB4");
			setStateVariableSeverity(StateVariableTypeAlarmCU,"gib_data_retrieving_error",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
			failed=true;
		}
		else
		{
			this->CheckGibsAlarms(CUAlarmsDataset,o_alarms,4);
		}
		if ((i_setPointBehaviour != NULL) && (*i_setPointBehaviour != 0) && (!failed))
		{
			//snapshots exists.
			SCLAPP_ << "ALEDEBUG: Controlling setpoint variations" ;
			bool goneOutOfSet=false;
			c_data::CDataWrapper *currentSnap=NULL;
			c_data::CDWShrdPtr  *currentGibDataset=NULL;
			std::string currentGIB;
			for (int i=1; i <=4;i++)
			{
				switch (i)
				{
					case 1:  currentSnap=&(this->snap1); currentGibDataset=&(this->GIB1Dataset);currentGIB=this->GIB1Name; break;
					case 2:  currentSnap=&(this->snap2); currentGibDataset=&(this->GIB2Dataset);currentGIB=this->GIB2Name; break;
					case 3:  currentSnap=&(this->snap3); currentGibDataset=&(this->GIB3Dataset);currentGIB=this->GIB3Name; break;
					case 4:  currentSnap=&(this->snap4); currentGibDataset=&(this->GIB4Dataset);currentGIB=this->GIB4Name; break;
					default:  break;
				}
				int numOfChannels=(*currentGibDataset)->getInt32Value("numberOfChannels");
				std::string  completeAttrName=currentGIB+"/voltage_channel_resolution";
				//::driver::misc::ChaosDatasetAttribute* voltResAttribute = new ::driver::misc::ChaosDatasetAttribute(completeAttrName);
				double voltageResolution=0.5;
				//voltResAttribute;
				//voltResAttribute->get<double>(voltageResolution);
				//delete (voltResAttribute);
				char num[16];
				for (int ch=0; ch< numOfChannels; ch++)
				{
					sprintf(num,"%d",ch);
					std::string key="CH" + std::string(num);
					//double valueFromSnap= currentSnap->getDoubleValue(key);
					double valueFromDataset= (*currentGibDataset)->getDoubleValue(key);
					SCLAPP_ << "key: " << key << " snap value " << this->snap1.getCompliantJSONString();
					//if ( std::fabs(valueFromDataset))

				}


			}




		}
		this->DecodeAlarmMaskAndRaiseAlarms();


	}
	catch (int  e)
	{
		SCLDBG_ << "ALEDEBUG EXCEPTION CATCHED";
	}
	getAttributeCache()->setOutputDomainAsChanged();
}
// empty correlation handler
void own::CmdCCTDefault::ccHandler() {
}
// empty timeout handler
bool own::CmdCCTDefault::timeoutHandler() {
	SCLDBG_ << "Timeout Handler Default "; 
	return false;
}



bool own::CmdCCTDefault::UpdateVoltagesFromDataset(chaos::common::data::CDWShrdPtr fetched, double* destination)
{
	int numOfChannels= fetched->getInt32Value("numberOfChannels");
	if (numOfChannels == 24)
	{
		for (int i=0; i < numOfChannels; i++)
		{
			char index[10];
			sprintf(index,"%d",i);
			std::string chan=(std::string)"CH"+index;
			double tmp=fetched->getDoubleValue(chan);
			destination[i]=tmp;
		}
	}
	else
	{
		SCLERR_ << "ERROR NOT FOUND NUMBER OF CHANNELS";
		return false;
	}
	return true;
}
bool own::CmdCCTDefault::CheckGibsAlarms(chaos::common::data::CDWShrdPtr fetchedAlarm,uint64_t* alarmBitMask,uint8_t gibNum) {

	ChaosStringSet keys;
	fetchedAlarm->getAllKey(keys);
	for (ChaosStringSetIterator i= keys.begin();i != keys.end();i++)
	{
		if (((*i).find("dpck") != std::string::npos)  || ((*i).find("ndk") != std::string::npos) )
			continue;

		if ((*i) == "gib_unreachable")
		{
			if (fetchedAlarm->getInt32Value(*i) > 0)
			{
				switch( gibNum)
				{
					case 1 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_unreachable); break;
					case 2 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_unreachable); break;
					case 3 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_unreachable); break;
					case 4 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_unreachable); break;
				}
			}
			else
			{
				switch( gibNum)
				{
					case 1 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_unreachable); break;
					case 2 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_unreachable); break;
					case 3 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_unreachable); break;
					case 4 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_unreachable); break;
				}
			}
		}
		if ((*i) == "setPoint_not_reached")
		{
			if (fetchedAlarm->getInt32Value(*i) > 0)
			{
				switch( gibNum)
				{
					case 1 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_setpointnotreached); break;
					case 2 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_setpointnotreached); break;
					case 3 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_setpointnotreached); break;
					case 4 : UPMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_setpointnotreached); break;
				}
			}
			else
			{
				switch( gibNum)
				{
					case 1 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_setpointnotreached); break;
					case 2 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_setpointnotreached); break;
					case 3 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_setpointnotreached); break;
					case 4 : DOWNMASK(*alarmBitMask,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_setpointnotreached); break;
				}
			}
		}

	}
	return true;
}
void own::CmdCCTDefault::DecodeAlarmMaskAndRaiseAlarms()
{
	if (CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_unreachable) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_unreachable) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_unreachable) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_unreachable) 
	   )
	{
			//metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB4");
			setStateVariableSeverity(StateVariableTypeAlarmDEV,"gib_unreachable",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
	}
	else
	{
		setStateVariableSeverity(StateVariableTypeAlarmDEV,"gib_unreachable",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	}
	if (CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB1_setpointnotreached) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB2_setpointnotreached) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB3_setpointnotreached) ||
		CHECKMASK(*o_alarms,::common::ccaltcontroller::CCALTCONTROLLER_GIB4_setpointnotreached) 
	   )
	{
			//metadataLogging(chaos::common::metadata_logging::StandardLoggingChannel::LogLevelError," cannot retrieve alarms from GIB4");
			setStateVariableSeverity(StateVariableTypeAlarmDEV,"out_of_set",chaos::common::alarm::MultiSeverityAlarmLevelHigh);
	}
	else
	{
		setStateVariableSeverity(StateVariableTypeAlarmDEV,"out_of_set",chaos::common::alarm::MultiSeverityAlarmLevelClear);
	}


}