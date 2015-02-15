#include "NxGraphicsPreCompiled.h"
#include <OgreZip.h>
 

// https://github.com/julapy/ofxFlash/blob/master/src/core/ofxFlashXFLBuilder.cpp

namespace Nx {

	struct DOMSettings
	{
		string mBackgroundColor;
		int mFrameRate;
		int mWidth;
		int mHeight;
		string mVersionInfo;
	};

	struct DOMBitmapItem
	{
		string name;
		string itemID;
		string sourceExternalFilepath;
		int sourceLastImported;
		string sourcePlatform;
		int externalFileSize;
		string originalCompressionType;
		int quality;
		string href;
		string bitmapDataHRef;
		int frameRight;
		int frameBottom;
	};

	enum LibraryItemType
	{
	  DOMBitmapItem,
	  DOMVideoItem,
	  DOMFolderItem,
	  DOMFontItem,
	  DOMSoundItem,
	  DOMCompiledClipItem,
	  DOMSymbolItem,
	  DOMComponentItem
	};


	struct DomLibrary
	{
		LibraryItemType mType;
		std::string mName;
		std::string mHref;
	};

	struct DomSymbols
	{
		LibraryItemType mType;
		std::string mLabelName;
		std::string mlibraryName;
	};

	struct DomTimelines
	{
		LibraryItemType mType;
		std::string mLayerName;
		std::string mLibraryItemName;
		unsigned long mFrameCount;
		std::string mFilename;
		float mMatrixTx;
		float mMatrixTy;
		float mPointx;
		float mPointy;


		Nx::Vector2 mPos;
		Nx::Vector2 mScale;
		float mRot;

	};

	std::string mFolderExtractName = "./uncompressed/";
	std::vector< DomLibrary > mLibrary;
	std::vector< DomSymbols > mSymbols;
	std::vector< DomTimelines > mTimelines;


	const std::string & GetLibraryNameFromLabel( const std::string & LabelName )
	{
		std::string libItemName;
		for( int i = 0 ; i < mSymbols.size(); i++ ){
			if( 0 == mSymbols[i].mLabelName.compare(LabelName) ){
				libItemName = mSymbols[i].mlibraryName; break;
			}
		
		}
		return libItemName;
	}

	LibraryItemType GetItemType( const std::string & Name )
	{
		if( 0 == Name.compare( "DOMVideoItem" ) ) {

			LogMsg("Looking For item type : " + Name  + " returning DOMVideoItem ");
			return DOMVideoItem;
		}
		else if( 0 == Name.compare( "DOMBitmapItem" ) ) {
				LogMsg("Looking For item type : " + Name  + " returning DOMBitmapItem ");
			return DOMBitmapItem;
		}

		else if( 0 == Name.compare( "DOMVideoInstance" ) ) {
			LogMsg("Looking For item type : " + Name  + " returning DOMVideoInstance ");
			return DOMVideoItem;
		}

		else if( 0 == Name.compare( "DOMBitmapInstance" ) ) {
			LogMsg("Looking For item type : " + Name  + " returning DOMBitmapInstance ");
			return DOMBitmapItem;
		}
	}

