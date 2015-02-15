#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>

namespace Nx {

class WavefrontObj
{
public:
	WavefrontObj(void);
	~WavefrontObj(void);
	unsigned int loadObj(const char *fname, bool textured); // load a wavefront obj returns number of triangles that were loaded.  Data is persists until the class is destructed.
	int mVertexCount;
	int mTriCount;
	int * mIndices;
	float * mVertices;
	float * mTexCoords;
};

static bool MediaFileExists( const char * fname )
{
	FILE *fp=fopen(fname,"rb");
	if(fp == NULL)
	{
		return false;
	}
	else
	{
		fclose(fp);
		return true;
	}
}

char * FindMediaFile(const char *fname,char *buff)
{
	char curPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, curPath);
	strcat(curPath, "\\");
	strcpy(buff, curPath);
	strcat(buff,fname);
	if(MediaFileExists(buff))
		return buff;

	char basePath[MAX_PATH];
	GetModuleFileName(NULL, basePath, MAX_PATH);
	char* pTmp = strrchr(basePath, '\\');
	if(pTmp != NULL){
		*pTmp = 0;
		SetCurrentDirectory(basePath);
		pTmp = strrchr(basePath, '\\');
		if(pTmp != NULL){
			*pTmp = 0;
		}
		pTmp = strrchr(basePath, '\\');
		if(pTmp != NULL){
			*(pTmp + 1) = 0;
		}
	}
	// printf("base path is: %s\n", basePath);
	strcpy(buff, basePath);
	strcat(buff,"Samples\\Media\\");
	strcat(buff,fname);
	if(MediaFileExists(buff))
		return buff;

	strcpy(buff, basePath);
	strcat(buff,"Samples\\compiler\\vc8win32\\");
	strcat(buff,fname);
	if(MediaFileExists(buff))
		return buff;

	strcpy(buff, basePath);
	strcat(buff,"Bin\\win32\\");
	strcat(buff,fname);
	if(MediaFileExists(buff))
		return buff;

	strcpy(buff,fname);
	if(MediaFileExists(buff))
		return buff;

	printf("Can't find media file: %s\n", fname);

	strcpy(buff,fname);
	return buff;
}

char* GetTempFilePath(char *buff)
{
	strcpy(buff,"");
	return buff;
}

typedef std::vector< int > IntVector;
typedef std::vector< float > FloatVector;

namespace WAVEFRONT
{

#if defined(__APPLE__) || defined(__CELLOS_LV2__) || defined(LINUX)
#define stricmp(a, b) strcasecmp((a), (b))
#endif

/*******************************************************************/
/******************** InParser.h  ********************************/
/*******************************************************************/
class InPlaceParserInterface
{
public:
	virtual int ParseLine(int lineno,int argc,const char **argv) =0;  // return TRUE to continue parsing, return FALSE to abort parsing process
};

enum SeparatorType
{
	ST_DATA,        // is data
	ST_HARD,        // is a hard separator
	ST_SOFT,        // is a soft separator
	ST_EOS          // is a comment symbol, and everything past this character should be ignored
};

class InPlaceParser
{
public:
	InPlaceParser(void)
	{
		Init();
	}

	InPlaceParser(char *data,int len)
	{
		Init();
		SetSourceData(data,len);
	}

	InPlaceParser(const char *fname)
	{
		Init();
		SetFile(fname);
	}

	~InPlaceParser(void);

	void Init(void)
	{
		mQuoteChar = 34;
		mData = 0;
		mLen  = 0;
		mMyAlloc = false;
		for (int i=0; i<256; i++)
		{
			mHard[i] = ST_DATA;
			mHardString[i*2] = i;
			mHardString[i*2+1] = 0;
		}
		mHard[0]  = ST_EOS;
		mHard[32] = ST_SOFT;
		mHard[9]  = ST_SOFT;
		mHard[13] = ST_SOFT;
		mHard[10] = ST_SOFT;
	}

	void SetFile(const char *fname); // use this file as source data to parse.

	void SetSourceData(char *data,int len)
	{
		mData = data;
		mLen  = len;
		mMyAlloc = false;
	};

	int  Parse(InPlaceParserInterface *callback); // returns true if entire file was parsed, false if it aborted for some reason

	int ProcessLine(int lineno,char *line,InPlaceParserInterface *callback);

	const char ** GetArglist(char *source,int &count); // convert source string into an arg list, this is a destructive parse.

	void SetHardSeparator(char c) // add a hard separator
	{
		mHard[c] = ST_HARD;
	}

	void SetHard(char c) // add a hard separator
	{
		mHard[c] = ST_HARD;
	}


	void SetCommentSymbol(char c) // comment character, treated as 'end of string'
	{
		mHard[c] = ST_EOS;
	}

	void ClearHardSeparator(char c)
	{
		mHard[c] = ST_DATA;
	}


	void DefaultSymbols(void); // set up default symbols for hard seperator and comment symbol of the '#' character.

	bool EOS(char c)
	{
		if ( mHard[c] == ST_EOS )
		{
			return true;
		}
		return false;
	}

	void SetQuoteChar(char c)
	{
		mQuoteChar = c;
	}

private:


	inline char * AddHard(int &argc,const char **argv,char *foo);
	inline bool   IsHard(char c);
	inline char * SkipSpaces(char *foo);
	inline bool   IsWhiteSpace(char c);
	inline bool   IsNonSeparator(char c); // non seperator,neither hard nor soft

