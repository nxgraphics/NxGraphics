#ifndef __NXSOUNDFFT_H__
#define __NXSOUNDFFT_H__

#include "NxSound_Prerequisites.h"

struct NxFFtStruct;

namespace NxSound_Namespace {

class NxSound_Export NxSoundFFT
{
public :
	//! NxSoundFFT constructor.
	NxSoundFFT();
	//! NxSoundFFT destructor.
	~NxSoundFFT();
	//! Initialize FFT
	void Initialize( int NumSamplesIn, int BinsOut );
	//! Get FFT total bins number.
	unsigned int GetNumBins() ;
	//! Perform FFT
	bool PerformFFT( float * PcmAudio );
	//! Get Spectrum
	void GetSpectrum( float * Spectrum );

//////////////



	void setData( float * data );
	void setDataSize( int dataSize );


	float * getAmplitude();
	int getBinSize() { return mBinSize; }
	float * getData();
	int getDataSize() { return mDataSize; }
	float * getImaginary();
	float * getPhase();
	float * getReal();
	void dispose();
	// Performs FFT
	void transform();
	// Sets amplitude and phase arrays
	void cartesianToPolar();

	NxFFtStruct * mFFt;

	// Arrays
	float * mAmplitude;
	float * mData;
	float * mImag;
	float * mInverseWindow;
	float * mPhase;
	float * mReal;
	float * mWindow;
	float * mWindowedData;

	// Dimensions
	int mBinSize;
	int mDataSize;
	float mWindowSum;

	// Flags
	bool mCartesianNormalized;
	bool mCartesianUpdated;
	bool mDataNormalized;
	bool mDataUpdated;
	bool mPolarNormalized;
	bool mPolarUpdated;
	// Filter frequencies
	float mFrequencyHigh;
	float mFrequencyLow;

	// Running flag
	bool mRunning;



};


};

#endif