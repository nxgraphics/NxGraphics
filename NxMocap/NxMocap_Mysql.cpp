#include "NxMocapPreCompiled.h"

#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#else
#include <unistd.h>
#endif


#include "../Nxdeps/include/libmysql/mysql.h"

namespace NxMocap_Namespace {

class NxSqlHelper
{
public :
NxSqlHelper() : mConnection(0)
{

}

~NxSqlHelper()
{

}

st_mysql * mConnection ;
st_mysql_res * mResult ;

};


NxMocapMysql::NxMocapMysql()
{
	mSql = new NxSqlHelper();
}

NxMocapMysql::~NxMocapMysql()
{
	delete mSql;
}
 
bool NxMocapMysql::Connect( const std::string & Server, const std::string & User, const std::string & Password ) {
	if( mSql->mConnection ) { mysql_close( mSql->mConnection ); }
	mSql->mConnection = NULL ;
	mSql->mConnection = mysql_init(NULL);

	if(mysql_real_connect( mSql->mConnection, Server.c_str(), User.c_str(), Password.c_str() , "", 0, 0, 0 ) == NULL ) {
		Log( "could not connect to Mysql Server ! using password : " + NxMocapUtils::ToString( Password.empty() ) );
		mysql_close( mSql->mConnection );
		mSql->mConnection = NULL;
		return false;
	}
	//store successful connection parameters
	mServerIP = Server;
	mServerUserName = User ;
	mServerUserPassword = Password ;
	Log( "Connected to Mysql Server : " + Server );
	return true ;
}

void NxMocapMysql::Disconnect() {
	if( mSql->mConnection ) { 
		mysql_close( mSql->mConnection );
		mSql->mConnection = NULL ;
	}
}

bool NxMocapMysql::IsConnected() {
	return ( mSql->mConnection > 0 ) ? true : false;
}

bool NxMocapMysql::SelectDatabase( const std::string & DatabaseName ) {
	if( mysql_select_db( mSql->mConnection, DatabaseName.c_str() ) < 0) {
		Log( "Unable to select Mysql Database : " + DatabaseName );
		return 0;
	}
	return true;
}

void NxMocapMysql::Query( const std::string & QueryString ) {
	mysql_query( mSql->mConnection, QueryString.c_str() );
	mSql->mResult = mysql_store_result( mSql->mConnection );
}

unsigned long long NxMocapMysql::NumRows() {
	return mysql_num_rows(mSql->mResult);
}

void NxMocapMysql::FreeResults() {
	mysql_free_result( mSql->mResult );
}

char ** NxMocapMysql::FetchRow() {
	return mysql_fetch_row( mSql->mResult );
}

bool NxMocapMysql::CreateDatabase( const std::string & database ) {
	if( !mSql->mConnection){ return false; }
	if( mysql_select_db( mSql->mConnection, database.c_str() ) != 0 ) { // if database doesnt exist
		string query_create("create database "); 
		query_create.append( database.c_str() ) ;
		query_create.append( " " ) ;
		if( mysql_query( mSql->mConnection, query_create.c_str() )!=0) {	
			Log( "Mysql Create Database Failed for : " + database ); return false; 
		}
		else {
			Log( "Mysql Database Created : " + database );
			mysql_select_db( mSql->mConnection, database.c_str() ); // select the database for query
		}

		//if(mysql_query( mSql->mConnection , "CREATE TABLE `data` (`id` int(10) NOT NULL auto_increment,`values` text NOT NULL ,`TimeStamp` double(20,6) NOT NULL, KEY `id` (`id`)) TYPE=MyISAM;")!=0)
		//{
		//	return false; 
		//} 
	}
	else {
		Log( "Mysql Database Already exists :" + database ); return false; 
	}    

	return true ;
}

bool NxMocapMysql::DeleteDatabase( const std::string & database ) {
	if( !mSql->mConnection){ return false ;}
	string query_create("drop database "); 
	query_create.append( database.c_str() ) ;
	query_create.append( " " ) ;

	if(!mysql_query( mSql->mConnection, query_create.c_str() ) ) {
		Log( "Mysql Delete Database Failed : " + database ); return false ;
	}

	Log( "Mysql Deleted Database : " + database );
	return true ;
}

bool NxMocapMysql::DatabaseExists( const std::string & database ) {
	if( !mSql->mConnection ){ return false; }
	if( mysql_select_db( mSql->mConnection, database.c_str() )){ return false ;}
	return true ;
}

void NxMocapMysql::ListDatabases( std::vector<std::string> & DatabaseList )
{
	if( !mSql->mConnection ) return;
	DatabaseList.clear();
	MYSQL_RES * Mysql_Result = mysql_list_dbs( mSql->mConnection, NULL );
	MYSQL_ROW Mysql_Row ;
	while ( (Mysql_Row = mysql_fetch_row(Mysql_Result))) {
		std::ostringstream os;
		os << Mysql_Row[0];
		std::string buffer(os.str());
		DatabaseList.push_back( buffer ); 
	}
	mysql_free_result( Mysql_Result );
}

const std::string & NxMocapMysql::GetServerIp() const {
	return mServerIP ;
}

const std::string & NxMocapMysql::GetUsername() const {
	return mServerUserName ;
}

const std::string & NxMocapMysql::GetPassword() const {
	return mServerUserPassword ;
}

}//end namespace