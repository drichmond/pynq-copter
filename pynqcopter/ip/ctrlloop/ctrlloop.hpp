//##############################################################################
// Copyright (c) 2018, The Regents of the University of California All
// rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
//     * Neither the name of The Regents of the University of California
//       nor the names of its contributors may be used to endorse or
//       promote products derived from this software without specific
//       prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL REGENTS OF THE
// UNIVERSITY OF CALIFORNIA BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//##############################################################################
#ifndef __CTRLLOOP_HPP
#define __CTRLLOOP_HPP

#include "ap_int.h"

#define IOMEM_SPACE_SIZE 4096
#define REG_SPACE_SIZE 128


/* delay_util_ms - Delay for a set number of milliseconds. The length of the
   delay is equal to the frequency of the overlay (F_OVERLAY_HZ) times the
   duration in milliseconds (MILLISECONDS), divided by the number of
   milliseconds in a second (MSEC_PER_SEC)
 */
template <unsigned long long MILLISECONDS, unsigned long long F_OVERLAY_HZ = 100000000ULL>
void delay_until_ms(){
#define MSEC_PER_SEC 1000ULL
#pragma HLS INLINE
#pragma HLS PROTOCOL floating
	volatile char dummy;
	ap_uint<64> ctr;
	ap_uint<64> cyc = (F_OVERLAY_HZ * MILLISECONDS / MSEC_PER_SEC);
	for (ctr = 0; ctr < cyc; ++ctr){
		dummy = dummy;
	}
	return;
#undef MSEC_PER_SEC
}

/* ctrlloop - A control loop function. Using the AP_AUTORESTART bit of the
 * AP_CTRL register this loop implements a real-time control loop.
 *
 * mem - ap_uint<32> - An array representing the peripheral memory space. This
 * will become an AXI-Master bus through the use of pragmas
 *
 * regs - ap_uint<32> - An array representing the internal memory space
 * (registers) of the core. The function can read and write to locations
 * here. This will be accessible from the ARM core using the CTRL AXI-Slave bus
 *
 * buttons - const ap_uint<4> - An ap_uint representing the current value on the
 * 4 buttons switches on the PYNQ board. The const specifier will make this
 * default to a 4-bit input port on the HLS core
 * 
 * leds - ap_uint<4>& - An ap_uint for assigning values to the LEDs. Using
 * pragmas this will become a 4-bit output port
 */
void ctrlloop(ap_uint<32> iomem [IOMEM_SPACE_SIZE],
	ap_uint<32> regs[REG_SPACE_SIZE],
	const ap_uint<4> buttons,
	const ap_uint<1> interrupt,
	ap_uint<4>& leds);
#endif