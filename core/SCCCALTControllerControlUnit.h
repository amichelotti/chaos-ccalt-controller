/*
SCCCALTControllerControlUnit.h
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
#ifndef __CCALTController__SCCCALTControllerControlUnit__
#define __CCALTController__SCCCALTControllerControlUnit__

#include <driver/misc/core/ChaosDatasetAttribute.h>
#include <chaos/cu_toolkit/control_manager/SCAbstractControlUnit.h>
//#include <chaos/cu_toolkit/driver_manager/driver/BasicIODriverInterface.h>
//#include <driver/misc/core/ChaosDatasetAttributeSinchronizer.h>
//#include <driver/misc/core/SCDataSync.h>
#include <driver/CCALTController/core/ChaosCCALTControllerInterface.h>
using namespace chaos;
namespace driver {
	namespace ccaltcontroller { //public ::driver::misc::SCDataSync
		class SCCCALTControllerControlUnit: public chaos::cu::control_manager::SCAbstractControlUnit {
		//class SCCCALTControllerControlUnit: public ::driver::misc::SCDataSync {
			PUBLISHABLE_CONTROL_UNIT_INTERFACE(SCCCALTControllerControlUnit)
			std::string device_hw;
			chaos::driver::ccaltcontroller::ChaosCCALTControllerInterface *ccaltcontroller_drv;
			bool waitOnCommandID(uint64_t command_id);
		protected:
			/* Define the Control Unit Dataset and Actions */
			void unitDefineActionAndDataset();
			void unitDefineCustomAttribute();
			/*(Optional) Initialize the Control Unit and all driver, with received param from MetadataServer*/
			void unitInit();
			/*(Optional) Execute the work, this is called with a determined delay, it must be as fast as possible*/
			void unitStart();
			/*(Optional) The Control Unit will be stopped*/
			void unitStop();
			/*(Optional) The Control Unit will be deinitialized and disposed*/
			void unitDeinit();
			//!restore method
			bool unitRestoreToSnapshot(chaos::cu::control_manager::AbstractSharedDomainCache * const snapshot_cache);
			// handler declaration
			bool setBehaviour(const std::string &name,int32_t value,uint32_t size);
			//end handler declaration
			//chaos::cu::driver_manager::driver::BasicIODriverInterface *driver;
			//::driver::misc::ChaosDatasetAttribute* Gib1;
		public:
			/*Construct a new CU with an identifier*/
			SCCCALTControllerControlUnit(const std::string& _control_unit_id,const std::string& _control_unit_param,const ControlUnitDriverList& _control_unit_drivers);
			/*Base Destructor*/
			~SCCCALTControllerControlUnit();
		
		};
	}
}
#endif