	bool   mMyAlloc; // whether or not *I* allocated the buffer and am responsible for deleting it.
	char  *mData;  // ascii data to parse.
	int    mLen;   // length of data
	SeparatorType  mHard[256];
	char   mHardString[256*2];
	char           mQuoteChar;
};

/*******************************************************************/
/******************** InParser.cpp  ********************************/
/*******************************************************************/
void InPlaceParser::SetFile(const char *fname)
{
	if ( mMyAlloc )
	{
		free(mData);
	}
	mData = 0;
	mLen  = 0;
	mMyAlloc = false;

	char buff[512];

	FILE *fph = fopen(FindMediaFile(fname,buff),"rb");
	if ( fph )
	{
		fseek(fph,0L,SEEK_END);
		mLen = ftell(fph);
		fseek(fph,0L,SEEK_SET);
		if ( mLen )
		{
			mData = (char *) malloc(sizeof(char)*(mLen+1));
			int ok = fread(mData, mLen, 1, fph);
			if ( !ok )
			{
				free(mData);
				mData = 0;
			}
			else
			{
				mData[mLen] = 0; // zero byte terminate end of file marker.
				mMyAlloc = true;
			}
		}
		fclose(fph);
	}
}

InPlaceParser::~InPlaceParser(void)
{
	if ( mMyAlloc )
	{
		free(mData);
	}
}

#define MAXARGS 512

bool InPlaceParser::IsHard(char c)
{
	return mHard[c] == ST_HARD;
}

char * InPlaceParser::AddHard(int &argc,const char **argv,char *foo)
{
	while ( IsHard(*foo) )
	{
		const char *hard = &mHardString[*foo*2];
		if ( argc < MAXARGS )
		{
			argv[argc++] = hard;
		}
		foo++;
	}
	return foo;
}

bool   InPlaceParser::IsWhiteSpace(char c)
{
	return mHard[c] == ST_SOFT;
}

char * InPlaceParser::SkipSpaces(char *foo)
{
	while ( !EOS(*foo) && IsWhiteSpace(*foo) ) foo++;
	return foo;
}

bool InPlaceParser::IsNonSeparator(char c)
{
	if ( !IsHard(c) && !IsWhiteSpace(c) && c != 0 ) return true;
	return false;
}


int InPlaceParser::ProcessLine(int lineno,char *line,InPlaceParserInterface *callback)
{
	int ret = 0;

	const char *argv[MAXARGS];
	int argc = 0;

	char *foo = line;

	while ( !EOS(*foo) && argc < MAXARGS )
	{

		foo = SkipSpaces(foo); // skip any leading spaces

		if ( EOS(*foo) ) break;

		if ( *foo == mQuoteChar ) // if it is an open quote
		{
			foo++;
			if ( argc < MAXARGS )
			{
				argv[argc++] = foo;
			}
			while ( !EOS(*foo) && *foo != mQuoteChar ) foo++;
			if ( !EOS(*foo) )
			{
				*foo = 0; // replace close quote with zero byte EOS
				foo++;
			}
		}
		else
		{

			foo = AddHard(argc,argv,foo); // add any hard separators, skip any spaces

			if ( IsNonSeparator(*foo) )  // add non-hard argument.
			{
				bool quote  = false;
				if ( *foo == mQuoteChar )
				{
					foo++;
					quote = true;
				}

				if ( argc < MAXARGS )
				{
					argv[argc++] = foo;
				}

				if ( quote )
				{
					while (*foo && *foo != mQuoteChar ) foo++;
					if ( *foo ) *foo = 32;
				}

				// continue..until we hit an eos ..
				while ( !EOS(*foo) ) // until we hit EOS
				{
					if ( IsWhiteSpace(*foo) ) // if we hit a space, stomp a zero byte, and exit
					{
						*foo = 0;
						foo++;
						break;
					}
					else if ( IsHard(*foo) ) // if we hit a hard separator, stomp a zero byte and store the hard separator argument
					{
						const char *hard = &mHardString[*foo*2];
						*foo = 0;
						if ( argc < MAXARGS )
						{
							argv[argc++] = hard;
						}
						foo++;
						break;
					}
					foo++;
				} // end of while loop...
			}
		}
	}

	if ( argc )
	{
		ret = callback->ParseLine(lineno, argc, argv );
	}

	return ret;
}

int  InPlaceParser::Parse(InPlaceParserInterface *callback) // returns true if entire file was parsed, false if it aborted for some reason
{
	assert( callback );
	if ( !mData ) return 0;

	int ret = 0;

	int lineno = 0;

	char *foo   = mData;
	char *begin = foo;


	while ( *foo )
	{
		if ( *foo == 10 || *foo == 13 )
		{
			lineno++;
			*foo = 0;

			if ( *begin ) // if there is any data to parse at all...
			{
				int v = ProcessLine(lineno,begin,callback);
				if ( v ) ret = v;
			}

			foo++;
			if ( *foo == 10 ) foo++; // skip line feed, if it is in the carraige-return line-feed format...
			begin = foo;
		}
		else
		{
			foo++;
		}
	}

	lineno++; // lasst line.

	int v = ProcessLine(lineno,begin,callback);
	if ( v ) ret = v;
	return ret;
}


void InPlaceParser::DefaultSymbols(void)
{
	SetHardSeparator(',');
	SetHardSeparator('(');
	SetHardSeparator(')');
	SetHardSeparator('=');
	SetHardSeparator('[');
	SetHardSeparator(']');
	SetHardSeparator('{');
	SetHardSeparator('}');
	SetCommentSymbol('#');
}


const char ** InPlaceParser::GetArglist(char *line,int &count) // convert source string into an arg list, this is a destructive parse.
{
	const char **ret = 0;

	static const char *argv[MAXARGS];
	int argc = 0;

	char *foo = line;

	while ( !EOS(*foo) && argc < MAXARGS )
	{

		foo = SkipSpaces(foo); // skip any leading spaces

		if ( EOS(*foo) ) break;

		if ( *foo == mQuoteChar ) // if it is an open quote
		{
			foo++;
			if ( argc < MAXARGS )
			{
				argv[argc++] = foo;
			}
			while ( !EOS(*foo) && *foo != mQuoteChar ) foo++;
			if ( !EOS(*foo) )
			{
				*foo = 0; // replace close quote with zero byte EOS
				foo++;
			}
		}
		else
		{

			foo = AddHard(argc,argv,foo); // add any hard separators, skip any spaces

			if ( IsNonSeparator(*foo) )  // add non-hard argument.
			{
				bool quote  = false;
				if ( *foo == mQuoteChar )
				{
					foo++;
					quote = true;
				}

				if ( argc < MAXARGS )
				{
					argv[argc++] = foo;
				}

				if ( quote )
				{
					while (*foo && *foo != mQuoteChar ) foo++;
					if ( *foo ) *foo = 32;
				}

				// continue..until we hit an eos ..
				while ( !EOS(*foo) ) // until we hit EOS
				{
					if ( IsWhiteSpace(*foo) ) // if we hit a space, stomp a zero byte, and exit
					{
						*foo = 0;
						foo++;
						break;
					}
					else if ( IsHard(*foo) ) // if we hit a hard separator, stomp a zero byte and store the hard separator argument
					{
						const char *hard = &mHardString[*foo*2];
						*foo = 0;
						if ( argc < MAXARGS )
						{
							argv[argc++] = hard;
						}
						foo++;
						break;
					}
					foo++;
				} // end of while loop...
			}
		}
	}

	count = argc;
	if ( argc )
	{
		ret = argv;
	}

	return ret;
}

/*******************************************************************/
/******************** Geometry.h  ********************************/
/*******************************************************************/

class GeometryVertex
{
public:
	float        mPos[3];
	float        mNormal[3];
	float        mTexel[2];
};


class GeometryInterface
{
public:

