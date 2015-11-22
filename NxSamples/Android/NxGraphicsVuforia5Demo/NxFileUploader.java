package com.hotstuff.main;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

//import org.apache.commons.codec.binary.Base64;
import org.apache.http.util.ByteArrayBuffer;

import android.os.AsyncTask;
import android.util.Log;

//Read more: http://getablogger.blogspot.com/2008/01/android-how-to-post-file-to-php-server.html#ixzz3rreXb2jp


public class NxFileUploader  extends AsyncTask<String, Void, String> {
	
	
	 HttpURLConnection conn = null;
	 
	 String mFileName = null;
	 String mUrl = null;
	 
	 
    protected String doInBackground(String... urls) {
    	
    	
    	sendFile( mFileName , mUrl );
    	
    return "";
    }
	
	

	 
	 public NxFileUploader( String url, String filename ) { 
		 mUrl = url;
		 mFileName = filename;
		 
		 
	 }

 public   void sendFile( String filePath , String urlString) { 
	 
	 
	  String exsistingFileName = filePath;//"/sdcard/uploader/data/testfile";

      String lineEnd = "\r\n";
      String twoHyphens = "--";
      String boundary = "*****";
      try {
          // ------------------ CLIENT REQUEST

        //  Log.e(Tag, "Inside second Method");

          FileInputStream fileInputStream = new FileInputStream(new File(
                  exsistingFileName));

          // open a URL connection to the Servlet

          URL url = new URL(urlString);

          // Open a HTTP connection to the URL

            conn = (HttpURLConnection) url.openConnection();

          // Allow Inputs
          conn.setDoInput(true);

          // Allow Outputs
          conn.setDoOutput(true);

          // Don't use a cached copy.
          conn.setUseCaches(false);

          // Use a post method.
          conn.setRequestMethod("POST");

          conn.setRequestProperty("Connection", "Keep-Alive");

          conn.setRequestProperty("Content-Type",
                  "multipart/form-data;boundary=" + boundary);

          DataOutputStream dos = new DataOutputStream(conn.getOutputStream());

          dos.writeBytes(twoHyphens + boundary + lineEnd);
          dos
                  .writeBytes("Content-Disposition: post-data; name=uploadedfile;filename="
                          + exsistingFileName + "" + lineEnd);
          dos.writeBytes(lineEnd);

        //  Log.e(Tag, "Headers are written");

          // create a buffer of maximum size

          int bytesAvailable = fileInputStream.available();
          int maxBufferSize = 1000;
          // int bufferSize = Math.min(bytesAvailable, maxBufferSize);
          byte[] buffer = new byte[bytesAvailable];

          // read file and write it into form...

          int bytesRead = fileInputStream.read(buffer, 0, bytesAvailable);

          while (bytesRead > 0) {
              dos.write(buffer, 0, bytesAvailable);
              bytesAvailable = fileInputStream.available();
              bytesAvailable = Math.min(bytesAvailable, maxBufferSize);
              bytesRead = fileInputStream.read(buffer, 0, bytesAvailable);
          }

          // send multipart form data necesssary after file data...

          dos.writeBytes(lineEnd);
          dos.writeBytes(twoHyphens + boundary + twoHyphens + lineEnd);

          // close streams
         // Log.e(Tag, "File is written");
          fileInputStream.close();
          dos.flush();
          dos.close();

      } catch (MalformedURLException ex) {
          Log.e("", "error: " + ex.getMessage(), ex);
      }

      catch (IOException ioe) {
          Log.e("", "error: " + ioe.getMessage(), ioe);
      }

      try {
          BufferedReader rd = new BufferedReader(new InputStreamReader(conn
                  .getInputStream()));
          String line;
          while ((line = rd.readLine()) != null) {
              Log.e("Dialoge Box", "Message: " + line);
          }
          rd.close();

      } catch (IOException ioex) {
          Log.e("MediaPlayer", "error: " + ioex.getMessage(), ioex);
      }	 
	 
	 
 }

}

 
