#include "NxGraphicsPreCompiled.h"
#include <algorithm> 
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#define PATHMAXLEN 260

extern "C" 
{
#include "../nxdeps/include/Fast/fast.h"
}
 
namespace Nx {

NxUtils::NxUtils()
{

}

NxUtils::~NxUtils()
{

}

void NxUtils::DetectCorners( float * Cotrners2D, const unsigned char * im, int xsize, int ysize, int stride, int b, int* ret_num_corners, bool nonmax_sup,  NxFastCornerDetectionType Type  )
{
	//xy * ret = fast12_detect( im,  xsize, ysize,  stride,  b,  ret_num_corners);
	//xy * ret = fast9_detect_nonmax( im,  xsize, ysize,  stride,  b,  ret_num_corners);

	xy * corners = NULL;
	xy * nonmax_corners = NULL;

	int num_corners=0, num_nonmax=0;
	int* scores=0;
	
	if(Type == NxFast9)
		corners = fast9_detect(im,  xsize, ysize,  stride,  b,  &num_corners );
	else if(Type == NxFast10)
		corners = fast10_detect(im,  xsize, ysize,  stride,  b,  &num_corners );
	else if(Type == NxFast11)
		corners = fast11_detect(im,  xsize, ysize,  stride,  b,  &num_corners );
	else
		corners = fast12_detect(im,  xsize, ysize,  stride,  b,  &num_corners );
	
	//Compute scores
	if(nonmax_sup)
	{
		if(Type == NxFast9)
			scores = fast9_score(im, stride, corners, num_corners, b);
		else if(Type == NxFast10)
			scores = fast10_score(im, stride, corners, num_corners, b);
		else if(Type == NxFast11)
			scores = fast11_score(im, stride, corners, num_corners, b);
		else
			scores = fast12_score(im, stride, corners, num_corners, b);
	}

	 

	*ret_num_corners = num_corners;

	int y = 0;
	int numCorners= *ret_num_corners;
	for(int i = 0; i < numCorners; i++){
		Cotrners2D[y]  = corners[i].x;
		Cotrners2D[y+1]  = corners[i].y;
		y += 2;
	}
	//*ret_corners = corners;

	//Do nonmax suppression if need be
	if(nonmax_sup)
	{
		nonmax_corners = nonmax_suppression(corners, scores, num_corners, & num_nonmax);

	 *ret_num_corners = num_nonmax; 
		int y = 0;
		int numCorners= *ret_num_corners;
		for(int i = 0; i < numCorners; i++){
			Cotrners2D[y]  = nonmax_corners[i].x;
			Cotrners2D[y+1]  = nonmax_corners[i].y;
			y += 2;
		}
		

		free(corners);
	}

	free(scores);
 
}
/*
#ifdef WIN32
#define realpath(N,R) _fullpath((R),(N),PATHMAXLEN)
#endif
*/

 
std::string NxUtils::GetFullPath( std::string Path )
{

	char full[PATHMAXLEN];
	
 
	#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	if( _fullpath( full, ( const char * ) Path.c_str(), PATHMAXLEN ) != NULL ){

	}
	#elif NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_ANDROID
	if(  true ){
		realpath( ( const char * ) Path.c_str(), full ) ;

	}
	#endif

	//else {
	//	std::string PathIncorrect = std::string( "Invalid Path : " + Path );
	//} 
	
	return std::string( full );	
}

void NxUtils::SplitFileName( std::string& Filename )
{
	string fullPath(Filename);
	replace(fullPath.begin(),fullPath.end(),'\\','/');
	string::size_type lastSlashPos = fullPath.find_last_of('/');
	if (lastSlashPos==std::string::npos)//didnt find a slach
	{ 
		Filename=""; 
	}
	else// found a slach
	{
		Filename=fullPath.substr(lastSlashPos+1,fullPath.size()-lastSlashPos-1);
	}
}

void NxUtils::SplitFilepath( std::string& Path )
{
	string fullPath(Path);
	replace(fullPath.begin(),fullPath.end(),'\\','/');
	string::size_type lastSlashPos = fullPath.find_last_of('/');
	if (lastSlashPos==std::string::npos)//didnt find a slach
	{ Path=""; }
	else// found a slach
	{
		Path=fullPath.substr(0,lastSlashPos);
	}
}

std::string NxUtils::GetCurrentWorkingDirectory()
{
	std::string workingDirectory = "";
	char currentPath[PATHMAXLEN];
	getcwd(currentPath, PATHMAXLEN);
	workingDirectory = currentPath;
	return workingDirectory + "\\";
}