	virtual void NodeTriangle(const GeometryVertex *v1,const GeometryVertex *v2,const GeometryVertex *v3, bool textured)
	{
	}

};


/*******************************************************************/
/******************** Obj.h  ********************************/
/*******************************************************************/


class OBJ : public InPlaceParserInterface
{
public:
  int LoadMesh(const char *fname,GeometryInterface *callback, bool textured);
  int ParseLine(int lineno,int argc,const char **argv);  // return TRUE to continue parsing, return FALSE to abort parsing process
private:

  void GetVertex(GeometryVertex &v,const char *face) const;

  FloatVector     mVerts;
  FloatVector     mTexels;
  FloatVector     mNormals;

  bool            mTextured;

  GeometryInterface *mCallback;
};


/*******************************************************************/
/******************** Obj.cpp  ********************************/
/*******************************************************************/

int OBJ::LoadMesh(const char *fname,GeometryInterface *iface, bool textured)
{
  mTextured = textured;
  int ret = 0;

  mVerts.clear();
  mTexels.clear();
  mNormals.clear();

  mCallback = iface;

  InPlaceParser ipp(fname);

  ipp.Parse(this);

return ret;
}

static const char * GetArg(const char **argv,int i,int argc)
{
  const char * ret = 0;
  if ( i < argc ) ret = argv[i];
  return ret;
}

void OBJ::GetVertex(GeometryVertex &v,const char *face) const
{
  v.mPos[0] = 0;
  v.mPos[1] = 0;
  v.mPos[2] = 0;

  v.mTexel[0] = 0;
  v.mTexel[1] = 0;

  v.mNormal[0] = 0;
  v.mNormal[1] = 1;
  v.mNormal[2] = 0;

  int index = atoi( face )-1;

  const char *texel = strstr(face,"/");

  if ( texel )
  {
    int tindex = atoi( texel+1) - 1;

    if ( tindex >=0 && tindex < (int)(mTexels.size()/2) )
    {
    	const float *t = &mTexels[tindex*2];

      v.mTexel[0] = t[0];
      v.mTexel[1] = t[1];

    }

    const char *normal = strstr(texel+1,"/");
    if ( normal )
    {
      int nindex = atoi( normal+1 ) - 1;

      if (nindex >= 0 && nindex < (int)(mNormals.size()/3) )
      {
      	const float *n = &mNormals[nindex*3];

        v.mNormal[0] = n[0];
        v.mNormal[1] = n[1];
        v.mNormal[2] = n[2];
      }
    }
  }

  if ( index >= 0 && index < (int)(mVerts.size()/3) )
  {

    const float *p = &mVerts[index*3];

    v.mPos[0] = p[0];
    v.mPos[1] = p[1];
    v.mPos[2] = p[2];
  }

}

int OBJ::ParseLine(int lineno,int argc,const char **argv)  // return TRUE to continue parsing, return FALSE to abort parsing process
{
  int ret = 0;

  if ( argc >= 1 )
  {
    const char *foo = argv[0];
    if ( *foo != '#' )
    {
      if ( stricmp(argv[0],"v") == 0 && argc == 4 )
      {
        float vx = (float) atof( argv[1] );
        float vy = (float) atof( argv[2] );
        float vz = (float) atof( argv[3] );
        mVerts.push_back(vx);
        mVerts.push_back(vy);
        mVerts.push_back(vz);
      }
      else if ( stricmp(argv[0],"vt") == 0 && (argc == 3 || argc == 4))
      {
        // ignore 4rd component if present
        float tx = (float) atof( argv[1] );
        float ty = (float) atof( argv[2] );
        mTexels.push_back(tx);
        mTexels.push_back(ty);
      }
      else if ( stricmp(argv[0],"vn") == 0 && argc == 4 )
      {
        float normalx = (float) atof(argv[1]);
        float normaly = (float) atof(argv[2]);
        float normalz = (float) atof(argv[3]);
        mNormals.push_back(normalx);
        mNormals.push_back(normaly);
        mNormals.push_back(normalz);
      }
      else if ( stricmp(argv[0],"f") == 0 && argc >= 4 )
      {
        GeometryVertex v[32];

        int vcount = argc-1;

        for (int i=1; i<argc; i++)
        {
          GetVertex(v[i-1],argv[i] );
        }

        // need to generate a normal!
#if 0 // not currently implemented
        if ( mNormals.empty() )
        {
          Vector3d<float> p1( v[0].mPos );
          Vector3d<float> p2( v[1].mPos );
          Vector3d<float> p3( v[2].mPos );

          Vector3d<float> n;
          n.ComputeNormal(p3,p2,p1);

          for (int i=0; i<vcount; i++)
          {
            v[i].mNormal[0] = n.x;
            v[i].mNormal[1] = n.y;
            v[i].mNormal[2] = n.z;
          }

        }
#endif

		mCallback->NodeTriangle(&v[0],&v[1],&v[2], mTextured);

        if ( vcount >=3 ) // do the fan
        {
          for (int i=2; i<(vcount-1); i++)
          {
			mCallback->NodeTriangle(&v[0],&v[i],&v[i+1], mTextured);
          }
        }

      }
    }
  }

  return ret;
}




class BuildMesh : public GeometryInterface
{
public:

