/* Copyright (c) 2015 Wojciech Owczarek,
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   piservo.c
 * @date   Sat Jan 9 16:14:10 2015
 *
 * @brief  (new) PI servo implementation
 *
 */

#include "../ptpd.h"
#include "piservo.h"

#define THIS_COMPONENT "servo: "

static double feed (PIservo*, Integer32, double);
static void prime (PIservo *, double);
static void reset (PIservo*);


void
setupPIservo(PIservo *self) {
    memset(self, 0, sizeof(PIservo));
    self->feed = feed;
    self->prime = prime;
    self->reset = reset;
    self->delayFactor = 1.0;
}

static double
feed (PIservo* self, Integer32 input, double tau) {

    TimeInternal now, delta;

    Boolean runningMaxOutput;

    self->input = input;

    switch(self->tauMethod) {
	case DT_MEASURED:
	    getSystemClock()->getTimeMonotonic(getSystemClock(), &now);
	    if(isTimeZero(&self->lastUpdate)) {
		self->tau = 1.0;
	    } else {
		subTime(&delta, &now, &self->lastUpdate);
		self->tau = delta.seconds + delta.nanoseconds / 1E9;
	    }

            if(self->tau > (self->maxTau * tau)) {
                self->tau = (self->maxTau + 0.0) * tau;
	    }
	    break;

	case DT_CONSTANT:
	    self->tau = tau;
	break;

	default:
	    self->tau = 1.0;
    }

    if(self->tau <= ZEROF) {
	self->tau = 1.0;
    }

    self->integral = clampDouble(self->integral, self->maxOutput);
    self->output = clampDouble(self->output, self->maxOutput);

    if (self->kP < 0.000001)
	    self->kP = 0.000001;
    if (self->kI < 0.000001)
	    self->kI = 0.000001;

    self->integral += (self->tau / self->delayFactor) * ((input + 0.0 ) * self->kI);
    self->output = (self->kP * (input + 0.0) ) + self->integral;

    self->integral = clampDouble(self->integral, self->maxOutput);
    self->output = clampDouble(self->output, self->maxOutput);

    runningMaxOutput = (fabs(self->output) >= self->maxOutput);

    if(self->controller) {
	DBG("%s tau %.09f input %d fabs %f out %f, mo %f\n", self->controller->name, self->tau, input, fabs(self->output), self->output, self->maxOutput);
    }

    if(runningMaxOutput && !self->runningMaxOutput) {
	    WARNING(THIS_COMPONENT"Clock %s servo now running at maximum output\n", self->controller->name);
    }

    self->runningMaxOutput = runningMaxOutput;

    if(self->tauMethod == DT_MEASURED) {
	self->lastUpdate = now;
    }

    self->_updated = TRUE;
    self->_lastInput = self->input;
    self->_lastOutput = self->output;

    return self->output;

}

static void
prime (PIservo *self, double integral) {

    Boolean runningMaxOutput;

    integral = clampDouble(integral, self->maxOutput);

    self->integral = integral;
    self->output = integral;

    runningMaxOutput = (fabs(self->output) >= self->maxOutput);

    if(runningMaxOutput && !self->runningMaxOutput) {
	    DBG(THIS_COMPONENT"Clock %s servo now running at maximum output\n", self->controller->name);
    }

    self->runningMaxOutput = runningMaxOutput;

}

static void
reset (PIservo* self) {

    self->input = 0;
    self->output = 0;
    self->integral = 0;
    self->_updated = FALSE;
    self->_lastInput = 0;
    self->_lastOutput = 0;
    self->delayFactor = 1.0;

}