void NxUtils::ToLower( std::string& Str )
{
	std::transform(Str.begin(), Str.end(), Str.begin(), ::tolower );
}

std::vector<std::string> NxUtils::SplitString(const std::string& s, const std::string& delim, const bool keep_empty )
{
	std::vector<std::string> result;
    if (delim.empty())  {
        result.push_back(s);
        return result;
    }
	std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        string temp(substart, subend);
        if (keep_empty || !temp.empty())  {
            result.push_back(temp);
        }
        if (subend == s.end())  {
            break;
        }
        substart = subend + delim.size();
    }
    return result;
}

int NxUtils::SearchDirectory(std::vector<std::string> &refvecFiles,
                    const std::string &refcstrRootDirectory,
                    const std::string &refcstrExtension,
                    bool bSearchSubdirectories = true)
{
	#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
   std::string strFilePath; // Filepath
   std::string strPattern; // Pattern
   std::string strExtension; // Extension
   HANDLE hFile; // Handle to file
   WIN32_FIND_DATA FileInformation; // File information
   
   strPattern = refcstrRootDirectory + "\\*.*";
   
   hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
   if(hFile != INVALID_HANDLE_VALUE)
   {
      do
      {
         if(FileInformation.cFileName[0] != '.')
         {
            strFilePath.erase();
            strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;
            
            if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
               if(bSearchSubdirectories)
               {
                  // Search subdirectory
                  int iRC = SearchDirectory(refvecFiles,
                     strFilePath,
                     refcstrExtension,
                     bSearchSubdirectories);
				  if(iRC){ return iRC; }
               }
            }
            else
            {
               // Check extension
               strExtension = FileInformation.cFileName;
			   ToLower(strExtension);
               strExtension = strExtension.substr(strExtension.rfind(".") + 1);
               
               if(strExtension == refcstrExtension) {
                  refvecFiles.push_back(strFilePath); // Save filename
               }
            }
         }
      } while(::FindNextFile(hFile, &FileInformation) == TRUE);
      
      // Close handle
      ::FindClose(hFile);
      
      DWORD dwError = ::GetLastError();
	  if(dwError != ERROR_NO_MORE_FILES){ return dwError; }
   }
#endif
   
   return 0;
}

int NxUtils::ListDirectories( std::vector<std::string> &refvecDirectory, const std::string &refcstrRootDirectory, bool bSearchSubdirectories = true)
{

#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
   std::string strFilePath; // Filepath
   std::string strPattern; // Pattern
   std::string strExtension; // Extension
   HANDLE hFile; // Handle to file
   WIN32_FIND_DATA FileInformation; // File information
   
   strPattern = refcstrRootDirectory + "\\*.*";
   
   hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
   if(hFile != INVALID_HANDLE_VALUE)
   {
      do
      {
         if(FileInformation.cFileName[0] != '.')
         {
            strFilePath.erase();
            strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;
            
            if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // directory
            {
				refvecDirectory.push_back( FileInformation.cFileName ); // Save directory

               if(bSearchSubdirectories)
               {
                  // Search subdirectory
                  int iRC = ListDirectories(refvecDirectory,strFilePath,bSearchSubdirectories);
				  if(iRC){ return iRC; }
               }
            }
            else // file
            {

            }
         }
      } while(::FindNextFile(hFile, &FileInformation) == TRUE);
      
      // Close handle
      ::FindClose(hFile);
      
      DWORD dwError = ::GetLastError();
	  if(dwError != ERROR_NO_MORE_FILES){ return dwError; }
   }

#endif
   
   return 0;
}

string NxUtils::UnicodeToAnsi( wstring str)
{
	size_t const mbs_len = wcstombs(NULL, str.c_str(), 0);
	std::vector<char> tmp(mbs_len + 1);
	wcstombs(&tmp[0], str.c_str(), tmp.size());
	string dest = "";
	dest.assign(tmp.begin(), tmp.end() - 1);
	return dest;
}

wstring NxUtils::AnsiToUnicode( string str)
{
	size_t const wcs_len = mbstowcs(NULL, str.c_str(), 0);
	std::vector<wchar_t> tmp(wcs_len + 1);
	mbstowcs(&tmp[0], str.c_str(), str.size());
	wstring dest = L"";
	dest.assign(tmp.begin(), tmp.end() - 1);
	return dest;
}

float NxUtils::GetRandom(float x, float y) 
{
	float high = 0;
	float low = 0;
	float randNum = 0;
	if (x == y) return x;
	high = max(x,y);
	low = min(x,y);
	randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
	return randNum;
}

}