	int GetIndex(const float *p, const float *texCoord)
	{

		int vcount = mVertices.size()/3;

		if(vcount>0)
		{
			//New MS STL library checks indices in debug build, so zero causes an assert if it is empty.
			const float *v = &mVertices[0];
			const float *t = texCoord != NULL ? &mTexCoords[0] : NULL;

			for (int i=0; i<vcount; i++)
			{
				if ( v[0] == p[0] && v[1] == p[1] && v[2] == p[2] )
				{
					if (texCoord == NULL || (t[0] == texCoord[0] && t[1] == texCoord[1]))
					{
						return i;
					}
				}
				v+=3;
				if (t != NULL)
					t += 2;
			}
		}

		mVertices.push_back( p[0] );
		mVertices.push_back( p[1] );
		mVertices.push_back( p[2] );

		if (texCoord != NULL)
		{
			mTexCoords.push_back( texCoord[0] );
			mTexCoords.push_back( texCoord[1] );
		}

		return vcount;
	}

	virtual void NodeTriangle(const GeometryVertex *v1,const GeometryVertex *v2,const GeometryVertex *v3, bool textured)
	{
		mIndices.push_back( GetIndex(v1->mPos, textured ? v1->mTexel : NULL) );
		mIndices.push_back( GetIndex(v2->mPos, textured ? v2->mTexel : NULL) );
		mIndices.push_back( GetIndex(v3->mPos, textured ? v3->mTexel : NULL) );
	}

  const FloatVector& GetVertices(void) const { return mVertices; };
  const FloatVector& GetTexCoords(void) const { return mTexCoords; };
  const IntVector& GetIndices(void) const { return mIndices; };

private:
  FloatVector     mVertices;
  FloatVector     mTexCoords;
  IntVector       mIndices;
};

};

using namespace WAVEFRONT;

WavefrontObj::WavefrontObj(void)
{
	mVertexCount = 0;
	mTriCount    = 0;
	mIndices     = 0;
	mVertices    = NULL;
	mTexCoords   = NULL;
}

WavefrontObj::~WavefrontObj(void)
{
	delete mIndices;
	delete mVertices;
}

unsigned int WavefrontObj::loadObj(const char *fname, bool textured) // load a wavefront obj returns number of triangles that were loaded.  Data is persists until the class is destructed.
{

	unsigned int ret = 0;
	delete mVertices;
	mVertices = 0;
	delete mIndices;
	mIndices = 0;
	mVertexCount = 0;
	mTriCount = 0;

	BuildMesh bm;
	OBJ obj;
	obj.LoadMesh(fname,&bm, textured);

	const FloatVector &vlist = bm.GetVertices();
	const IntVector &indices = bm.GetIndices();
	if ( vlist.size() )
	{
		mVertexCount = vlist.size()/3;
		mVertices = new float[mVertexCount*3];
		memcpy( mVertices, &vlist[0], sizeof(float)*mVertexCount*3 );

		if (textured)
		{
			mTexCoords = new float[mVertexCount * 2];
			const FloatVector& tList = bm.GetTexCoords();
			memcpy( mTexCoords, &tList[0], sizeof(float) * mVertexCount * 2);
		}

		mTriCount = indices.size()/3;
		mIndices = new int[mTriCount*3*sizeof(int)];
		memcpy(mIndices, &indices[0], sizeof(int)*mTriCount*3);
		ret = mTriCount;
	}


	return ret;
}





