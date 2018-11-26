/*
AbstractCCALTController.h
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
#include <inttypes.h>
#include <string>

#ifndef __common_AbstractCCALTController_h__
#define __common_AbstractCCALTController_h__
namespace common {
	namespace ccaltcontroller {

#define UPMASK(bitmask,powerofTwo) \
bitmask |= powerofTwo 

#define DOWNMASK(bitmask,powerofTwo) \
bitmask=bitmask & (~powerofTwo) 

#define CHECKMASK(bitmask,powerofTwo) \
((bitmask & powerofTwo) != 0)

#define RAISEBIT(bitmask,bitToRaise) \
bitmask |= (1 << bitToRaise) 


#define DOWNBIT(bitmask,bitToLower) \
{ int mask = ~(1 << bitToLower); \
  bitmask=bitmask & mask; }

#define CHECKBIT(bitmask,bitToCheck)\
((bitmask & (1 << bitToCheck)) != 0) 


		typedef enum {
			CCALTCONTROLLER_STATE_ON = 0x1,
			CCALTCONTROLLER_STATE_PARTIALLY_ON = 0x2,
			CCALTCONTROLLER_STATE_PULSING = 0x4,
			CCALTCONTROLLER_STATE_UNKNOWN = 0x8
		} CCALTStates;


		typedef enum {
			CCALTCONTROLLER_GIB1_unreachable = 0x1,
			CCALTCONTROLLER_GIB2_unreachable = 0x2,
			CCALTCONTROLLER_GIB3_unreachable = 0x4,
			CCALTCONTROLLER_GIB4_unreachable = 0x8,
			CCALTCONTROLLER_GIB1_setpointnotreached = 0x10,
			CCALTCONTROLLER_GIB2_setpointnotreached = 0x20,
			CCALTCONTROLLER_GIB3_setpointnotreached = 0x40,
			CCALTCONTROLLER_GIB4_setpointnotreached = 0x80			
		} CCALTAlarms;
		class AbstractCCALTController {
		  public:
			AbstractCCALTController() {};
			virtual ~AbstractCCALTController() {};
			virtual int PowerOn(int32_t on_state)=0;
			virtual int SetVoltagesOnGib(std::string gibUID,int32_t channel,double voltage)=0;
			virtual int SetPulseOnGib(std::string gibUID,int32_t channel,int32_t amplitude,int32_t width,int32_t state)=0;
			virtual int GoToSetPoint()=0;
		};
	}
}//common
#endif
