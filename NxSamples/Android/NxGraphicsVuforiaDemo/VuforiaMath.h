

QCAR::Matrix44F Matrix44FTranspose(QCAR::Matrix44F m)
{
	QCAR::Matrix44F r;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			r.data[i*4+j] = m.data[i+4*j];
	return r;
}


float Matrix44FDeterminate(QCAR::Matrix44F& m)
{
	return m.data[12] * m.data[9] * m.data[6] * m.data[3] - m.data[8] * m.data[13] * m.data[6] * m.data[3] -
		m.data[12] * m.data[5] * m.data[10] * m.data[3] + m.data[4] * m.data[13] * m.data[10] * m.data[3] +
		m.data[8] * m.data[5] * m.data[14] * m.data[3] - m.data[4] * m.data[9] * m.data[14] * m.data[3] -
		m.data[12] * m.data[9] * m.data[2] * m.data[7] + m.data[8] * m.data[13] * m.data[2] * m.data[7] +
		m.data[12] * m.data[1] * m.data[10] * m.data[7] - m.data[0] * m.data[13] * m.data[10] * m.data[7] -
		m.data[8] * m.data[1] * m.data[14] * m.data[7] + m.data[0] * m.data[9] * m.data[14] * m.data[7] +
		m.data[12] * m.data[5] * m.data[2] * m.data[11] - m.data[4] * m.data[13] * m.data[2] * m.data[11] -
		m.data[12] * m.data[1] * m.data[6] * m.data[11] + m.data[0] * m.data[13] * m.data[6] * m.data[11] +
		m.data[4] * m.data[1] * m.data[14] * m.data[11] - m.data[0] * m.data[5] * m.data[14] * m.data[11] -
		m.data[8] * m.data[5] * m.data[2] * m.data[15] + m.data[4] * m.data[9] * m.data[2] * m.data[15] +
		m.data[8] * m.data[1] * m.data[6] * m.data[15] - m.data[0] * m.data[9] * m.data[6] * m.data[15] -
		m.data[4] * m.data[1] * m.data[10] * m.data[15] + m.data[0] * m.data[5] * m.data[10] * m.data[15] ;
}