#define TEAR_MEMORY_FACTOR 3

struct Nx3DObjectPhysicCloth::RenderBufferVertexElement
{
	NxVec3 position;
	NxVec3 normal;
	float texCoord[2];
};

Nx3DObjectPhysicCloth::Nx3DObjectPhysicCloth( SceneNode * Node  ) : NxEntity(),
mVertexRenderBuffer(NULL), mScene(NULL), mCloth(NULL), mClothMesh(NULL), mIndexRenderBuffer(NULL), mTempTexCoords(NULL), mNumTempTexCoords(0), mTexId(0), mTeared(false)
{

	mType = NxTypeCloth ;
	Nx3D_Scene * scener = NxScene3DManager::getSingleton(). GetNx3DSceneFromOgreSceneManager( Node->getCreator()  );
	//scener->Lock();
	mInitDone = false;
	MatName = "ClothMaterial" ;
	NxClothDesc clothDesc;
	clothDesc.globalPose.t = NxVec3(Node->getPosition().x, Node->getPosition().y  , Node->getPosition().z );
	clothDesc.globalPose.M.rotX(-NxHalfPiF32);



	//Thickness of the cloth.
	//The thickness is usually a fraction of the overall extent of the cloth and
	//should not be set to a value greater than that.	A good value is the maximal
	//distance between two adjacent cloth particles in their rest pose. Visual
	//artifacts or collision problems may appear if the thickness is too small.
	//<b>Default:</b> 0.01 <br>
	//<b>Range:</b> [0,inf)

	clothDesc.thickness = 0.01;
	clothDesc.density = 1.80f; // was 1.0f / heavier / lighter
	//
	// Set the bendint stiffness (0-1)
	//   NX_CLF_BENDING must be set.
	clothDesc.bendingStiffness = 0.5f;//1.0;
	clothDesc.dampingCoefficient = 0.5;
	//
	// Set the stretching stiffness (0-1)
	//   The difficulty to stretch the cloth.
	//   1 : Not allowed to stretch
	clothDesc.stretchingStiffness = 1;
	//
	//
	// Set the friction of the cloth
	// Defines how much energy is lost due to interior motion of the cloth.
	clothDesc.friction = 0.5;//   0.5; 
	clothDesc.pressure = 1.0;
	clothDesc.tearFactor = 1.5;
	clothDesc.attachmentTearFactor = 1.5;
	clothDesc.wakeUpCounter = 0.4 ;
	clothDesc.sleepLinearVelocity = -1 ;
    clothDesc.collisionResponseCoefficient = 0.2;
	clothDesc.attachmentResponseCoefficient = 0.2;


 
 
	//Number of solver iterations. 
	//Note: Small numbers make the simulation faster while the cloth gets less stiff.
	//<b>Default:</b> 5
	//<b>Range:</b> [1,inf)
 
	clothDesc.solverIterations = 15;//10;//5;
	//clothDesc.flags |= NX_CLF_STATIC;
	//clothDesc.flags |= NX_CLF_DISABLE_COLLISION;
	//clothDesc.flags |= NX_CLF_VISUALIZATION;

 
	//Enable/disable gravity. If off, the cloth is not subject to the gravitational force
	//of the rigid body scene.
 
	clothDesc.flags |= NX_CLF_GRAVITY;


	// Cloth bending is allowed.
	//clothDesc.flags |= NX_CLF_BENDING;
	//clothDesc.flags |= NX_CLF_BENDING_ORTHO;
	//clothDesc.flags |= NX_CLF_DAMPING;
	//clothDesc.flags |= NX_CLF_COMDAMPING;

	//Set 2 way collision (cloth <-> rigid)
	//Enable/disable two way collision of cloth with the rigid body scene.
	//In either case, cloth is influenced by colliding rigid bodies.
	//If NX_CLF_COLLISION_TWOWAY is not set, rigid bodies are not influenced by 
	//colliding pieces of cloth. Use NxClothDesc.collisionResponseCoefficient to
	//control the strength of the feedback force on rigid bodies.
	//When using two way interaction care should be taken when setting the density of the attached objects.
	//For example if an object with a very low or high density is attached to a cloth then the simulation 
	//may behave poorly. This is because impulses are only transfered between the cloth and rigid body solver
	//outside the solvers.
	//Two way interaction works best when NX_SF_SEQUENTIAL_PRIMARY is set in the primary scene. If not set,
	//collision and attachment artifacts may happen.

	clothDesc.flags |= NX_CLF_COLLISION_TWOWAY;

	//clothDesc.flags |= NX_CLF_DISABLE_COLLISION;

	


	/**
	\brief Uses the information provided by NxCloth.setConstrainPositions() and
	NxCloth.setConstrainCoefficients() to make the cloth less stretchy.

	If the flag is set, the solver pulls vertices 
	with a maxDistance greater zero towards vertices with maxDistance zero (attached). 
	This reduces the stretchiness even for low solver iteration counts. The process is
	non-physical and can yield small ghost forces. The NxCloth.hardStretchLimitationFactor
	defines by what factor the cloth is allowed to stretch.

	@see NxCloth.setConstrainPositions() NxCloth.setConstrainCoefficients() NxCloth.hardStretchLimitationFactor
	*/
	clothDesc.flags |= NX_CLF_HARD_STRETCH_LIMITATION;


 
	//brief Enable/disable self-collision handling within a single piece of cloth.
 	//clothDesc.flags |= NX_CLF_SELFCOLLISION;

	//Allows cloth to collide with soft bodies and with other cloth.
	//clothDesc.flags |= NX_CLF_INTER_COLLISION; // creashes
	



	// Set the hardware flag
	clothDesc.flags |= NX_CLF_HARDWARE;
	if (!NxScene3DPhysicsManager::getSingleton().HasPhysxHardwareAcceleration())
	{
		clothDesc.flags &= ~NX_FF_HARDWARE;
		LogMsg("Cloth Created in Software Mode.");
	}
	else
	{
		LogMsg("Cloth Created in Hardware Mode.");
	}






	static int index = 0;
 	mManualObj = Node->getCreator()->createManualObject("ClothObj_" + Ogre::StringConverter::toString( index ) );
	index++;
 
	SceneNode * mClothNode = Node->getCreator()->getRootSceneNode()->createChildSceneNode();
	mClothNode->attachObject(mManualObj);

	//mManualObj->setMaterialName( 0 , "Examples/LightRibbonTrail" );

	//NxVec3* offset = new NxVec3( Node->getPosition().x , Node->getPosition().y, Node->getPosition().z );

	float ClothSize = 5.0f; // 10

	Create( clothDesc, "../../NxMedia/Video/OGRE/media/Physics/mycloth.obj", ClothSize , &VecToPhysx( Node->getPosition())  ,"text" );
	//scener->Unlock();


	//mCloth->setConstrainPositions()

}

