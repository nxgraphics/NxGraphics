package com.hotstuff.main;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.CookieHandler;
import java.net.CookieManager;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;

import android.os.AsyncTask;
import android.util.Log;

import com.apache.ivy.ApacheURLLister;

 

public class DownloadFilesTask extends AsyncTask<Void, Integer, String> {
	
	public static enum DownloadFilesTaskMethod {  
		MethodGet,
		MethodPost
	}
	
	 public interface DownloadCallback{
		public void OnFinished();
		public void onMessage( String msg, int progress1, int progress2  );
		public void onError( int errorType );
	}
	 	
	 String mCurrentTask = "";
	 String mCurrentMsg = "";
	 int mCurrentprogress01 = 0;
	 int mCurrentprogress02 = 0;	
	 DownloadFilesTask mMediaTask = null;
		
	 String TAG = "DownloadFileTask";

	 private DownloadCallback mCallback = null;
	 private Boolean mIsFiles = false;
	 private Boolean mCancel = false;
	 private ArrayList<String> mDstFolder = null;
	 private ArrayList<String> mMedia = null; 
	 private String mSessionId = null;
	 private DownloadFilesTaskMethod mMethod = null;
	 private String mMethodParameters = null;	 

	 public DownloadFilesTask( Boolean isFiles, ArrayList<String> media, ArrayList<String> dstFolder, String sessionId, DownloadCallback call ) { 
		 mCallback = call;
		 mIsFiles = isFiles; 
		 mMedia = media;
		 mDstFolder = dstFolder;
		 mSessionId = sessionId;
		 mMethod = DownloadFilesTaskMethod.MethodGet;
	 }
	 
	 public DownloadFilesTask setMethod( DownloadFilesTaskMethod method, String parameters ) { 
		 mMethod = method;
		 mMethodParameters = parameters;
		 return this;
	 }
	 
	 public void sendMsg( String task, String msg, int progress01, int progress02 ) { 
   	
		mCurrentTask = task;
		mCurrentMsg = msg;
		mCurrentprogress01 = progress01;
		mCurrentprogress02 = progress02;
		publishProgress(0);
   	
   }
   
	public  boolean UrlExists(String URLName){
	    try {
	      HttpURLConnection.setFollowRedirects(false);
	      // note : you may also need
	      // HttpURLConnection.setInstanceFollowRedirects(false)
	      HttpURLConnection con = (HttpURLConnection) new URL(URLName).openConnection();
	      con.setRequestMethod("HEAD");
	      return (con.getResponseCode() == HttpURLConnection.HTTP_OK);
	    }
	    catch (Exception e) {
	       e.printStackTrace();
	       return false;
	    }
 	  }	  	

	public void CopyContents( String urli, File folder ) {			 
		if(  !UrlExists( urli ) ) { 
			Log.d( TAG, "Warning: remote url folder doesnt exist : " + urli );
			return; 	
		} 
			
		URL url = null;
		try {
			url = new URL(urli);
		} catch (MalformedURLException e1) {
			e1.printStackTrace();
		}
		 
       ApacheURLLister lister = new ApacheURLLister();
       List files = null;
       List folders = null;
		try {
			files = lister.listFiles(url);
			folders = lister.listDirectories(url);
		} catch (IOException e1) {
			e1.printStackTrace();
		}

       for( Iterator iter = files.iterator(); iter.hasNext(); ) {
           URL fileUrl = (URL) iter.next();
           try {
				httpFileDownload(fileUrl, folder  );	
			} catch ( Exception e ) {
				e.printStackTrace();
			}
       }

       for ( Iterator iter = folders.iterator(); iter.hasNext();) {
           URL folderUrl = (URL) iter.next();
           try {
           	File foldertocreate = new File( folderUrl.getPath() );
           	String dtf = folder.getAbsolutePath()+"/"+foldertocreate.getName()+"/" ;
           	String Dst = "http://"+folderUrl.getHost()+folderUrl.getPath(); 
           	downloadFolderToLocal( Dst, dtf ); 
			} catch (Exception e) {
				e.printStackTrace();
			}
       }	        
	
	} 	    

	// TODO : make one function for the two below, param boolean.
	public void downloadFolderToLocal( String urlFile, String dstFolder ) {
		File file2 = new File( dstFolder );	
		if( !file2.exists() ) { 
			if( !file2.mkdirs() ) Log.e(TAG,"could not create directory in downloadFolderToLocal at: " + dstFolder);
		}				 
		try { 
			CopyContents( urlFile, file2 );
		} catch ( Exception e ) {	
			Log.e(TAG,"downloadFileToLocal => httpFileDownload Error :" + e.getMessage());	          
		}
	 }

