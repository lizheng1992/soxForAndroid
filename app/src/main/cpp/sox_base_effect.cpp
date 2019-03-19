#include "./sox_base_effect.h"

SoxBaseEffect::SoxBaseEffect(double sampleRate, int channels) {
	this->inputSampleRate = sampleRate;
	this->inputChannels = channels;
}

SoxBaseEffect::SoxBaseEffect() {
}
SoxBaseEffect::~SoxBaseEffect() {
}

void SoxBaseEffect::initSignal() {
	signal.precision = 16;
	signal.channels = inputChannels;
	signal.rate = inputSampleRate;
	signal.mult = NULL;
}

void SoxBaseEffect::initEncoding() {
	encoding.encoding = (sox_encoding_t) 1;
	encoding.bits_per_sample = 16;
	encoding.reverse_bytes = (sox_option_t) 0;
	encoding.reverse_nibbles = (sox_option_t) 0;
	encoding.reverse_bits = (sox_option_t) 0;
	encoding.compression = INFINITY;
	encoding.opposite_endian = (sox_bool) 0;
}

void SoxBaseEffect::initGlobalParams(int bufferSize) {
	sox_globals.bufsiz = bufferSize;
}

int SoxBaseEffect::init(int bufferSize) {
	sox_init();
	initGlobalParams(bufferSize);
	initEncoding();
	initSignal();
	return 1;
}
