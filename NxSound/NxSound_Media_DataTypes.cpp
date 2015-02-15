#include "NxSoundPreCompiled.h"

#include "STK/include/Stk.h"

using namespace stk;

namespace NxSound_Namespace {
 
NxSoundFrames :: NxSoundFrames( unsigned int nFrames, unsigned int nChannels ) : nFrames_( nFrames ), nChannels_( nChannels )
{
  size_ = nFrames_ * nChannels_;
  bufferSize_ = size_;

  if ( size_ > 0 ) {
    NxData_ = (StkFloat *) calloc( size_, sizeof( StkFloat ) );
	#if defined(_STK_DEBUG_)
		if ( NxData_ == NULL ) {
		  std::string error = "StkFrames: memory allocation error in constructor!";
		  Stk::handleError( error, StkError::MEMORY_ALLOCATION );
		}
	#endif
  }
  else NxData_ = 0;

  dataRate_ = Stk::sampleRate();
}

NxSoundFrames :: NxSoundFrames( const StkFloat& value, unsigned int nFrames, unsigned int nChannels ) : nFrames_( nFrames ), nChannels_( nChannels )
{
  size_ = nFrames_ * nChannels_;
  bufferSize_ = size_;
  if ( size_ > 0 ) {
    NxData_ = (StkFloat *) malloc( size_ * sizeof( StkFloat ) );
	#if defined(_STK_DEBUG_)
		if ( NxData_ == NULL ) {
		  std::string error = "StkFrames: memory allocation error in constructor!";
		  Stk::handleError( error, StkError::MEMORY_ALLOCATION );
		}
	#endif
    for ( long i=0; i<(long)size_; i++ ) NxData_[i] = value;
  }
  else NxData_ = 0;

  dataRate_ = Stk::sampleRate();
}

NxSoundFrames :: ~NxSoundFrames()
{
  if ( NxData_ ) free( NxData_ );
}

NxSoundFrames :: NxSoundFrames( const NxSoundFrames& f ) : size_(0), bufferSize_(0)
{
  resize( f.frames(), f.channels() );
  dataRate_ = Stk::sampleRate();
  for ( unsigned int i=0; i<size_; i++ ) NxData_[i] = f[i];
}

NxSoundFrames& NxSoundFrames :: operator= ( const NxSoundFrames& f )
{
  size_ = 0;
  bufferSize_ = 0;
  resize( f.frames(), f.channels() );
  dataRate_ = Stk::sampleRate();
  for ( unsigned int i=0; i<size_; i++ ) NxData_[i] = f[i];
  return *this;
}

void NxSoundFrames :: resize( size_t nFrames, unsigned int nChannels )
{
  nFrames_ = nFrames;
  nChannels_ = nChannels;

  size_ = nFrames_ * nChannels_;
  if ( size_ > bufferSize_ ) {
    if ( NxData_ ) free( NxData_ );
    NxData_ = (StkFloat *) malloc( size_ * sizeof( StkFloat ) );
	#if defined(_STK_DEBUG_)
		if ( data_ == NULL ) {
		  std::string error = "StkFrames::resize: memory allocation error!";
		  Stk::handleError( error, StkError::MEMORY_ALLOCATION );
		}
	#endif
    bufferSize_ = size_;
  }
}

void NxSoundFrames :: resize( size_t nFrames, unsigned int nChannels, StkFloat value )
{
  this->resize( nFrames, nChannels );

  for ( size_t i=0; i<size_; i++ ) NxData_[i] = value;
}

StkFloat NxSoundFrames :: interpolate( StkFloat frame, unsigned int channel ) const
{
	#if defined(_STK_DEBUG_)
	  if ( frame < 0.0 || frame > (StkFloat) ( nFrames_ - 1 ) || channel >= nChannels_ ) {
		std::ostringstream error;
		error << "StkFrames::interpolate: invalid frame (" << frame << ") or channel (" << channel << ") value!";
		Stk::handleError( error.str(), StkError::MEMORY_ACCESS );
	  }
	#endif

  size_t iIndex = ( size_t ) frame;                    // integer part of index
  StkFloat output, alpha = frame - (StkFloat) iIndex;  // fractional part of index

  iIndex = iIndex * nChannels_ + channel;
  output = NxData_[ iIndex ];
  if ( alpha > 0.0 )
    output += ( alpha * ( NxData_[ iIndex + nChannels_ ] - output ) );

  return output;
}

}//namespace