Nx3DObjectPhysicCloth::~Nx3DObjectPhysicCloth()
{
	/*
	if (mInitDone) 
	{
		mSceneNode->detachObject( mManualObj );
		mSceneNode->getCreator()->destroyManualObject( mManualObj ); 
		mSceneNode->getCreator()->destroySceneNode( mSceneNode );// crash here
	 
		if (mCloth) mScene->releaseCloth( *mCloth );
		if (mClothMesh) mScene->getPhysicsSDK().releaseClothMesh( *mClothMesh );
		releaseReceiveBuffers();
		// temp texcoords are only used temporary and should be deallocated way earlier
		assert( mTempTexCoords == NULL );

		// delete the rendering buffers
		free( mVertexRenderBuffer );
		free( mIndexRenderBuffer );
	}
	*/
}

bool Nx3DObjectPhysicCloth::Create( NxClothDesc &desc, char *objFileName, float scale, NxVec3* offset, char *texFilename   )
{
	NxClothMeshDesc meshDesc;
	bool res = generateObjMeshDesc( meshDesc, objFileName, scale, offset, texFilename != NULL );
	if( !res ){ LogMsg("Could not create Wavefront Physics Cloth Object !"  ); }
	Nx3D_Scene * scener = NxScene3DManager::getSingleton(). GetNx3DSceneFromOgreSceneManager( mNode->getCreator()  );
	init(  &scener->GetPhysicScene()->GetPhysicsScene3D() , desc, meshDesc);
	return true;
}

void Nx3DObjectPhysicCloth::Update( float time )
{
	render();
}

void Nx3DObjectPhysicCloth::SetMaterialName( const std::string & MaterialName )
{
	MatName = MaterialName ;
}

void Nx3DObjectPhysicCloth::init( NxScene * scene , NxClothDesc &desc, NxClothMeshDesc &meshDesc)
{
	mScene = scene;
	if(!mScene ){ LogMsg("Physics Cloth Object Doesnt have a scene"  ); return ;}

	// if we want tearing we must tell the cooker
	// this way it will generate some space for particles that will be generated during tearing
	
	if (desc.flags & NX_CLF_TEARABLE)
	{ 
		meshDesc.flags |= NX_CLOTH_MESH_TEARABLE; 
	}

 

	//desc.flags |= NX_CLF_VISUALIZATION;// Enable Debug rendering for this cloth

	if(!cookMesh(meshDesc))
	{
		LogMsg("Could not cook Cloth mesh");
	}

	releaseMeshDescBuffers(meshDesc);
	allocateReceiveBuffers(meshDesc.numVertices, meshDesc.numTriangles); 

	desc.clothMesh = mClothMesh;
	desc.meshData = *mReceiveBuffers;

	if(!desc.isValid())
	{
		LogMsg("Physics Cloth Object Description Not Valid" ); 
		return ;
	}

	/* A compartment is a portion of the scene that can be simulated on a different hardware device than other parts of the scene.
	NxCompartmentDesc compartmentDesc;
	compartmentDesc.type = NX_SCT_RIGIDBODY;
	compartmentDesc.deviceCode = NX_DC_PPU_0;
	compartmentDesc.gridHashCellSize = 1.0f;
	compartmentDesc.gridHashTablePower = 6;
	NxCompartment *compartmentAbove = scene->createCompartment(compartmentDesc);
	desc.compartment = compartmentAbove; 
	*/

	mCloth = scene->createCloth(desc);

	NxGroupsMask groupsMask;
	groupsMask.bits0 = 1; 
	groupsMask.bits1 = 1; 
	groupsMask.bits2 = 1; 
	groupsMask.bits3 = 1; 
	mCloth->setGroupsMask(groupsMask);

	mCloth->setGroup( GROUP_COLLIDABLE_PUSHABLE );

	if( !mCloth ){ LogMsg("NxGraphics: could not create Physics Cloth Object"  ); }
	mInitDone = true;

}

