/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
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
-----------------------------------------------------------------------------
*/
#ifndef __NXPREREQUISITES_H__
#define __NXPREREQUISITES_H__

#include "NxPlatform.h"
#include "NxEnums.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <limits>
#include <iostream>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>

#ifdef _WIN32
#include <memory>
using std::tr1::shared_ptr;
#else
#include <tr1/memory>
using std::tr1::shared_ptr;
#endif

using namespace std;
 

namespace OgreMax
{
	class OgreMaxScene;
}

namespace Ogre
{
	namespace RTShader
	{
		class ShaderGenerator;
	}

	namespace Canvas
	{
		class Context;
		class Texture;
	}

	class Terrain;
    class TerrainGlobalOptions;  
    class TerrainGroup;
	class TextAreaOverlayElement;
	class ParticleSystem;
	class ParticleEmitter;

	class OverlaySystem;
	class OverlayElement;
	class Overlay;
	class Font;

	class RenderWindow;
	class Camera ;
	class SceneManager;
	class Viewport; 
	class Root;
	class RenderSystem;

	class Vector3;
	class Quaternion;

	class Entity;
	class Rectangle2D;
	class SceneNode;
	class Light;
	class Skeleton;
	class SkeletonInstance;
	class Bone;
	class ParticleAffector;
	class ManualObject;
	class RenderTexture;
	class AnimationState;
	class Ray;
	class Plane;
	class Image;


	//class HardwareVertexBufferSharedPtr;
	//! Layer Pixel Buffer.
	//class HardwarePixelBufferSharedPtr;

	class Compositor;
	class HighLevelGpuProgram;
	class TextureUnitState;
	class Pass;
	class Technique;
	//class MaterialPtr;
	class Material;
	class PixelBox;
	class HardwarePixelBuffer;
	class HardwareVertexBuffer;
	class Texture;

	class VertexDeclaration;	
	class PatchMesh ;


}

namespace Forests
{
	class PagedGeometry;
}

namespace OIS
{
	class KeyEvent;
	class MouseEvent;
	class JoyStickEvent;
	/*class MouseButtonID;*/
	class MouseState;
 
}
 
namespace NxDevice_Namespace
{
	class NxDeviceManager;
	class NxDevice_Hardware;
}

namespace NxSound_Namespace
{
	class NxSoundManager; 
	class NxSoundMediaFile;
	class NxSoundDeviceOutput;
	class NxSoundMediaFileWav;
	class NxSoundEffect3D;
	class NxSoundMediaFileBuffer;
}

namespace NxVideo_Namespace
{
	struct NxVideoPixelFormatInfo;
	class NxVideo_URL;
	class NxVideo_Avi_Player;
	class NxVideo_Avi_Recorder;
	class NxVideoStream;
	class NxVideo_Flash_Reader;
	class NxVideo_FLV_Reader;
	class NxVideo_Flash_Creator;
	class NxVideo_Grabber;
	class NxVideoBuffer;
	class NxVideo_Player;
	class NxVideoBuffer;
 
}

namespace NxMocap_Namespace
{
	class NxMocapPlayer;
	struct NxMocapJoint;
}

using namespace NxDevice_Namespace;
using namespace NxVideo_Namespace;
using namespace NxSound_Namespace;
using namespace NxMocap_Namespace;




namespace Nx {
#if NXGRAPHICS_DOUBLE_PRECISION == 1
	typedef double Real;
#else
	typedef float Real;
#endif
	class Radian;
	class Degree;
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Quaternion;
}

