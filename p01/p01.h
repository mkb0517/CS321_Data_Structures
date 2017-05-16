/**
 * p01.h
 *
 *  Created on: Jan 17, 2016
 *	@author		: Matthew Brown
 *	@description: This lab was created to teach us about operator overloading for classes
 *  			: and the difference between overloaded class and friend operators.
 *				: We needed to create a custom vector and matrix class to handle vector
 *				: and matrix multiplication for the purpose of computer graphics.
 *				: This program requires g++, cstd-11, pthread, and 
 */

/**
 * Vec class to handle our vector components.
 */
template <typename T = double, int n = 4>
class vec {
	T arr[n];
public:
	/**
	 * Default vec constructor.
	 * Pre: a vec call without any parameters.
	 * Post: a vec object with and array of 0,0,0,1 for it's values.
	 */
	vec() : arr{0, 0, 0, 1}{}
	/**
	 * Overloaded vec constructor.
	 * takes params x0,x1,x2,x3 and inserts them into the vec object array.
	 */
	vec(T x0, T x1, T x2, T x3) : arr{x0, x1, x2, x3}{}
	/**
	 * Copy constructor for vec class.
	 * @param x Vector we want to copy
	 */
	vec(const vec<T> &x){
		for (int i=0; i<n; i++){
			(*this)[i] = x[i];
		}
	}
	vec<T> operator=(const vec<T> &x){
		for (int i=0; i<n; i++){
			(*this)[i] = x[i];
		}
		return *this;
	}
	/**
	 * Overloaded subscript operator to return an address to a spot in the array.
	 * @param i index of the array we're interested in
	 * @return address of the array at i.
	 */
	T &operator[] (int i){
		return arr[i];
	}
	/**
	 * Overloaded subscript operator to return the value at i.
	 * @param i index of the array we're interested in
	 * @return value of the array at i.
	 */
	T operator[](int i) const{
		return arr[i];
	}
	/**
	 * Overloaded multiplication assignment operator for vectors.
	 * @param x vector we are going to multiply by.
	 * @return an augmented vector that has the other vector multiplied into it.
	 */
	vec<T> operator*=(T x){
		for (int i=0; i<n; i++){
			arr[i]=arr[i]*x;
		}
		return *this;
	}
	/**
	 * Overloaded addition operator for vector addition.
	 * @param x vector we want to add to our vector.
	 * @return a vector with the sun of the two vectors.
	 */
	vec<T> operator+(const vec<T> &x){
		vec<T> y(0,0,0,0);
		for (int i=0; i<n; i++){
			y[i] = arr[i]+x[i];
		}
		return y;
	}
	/**
	 * Overloaded Addition assignment operator for vectors.
	 * @param x vector we are adding to our current vector
	 * @return the original vector with the second vector added to it.
	 */
	vec<T> operator+=(const vec<T> &x){
		for (int i=0; i<n; i++){
			arr[i] = arr[i] + x[i];
		}
		return *this;
	}
	/**
	 * Overloaded subtraction operator for vectors.
	 * @param x vector we want to subtract from our working vector.
	 * @return difference of the two vectors.
	 */
	vec<T> operator-(const vec<T> &x){
		vec<T> y(0,0,0,0);
		for (int i=0; i<n; i++) y[i] = arr[i]-x[i];
		return y;
	}
	/**
	 * Overloaded subtraction assignment operator for vectors.
	 * @param x the vector we are subtracting from our working vector.
	 * @return the original vector less the second vector.
	 */
	vec<T> operator-=(const vec<T> &x){
		for (int i=0; i<n; i++){
			arr[i] = arr[i] - x[i];
		}
		return *this;
	}
	/**
	 * Overloaded << operator to cout our vector.
	 * @param out ostream&  to store the vector info.
	 * @param a vector we want to output.
	 * @return return ostream& object with the vector info in it.
	 */
	template <class os>
	friend os &operator<<(os &out,const vec<T> a) {
		for(int i=0; i<n; i++) out << a[i] << ' ';
		out << (char)10;
		return out;
	}
};
/**
 * mat class to hold our matrix information.
 */