bool Nx3DObjectPhysicCloth::generateObjMeshDesc(NxClothMeshDesc &desc, char *filename, float scale, NxVec3* offset, bool textured)
{
	WavefrontObj wo;
	wo.loadObj(filename, textured);
	if (wo.mVertexCount == 0) return false;


	// to do here : generate an ogre3d mesh to support all clothes


	NxVec3 myOffset(0.f);
	if (offset != NULL){ myOffset = *offset; }

	desc.numVertices				= wo.mVertexCount;
	desc.numTriangles				= wo.mTriCount;
	desc.pointStrideBytes			= sizeof(NxVec3);
	desc.triangleStrideBytes		= 3*sizeof(NxU32);
	desc.vertexMassStrideBytes		= sizeof(NxReal);
	desc.vertexFlagStrideBytes		= sizeof(NxU32);
	desc.points						= (NxVec3*)malloc(sizeof(NxVec3)*desc.numVertices);
	desc.triangles					= (NxU32*)malloc(sizeof(NxU32)*desc.numTriangles*3);
	desc.vertexMasses				= 0;
	desc.vertexFlags				= 0;
	desc.flags						= NX_CLOTH_MESH_WELD_VERTICES;
	desc.weldingDistance			= 0.0001f;

	mMaxVertices = TEAR_MEMORY_FACTOR * wo.mVertexCount;
	mMaxIndices  = 3 * wo.mTriCount;

	mTriCount = wo.mTriCount;

	// copy positions and indices
	NxVec3 *vSrc = (NxVec3*)wo.mVertices;
	NxVec3 *vDest = (NxVec3*)desc.points;
	for (int i = 0; i < wo.mVertexCount; i++, vDest++, vSrc++)
	{
		*vDest = (*vSrc)*scale + myOffset;
	}

	memcpy((NxU32*)desc.triangles, wo.mIndices, sizeof(NxU32)*desc.numTriangles*3);

	if (textured)
	{
		mNumTempTexCoords = desc.numVertices;
		mTempTexCoords = (float *)malloc(sizeof(float) * mNumTempTexCoords * 2);
		memcpy((NxU32*)mTempTexCoords, wo.mTexCoords, sizeof(float)*mNumTempTexCoords*2);

		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
		for (NxU32 i = 0; i < desc.numTriangles; i++)
		{
			assert((desc.flags & NX_CF_16_BIT_INDICES) == 0);
			NxU32* tri = (NxU32*)(((char*)desc.triangles) + (desc.triangleStrideBytes * i));
			mIndexRenderBuffer[3*i+0] = tri[0];
			mIndexRenderBuffer[3*i+1] = tri[1];
			mIndexRenderBuffer[3*i+2] = tri[2];
		}
	}
	else
	{
		mNumTempTexCoords = 0;
		mTempTexCoords = NULL;
	}

	return true;
}

void Nx3DObjectPhysicCloth::generateTearLines(NxClothMeshDesc& desc, unsigned int w, unsigned int h)
{
	// allocate flag buffer
	if(desc.vertexFlags == 0)
		desc.vertexFlags = malloc(sizeof(NxU32)*desc.numVertices);

	// create tear lines
	NxU32* flags = (NxU32*)desc.vertexFlags;
	NxU32 y;
	for(y = 0; y < h; y++)
	{
		NxU32 x;
		for(x = 0; x < w; x++)
		{
			if(((x + y) % 16 == 0) || ((x - y + 16) % 16 == 0))
				flags[y * w + x] = NX_CLOTH_VERTEX_TEARABLE;
			else
				flags[y * w + x] = 0;
		}
	}
}

void Nx3DObjectPhysicCloth::releaseMeshDescBuffers(const NxClothMeshDesc& desc)
{
	NxVec3* p = (NxVec3*)desc.points;
	NxU32* t = (NxU32*)desc.triangles;
	NxReal* m = (NxReal*)desc.vertexMasses;
	NxU32* f = (NxU32*)desc.vertexFlags;
	free(p);
	free(t);
	free(m);
	free(f);
}

bool Nx3DObjectPhysicCloth::cookMesh(NxClothMeshDesc& desc)
{
	// Store correct number to detect tearing mesh in time
	mLastNumVertices = desc.numVertices;

	// we cook the mesh on the fly through a memory stream
	// we could also use a file stream and pre-cook the mesh
	if (!NxInitCooking(NULL, NULL)) 
	{
		LogMsg("NxGraphics: failed to initialize cooker for Physics Cloth Object"  );
		return false;
	}

	MemoryWriteBuffer wb;
	assert(desc.isValid());
	bool success = NxCookClothMesh( desc, wb );

	if (!success) 
		return false;

	MemoryReadBuffer rb(wb.data);
	mClothMesh = mScene->getPhysicsSDK().createClothMesh(rb);
	return true;
}