namespace Nx {

#define NXGRAPHICS_VERSION_MAJOR 1
#define NXGRAPHICS_VERSION_MINOR 7
#define NXGRAPHICS_VERSION_PATCH 1
#define NXGRAPHICS_VERSION_SUFFIX ""
#define NXGRAPHICS_VERSION_NAME "Beta"

#define NXGRAPHICS_VERSION ((NXGRAPHICS_VERSION_MAJOR << 16) | (NXGRAPHICS_VERSION_MINOR << 8) | NXGRAPHICS_VERSION_PATCH)


class NxGraphics;

class Nx3D_Scene;
class Nx3DScenePtr;
class Nx3DSceneDesc;
class Nx3DSceneOgreAr ;
class NxEngine;
class NxPostEffectManager;

class NxContentManager;

class NxFrameListener;

class NxOverlayManager;
class NxOverlay;
class NxOverlayContainer;
class NxOverlayElement;
class NxOverlayText;
class NxOverlayElement;

class NxPass;
class NxTechnique;
class NxTextureUnit;
class MaterialNx;
class NxVertexShader;
class NxPixelShader;
class NxGeometryShader;

class Mixing_layers;
class NxTexture;
class NxTextureVideo;
class NxTexture3D;
class NxTextureImage;
class NxTextureCamera;
class NxTextureWeb;

class NxFontManager;
class NxFont;

class ProgressBar;

class NxStateManager;
class NxState;

class NxImage ;
class NxImagePtr;
class NxImageCanvas;
class NxPluginDescription ;
class NxPluginParam ;
class NxPlugin ;
class NxFrameBuffer;
class MixerManager;
class NxInputManager;
class NxInputListener;
class NxInput;
class NxMonitor;
class NxScreen;
class NxScreenManager;
class NxScriptManager;
class NxShader;
class NxLog;
class NxLayer;
class NxLine;
class NxBillboardText;
class NxParticles;
class NxTriangles;
class NxDelaunay;
class Nx3DSceneObject;
class NxViewport;
class NxSceneManager;
class NxRenderWindow;
class NxRenderWindowEventListener;
class NxRenderWindowDesc;



class Nx3DObjectPhysicCloth;
class Nx3DObjectPhysicClothe;
class Nx3DObjectPhysicFluid;
class Nx3DObjectPhysicLine;
class Nx3DObjectText;
class NxPhysicsFluid;

 
class NxParticleSystem;
class NxParticleEmitter;
class NxParticleSystemAffector;
//physics
class NxPhysicsBoxShape;
class NxPhysicsCapsuleShape;
class NxPhysicsPlaneShape;
class NxPhysicsMeshShape;
class NxPhysicsForceField;
class NxPhysicsRevoluteJoint;
class NxJointRevoluteDesc;
class NxPhysicsD6Joint;
class NxJointD6Desc;
class NxPhysicsController;
class NxPhysicsControllerCallback;


class NxCamera;


class NxScenePhysics;
class NxSceneTools;

class NxTerrainDesc;
class NxScene3DObjectTerrain;
class NxScene3DObjectPagedGrass;
class NxPagedGrassDesc;
class NxPagedGrassLayerDesc;

class NxScene3DObjectPagedEntities;
class NxPagedEntitiesDesc;

class NxScene3DObjectCharacter;
class NxCharacterController;
class NxPhysics3DDebugDraw;
class MyFluid;



class NxPhysics2DDebugDraw;
class NxScene3DPhysics3DNotify;
class Nx3D_SceneCallback;
class Nx3DSceneListener;

class NxNode;
class NxPhysicsActor;
class NxScene3DObjectLight;
class NxScene3DObjectSound;
class NxEntity;
class NxRectangle2D;

class NxVector3;
class NxPluginManager;

struct NxMonitorInfo;

// NxGui

class NxGuiManager;
class NxGuiColours;
class NxWidget;
class NxPanel;
class NxButton;
class NxButtonToggle;
class NxButtonRadio;
class NxSlider;
class NxLabel;

class NxGuiRectangle2D;
class NxRectangle;
class NxText;
class NxText2D;
class NxLine;
class NxLine2D;



template < typename T > 
struct vector 
{ 
	typedef typename std::vector<T> type;    
}; 

template < typename T > 
struct list 
{ 
	typedef typename std::list<T> type;    
}; 

template <typename K, typename V, typename P = std::less<K>  > 
struct map 
{ 
	typedef typename std::map<K, V, P> type; 
};

template <class Derived>
class CountedObj
{
    static int count;
protected:
    CountedObj(){
        ++count;
    }