QCAR::Matrix44F Matrix44FInverse(QCAR::Matrix44F& m)
{
	QCAR::Matrix44F r;

	float det = 1.0f / Matrix44FDeterminate(m);

	r.data[0]   = m.data[6]*m.data[11]*m.data[13] - m.data[7]*m.data[10]*m.data[13]
	+ m.data[7]*m.data[9]*m.data[14] - m.data[5]*m.data[11]*m.data[14]
	- m.data[6]*m.data[9]*m.data[15] + m.data[5]*m.data[10]*m.data[15];

	r.data[4]   = m.data[3]*m.data[10]*m.data[13] - m.data[2]*m.data[11]*m.data[13]
	- m.data[3]*m.data[9]*m.data[14] + m.data[1]*m.data[11]*m.data[14]
	+ m.data[2]*m.data[9]*m.data[15] - m.data[1]*m.data[10]*m.data[15];

	r.data[8]   = m.data[2]*m.data[7]*m.data[13] - m.data[3]*m.data[6]*m.data[13]
	+ m.data[3]*m.data[5]*m.data[14] - m.data[1]*m.data[7]*m.data[14]
	- m.data[2]*m.data[5]*m.data[15] + m.data[1]*m.data[6]*m.data[15];

	r.data[12]  = m.data[3]*m.data[6]*m.data[9] - m.data[2]*m.data[7]*m.data[9]
	- m.data[3]*m.data[5]*m.data[10] + m.data[1]*m.data[7]*m.data[10]
	+ m.data[2]*m.data[5]*m.data[11] - m.data[1]*m.data[6]*m.data[11];

	r.data[1]   = m.data[7]*m.data[10]*m.data[12] - m.data[6]*m.data[11]*m.data[12]
	- m.data[7]*m.data[8]*m.data[14] + m.data[4]*m.data[11]*m.data[14]
	+ m.data[6]*m.data[8]*m.data[15] - m.data[4]*m.data[10]*m.data[15];

	r.data[5]   = m.data[2]*m.data[11]*m.data[12] - m.data[3]*m.data[10]*m.data[12]
	+ m.data[3]*m.data[8]*m.data[14] - m.data[0]*m.data[11]*m.data[14]
	- m.data[2]*m.data[8]*m.data[15] + m.data[0]*m.data[10]*m.data[15];

	r.data[9]   = m.data[3]*m.data[6]*m.data[12] - m.data[2]*m.data[7]*m.data[12]
	- m.data[3]*m.data[4]*m.data[14] + m.data[0]*m.data[7]*m.data[14]
	+ m.data[2]*m.data[4]*m.data[15] - m.data[0]*m.data[6]*m.data[15];

	r.data[13]  = m.data[2]*m.data[7]*m.data[8] - m.data[3]*m.data[6]*m.data[8]
	+ m.data[3]*m.data[4]*m.data[10] - m.data[0]*m.data[7]*m.data[10]
	- m.data[2]*m.data[4]*m.data[11] + m.data[0]*m.data[6]*m.data[11];

	r.data[2]   = m.data[5]*m.data[11]*m.data[12] - m.data[7]*m.data[9]*m.data[12]
	+ m.data[7]*m.data[8]*m.data[13] - m.data[4]*m.data[11]*m.data[13]
	- m.data[5]*m.data[8]*m.data[15] + m.data[4]*m.data[9]*m.data[15];

	r.data[6]   = m.data[3]*m.data[9]*m.data[12] - m.data[1]*m.data[11]*m.data[12]
	- m.data[3]*m.data[8]*m.data[13] + m.data[0]*m.data[11]*m.data[13]
	+ m.data[1]*m.data[8]*m.data[15] - m.data[0]*m.data[9]*m.data[15];

	r.data[10]  = m.data[1]*m.data[7]*m.data[12] - m.data[3]*m.data[5]*m.data[12]
	+ m.data[3]*m.data[4]*m.data[13] - m.data[0]*m.data[7]*m.data[13]
	- m.data[1]*m.data[4]*m.data[15] + m.data[0]*m.data[5]*m.data[15];

	r.data[14]  = m.data[3]*m.data[5]*m.data[8] - m.data[1]*m.data[7]*m.data[8]
	- m.data[3]*m.data[4]*m.data[9] + m.data[0]*m.data[7]*m.data[9]
	+ m.data[1]*m.data[4]*m.data[11] - m.data[0]*m.data[5]*m.data[11];

	r.data[3]   = m.data[6]*m.data[9]*m.data[12] - m.data[5]*m.data[10]*m.data[12]
	- m.data[6]*m.data[8]*m.data[13] + m.data[4]*m.data[10]*m.data[13]
	+ m.data[5]*m.data[8]*m.data[14] - m.data[4]*m.data[9]*m.data[14];

	r.data[7]  = m.data[1]*m.data[10]*m.data[12] - m.data[2]*m.data[9]*m.data[12]
	+ m.data[2]*m.data[8]*m.data[13] - m.data[0]*m.data[10]*m.data[13] 
	- m.data[1]*m.data[8]*m.data[14] + m.data[0]*m.data[9]*m.data[14];

	r.data[11]  = m.data[2]*m.data[5]*m.data[12] - m.data[1]*m.data[6]*m.data[12]
	- m.data[2]*m.data[4]*m.data[13] + m.data[0]*m.data[6]*m.data[13]
	+ m.data[1]*m.data[4]*m.data[14] - m.data[0]*m.data[5]*m.data[14];

	r.data[15]  = m.data[1]*m.data[6]*m.data[8] - m.data[2]*m.data[5]*m.data[8]
	+ m.data[2]*m.data[4]*m.data[9] - m.data[0]*m.data[6]*m.data[9]
	- m.data[1]*m.data[4]*m.data[10] + m.data[0]*m.data[5]*m.data[10];

	for (int i = 0; i < 16; i++)
		r.data[i] *= det;

	return r;
}


void multiplyMatrix(float *matrixA, float *matrixB, float *matrixC)
{
	int i, j, k;
	float aTmp[16];

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			aTmp[j * 4 + i] = 0.0;

			for (k = 0; k < 4; k++)
				aTmp[j * 4 + i] += matrixA[k * 4 + i] * matrixB[j * 4 + k];
		}
	}

	for (i = 0; i < 16; i++)
		matrixC[i] = aTmp[i];
}



void setRotationMatrix(float angle, float x, float y, float z,  float *matrix)
{
	double radians, c, s, c1, u[3], length;
	int i, j;

	radians = (angle * M_PI) / 180.0;

	c = cos(radians);
	s = sin(radians);

	c1 = 1.0 - cos(radians);

	length = sqrt(x * x + y * y + z * z);

	u[0] = x / length;
	u[1] = y / length;
	u[2] = z / length;

	for (i = 0; i < 16; i++)
		matrix[i] = 0.0;

	matrix[15] = 1.0;

	for (i = 0; i < 3; i++)
	{
		matrix[i * 4 + (i + 1) % 3] = u[(i + 2) % 3] * s;
		matrix[i * 4 + (i + 2) % 3] = -u[(i + 1) % 3] * s;
	}

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			matrix[i * 4 + j] += c1 * u[i] * u[j] + (i == j ? c : 0.0);
	}
}