	public void downloadFileToLocal( String urlFile, String dstFolder ) { 
		File file2 = new File( dstFolder );
		if( !file2.exists() ) { 
			if( !file2.mkdirs() ) Log.e(TAG,"could not create directory in downloadFileToLocal at: " + dstFolder);
		}
							 
		try {
			httpFileDownload( new URL(urlFile), file2 );
		} catch ( Exception e ) {	
			 Log.e(TAG,"downloadFileToLocal Error :" +   e.getMessage());	         
		}	 
	}		 
	 
	 public Boolean isSourceUrlDirectory( URL url ) { 
		 File sourceFile = new File(url.getPath());
		 return sourceFile.isDirectory();
	 }
	 
	 
		public static int defaultTimeoutConnection = (1000 * 60) * 15 ; // 5 minutes
		public static int defaultReceiveDataTimeout = (1000 * 60) * 480 ; // 8 hours
	 
	 public void httpFileDownload( URL url, File destFolder ) throws Exception {
		 
		 CookieManager cookieManager = new CookieManager();
		 CookieHandler.setDefault(cookieManager);
		 
		 
		  if( isSourceUrlDirectory( url )) {
			  sendMsg( "Downloading Directory:", url.getFile(), 0, mCurrentprogress02 );
		  }else{  
			  sendMsg( "Downloading File:", url.getFile(), 0, mCurrentprogress02 );
			  
			  Log.e("httpFileDownload Downloading File:", url.getFile() );	   
		  }

	      File sourceFile = new File(url.getPath());
	      File destinationFile = null;
	      if( destFolder.isDirectory() ) {  	
	    	  destinationFile = new File(destFolder.getPath() + "/" + sourceFile.getName());
	      }
	      else {  destinationFile = destFolder; }
	      
		destinationFile.getParentFile().mkdirs();
		InputStream bis2 = null;
		FileOutputStream bos2 = null;
		HttpURLConnection c = null;
		
		
		 
		  
		/*
 CookieManager cookieManager = new CookieManager();
    CookieHandler.setDefault(cookieManager);

    HttpCookie cookie = new HttpCookie("lang", "fr");
    cookie.setDomain("twitter.com");
    cookie.setPath("/");
    cookie.setVersion(0);
    cookieManager.getCookieStore().add(new URI("http://twitter.com/"), cookie);		
		*/
	        
	        try {
	        	
	        	Log.e("","opening connection: " + url.toString() );	 
	 
	            c = (HttpURLConnection) url.openConnection();
	            
	            
	            Log.e("","opened! : " + url.toString() );	 
	            c.setConnectTimeout(  defaultTimeoutConnection );
	            c.setReadTimeout(  defaultReceiveDataTimeout );

	            
	            
	            
	            // HttpURLConnection.setFollowRedirects(true);
	            
	            // get method
	           // private final String USER_AGENT = "Mozilla/5.0";
	           // c.setRequestMethod("GET");
	           // c.setRequestProperty("User-Agent", USER_AGENT);
	            
	            if(mSessionId != null ) { 
	            	c.setRequestProperty("Cookie", "PHPSESSID=" + mSessionId );
	            	
	            }
	            
	             
	            if( mMethod != null ) {
	            	
	            	if( mMethod == DownloadFilesTaskMethod.MethodPost ) { 
	            		String USER_AGENT = "Mozilla/5.0";
	            		c.setDoOutput(true);
	            		c.setRequestMethod("POST");
	            		c.setRequestProperty("Connection","Keep-Alive");
	    	    		c.setRequestProperty("User-Agent", USER_AGENT);
	    	    		c.setRequestProperty("Accept-Language", "en-US,en;q=0.5");
	    	    		
	    	    		c.setRequestProperty("Content-type", "application/x-www-form-urlencoded");
	    	    		 
	    	    		  
	    	    		DataOutputStream wr = new DataOutputStream(c.getOutputStream());
	    	    		wr.writeBytes(mMethodParameters);
	    	    		wr.flush();
	    	    		wr.close();	      
	            	}else /*if(  mMethod == DownloadFilesTaskMethod.MethodGet )*/ { 
	            		
	            		
	              		 
	            		String USER_AGENT = "Mozilla/5.0";
	            	 
	            		c.setRequestMethod("GET");
	            		c.setRequestProperty("Connection","Keep-Alive");
	    	    		//c.setRequestProperty("User-Agent", USER_AGENT);
	    	    		//c.setRequestProperty("Accept-Language", "en-US,en;q=0.5");
	    	    		/*
	    	    		DataOutputStream wr = new DataOutputStream(c.getOutputStream());
	    	    		wr.writeBytes(mMethodParameters);
	    	    		wr.flush();
	    	    		wr.close();	      
	            		*/
	            		
	            	}
	            	
	            	
	            	
	            } 
	            

	            	
 
	            
	            try {
	            	 Log.e("","connecting  to : " + url.toString() );	
	            	 
	            	 
	            	c.connect();
	            } catch (NullPointerException npe) {
	                Log.w("", "Bad URI? " + url);
	                throw new IOException(npe.toString());
	              } catch (IllegalArgumentException iae) {
	                Log.w("", "Bad URI? " + url);
	                throw new IOException(iae.toString());
	              } catch (SecurityException se) {
	                Log.w("", "Restricted URI? " + url);
	                throw new IOException(se.toString());
	              } catch (IndexOutOfBoundsException ioobe) {
	                Log.w("", "Bad URI? " + url);
	                throw new IOException(ioobe.toString());
	              }
	            
	            int status;

	            try {
	            	
	            	Log.e("","getResponseCode .. " );	
	            	
	            	status=  c.getResponseCode();
	            	Log.e("","HTTP answer :" + String.valueOf( status));	
	            	
	                
	                
	                if( mCallback != null && ( status == 404 ) ) mCallback.onError( status ) ;
	                
	                
	              } catch (NullPointerException npe) {
	            	  // this is maybe this Android bug: http://code.google.com/p/android/issues/detail?id=15554
	            	  Log.w("", "Bad URI? " + url);
	            	  throw new IOException(npe.toString());
	              } catch (NumberFormatException nfe) {
	            	  // Again seen this in the wild for bad header fields in the server response!
	            	  Log.w("", "Bad server status? " + url);
	            	  throw new IOException(nfe.toString());
	              }	    
	            
	            Log.e(""," getContentLength ... ");	

	            int lenght = c.getContentLength();
	            
	            
	            Log.e("content lentgh received", "===> lenght : " + String.valueOf( lenght  ) );
	            
	            try{
	            	
	            	 //bis2 = c.getInputStream();
 
	            	if(status != 200 ) { 
	            		bis2 = c.getErrorStream();
	            		for (int i = 0; i < bis2.available(); i++) {
	            			 Log.e("",  ""+ bis2.read() );	  
	            		}
	            	}	
	            	else { 
	            		bis2 = c.getInputStream();
	            	}
	            		 
	            	
	            	
	            	
	            	
	            }catch(  IOException e ) { 
	            	Log.e("",  "ERROR");
	            	e.printStackTrace(); // <<== does the file really exist ??
	            }
	            
	            
	            Log.e("",  "getting path : " + destinationFile.getPath() );	 
	            
	            bos2 = new FileOutputStream(destinationFile.getPath());
		 		int inc = 0;
		   		int lastReadCnt = 0;
		   		byte[] buffer = new byte[4096];

		   		String urlStr = url.toString();
		   		String fileName = urlStr.substring( urlStr.lastIndexOf('/')+1, urlStr.length() );
		   		while( (lastReadCnt = bis2.read(buffer)) != -1 ){
		   			bos2.write(buffer, 0, lastReadCnt);
		   	        inc += lastReadCnt;
		   	        float x = (float) (Math.round((inc*100.0f/lenght)*10.0) / 10.0);
		   	        sendMsg( "Downloading File: ", " \n "+fileName + " \n "+Float.toString( x ) + "%", (int)x, mCurrentprogress02 );

		   		}	            
	        } finally {

	        	c.disconnect();
	        	 
	            if (bis2 != null) { 
	                try {
	                    bis2.close();
	                } catch (IOException ioe) {
	                	System.out.println("bis2 error" );
	                    ioe.printStackTrace();
	                }	
	            }

	            if (bos2 != null) {
	                try {
	                    bos2.close();
	                } catch (IOException ioe) {
	                	System.out.println("bos2 error" );
	                    ioe.printStackTrace();
	                }
	            }	

	        }
	    }

