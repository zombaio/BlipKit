/**
 * Copyright (c) 2012-2014 Simon Schoenenberger
 * http://blipkit.monoxid.net/
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _BK_TIME_H_
#define _BK_TIME_H_

#include "BKBase.h"

/**
 * Helper functions for the opaque type `BKTime`
 * Time is measured in fraction of samples
 */

#if BK_USE_64_BIT

#define BK_TIME_ZERO 0
#define BK_TIME_MAX INT64_MAX

static BKTime BKTimeMake (BKInt samples, BKFUInt20 frac)
{
	return (samples << BK_FINT20_SHIFT) + frac;
}

static BKInt BKTimeGetTime (BKTime a)
{
	return (BKInt) (a >> BK_FINT20_SHIFT);
}

static BKFUInt20 BKTimeGetFrac (BKTime a)
{
	return (a & BK_FINT20_SHIFT);
}

static BKFUInt20 BKTimeGetFUInt20 (BKTime a)
{
	return (BKFUInt20) a;
}

static BKTime BKTimeAdd (BKTime a, BKTime b)
{
	return a + b;
}

static BKTime BKTimeAddFUInt20 (BKTime a, BKFUInt20 b)
{
	return a + (BKTime) b;
}

static BKTime BKTimeSub (BKTime a, BKTime b)
{
	return a - b;
}

static BKTime BKTimeSubFUInt20 (BKTime a, BKFUInt20 b)
{
	return a - (BKTime) b;
}

static BKInt BKTimeIsEqual (BKTime a, BKTime b)
{
	return a == b;
}

static BKInt BKTimeIsGreater (BKTime a, BKTime b)
{
	return a > b;
}

static BKInt BKTimeIsGreaterFUInt20 (BKTime a, BKFUInt20 b)
{
	return a > (BKInt) b;
}

static BKInt BKTimeIsGreaterEqual (BKTime a, BKTime b)
{
	return a >= b;
}

static BKInt BKTimeIsLess (BKTime a, BKTime b)
{
	return a < b;
}

static BKInt BKTimeIsLessFUInt20 (BKTime a, BKFUInt20 b)
{
	return a < (BKTime) b;
}

static BKInt BKTimeIsLessEqual (BKTime a, BKTime b)
{
	return a <= b;
}

#else /* ! BK_USE_64_BIT */

#define BK_TIME_ZERO ((BKTime) {0, 0})
#define BK_TIME_MAX ((BKTime) {BK_INT_MAX, BK_FINT20_FRAC})

static BKTime BKTimeMake (BKInt samples, BKFUInt20 frac)
{
	BKTime time;

	time.time = samples + (frac >> BK_FINT20_SHIFT);
	time.frac = frac & BK_FINT20_FRAC;

	return time;
}

static BKInt BKTimeGetTime (BKTime a)
{
	return (BKInt) a.time;
}

static BKFUInt20 BKTimeGetFrac (BKTime a)
{
	return a.frac;
}

static BKFUInt20 BKTimeGetFUInt20 (BKTime a)
{
	return (a.time << BK_FINT20_SHIFT) + a.frac;
}

static BKTime BKTimeAdd (BKTime a, BKTime b)
{
	BKTime    time;
	BKFUInt20 frac;

	frac = a.frac + b.frac;

	time.frac = frac & BK_FINT20_FRAC;
	time.time = a.time + b.time + (frac >> BK_FINT20_SHIFT);

	return time;
}

static BKTime BKTimeAddFUInt20 (BKTime a, BKFUInt20 b)
{
	BKFUInt20 frac;

	frac = a.frac + b;

	a.frac = frac & BK_FINT20_FRAC;
	a.time += (frac >> BK_FINT20_SHIFT);

	return a;
}

static BKTime BKTimeSub (BKTime a, BKTime b)
{
	BKTime    time;
	BKFUInt20 frac;

	frac = a.frac - b.frac;

	time.frac = frac & BK_FINT20_FRAC;
	time.time = a.time - b.time + ((BKInt) frac >> BK_FINT20_SHIFT);

	return time;
}

static BKTime BKTimeSubFUInt20 (BKTime a, BKFUInt20 b)
{
	BKFUInt20 frac;

	frac = a.frac - b;

	a.frac = frac & BK_FINT20_FRAC;
	a.time = a.time + ((BKInt) frac >> BK_FINT20_SHIFT);

	return a;
}

static BKInt BKTimeIsEqual (BKTime a, BKTime b)
{
	return (a.time == b.time) && (a.frac == b.frac);
}

static BKInt BKTimeIsGreater (BKTime a, BKTime b)
{
	return (a.time > b.time) || ((a.time == b.time) && (a.frac > b.frac));
}

static BKInt BKTimeIsGreaterFUInt20 (BKTime a, BKFUInt20 b)
{
	BKTime time = {b >> BK_FINT20_SHIFT, b & BK_FINT20_FRAC};

	return BKTimeIsGreater (a, time);
}

static BKInt BKTimeIsGreaterEqual (BKTime a, BKTime b)
{
	return (a.time > b.time) || ((a.time == b.time) && (a.frac >= b.frac));
}

static BKInt BKTimeIsLess (BKTime a, BKTime b)
{
	return (a.time < b.time) || ((a.time == b.time) && (a.frac < b.frac));
}

static BKInt BKTimeIsLessFUInt20 (BKTime a, BKFUInt20 b)
{
	BKTime time = {b >> BK_FINT20_SHIFT, b & BK_FINT20_FRAC};

	return BKTimeIsLess (a, time);
}

static BKInt BKTimeIsLessEqual (BKTime a, BKTime b)
{
	return (a.time < b.time) || ((a.time == b.time) && (a.frac <= b.frac));
}

#endif /* BK_USE_64_BIT */

#endif /* ! _BK_TIME_H_ */