void Nx3DObjectPhysicCloth::allocateReceiveBuffers( int numVertices, int numTriangles )
{
	// here we setup the buffers through which the SDK returns the dynamic cloth data
	// we reserve more memory for vertices than the initial mesh takes
	// because tearing creates new vertices
	// the SDK only tears cloth as long as there is room in these buffers

	//NxU32 maxVertices = TEAR_MEMORY_FACTOR * numVertices;
	//NxU32 maxIndices = 3*numTriangles;

	if (mVertexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Vertices if it hasn't been done before
		mVertexRenderBuffer = (RenderBufferVertexElement*)malloc(sizeof(RenderBufferVertexElement) * mMaxVertices);
		memset(mVertexRenderBuffer, 0, sizeof(RenderBufferVertexElement) * mMaxVertices);
	}

	if (mIndexRenderBuffer == NULL)
	{
		// Allocate Render Buffer for Indices if it hasn't been done before
		mIndexRenderBuffer = (NxU32*)malloc(sizeof(NxU32) * mMaxIndices);
		memset(mIndexRenderBuffer, 0, sizeof(NxU32) * mMaxIndices);
	}

	mReceiveBuffers = new NxMeshData();

	mReceiveBuffers->verticesPosBegin         = &(mVertexRenderBuffer[0].position.x);
	mReceiveBuffers->verticesNormalBegin      = &(mVertexRenderBuffer[0].normal.x);
	mReceiveBuffers->verticesPosByteStride    = sizeof(RenderBufferVertexElement);
	mReceiveBuffers->verticesNormalByteStride = sizeof(RenderBufferVertexElement);
	mReceiveBuffers->maxVertices              = mMaxVertices;
	mReceiveBuffers->numVerticesPtr           = &mNumVertices;

	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices = 3*numTriangles;
	mReceiveBuffers->indicesBegin             = mIndexRenderBuffer;
	mReceiveBuffers->indicesByteStride        = sizeof(NxU32);
	mReceiveBuffers->maxIndices               = maxIndices;
	mReceiveBuffers->numIndicesPtr            = &mNumIndices;

	if (mNumTempTexCoords > 0)
	{
		// Copy Tex Coords from temp buffers to graphics buffer
		assert(mNumTempTexCoords == numVertices);

		for (NxU32 i = 0; i < mNumTempTexCoords; i++)
		{
			mVertexRenderBuffer[i].texCoord[0] = mTempTexCoords[2*i+0];
			mVertexRenderBuffer[i].texCoord[1] = mTempTexCoords[2*i+1];
		}

		// Get rid of temp buffer
		mNumTempTexCoords = 0;
		free (mTempTexCoords);
		mTempTexCoords = NULL;
	}

	// the parent index information would be needed if we used textured cloth
	mReceiveBuffers->parentIndicesBegin       = (NxU32*)malloc(sizeof(NxU32)*mMaxVertices);
	mReceiveBuffers->parentIndicesByteStride  = sizeof(NxU32);
	mReceiveBuffers->maxParentIndices         = mMaxVertices;
	mReceiveBuffers->numParentIndicesPtr      = &mNumParentIndices;

	mReceiveBuffers->dirtyBufferFlagsPtr = &mMeshDirtyFlags;

	// init the buffers in case we want to draw the mesh 
	// before the SDK as filled in the correct values
	mMeshDirtyFlags = 0;
	mNumVertices = 0;
	mNumIndices = 0;
	mNumParentIndices = 0;
}

void Nx3DObjectPhysicCloth::releaseReceiveBuffers()
{
	// Parent Indices is always allocated
	free (mReceiveBuffers->parentIndicesBegin);

	mReceiveBuffers->setToDefault();
}

void Nx3DObjectPhysicCloth::render()
{
	static NxU32 numVertices = mNumVertices;
	NxU32 numElements = mNumIndices;
	numVertices = mNumVertices;

	mManualObj->clear();//Before rendering,clear buffer.
	mManualObj->begin( MatName, Ogre::RenderOperation::OT_TRIANGLE_LIST );

	for( NxU32 i=0; i<numVertices; i++ )//First,Copy vertex buffer
	{
		Ogre::Vector3 pos = Ogre::Vector3(mVertexRenderBuffer[i].position.x, mVertexRenderBuffer[i].position.y, mVertexRenderBuffer[i].position.z);
		mManualObj->position(pos);
		Ogre::Vector3 normal = Ogre::Vector3(mVertexRenderBuffer[i].normal.x, mVertexRenderBuffer[i].normal.y, mVertexRenderBuffer[i].normal.z);
		mManualObj->normal(normal);
		mManualObj->textureCoord(mVertexRenderBuffer[i].texCoord[0],mVertexRenderBuffer[i].texCoord[1]);
	}

	for(NxU32 j=0;j<mTriCount;j++)	//Second copy Index buffer
	{
		mManualObj->triangle(mIndexRenderBuffer[j*3],mIndexRenderBuffer[j*3+1],mIndexRenderBuffer[j*3+2]);
	}

	mManualObj->end();
}

void Nx3DObjectPhysicCloth::DetachCloth()
{
	mCloth->detachFromShape( mCloth->getVertexAttachmentShape( 0 ) );
}
/*
NxCloth * Nx3DObjectPhysicCloth::GetNxCloth() 
{
	return mCloth; 
}
*/

}//namespace