	    @Override
	    protected void onPreExecute() {
	        super.onPreExecute();
	        
	    } 
	    
		@Override
		protected void onCancelled() {
			mCancel = false;
		}
	    
	    protected String doInBackground( Void... params) {

	    	int TotalProgress = 0;
	    	for(int i = 0; i < mMedia.size(); i++ ) {  
	    		if( mCancel ) break;
	    		if( mIsFiles ) { 
	    			downloadFileToLocal( mMedia.get(i), mDstFolder.get(i) );
	    		}else{ 
	    			downloadFolderToLocal( mMedia.get(i), mDstFolder.get(i) );
	    		}
	    		TotalProgress += 100/mMedia.size();
	    		mCurrentprogress02 = TotalProgress;
	    		sendMsg(mCurrentTask, mCurrentMsg, mCurrentprogress01, mCurrentprogress02 );	
	    	}
	    	return null;
	    }
	    
	    @Override
	     protected void onProgressUpdate(Integer... progress)  {
	    	
	    	mCallback.onMessage( mCurrentTask + mCurrentMsg, mCurrentprogress01, mCurrentprogress02 );
	      
	     }
	    
	    protected void onPostExecute(String file_url) {		    	
	    	if( mCallback != null ) mCallback.OnFinished();
	    	Log.d( TAG, "Successfully downloaded files !");
	    } 
}