template <typename T = double, int n = 4, int m = 4>
class mat {
	T a[n][m];
public:
	/**
	 * Default constructor for mat class.
	 * Pre: Initialize call for mat class.
	 * Post: mat object with a 4x4 array with 1 in the diagonal.
	 */
	mat() : a{1,0,0,0,
			  0,1,0,0,
			  0,0,1,0,
			  0,0,0,1}{}
	/**
	 * Overloaded constructor for mat class.
	 * Pre: Takes 16 parameters then initializes the matrix in that order.
	 * Post: Returns a mat object with a 4x4 array with the passed parameters.
	 */
	mat(T sx, T pz, T ny, T tx,
		T nz, T sy, T px, T ty,
		T py, T nx, T sz, T tz,
		T x1, T x2, T x3, T x4) :
				a{sx,pz,ny,tx,
				  nz,sy,px,ty,
				  py,nx,sz,tz,
				  x1,x2,x3,x4}{}
	/**
	 * Overloaded mat constructor to create a matrix with x for the diagonal and 0s elsewhere.
	 * @param x Value of the diagonal in the 4x4 matrix
	 * Returns a matrix with x as the diagonal.
	 */
	mat(T x) : a{x, 0, 0, 0,
				 0, x, 0, 0,
				 0, 0, x, 0,
				 0, 0, 0, x}{}
	/**
	 * Matrix copy constructor for the rule of five.
	 * @param old The matrix we want to copy.
	 */
	mat(const mat& old){
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				this->a[i][j] = old.a[i][j];
			}
		}
	}
	/**
	 * Overloaded matrix assignment operator to follow the rule of 5.
	 * @param em matrix we want to assign the value from.
	 * @return the working matrix with the values of em.
	 */
	mat<T>& operator=(const mat& em){
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				this->a[i][j] = em[i][j];
			}
		}
		return *this;
	}
	/**
	 * Default mat destructor
	 */
	~mat(){}
	/**
	 * Overloaded subscript operator for matrix
	 * @param i index of array of T that we are interested in
	 * @return pointer to an array of T to be evaluated with the second index.
	 */
	T *operator[](int i) {
		return a[i];
	}
	/**
	 * Overloaded subscript operator for reading values from a matrix
	 * @param i index of the array of T that we are interested in.
	 * @return a pointer to an array of T to read values with the second index.
	 */
	T const *operator[](int i) const {
		return a[i];
	}
	/**
	 * Overloaded * operator for matrix multiplication
	 * @param x Matrix we want to multiply our matrix by.
	 * @return a matrix with the multiplied values in it.
	 */
	mat<T> operator*(const mat<T> &x) const {
		mat<T> y{0};
		for (int i=0; i<n; i++){
			for (int j=0; j<n; j++){
				for (int k=0; k<m; k++){
					y[i][j] += a[i][k]*x[k][j];
				}
			}
		}
		return y;
	}
	/**
	 * Overloaded * operator to multiply our matrix with a vector.
	 * @param x the vector we want to multiple our matrix by.
	 * @return a vector with the multiplied values.
	 */
	vec<T> operator*(const vec<T> &x) const {
		vec<T> y;
		for (int i=0; i<n; i++){
			y[i] = 0;
			for(int j=0; j<m; j++){
				y[i] += a[i][j]*x[j];
			}
		}
		return y;
	}
	/**
	 * Overloaded matrix multiplication assignment operator.
	 * @param x Matrix we want to multiply our working matrix by.
	 * @return The result of multiplying the two matrices.
	 */
	mat<T> operator*=(const mat<T> &x){
		mat<T> y{0};
		for (int i=0; i<n; i++){
			for (int j=0; j<n; j++){
				for (int k=0; k<m; k++){
					y[i][j] += (*this)[i][k]*x[k][j];
				}
			}
		}
		return *this = y;
	}
	/**
	 * Overloaded matrix assignment operator to multiply a matrix by a scalar.
	 * @param x scalar quantity.
	 * @return a matrix that has been multiplied by x.
	 */
	mat<T> operator*=(T x){
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				a[i][j] = a[i][j]*x;
			}
		}
		return *this;
	}
	/**
	 * Overloaded addition operator for matrix addition.
	 * @param x The matrix we want to add.
	 * @return A matrix with the sum of the two matrices.
	 */
	mat<T> operator+(const mat<T> &x){
		mat<T> y{0};
		for (int i=0; i<n; i++){
			for (int j=0; j<n; j++){
				y[i][j] = (*this)[i][j]+x[i][j];
			}
		}
		return y;
	}
	/**
	 * Overloaded addition assignment operator for matrix addition.
	 * @param x Matrix we are adding to the working matrix.
	 * @return the original matrix with the new matrix added to it.
	 */
	mat<T> operator+=(const mat<T> &x){
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				a[i][j] = a[i][j]+x[i][j];
			}
		}
		return *this;
	}
	/**
	 * Overloaded subtraction operator for matrix subtraction.
	 * @param x Matrix we are subtracting from the calling matrix.
	 * @return a matrix with the difference of the two matrices.
	 */
	mat<T> operator-(const mat<T> &x){
		mat<T> y{0};
		for (int i=0; i<n; i++){
			for (int j=0; j<n; j++){
				y[i][j] = (*this)[i][j]-x[i][j];
			}
		}
		return y;
	}
	/**
	 * Overloaded subtraction assignment operator for matrix subtraction.
	 * @param x matrix we want to subtract from the working matrix.
	 * @return the original matrix with the new matrix subtracted from it.
	 */
	mat<T> operator-=(const mat<T> &x){
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				a[i][j] = a[i][j]-x[i][j];
			}
		}
		return *this;
	}
	/**
	 * Overloaded << operator to output the data in our matrix.
	 * @param out stream& to hold our output.
	 * @param a matrix we want to output the data of.
	 * @return an os& with the data of the matrix and formatting.
	 */
	template <class os>
	friend os &operator<<(os &out,mat<T> a) {
	  for (int i=0; i<n; i++){
		  for (int j=0; j<m; j++){
			  out << a[i][j] << ' ';
		  }
		  out << (char)10;
	  }
    return out;
	}
};

/**
 * Transpose function to find the transpose of a matrix.
 * @param x The matrix we want the transpose of.
 * @return the Transpose of the given matrix.
 */
template<typename T=double, int n=4, int m=4>
mat<T> transpose(const mat<T> &x){
	mat<T> y{0};
	for (int i=0; i<n; i++){
		for (int j=0; j<m; j++){
			y[i][j]=x[j][i];
		}
	}
	return y;
}
