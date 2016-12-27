/****************************************************************************
Copyright (c) 2011-2013,WebJet Business Division,CYOU
 
http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __PROFILE_SYSTEM_H__
#define __PROFILE_SYSTEM_H__
#include "profilesystem/DeviceStats.h"
#include "profilesystem/IoStats.h"
#include "profilesystem/TickStats.h"

#ifdef __PROFILER__ 
namespace Profile
{
	Timing::MicrosecondTick GetusTicks();
}

#	define PROFILER_COPYDEVICESTATS( out )  out = Profile::DeviceStats::GetDeviceState()
#	define PROFILER_ADDDEVICESTATEVAL( Value, Num ) Profile::DeviceStats::GetDeviceState().Value += Num
#	define PROFILER_RESETDEVICESTATS()  Profile::DeviceStats::GetDeviceState().ResetStats()
#	define PROFILER_GETDEVICESTATEVAL( Value )  Profile::DeviceStats::GetDeviceState().Value

#	define PROFILER_COPYTICKSTATS( out )  out = Profile::TickStats::GetTickStats()
#	define PROFILER_ADDDTICKBEGIN( Value ) Timing::MicrosecondTick __tick_begin_##Value = (Timing::MicrosecondTick)Profile::GetusTicks();
#	define PROFILER_ADDDTICKEND( Value ) Profile::TickStats::GetTickStats().Value += (int)(Profile::GetusTicks() - __tick_begin_##Value);
#	define PROFILER_RESETICKSTATS() Profile::TickStats::GetTickStats().ResetStats()
#	define PROFILER_GETTICKSTATEVAL( Value ) Profile::TickStats::GetTickStats().Value

#ifdef __PROFILER_IO__ 
//log
#	define PROFILER_LOG(s,a)  n_printf(s,a)
#	define PROFILER_LOG_TIMESTAMP \
									{ \
										Timing::MicrosecondTick __tmpUSTick = (Timing::MicrosecondTick)Profile::GetusTicks(); \
										int __tmpMid	 =  (int)(__tmpUSTick /1000); \
										int __tmpMSTick  = __tmpMid % 1000; \
										__tmpMid		 = __tmpMid /1000; \
										int __tmpSTick   = __tmpMid % 60; \
										__tmpMid		 = __tmpMid /60; \
										int __tmpMinTick = __tmpMid % 3600; \
										n_printf("[%d: %d.%d.%d] ",__tmpMinTick,__tmpSTick,__tmpMSTick,__tmpUSTick%1000); \
									} 
#	define PROFILER_LOGTIMES(s,a) \
									PROFILER_LOG_TIMESTAMP \
									n_printf(s,a); 

//io
#	define PROFILER_IO_LOGTIMES(s,a) \
										PROFILER_LOG_TIMESTAMP \
										n_printf("[profile][io] "); \
										n_printf(s,a); \
										n_printf("\n");
#	define PROFILER_IO_LOGTIMES2(str,arg1,arg2) \
										PROFILER_LOG_TIMESTAMP \
										n_printf("[profile][io] "); \
										n_printf(str,arg1,arg2); \
										n_printf("\n");
#	define PROFILER_IO_ADDVAL( Value, Num ) Profile::IoStats::GetState().Value += Num
#	define PROFILER_IO_TICKBEGIN( Value ) Timing::MicrosecondTick __tick_begin_##Value = (Timing::MicrosecondTick)Profile::GetusTicks()
#	define PROFILER_IO_TICKEND( Value,delta ) Timing::MicrosecondTick __tick_delta_##delta= (int)(Profile::GetusTicks() - __tick_begin_##Value)
#	define PROFILER_IO_READ_RES(restype,delta) \
												Profile::IoStats::GetState().restype.mTotalNum++; \
												Profile::IoStats::GetState().restype.mTotalConsume += __tick_delta_##delta;

#endif //__PROFILER_IO__
											
										

#else

#	define PROFILER_COPYDEVICESTATS( out )
#	define PROFILER_ADDDEVICESTATEVAL( Value, Num )
#	define PROFILER_RESETDEVICESTATS()
#	define PROFILER_GETDEVICESTATEVAL( Value ) 0

#	define PROFILER_COPYTICKSTATS( out )
#	define PROFILER_ADDDTICKBEGIN( Value )
#	define PROFILER_ADDDTICKEND( Value )
#	define PROFILER_RESETICKSTATS()
#	define PROFILER_GETTICKSTATEVAL( Value ) 0


#endif


#endif //__PROFILE_SYSTEM_H__