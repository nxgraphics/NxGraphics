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
#ifndef __NXMOCAPPLAYERVECTOR3_H__
#define __NXMOCAPPLAYERVECTOR3_H__

namespace NxMocap_Namespace {
 
    class NxMocap_Export NxVector3
    {
    public:
		float x, y, z;

    public:
        inline NxVector3()
        {
        }

        inline NxVector3( const float fX, const float fY, const float fZ ) : x( fX ), y( fY ), z( fZ )
        {
        }

        inline explicit NxVector3( const float afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

        inline explicit NxVector3( const int afCoordinate[3] )
        {
            x = (float)afCoordinate[0];
            y = (float)afCoordinate[1];
            z = (float)afCoordinate[2];
        }

        inline explicit NxVector3( float* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

        inline explicit NxVector3( const float scaler )
            : x( scaler )
            , y( scaler )
            , z( scaler )
        {
        }

		inline float operator [] ( const size_t i ) const
        {
            assert( i < 3 );

            return *(&x+i);
        }

		inline float& operator [] ( const size_t i )
        {
            assert( i < 3 );

            return *(&x+i);
        }
		/// Pointer accessor for direct copying
		inline float* ptr()
		{
			return &x;
		}
		/// Pointer accessor for direct copying
		inline const float* ptr() const
		{
			return &x;
		}

        /** Assigns the value of the other vector.
            @param
                rkVector The other vector
        */
        inline NxVector3& operator = ( const NxVector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;

            return *this;
        }

        inline NxVector3& operator = ( const float fScaler )
        {
            x = fScaler;
            y = fScaler;
            z = fScaler;

            return *this;
        }

        inline bool operator == ( const NxVector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

        inline bool operator != ( const NxVector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

        // arithmetic operations
        inline NxVector3 operator + ( const NxVector3& rkVector ) const
        {
            return NxVector3(
                x + rkVector.x,
                y + rkVector.y,
                z + rkVector.z);
        }

        inline NxVector3 operator - ( const NxVector3& rkVector ) const
        {
            return NxVector3(
                x - rkVector.x,
                y - rkVector.y,
                z - rkVector.z);
        }

        inline NxVector3 operator * ( const float fScalar ) const
        {
            return NxVector3(
                x * fScalar,
                y * fScalar,
                z * fScalar);
        }

        inline NxVector3 operator * ( const NxVector3& rhs) const
        {
            return NxVector3(
                x * rhs.x,
                y * rhs.y,
                z * rhs.z);
        }

        inline NxVector3 operator / ( const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            return NxVector3(
                x * fInv,
                y * fInv,
                z * fInv);
        }

        inline NxVector3 operator / ( const NxVector3& rhs) const
        {
            return NxVector3(
                x / rhs.x,
                y / rhs.y,
                z / rhs.z);
        }

        inline const NxVector3& operator + () const
        {
            return *this;
        }

        inline NxVector3 operator - () const
        {
            return NxVector3(-x, -y, -z);
        }

        // overloaded operators to help Vector3
        inline friend NxVector3 operator * ( const float fScalar, const NxVector3& rkVector )
        {
            return NxVector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend NxVector3 operator / ( const float fScalar, const NxVector3& rkVector )
        {
            return NxVector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend NxVector3 operator + (const NxVector3& lhs, const float rhs)
        {
            return NxVector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

        inline friend NxVector3 operator + (const float lhs, const NxVector3& rhs)
        {
            return NxVector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend NxVector3 operator - (const NxVector3& lhs, const float rhs)
        {
            return NxVector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend NxVector3 operator - (const float lhs, const NxVector3& rhs)
        {
            return NxVector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline NxVector3& operator += ( const NxVector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline NxVector3& operator += ( const float fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline NxVector3& operator -= ( const NxVector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline NxVector3& operator -= ( const float fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline NxVector3& operator *= ( const float fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline NxVector3& operator *= ( const NxVector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline NxVector3& operator /= ( const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

        inline NxVector3& operator /= ( const NxVector3& rkVector )
        {
            x /= rkVector.x;
            y /= rkVector.y;
            z /= rkVector.z;

            return *this;
        }


   
        inline float dotProduct(const NxVector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

    

       
        inline float normalise()
        {
            float fLength = sqrt( x * x + y * y + z * z );

            // Will also work for zero-sized vectors, but will change nothing
            if ( fLength > 1e-08 )
            {
                float fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }

        /** Calculates the cross-product of 2 vectors, i.e. the vector that
            lies perpendicular to them both.
            @remarks
                The cross-product is normally used to calculate the normal
                vector of a plane, by calculating the cross-product of 2
                non-equivalent vectors which lie on the plane (e.g. 2 edges
                of a triangle).
            @param
                vec Vector which, together with this one, will be used to
                calculate the cross-product.
            @returns
                A vector which is the result of the cross-product. This
                vector will <b>NOT</b> be normalised, to maximise efficiency
                - call Vector3::normalise on the result if you wish this to
                be done. As for which side the resultant vector will be on, the
                returned vector will be on the side from which the arc from 'this'
                to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
                = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
				This is because OGRE uses a right-handed coordinate system.
            @par
                For a clearer explanation, look a the left and the bottom edges
                of your monitor's screen. Assume that the first vector is the
                left edge and the second vector is the bottom edge, both of
                them starting from the lower-left corner of the screen. The
                resulting vector is going to be perpendicular to both of them
                and will go <i>inside</i> the screen, towards the cathode tube
                (assuming you're using a CRT monitor, of course).
        */
        inline NxVector3 crossProduct( const NxVector3& rkVector ) const
        {
            return NxVector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
        }

        /** Returns a vector at a point half way between this and the passed
            in vector.
        */
        inline NxVector3 midPoint( const NxVector3& vec ) const
        {
            return NxVector3(
                ( x + vec.x ) * 0.5f,
                ( y + vec.y ) * 0.5f,
                ( z + vec.z ) * 0.5f );
        }

        /** Returns true if the vector's scalar components are all greater
            that the ones of the vector it is compared against.
        */
        inline bool operator < ( const NxVector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

        /** Returns true if the vector's scalar components are all smaller
            that the ones of the vector it is compared against.
        */
        inline bool operator > ( const NxVector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }

        /** Sets this vector's components to the minimum of its own and the
            ones of the passed in vector.
            @remarks
                'Minimum' in this case means the combination of the lowest
                value of x, y and z from both vectors. Lowest is taken just
                numerically, not magnitude, so -1 < 0.
        */
        inline void makeFloor( const NxVector3& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

        /** Sets this vector's components to the maximum of its own and the
            ones of the passed in vector.
            @remarks
                'Maximum' in this case means the combination of the highest
                value of x, y and z from both vectors. Highest is taken just
                numerically, not magnitude, so 1 > -3.
        */
        inline void makeCeil( const NxVector3& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

  
 
        inline NxVector3 normalisedCopy(void) const
        {
            NxVector3 ret = *this;
            ret.normalise();
            return ret;
        }

 



		// special points
        static const NxVector3 ZERO;
        static const NxVector3 UNIT_X;
        static const NxVector3 UNIT_Y;
        static const NxVector3 UNIT_Z;
        static const NxVector3 NEGATIVE_UNIT_X;
        static const NxVector3 NEGATIVE_UNIT_Y;
        static const NxVector3 NEGATIVE_UNIT_Z;
        static const NxVector3 UNIT_SCALE;

        /** Function for writing to a stream.
        */
        inline NxMocap_Export friend std::ostream& operator << ( std::ostream& o, const NxVector3& v )
        {
            o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
            return o;
        }
    };

template <int DIMENSION, typename TYPE>
class Tuple
{
public:
    // Construction and destruction.  The default constructor does not
    // initialize the tuple elements for native elements.  The tuple elements
    // are initialized for class data whenever TYPE initializes during its
    // default construction.
    Tuple ();
    Tuple (const Tuple& tuple);
    ~Tuple ();

    // Coordinate access.
    inline operator const TYPE* () const;
    inline operator TYPE* ();
    inline const TYPE& operator[] (int i) const;
    inline TYPE& operator[] (int i);

    // Assignment.
    Tuple& operator= (const Tuple& tuple);

    // Comparison.  The inequalities make the comparisons using memcmp, thus
    // treating the tuple as an array of unsigned bytes.
    bool operator== (const Tuple& tuple) const;
    bool operator!= (const Tuple& tuple) const;
    bool operator<  (const Tuple& tuple) const;
    bool operator<= (const Tuple& tuple) const;
    bool operator>  (const Tuple& tuple) const;
    bool operator>= (const Tuple& tuple) const;

protected:
    TYPE mTuple[DIMENSION];
};
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::Tuple ()
{
    // Uninitialized for native data.  Initialized for class data as long as
    // TYPE's default constructor initializes its own data.
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::Tuple (const Tuple& tuple)
{
    for (int i = 0; i < DIMENSION; ++i)
    {
        mTuple[i] = tuple.mTuple[i];
    }
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::~Tuple ()
{
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline Tuple<DIMENSION,TYPE>::operator const TYPE* () const
{
    return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline Tuple<DIMENSION,TYPE>::operator TYPE* ()
{
    return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline const TYPE& Tuple<DIMENSION,TYPE>::operator[] (int i) const
{
    return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline TYPE& Tuple<DIMENSION,TYPE>::operator[] (int i)
{
    return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>& Tuple<DIMENSION,TYPE>::operator= (const Tuple& tuple)
{
    for (int i = 0; i < DIMENSION; ++i)
    {
        mTuple[i] = tuple.mTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator== (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) == 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator!= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) != 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator< (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) < 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator<= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) <= 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator> (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) > 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator>= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) >= 0;
}

template <int NUMROWS, int NUMCOLS, typename TYPE>
class Table
{
public:
    // Construction and destruction.  The default constructor does not
    // initialize the array elements for native elements.  The array elements
    // are initialized for class data whenever TYPE initializes during its
    // default construction.
    Table ();
    Table (const Table& table);
    ~Table ();

    // Coordinate access.
    inline operator const TYPE* () const;
    inline operator TYPE* ();
    inline const TYPE* operator[] (int row) const;
    inline TYPE* operator[] (int row);
    inline const TYPE& operator() (int row, int col) const;
    inline TYPE& operator() (int row, int col);
    void SetRow (int row, const Tuple<NUMCOLS,TYPE>& tuple);
    Tuple<NUMCOLS,TYPE> GetRow (int row) const;
    void SetColumn (int col, const Tuple<NUMROWS,TYPE>& tuple);
    Tuple<NUMROWS,TYPE> GetColumn (int col) const;

    // Assignment.
    Table& operator= (const Table& table);

    // Comparison.  The inequalities make the comparisons using memcmp, thus
    // treating the tuple as an array of unsigned bytes.
    bool operator== (const Table& table) const;
    bool operator!= (const Table& table) const;
    bool operator<  (const Table& table) const;
    bool operator<= (const Table& table) const;
    bool operator>  (const Table& table) const;
    bool operator>= (const Table& table) const;

protected:
    // The array is stored in row-major order.
    enum { NUMENTRIES = NUMROWS*NUMCOLS };
    TYPE mEntry[NUMENTRIES];
};

template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::Table ()
{
    // Uninitialized for native data.  Initialized for class data as long as
    // TYPE's default constructor initializes its own data.
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::Table (const Table& table)
{
    for (int i = 0; i < NUMENTRIES; ++i)
    {
        mEntry[i] = table.mEntry[i];
    }
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::~Table ()
{
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline Table<NUMROWS,NUMCOLS,TYPE>::operator const TYPE* () const
{
    return mEntry;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline Table<NUMROWS,NUMCOLS,TYPE>::operator TYPE* ()
{
    return mEntry;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline const TYPE* Table<NUMROWS,NUMCOLS,TYPE>::operator[] (int row) const
{
    return &mEntry[NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline TYPE* Table<NUMROWS,NUMCOLS,TYPE>::operator[] (int row)
{
    return &mEntry[NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline const TYPE& Table<NUMROWS,NUMCOLS,TYPE>::operator() (int row, int col)
    const
{
    return mEntry[col + NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline TYPE& Table<NUMROWS,NUMCOLS,TYPE>::operator() (int row, int col)
{
    return mEntry[col + NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
void Table<NUMROWS,NUMCOLS,TYPE>::SetRow (int row,
    const Tuple<NUMCOLS,TYPE>& tuple)
{
    for (int c = 0, i = NUMCOLS*row; c < NUMCOLS; ++c, ++i)
    {
        mEntry[i] = tuple[c];
    }
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Tuple<NUMCOLS,TYPE> Table<NUMROWS,NUMCOLS,TYPE>::GetRow (int row) const
{
    Tuple<NUMROWS,TYPE> result;
    for (int c = 0, i = NUMCOLS*row; c < NUMCOLS; ++c, ++i)
    {
        result[c] = mEntry[i];
    }
    return result;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
void Table<NUMROWS,NUMCOLS,TYPE>::SetColumn (int col,
   const Tuple<NUMROWS,TYPE>& tuple)
{
    for (int r = 0, i = col; r < NUMROWS; ++r, i += NUMCOLS)
    {
        mEntry[i] = tuple[r];
    }
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Tuple<NUMROWS,TYPE> Table<NUMROWS,NUMCOLS,TYPE>::GetColumn (int col) const
{
    Tuple<NUMCOLS,TYPE> result;
    for (int r = 0, i = col; r < NUMROWS; ++r, i += NUMCOLS)
    {
        result[r] = mEntry[i];
    }
    return result;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>& Table<NUMROWS,NUMCOLS,TYPE>::operator= (
    const Table& table)
{
    for (int i = 0; i < NUMENTRIES; i++)
    {
        mEntry[i] = table.mEntry[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator== (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) == 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator!= (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) != 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator<  (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) < 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator<= (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) <= 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator>  (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) > 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator>= (const Table& table) const
{
    return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) >= 0;
}

template <typename Real>
class NxMatrix3 : public Table<3,3,Real>
{
public:
NxMatrix3 (float m00, float m01, float m02, float m10, float m11, float m12,float m20, float m21, float m22)
{
    mEntry[0] = m00;
    mEntry[1] = m01;
    mEntry[2] = m02;
    mEntry[3] = m10;
    mEntry[4] = m11;
    mEntry[5] = m12;
    mEntry[6] = m20;
    mEntry[7] = m21;
    mEntry[8] = m22;
}

};

class NxQuaternion
{
public:
NxQuaternion () { }
NxQuaternion (float w, float x, float y, float z): _w (w), _x (x), _y (y), _z (z) { }
void FromRotationMatrix (const NxMatrix3<float>& rot)
{
    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    const int next[3] = { 1, 2, 0 };

    float trace = rot(0,0) + rot(1,1) + rot(2,2);
    float root;

    if (trace > (float)0)
    {
        // |w| > 1/2, may as well choose w > 1/2
        root = sqrt( trace + (float )1);  // 2w
        mTuple[0] = ((float)0.5)*root;
        root = ((float)0.5)/root;  // 1/(4w)
        mTuple[1] = (rot(2,1) - rot(1,2))*root;
        mTuple[2] = (rot(0,2) - rot(2,0))*root;
        mTuple[3] = (rot(1,0) - rot(0,1))*root;
    }
    else
    {
        // |w| <= 1/2
        int i = 0;
        if (rot(1,1) > rot(0,0))
        {
            i = 1;
        }
        if (rot(2,2) > rot(i,i))
        {
            i = 2;
        }
        int j = next[i];
        int k = next[j];

        root = sqrt(rot(i,i) - rot(j,j) - rot(k,k) + (float)1);
        float* quat[3] = { &mTuple[1], &mTuple[2], &mTuple[3] };
        *quat[i] = ((float)0.5)*root;
        root = ((float)0.5)/root;
        mTuple[0] = (rot(k,j) - rot(j,k))*root;
        *quat[j] = (rot(j,i) + rot(i,j))*root;
        *quat[k] = (rot(k,i) + rot(i,k))*root;
    }

	_q[0] = mTuple[0];
	_q[1] = mTuple[1];
	_q[2] = mTuple[2];
	_q[3] = mTuple[3];
}

NxQuaternion operator* (const NxQuaternion  &tempQuaternion)
{
	float w = _q[0]*tempQuaternion._q[0] - (_q[1]*tempQuaternion._q[1] + _q[2]*tempQuaternion._q[2] + _q[3]*tempQuaternion._q[3]);

	float x = _q[0]*tempQuaternion._q[1] + tempQuaternion._q[0]*_q[1] + _q[2]*tempQuaternion._q[3] - _q[3]*tempQuaternion._q[2];
	float y = _q[0]*tempQuaternion._q[2] + tempQuaternion._q[0]*_q[2] + _q[3]*tempQuaternion._q[1] - _q[1]*tempQuaternion._q[3];
	float z = _q[0]*tempQuaternion._q[3] + tempQuaternion._q[0]*_q[3] + _q[1]*tempQuaternion._q[2] - _q[2]*tempQuaternion._q[1];

	_q[0] = w;
	_q[1] = x;
	_q[2] = y;
	_q[3] = z;
	
	return *this;
}

void normalize()
{
	float distance = (float)sqrt(_q[0]*_q[0] + _q[1]*_q[1] + _q[2]*_q[2] + _q[3]*_q[3]);
	if( distance == 0 )
	{
		_q[0] = 1.0;
		_q[1] = _q[2] = _q[3] = 0.0;
	}
	else
	{
		_q[0] /= distance;
		_q[1] /= distance;
		_q[2] /= distance;
		_q[3] /= distance;
	}
}
 
void fromAxisAngle( float angle, float x, float y, float z )
{
	float SinAngle = (float)sin( angle / 2 );
	float CosAngle = (float)cos( angle / 2);

	_q[0] = CosAngle;
	_q[1] = x * SinAngle;
	_q[2] = y * SinAngle;
	_q[3] = z * SinAngle;

	normalize();
}

void fromEulerAngles (float x, float y, float z)
{
	NxQuaternion QuaternionX, QuaternionY, QuaternionZ, QuaternionResult;
	
	QuaternionX.fromAxisAngle( x, 1, 0, 0 );
	QuaternionY.fromAxisAngle( y, 0, 1, 0 );
	QuaternionZ.fromAxisAngle( z, 0, 0, 1 );

	QuaternionResult = QuaternionZ * QuaternionX * QuaternionY;
	
	_q[0] = QuaternionResult._q[0];
	_q[1] = QuaternionResult._q[1];
	_q[2] = QuaternionResult._q[2];
	_q[3] = QuaternionResult._q[3];
}

NxQuaternion operator+ (const NxQuaternion  &q) const;
NxQuaternion &operator+= (const NxQuaternion  &q);

NxQuaternion operator- (const NxQuaternion  &q) const;
NxQuaternion &operator-= (const NxQuaternion  &q);

NxQuaternion operator* (float k) const;
NxQuaternion &operator*= (float k);

NxQuaternion operator* (const NxVector3  &v) const;
NxQuaternion &operator*= (const NxVector3  &v);

NxQuaternion operator/ (float k) const;
NxQuaternion &operator/= (float k);

NxQuaternion operator~ () const; // Quaternion conjugate
NxQuaternion operator- () const; // Quaternion negation


float mTuple[4];

  union
  {
    struct
    {
      float _w, _x, _y, _z;
    };

    float _q[4];
  };
};



}
#endif