// CAUGens http://yota.tehis.net/

#include "SC_PlugIn.h"

#define MAXWIDTH 256

static InterfaceTable *ft;

struct CA0 : public Unit {
	unsigned char rule[8];
	unsigned char rule2[32];
	unsigned char cell[MAXWIDTH];
	float counter;
	float xn, xm1, xm2, xm3, frac, c0, c1, c2, c3;
};
struct CA1 : public CA0 {};
struct CA2 : public CA0 {};

static void CA1_next(CA1 *unit, int inNumSamples);
static void CA1_Ctor(CA1 *unit);
static void CA2_next(CA2 *unit, int inNumSamples);
static void CA2_Ctor(CA2 *unit);


float CA1_evolve(CA1 *unit, unsigned char smpwd);
float CA1_evolve(CA1 *unit, unsigned char smpwd)
{
	float result, sign, div;

	unsigned char tmp[MAXWIDTH];
	memcpy(tmp, unit->cell, sizeof(unit->cell));

	result = 0.f;
	sign = 1.f - 2.f * tmp[0];
	div = 1;

	for (int i=0; i<smpwd; i++) {
		unit->cell[i] = unit->rule[
			tmp[sc_wrap(i-1, 0, MAXWIDTH-1)] * 4 +
			tmp[i]				  * 2 +
			tmp[sc_wrap(i+1, 0, MAXWIDTH-1)]
		];
		div += i;
		result += tmp[i] * i;
	}
	return sign * result / div;
}

void CA1_next(CA1 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);

	float xn = unit->xn;
	float counter = unit->counter;

	// rexcitation
	if (IN0(4)) {
		if (IN0(3)) {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%128%2;
		}
	}

	float spc;
	if (smprt < SAMPLERATE)
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
	else spc = 1.f;

	LOOP(inNumSamples,
		if (counter >= spc){
			counter -= spc;
			xn = CA1_evolve(unit, smpwd);
		}
		counter++;
		ZXP(out) = xn;
	)
	unit->counter = counter;
	unit->xn = xn;
}

void CA1_Ctor(CA1 *unit)
{
	unit->xn = 0.f;
	for (int i=0; i<8; i++)
		unit->rule[i] = 0x01 & ((unsigned char)IN0(2) >> i);

	if (IN0(3)) {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = 0x01 & ((unsigned char)IN0(3) >> i%8);
	} else {
		for (int i=0; i<MAXWIDTH; i++)
			unit->cell[i] = rand()%2;
	}
	SETCALC(CA1_next);
	unit->counter = 0.f;
}


float CA2_evolve(CA2 *unit, unsigned char smpwd);
float CA2_evolve(CA2 *unit, unsigned char smpwd)
{
	float result, sign, div;

	unsigned char tmp[MAXWIDTH];
	memcpy(tmp, unit->cell, sizeof(unit->cell));

	result = 0.f;
	sign = 1.f - 2.f * tmp[0];
	div = 1;

	for (int i=0; i<smpwd; i++) {
		unit->cell[i] = unit->rule2[
			tmp[sc_wrap(i-2, 0, MAXWIDTH-1)]		* 16 +
			tmp[sc_wrap(i-1, 0, MAXWIDTH-1)]		*  8 +
			tmp[i]									*  4 +
			tmp[sc_wrap(i+1, 0, MAXWIDTH-1)]		*  2 +
			tmp[sc_wrap(i+2, 0, MAXWIDTH-1)]
		];
		result += tmp[i] * i;
		div += i;
	}
	return sign * result / div;
}

void CA2_next(CA2 *unit, int inNumSamples)
{
	float *out = ZOUT(0);
	float smprt = IN0(0);
	float smpwd = IN0(1);

	float xn = unit->xn;
	float counter = unit->counter;

	// rexcitation
	if (IN0(3)) {
		if (IN0(2)) {
			for(int i=0; i<smpwd; i++)
				unit->cell[i] = 0x01 & ((unsigned char)IN0(2) >> i%8);
		} else {
			for (int i=0; i<smpwd; i++)
				unit->cell[i] = rand()%2;
		}
	}

	float spc;
	if (smprt < SAMPLERATE)
		spc = SAMPLERATE / sc_max(smprt, 0.001f);
	else spc = 1.f;

	LOOP(inNumSamples,
		if(counter >= spc){
			counter -= spc;
			xn = CA2_evolve(unit, smpwd);
		}
		counter++;
		ZXP(out) = xn
	)
	unit->counter = counter;
	unit->xn = xn;
}

void CA2_Ctor(CA2 *unit)
{
	unit->xn = 0.f;
	for(int i=0; i<31; i++)
		unit->rule2[i] = (bool)IN0(i+4); // create rule

	for(int i=0; i<MAXWIDTH; i++)
		unit->cell[i] = rand()%2;

	SETCALC(CA2_next);
	unit->counter = 0.f;
}

PluginLoad(CAUGens) {
	ft = inTable;
	DefineSimpleUnit(CA1);
	DefineSimpleUnit(CA2);
}
