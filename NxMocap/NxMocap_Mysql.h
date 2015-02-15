/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       __  ___                         
			   / | / /_  __ /  |/  /____   _____ ____ _ ____ 
			  /  |/ /| |/_// /|_/ // __ \ / ___// __ `// __ \
			 / /|  /_>  < / /  / // /_/ // /__ / /_/ // /_/ /
			/_/ |_//_/|_|/_/  /_/ \____/ \___/ \__,_// .___/ 
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
#ifndef __NXMOCAPMYSQL_H__
#define __NXMOCAPMYSQL_H__

#include "NxMocap_Prerequisites.h"

namespace NxMocap_Namespace {

class NxMocap_Export NxMocapMysql
{
public :
	NxMocapMysql();
	~NxMocapMysql();
	bool Connect( const std::string & Server, const std::string & User, const std::string & Password );
	bool IsConnected();
	void Disconnect();
	bool SelectDatabase( const std::string & DatabaseName );
	void Query( const std::string & QueryString );
	void FreeResults();
	unsigned long long NumRows();
	char ** FetchRow();
	bool CreateDatabase( const std::string & database );
	bool DeleteDatabase( const std::string & database );
	bool DatabaseExists( const std::string & database );
	void ListDatabases( std::vector<std::string> & DatabaseList );
	const std::string & GetServerIp() const;
	const std::string & GetUsername() const;
	const std::string & GetPassword() const;

private :
	std::string mServerIP ;
	std::string mServerUserName ;
	std::string mServerUserPassword ;
	NxSqlHelper * mSql;

};

}

#endif