    ~CountedObj(){
        --count;
    }
public:
    static int instance_count(){
        return count;
    }
};

template <class Derived>
int CountedObj<Derived>::count = 0;





template <typename T, typename IteratorType, typename ValType>
class IteratorWrapper
{
	private:
		IteratorWrapper();
	protected:
		IteratorType mBegin;
		IteratorType mCurrent;
		IteratorType mEnd;
	public:
		/// type you expect to get by funktions like peekNext(Value)
		typedef ValType ValueType;
		/// type you expect to get by funktions like peekNext(Value)Ptr
		typedef ValType* PointerType;

		 
		//\brief typedef to fulfill container interface
		//
		//Userfull if you want to use BOOST_FOREACH
		//\note there is no distinction between const_iterator and iterator.
		//\n keep this in mind if you want to derivate from this class. 
		 
		typedef IteratorType iterator;
		
		 
		//\brief typedef to fulfill container interface
		//
		//Userfull if you want to use BOOST_FOREACH
		//\note there is no distinction between const_iterator and iterator.
		//\n keep this in mind if you want to derivate from this class. 
		 
		typedef IteratorType const_iterator;
		

		IteratorWrapper ( IteratorType start, IteratorType last )
		: mBegin( start ), mCurrent ( start ), mEnd ( last )
		{
		}


		// Returns true if there are more items in the collection.  
		bool hasMoreElements ( ) const
		{
			return mCurrent != mEnd;
		}


		 // Moves the iterator on one element.  
		void moveNext (  )
		{
			++mCurrent;
		}

		// bookmark to the begin of the underlying collection  
		const IteratorType& begin() {return mBegin;}
		
		
		// full access to the current  iterator  
		IteratorType& current(){return mCurrent;}
		
		// bookmark to the end (one behind the last element) of the underlying collection  
		const IteratorType& end() {return mEnd;}
		
};

template <typename T, typename IteratorType>
class VectorIteratorWrapper : public IteratorWrapper<T, IteratorType, typename  T::value_type>
{
	public:
		typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::ValueType ValueType ; 
		typedef typename IteratorWrapper<T, IteratorType, typename  T::value_type>::PointerType PointerType ;
	
		VectorIteratorWrapper ( IteratorType start, IteratorType last )
		: IteratorWrapper<T, IteratorType, typename T::value_type>( start, last ){ }

		// Returns the next(=current) element in the collection, without advancing to the next.  
		ValueType peekNext() const{
			return *(this->mCurrent);
		}

		// Returns a pointer to the next(=current) element in the collection, without advancing to the next afterwards. 
		PointerType peekNextPtr()  const {
			return &(*(this->mCurrent) );
		}

		// Returns the next(=current) value element in the collection, and advances to the next.  
		ValueType getNext() {
			return *(this->mCurrent++);
		}	
};

template <typename T>
class NxVectorIterator : public VectorIteratorWrapper< T , typename T::iterator > {
	public:
		NxVectorIterator( typename T::iterator start, typename T::iterator last ) : VectorIteratorWrapper<T,  typename T::iterator>( start , last ){}
		explicit NxVectorIterator( T& c ) : VectorIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() ){}	
};


 template <typename T, typename IteratorType>
class MapIteratorWrapper  : public IteratorWrapper<T, IteratorType, typename T::mapped_type>
{
	public:
		/// redefined ValueType for a map/set
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::ValueType ValueType ; 
		/// redefined PointerType for a map/set
		typedef typename IteratorWrapper<T, IteratorType, typename  T::mapped_type>::PointerType PointerType ;	
		
		///unused, just to make it clear that map/set::value_type is not a ValueType
		typedef typename T::value_type PairType ; 
		/// type you expect to get by funktions like peekNextKey
		typedef typename T::key_type KeyType;
        
		MapIteratorWrapper ( IteratorType start, IteratorType last ) : IteratorWrapper<T, IteratorType, typename T::mapped_type>( start, last ) { }

        // Returns the next(=current) key element in the collection, without advancing to the next.  
        KeyType peekNextKey(void) const {
            return this->mCurrent->first;
        }

		// Returns the next(=current) value element in the collection, without advancing to the next.  
		ValueType peekNextValue (  ) const {
			return this->mCurrent->second;
		}