	void DoTimelines( NxGraphicsXml & DeSerializer, std::vector< DomTimelines > & mTimelines )
	{
		DeSerializer.StartElement( "timelines" ); 
		DeSerializer.StartElement( "DOMTimeline" ); 
		DeSerializer.StartElement( "layers" ); 

		int numOfTimelineTags =  DeSerializer.GetNumChildNodes();

		for( int i=0; i < numOfTimelineTags; i++ )
		{
			DomTimelines Layer;
			std::string name;
			DeSerializer.StartElement( i, name ); //"DOMLayer"
			DeSerializer.GetAttribute( "name", Layer.mLayerName  ); 
			DeSerializer.GetAttribute( "frameCount", Layer.mFrameCount  ); 
			DeSerializer.StartElement( "frames" ); 
			DeSerializer.StartElement( "DOMFrame" ); 
			DeSerializer.StartElement( "elements" ); 
			DeSerializer.StartElement( "DOMSymbolInstance" );
			DeSerializer.GetAttribute( "libraryItemName", Layer.mLibraryItemName ); 

			Layer.mFilename = GetLibraryNameFromLabel( Layer.mLibraryItemName );


			//
			DeSerializer.StartElement( "matrix" ); 
			DeSerializer.StartElement( "Matrix" ); 

			DeSerializer.GetAttribute( "tx", Layer.mMatrixTx ); 
			DeSerializer.GetAttribute( "ty", Layer.mMatrixTy ); 

			DeSerializer.EndElement(); 
			DeSerializer.EndElement();
			
			/////

			DeSerializer.StartElement( "transformationPoint" );
			DeSerializer.StartElement( "Point" ); 

			DeSerializer.GetAttribute( "x", Layer.mPointx );
			DeSerializer.GetAttribute( "y", Layer.mPointy );

			DeSerializer.EndElement(); 
			DeSerializer.EndElement(); 

			/*


			- <PropertyContainer id="Basic_Motion">
				+ <Property id="Motion_X" enabled="1">
				<Keyframe anchor="0,0" next="0,0" previous="0,0" roving="0" timevalue="0" /> 
				</Property>
				+ <Property id="Motion_Y" enabled="1">
				<Keyframe anchor="0,0" next="0,0" previous="0,0" roving="0" timevalue="0" /> 
				</Property>
				+ <Property id="Rotation_Z" enabled="1">
				<Keyframe anchor="0,0" next="0,0" previous="0,0" roving="0" timevalue="0" /> 
				</Property>
				</PropertyContainer>
				- <PropertyContainer id="Transformation">
				+ <Property id="Scale_X" enabled="1">
				<Keyframe anchor="0,100" next="0,100" previous="0,100" roving="0" timevalue="0" /> 
				</Property>
				+ <Property id="Scale_Y" enabled="1">
				<Keyframe anchor="0,100" next="0,100" previous="0,100" roving="0" timevalue="0" /> 
				</Property>
				+ <Property id="Skew_X" enabled="1">
				<Keyframe anchor="0,0" next="0,0" previous="0,0" roving="0" timevalue="0" /> 
				</Property>
				+ <Property id="Skew_Y" enabled="1">

				*/
 

			mTimelines.push_back( Layer );
			////

			DeSerializer.EndElement(); 
			DeSerializer.EndElement(); 
			DeSerializer.EndElement(); 
			DeSerializer.EndElement(); 
			DeSerializer.EndElement(); 

		}

		DeSerializer.EndElement(); 
		DeSerializer.EndElement(); 
		DeSerializer.EndElement(); 
	
	}


	void DoLibrary( NxGraphicsXml & DeSerializer, std::vector< DomLibrary > & mLibrary )
	{
		DeSerializer.StartElement( "media" );
		int numOfMediaTags =  DeSerializer.GetNumChildNodes();

		for( int i=0; i<numOfMediaTags; i++ )
		{
			std::string child;
			DeSerializer.StartElement( i, child ); 
			DomLibrary LibItem;
			LibItem.mType = GetItemType( child );
			std::string HrefName; 
			DeSerializer.GetAttribute( "href", HrefName ); 
			NxUtils::SplitFileName( HrefName );
			LibItem.mHref = mFolderExtractName + HrefName;
			DeSerializer.GetAttribute( "name", LibItem.mName ); 

			DeSerializer.EndElement(); 
			mLibrary.push_back( LibItem );
		}

		DeSerializer.EndElement( ); 

		//for( int i = 0 ; i < mLibrary.size(); i++ ) {
		//	Log( "Type: " + Ogre::StringConverter::toString( mLibrary[i].mType )   + " HREF: " + mLibrary[i].mHref + " Name: " + mLibrary[i].mName );
		//}
	}


