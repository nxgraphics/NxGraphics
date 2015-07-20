package com.hotstuff.main;

 

public class VuforiaShaders 
{
	
	public static final String CUBE_MESH_VERTEX_SHADER = " \n" +
			  "\n" +
			  "attribute vec4 vertexPosition; \n"+
			  "attribute vec4 vertexNormal; \n"+
			  "attribute vec2 vertexTexCoord; \n"+
			  "\n"+
			  "varying vec2 texCoord; \n"+
			  "varying vec4 normal; \n"+
			  "\n"+
			  "uniform mat4 modelViewProjectionMatrix; \n"+
			  "\n"+
			  "void main() \n"+
			  "{ \n"+
			  "   gl_Position = modelViewProjectionMatrix * vertexPosition; \n"+
			  "   normal = vertexNormal; \n"+
			  "   texCoord = vertexTexCoord; \n"+
			  "} \n";
	
	public static final String CUBE_MESH_FRAGMENT_SHADER = " \n" +
			 "\n" +
			 "precision mediump float; \n" +
			 " \n" +
			 "varying vec2 texCoord; \n" +
			 "varying vec4 normal; \n" +
			 " \n" +
			 "uniform sampler2D texSampler2D; \n" +
			 " \n" +
			 "void main() \n" +
			 "{ \n" +
			 "   gl_FragColor = texture2D(texSampler2D, texCoord); \n" +
			 "} \n";
	
	public static final String BTA_VERTEX_SHADER = 			
		"attribute vec4 vertexPosition;\n" +
	    "attribute vec2 vertexTexCoord;\n" +
	    "uniform mat4 projectionMatrix;\n" +
	    "uniform float touchLocation_x;\n" +
	    "uniform float touchLocation_y;\n" +
	
		"varying vec2 texCoord;\n" +
	   
	    "vec4 tmpVertex;\n" +
	
	    "void main()\n" +
	    "{\n" +
	    "    tmpVertex=vertexPosition;\n" +
	
	    "    vec2 directionVector=tmpVertex.xy-vec2(touchLocation_x, touchLocation_y);\n" +
	    "    float distance = length(directionVector);\n" +
	    "    float sinDistance = (sin(distance)+1.0);\n" +
	    "    float strength = 0.3;\n" +
	    "    directionVector=normalize(directionVector);\n" +
	
	    "    if (sinDistance>0.0)\n" + 
	    "    {\n" +
	    "        if (touchLocation_x>-1.0)\n" +
	    "        {\n" +
	    "            tmpVertex.xy+=(directionVector*(strength/sinDistance));\n" +
	    "        }\n" +
	    "    }\n" +
	   
	    "    gl_Position = projectionMatrix * tmpVertex;\n" +
	    "    texCoord = vertexTexCoord;\n" +
	    "}\n";
	
	public static final String BTA_FRAGMENT_SHADER =
		"precision mediump float;\n" +
	    "varying vec2 texCoord;\n" +
	    "uniform sampler2D texSampler2D;\n" +
	    "void main ()\n" +
	    "{\n" +
	    "    vec3 incoming = texture2D(texSampler2D, texCoord).rgb;\n" +
	    "    float colorOut=1.0- ((incoming.r+incoming.g+incoming.b)/3.0);\n" +
	    "    gl_FragColor.rgba = vec4(colorOut, colorOut, colorOut, 1.0);\n" +
	    "}\n";

}

