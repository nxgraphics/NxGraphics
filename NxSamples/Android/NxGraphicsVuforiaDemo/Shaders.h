const std::string vShaderStr =
	"#version 100 \n"

	"precision mediump int;\n"
	"precision mediump float;\n"

	"uniform mat4 worldViewProj;\n"
	"attribute vec4 vertex;\n"
	"attribute vec2 uv0;\n"
	"varying vec2 uv;\n"

	"void main()\n"
	"{ \n"
	"	gl_Position = worldViewProj * vertex;\n"
	"	uv = uv0;\n"
	"} \n";

//  original yuv
const std::string fShaderStr2 =
	"#version 100 \n"
	"precision mediump float; \n"
	"uniform sampler2D tex0; \n"
	"uniform sampler2D tex1; \n"
	"uniform sampler2D tex2; \n"
	"varying vec2 uv; \n"
	"const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027); \n"
	"const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968); \n"
	"const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000); \n"
	"const vec3 offset = vec3(-0.0625, -0.5, -0.5); \n"
	"void main() \n"
	"{ \n"
	"float y = texture2D(tex0, uv).r;\n"
	"float u = texture2D(tex1, uv).r;\n"
	"float v = texture2D(tex2, uv).r;\n"
	"vec3 yuv = vec3(y,u,v);\n"
	"yuv += offset;\n"
	"vec4 fragcolor = vec4(0.0, 0.0, 0.0, 1.0);\n"
	"fragcolor.r = dot(yuv, R_cf);\n"
	"fragcolor.g = dot(yuv, G_cf);\n"
	"fragcolor.b = dot(yuv, B_cf);\n"
	"gl_FragColor = vec4( fragcolor ); \n"
	"} \n";



const std::string fShaderStrok =
	"#version 100 \n"
	"precision mediump float; \n"
	"uniform sampler2D tex0; \n"
	"uniform sampler2D tex1; \n"
	"uniform sampler2D tex2; \n"
	"varying vec2 uv; \n"
	"const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027); \n"
	"const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968); \n"
	"const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000); \n"
	"const vec3 offset = vec3(-0.0625, -0.5, -0.5); \n"
	"void main() \n"
	"{ \n"
	"if( uv.x <= 0.5 ){ \n"
	"float y = texture2D(tex0, vec2(uv.x, uv.y )).r;\n"
	"float u = texture2D(tex1, vec2(uv.x , uv.y )).r;\n"
	"float v = texture2D(tex2, vec2(uv.x , uv.y )).r;\n"
	"float alpha = texture2D(tex0, vec2(uv.x+0.5, uv.y)).r;\n"
	"vec3 yuv = vec3(y,u,v);\n"
	"yuv += offset;\n"
	"vec4 fragcolor;\n"
	"fragcolor.r = dot(yuv, R_cf);\n"
	"fragcolor.g = dot(yuv, G_cf);\n"
	"fragcolor.b = dot(yuv, B_cf);\n"
	"fragcolor.a = alpha < 0.1 ? 0.0 : alpha;\n"
	"gl_FragColor = fragcolor; \n"
	"}else{ discard; }"
	"} \n";




const std::string fShaderStr =
	"#version 100 \n"
	"precision mediump float; \n"
	"uniform sampler2D tex0; \n"
	"uniform sampler2D tex1; \n"
	"uniform sampler2D tex2; \n"
	"varying vec2 uv; \n"
	"const vec3 R_cf = vec3(1.164383,  0.000000,  1.596027); \n"
	"const vec3 G_cf = vec3(1.164383, -0.391762, -0.812968); \n"
	"const vec3 B_cf = vec3(1.164383,  2.017232,  0.000000); \n"
	"const vec3 offset = vec3(-0.0625, -0.5, -0.5); \n"
	"void main() \n"
	"{ \n"
	"if( uv.x <= 0.5 ){ \n"
	"float y = texture2D(tex0, vec2(uv.x, uv.y )).r;\n"
	"float u = texture2D(tex1, vec2(uv.x , uv.y )).r;\n"
	"float v = texture2D(tex2, vec2(uv.x , uv.y )).r;\n"
	"float alpha = texture2D(tex0, vec2(uv.x+0.5, uv.y)).r;\n"
	"vec3 yuv = vec3(y,u,v);\n"
	"yuv += offset;\n"
	"vec4 fragcolor;\n"
	"fragcolor.r = dot(yuv, R_cf);\n"
	"fragcolor.g = dot(yuv, G_cf);\n"
	"fragcolor.b = dot(yuv, B_cf);\n"
	"fragcolor.a = alpha;\n"
	"if(fragcolor.a < 0.6) discard;\n"
	"else gl_FragColor = fragcolor; \n"
	"}else{ discard; }"
	"} \n";