	void DoSymbols( NxGraphicsXml & DeSerializer, std::vector< DomSymbols > & mSymbols )
	{
		DeSerializer.StartElement( "symbols" );
		int numOfSymbolsTags =  DeSerializer.GetNumChildNodes();
		for( int i=0; i< numOfSymbolsTags ; i++ )
		{
			DomSymbols SymbolItem;
			std::string child;
			DeSerializer.StartElement( i, child ); // DomSymbolItem
			DeSerializer.GetAttribute( "name", SymbolItem.mLabelName ); 
			DeSerializer.StartElement( "timeline" ); // timeline
			DeSerializer.StartElement( "DOMTimeline" ); // DOMTimeline
			DeSerializer.StartElement( "layers" ); // layers
			DeSerializer.StartElement( "DOMLayer" ); // DOMLayer
			DeSerializer.StartElement( "frames" ); // frames
			DeSerializer.StartElement( "DOMFrame" ); // DOMFrame	
			DeSerializer.StartElement( "elements" ); // elements	

			///////////////
			std::string ChildType;
			DeSerializer.StartElement( 0, ChildType ); // DOMVideoInstance // DOMBitmapInstance
			SymbolItem.mType = GetItemType( ChildType );
			DeSerializer.GetAttribute( "libraryItemName", SymbolItem.mLabelName ); 
			DeSerializer.EndElement();
			//////////////////////

			DeSerializer.EndElement( ); // elements
			DeSerializer.EndElement( ); // DOMFrame
			DeSerializer.EndElement(   ); // frames
			DeSerializer.EndElement(  ); // DOMLayer
			DeSerializer.EndElement(   ); // layers
			DeSerializer.EndElement( ); // DOMTimeline
			DeSerializer.EndElement(  ); // timeline
			DeSerializer.EndElement();  // DomSymbolItem
			mSymbols.push_back( SymbolItem );
		}

		DeSerializer.EndElement(); 
	
	}

NxImportXfl::NxImportXfl( Nx3D_Scene * Scene, const string & FileName )
{

	Ogre::ZipArchive * mZip = new ZipArchive( FileName, "Zip" );
	mZip->load();

	FileInfoListPtr mZipFileList = mZip->listFileInfo(true,false);
	Ogre::VectorIterator<Ogre::FileInfoList> file_list_itr(*mZipFileList);
	while( file_list_itr.hasMoreElements())
	{
		Ogre::FileInfo file_info = file_list_itr.getNext();
		DataStreamPtr text = mZip->open( file_info.filename );
		size_t ZipFileSize = text->size();
		char * byter = new char[ ZipFileSize ];
		text->read( byter , ZipFileSize )  ;

		std::string FileName = mFolderExtractName;
		FileName += file_info.filename.c_str();
		ofstream file( FileName.c_str(), ofstream::binary );
		if( file.is_open() ){
			file.write ( byter, ZipFileSize);
			file.close();
			delete[] byter;
		}
 
	}

	DataStreamPtr DomDoc = mZip->open( "DOMDocument.xml" );
	NxGraphicsXml * DeSerializer = new NxGraphicsXml();
	DeSerializer->Parse( DomDoc->getAsString().c_str() );

	std::string rootname;
	DeSerializer->GetRootElementName( rootname );
	DeSerializer->StartElement( rootname );


	DOMSettings mSettings;
	DeSerializer->GetAttribute( "backgroundColor", mSettings.mBackgroundColor );
	DeSerializer->GetAttribute( "frameRate", mSettings.mFrameRate ); 
	DeSerializer->GetAttribute( "width", mSettings.mWidth ); 
	DeSerializer->GetAttribute( "height", mSettings.mHeight ); 
	DeSerializer->GetAttribute( "versionInfo", mSettings.mVersionInfo );


	/////// MEDIA /////
	DoLibrary( *DeSerializer, mLibrary );
	///// SYMBOLS /////////
	DoSymbols( *DeSerializer, mSymbols );
	///// TIMELINES /////////
	DoTimelines( *DeSerializer, mTimelines );

}

NxImportXfl::~NxImportXfl()
{

}


}