        // Returns a pointer to the next/current value element in the collection, without advancing to the next afterwards.  
		const PointerType peekNextValuePtr (  )  const {
			return &(this->mCurrent->second);
		}

        // Returns the next(=current) value element in the collection, and advances to the next. 
        ValueType getNext() {
            return ((this->mCurrent++)->second) ;
        }
};

template <typename T>
class NxMapIterator : public MapIteratorWrapper<T,  typename T::iterator>{
	public:	
		NxMapIterator( typename T::iterator start, typename T::iterator last ) : MapIteratorWrapper<T,  typename T::iterator>(start , last ){}
		explicit NxMapIterator( T& c ): MapIteratorWrapper<T,  typename T::iterator> ( c.begin(), c.end() ){}	
};


template <typename T>
class NxConstMapIterator : public MapIteratorWrapper<T,  typename T::const_iterator>{
	public:
		NxConstMapIterator( typename T::const_iterator start, typename T::const_iterator last ) : MapIteratorWrapper<T,  typename T::const_iterator> (start , last ){}
		explicit NxConstMapIterator ( const T& c ) : MapIteratorWrapper<T,  typename T::const_iterator> (c.begin() , c.end() ) { }
};

 
template <class T> class NxAlloc 
{
public:
	// type definitions
	typedef T value_type;
	typedef T * pointer;
	typedef const T * const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	// rebind allocator to type U
	template <class U>
	struct rebind 
	{
	   typedef NxAlloc<U> other;
	};

	// return address of values
	pointer address (reference value) const 
	{
	   return &value;
	}
	const_pointer address (const_reference value) const 
	{
	   return &value;
	}

	// constructors and destructor
	//  - nothing to do because the allocator has no state
	 
	NxAlloc() throw()
	{

	}

	NxAlloc( const NxAlloc& ) throw() 
	{

	}

	template <class U>
	NxAlloc( const NxAlloc<U>& ) throw() 
	{
	}

	~NxAlloc() throw() 
	{

	}

	size_type max_size () const throw()  // return maximum number of elements that can be allocated
	{
	   return std::numeric_limits<std::size_t>::max() / sizeof(T);
	}

	// allocate but don't initialize num elements of type T
	pointer allocate (size_type num, const void * = 0 ) 
	{
	   // print message and allocate memory with global new
	   std::cerr << "allocate " << num << " element(s)" << " of size " << sizeof(T) << std::endl;
	   pointer ret = (pointer)(::operator new(num*sizeof(T)));
	   std::cerr << " allocated at: " << (void*)ret << std::endl;
	   return ret;
	}

	// initialize elements of allocated storage p with value value
	void construct (pointer p, const T& value) 
	{
	   new((void*)p)T(value);// initialize memory with placement new
	}

	// destroy elements of initialized storage p
	void destroy (pointer p) 
	{ 
	   p->~T();// destroy objects by calling their destructor
	}

	// deallocate storage p of deleted elements
	void deallocate (pointer p, size_type num) 
	{
	   std::cerr << "deallocate " << num << " element(s)" << " of size " << sizeof(T) << " at: " << (void*)p << std::endl;// print message and deallocate memory with global delete
	   ::operator delete((void*)p);
	}
};

	template <class T1, class T2>
	bool operator== (const NxAlloc<T1>&, const NxAlloc<T2>&) throw() 
	{
		return true;
	}

	template <class T1, class T2>
	bool operator!= (const NxAlloc<T1>&, const NxAlloc<T2>&) throw() 
	{
		return false;
	}

	// typedefs
	typedef unsigned char NxUChar;
	typedef unsigned short NxUShort;
	typedef unsigned int NxUInt;
	typedef long long NxInt64;
	typedef unsigned long long NxUInt64;
	typedef std::wstring NxWString;
	
	typedef std::basic_string< char, std::char_traits<char>, NxAlloc<char> > NxString;
	typedef std::pair< NxString, NxString > NxStringPair;	
	typedef std::vector< NxString , NxAlloc<NxString> > NxStringList;

 



 
}// NxGraphics_Namespace


#endif