void rotatePoseMatrix(float angle, float x, float y, float z, float* matrix)
{
	// Sanity check
	if (!matrix)
		return;

	float rotate_matrix[16];
	setRotationMatrix(angle, x, y, z, rotate_matrix);

	// matrix * scale_matrix
	multiplyMatrix(matrix, rotate_matrix, matrix);
}

float GetFovDegrees(){

	const QCAR::CameraCalibration& cameraCalibration = QCAR::CameraDevice::getInstance().getCameraCalibration();

	QCAR::Vec2F size = cameraCalibration.getSize();
	QCAR::Vec2F focalLength = cameraCalibration.getFocalLength();


	float fovRadians = 2 * atan(0.5f * size.data[0] / focalLength.data[0]);
	float fovDegrees = fovRadians * 180.0f / M_PI;


	float fovyRadians = 2 * atan(0.5f * size.data[1] / focalLength.data[1]);
	float fovyDegrees = fovyRadians * 180.0f / M_PI;

	return fovyDegrees;

}


void VuforiaMatrix2Nx( QCAR::Matrix44F mat1, Nx::Matrix4 & mat ) {
	//mat[0][0] = mat1.data[0]; mat[0][1] = mat1.data[1]; mat[0][2] = mat1.data[2]; mat[0][3] = mat1.data[3];
	//mat[1][0] = mat1.data[4]; mat[1][1] = mat1.data[5]; mat[1][2] = mat1.data[6]; mat[1][3] = mat1.data[7];
	//mat[2][0] = mat1.data[8]; mat[2][1] = mat1.data[9]; mat[2][2] = mat1.data[10];mat[2][3] = mat1.data[11];
	//mat[3][0] = mat1.data[12];mat[3][1] = mat1.data[13];mat[3][2] = mat1.data[14];mat[3][3] = mat1.data[15];

	mat[0][0] = mat1.data[0]; mat[0][1] = mat1.data[4]; mat[0][2] = mat1.data[8]; mat[0][3] = mat1.data[12];
	mat[1][0] = mat1.data[1]; mat[1][1] = mat1.data[5]; mat[1][2] = mat1.data[9]; mat[1][3] = mat1.data[13];
	mat[2][0] = mat1.data[2]; mat[2][1] = mat1.data[6]; mat[2][2] = mat1.data[10];mat[2][3] = mat1.data[14];
	mat[3][0] = mat1.data[3]; mat[3][1] = mat1.data[7]; mat[3][2] = mat1.data[11];mat[3][3] = mat1.data[15];


}


// https://developer.vuforia.com/forum/faq/technical-how-do-i-build-my-own-camera-projection-matrix
void GetProjectionMatrix( float nearPlane , float farPlane, Nx::Matrix4 & matres ) {

	const QCAR::CameraCalibration& cameraCalibration = QCAR::CameraDevice::getInstance().getCameraCalibration();

	// projectionMatrix = QCAR::Tool::getProjectionGL(cameraCalibration, nearPlane, farPlane);
	// The following code reproduces the projectionMatrix above using the camera parameters
	QCAR::Vec2F size = cameraCalibration.getSize();
	QCAR::Vec2F focalLength = cameraCalibration.getFocalLength();
	QCAR::Vec2F principalPoint = cameraCalibration.getPrincipalPoint();

	//LOGD("----> CAMERA CALIBRATION SIZE width: %f, height: %f ", size.data[0], size.data[1] );
	//LOGD("----> CAMERA principalPoint X: %f, Y: %f ", principalPoint.data[0], principalPoint.data[1] );
	//LOGD("----> CAMERA focalLength X: %f, Y: %f ", focalLength.data[0], focalLength.data[1] );

	float dx = principalPoint.data[0] - size.data[0] / 2;
	float dy = principalPoint.data[1] - size.data[1] / 2;

	float x =  2.0f * focalLength.data[0] / size.data[0];
	float y = -2.0f * focalLength.data[1] / size.data[1];
	float a =  2.0f * dx / size.data[0];
	float b = -2.0f * (dy + 1.0f) / size.data[1];
	float c = (farPlane + nearPlane) / (farPlane - nearPlane);
	float d = -nearPlane * (1.0f + c);

	QCAR::Matrix44F mat;
	mat.data[0] = x;      mat.data[1] = 0.0f;   mat.data[2] = 0.0f;  mat.data[3] = 0.0f;
	mat.data[4] = 0.0f;   mat.data[5] = y;      mat.data[6] = 0.0f;  mat.data[7] = 0.0f;
	mat.data[8] = a;      mat.data[9] = b;      mat.data[10] = c;    mat.data[11] = 1.0f;
	mat.data[12] = 0.0f;  mat.data[13] = 0.0f;  mat.data[14] = d;    mat.data[15] = 0.0f;

	VuforiaMatrix2Nx( mat, matres );

}

