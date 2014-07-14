/*
**	Author:		Martin Schwarz
**	Name:		mvm.hpp
**	Project:	mvm - Mars Vector Math
**	Compile:	-std=c++11 // include in other project
**	License:	WTFPL // COPYING.txt
*/

/*
**	TODO:
**		add math assign functions to matNM and quart
**		implement vertex rotation to quart
**		implement lerp function to vecN
**		implement slerp function to quart
**		implement curves and splines
**		add quaternion rotation function to matrix class
**		clean up minor naming inconsistencies
*/

#ifndef MVM
#define MVM

#include <iostream>
#include <cmath>

namespace mvm{
//###############################################################  declaration
	template<typename T, const unsigned int N> class vecN;	//Base Vector Class
	template<typename T, const unsigned int X, const unsigned int Y> class matNM;	//Base Matrix Class
	template<typename T> class quat;	//Base Quaternion Class

	template<typename T> class vec3;	//3 component vector
	template<typename T> class vec4;	//4 component vector

	typedef vec3<float> fvec3;
	typedef vec3<double> dvec3;
	typedef vec3<int> ivec3;
	typedef vec3<unsigned int> uvec3;

	typedef vec4<float> fvec4;
	typedef vec4<double> dvec4;
	typedef vec4<int> ivec4;
	typedef vec4<unsigned int> uvec4;

	typedef matNM<float, 4, 4> fmat44;
	typedef matNM<double, 4, 4> dmat44;
	typedef matNM<int, 4, 4> imat44;
	typedef matNM<unsigned int, 4, 4> umat44;

	typedef quat<float> fquat;
	typedef quat<double> dquat;

	template<typename T, const unsigned int X, const unsigned int Y>
	inline void identity_mat(matNM<T, X, Y> &res);	//identity matrix

	template<typename T, const unsigned int X, const unsigned int Y, const unsigned int A>
	inline matNM<T, X, Y> translat_mat(matNM<T, X, Y> &res, const vecN<T, A> &in);	//translation matrix

	template<typename T, const unsigned int X, const unsigned int Y, const unsigned int A>
	inline matNM<T, X, Y> translat_mat(matNM<T, X, Y> &res, T in[A]);	//translation matrix

	template<typename T, const unsigned int X, const unsigned int Y, const unsigned int A>
	inline matNM<T, X, Y> scale_mat(matNM<T, X, Y> &res, const vecN<T, A> &in);	//scaling matrix

	template<typename T, const unsigned int X, const unsigned int Y, const unsigned int A>
	inline matNM<T, X, Y> mul(const matNM<T, A, Y> &m1, const matNM<T, X, A> &m2);	//matrix multiplication

	template<typename T>
	inline matNM<T, 4, 4> rot(T rx, T ry, T rz);	//rotation matrix

	template<typename T>
	inline quat<T> rot(T a, T x_, T y_, T z_);	//rotation quaternion

	template<typename T>
	inline quat<T> rot(const T in[4]);	//rotation quaternion

	template<typename T>
	inline quat<T> rot(T a, const T in[3]);	//rotation quaternion

	template<typename T>
	inline quat<T> rot(T a, const vecN<T, 3> &in);	//rotation quaternion

	template<typename T>
	inline quat<T> rot(const vecN<T, 4> &in);	//rotation quaternion
}

namespace mvm{
//###############################################################  3 component vector
template<typename T> class vec3 : public vecN<T, 3>{	//3 component vector
public:
	typedef vecN<T, 3> base;

//###############################################################  constructor
	inline vec3(T in[3]){
		for(unsigned int i = 0; i < 3; i++){
			base::data[i] = in[i];
		}
	}

	inline vec3(const base &in) : base(in){}

	inline vec3(T x_ = 0, T y_ = 0, T z_ = 0){
		base::data[0] = x_;
		base::data[1] = y_;
		base::data[2] = z_;
	}

	//variables:
};

//###############################################################  4 component vector
template<typename T> class vec4 : public vecN<T, 4>{	//4 component vector
public:
	typedef vecN<T, 4> base;

//###############################################################  constructor
	inline vec4(T in[4]){
		for(unsigned int i = 0; i < 4; i++){
			base::data[i] = in[i];
		}
	}

	inline vec4(const base &in) : base(in){}

	inline vec4(T x_ = 0, T y_ = 0, T z_ = 0, T w_ = 0){
		base::data[0] = x_;
		base::data[1] = y_;
		base::data[2] = z_;
		base::data[3] = w_;
	}

	//variables:
};

//###############################################################  Base Vector Class
template<typename T, const unsigned int N> class vecN{	//N component vector
public:
//###############################################################  constructor
	inline vecN(){
		for(unsigned int i = 0; i < N; i++){
			data[i] = 0;
		}
	}

	inline vecN(T in[N]){
		for(unsigned int i = 0; i < N; i++){
			data[i] = in[i];
		}
	}

//###############################################################  assign operators and function
	inline void operator=(const vecN &in){
		for(unsigned int i = 0; i < N; i++){
			data[i] = in.data[i];
		}
		return;
	}

	inline void operator=(T in[N]){
		for(unsigned int i = 0; i < N; i++){
			data[i] = in[i];
		}
		return;
	}

	inline void set_values(T in[N]){
		for(unsigned int i = 0; i < N; i++){
			data[i] = in[i];
		}
		return;
	}

//###############################################################  stream operator
	inline friend std::ostream& operator<<(std::ostream &out, const vecN<T, N> &vec){
		out << "(" << vec.data[0];
		for(unsigned int i = 1; i < N; i++){
			out << ", " << vec.data[i];
		}
		out << ")";
		return out;
	}

//###############################################################  math operators
	inline vecN<T, N> operator+(const vecN<T, N> &in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] + in.data[i];
		}
		return res;
	}

	inline vecN<T, N> operator-(const vecN<T, N> &in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] - in.data[i];
		}
		return res;
	}

	inline vecN<T, N> operator*(const vecN<T, N> &in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] * in.data[i];
		}
		return res;
	}

	inline vecN<T, N> operator*(const T in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] * in;
		}
		return res;
	}

	inline vecN<T, N> operator/(const vecN<T, N> &in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] / in.data[i];
		}
		return res;
	}

	inline vecN<T, N> operator/(const T in) const{
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] / in;
		}
		return res;
	}

//###############################################################  math assign operators
	inline vecN<T, N> operator+=(const vecN<T, N> &in){
		for(unsigned int i = 0; i < N; i++){
			data[i] += in.data[i];
		}
		return *this;
	}

	inline vecN<T, N> operator-=(const vecN<T, N> &in){
		for(unsigned int i = 0; i < N; i++){
			data[i] += in.data[i];
		}
		return *this;
	}

	inline vecN<T, N> operator*=(const vecN<T, N> &in){
		for(unsigned int i = 0; i < N; i++){
			data[i] *= in.data[i];
		}
		return *this;
	}

	inline vecN<T, N> operator*=(const T in){
		for(unsigned int i = 0; i < N; i++){
			data[i] *= in;
		}
		return *this;
	}

	inline vecN<T, N> operator/=(const vecN<T, N> &in){
		for(unsigned int i = 0; i < N; i++){
			data[i] /= in.data[i];
		}
		return *this;
	}

	inline vecN<T, N> operator/=(const T in){
		for(unsigned int i = 0; i < N; i++){
			data[i] /= in;
		}
		return *this;
	}

//###############################################################  length functions
	inline T len(const vecN<T, N> &in) const{
		vecN<T, N> tmp;
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			tmp.data[i] = data[i] - in.data[i];
		}
		for(unsigned int i = 0; i < N; i++){
			res += tmp.data[i] * tmp.data[i];
		}
		return sqrt(res);
	}

	inline T len(T in[N]) const{
		vecN<T, N> tmp;
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			tmp.data[i] = data[i] - in[i];
		}
		for(unsigned int i = 0; i < N; i++){
			res += tmp.data[i] * tmp.data[i];
		}
		return sqrt(res);
	}

	inline T len() const{
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			res += data[i] * data[i];
		}
		return sqrt(res);
	}

	inline T len_nosqr(const vecN<T, N> &in) const{
		vecN<T, N> tmp;
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			tmp.data[i] = data[i] - in.data[i];
		}
		for(unsigned int i = 0; i < N; i++){
			res += tmp.data[i] * tmp.data[i];
		}
		return res;
	}

	inline T len_nosqr(T in[N]) const{
		vecN<T, N> tmp;
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			tmp.data[i] = data[i] - in[i];
		}
		for(unsigned int i = 0; i < N; i++){
			res += tmp.data[i] * tmp.data[i];
		}
		return res;
	}

	inline T len_nosqr() const{
		T res = 0;
		for(unsigned int i = 0; i < N; i++){
			res += data[i] * data[i];
		}
		return res;
	}

//###############################################################  normalize coordinate system
	inline vecN<T, N> norm() const{
		T len = this->len();
		vecN<T, N> res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[i] / len;
		}
		return res;
	}

	inline vecN<T, N> norm_self(){
		T len = this->len();
		for(unsigned int i = 0; i < N; i++){
			data[i] = data[i] / len;
		}
		return *this;
	}

//###############################################################  dot product
	inline T dot(const vecN<T, N> &in) const{
		T res;
		for(unsigned int i = 0; i < N; i++){
			res += data[i] * in.data[i];
		}
		return res;
	}

//###############################################################  reflection
	inline vecN<T, N> reflect(const vecN<T, N> &normal) const{
		vecN<T, N> t1 = this->norm(), t2 = normal.norm();
		T temp = t1.dot(t2 * 2);
		return t1 - t2 * temp;
	}

	inline vecN<T, N> reflect_self(const vecN<T, N> &normal){
		vecN<T, N> t1 = normal.norm();
		this->norm_self();
		T temp = this->dot(t1 * 2);
		*this = *this - t1 * temp;
		return *this;
	}

//###############################################################  cross product
	inline vecN<T, 3> cross(const vecN<T, N> &in) const{
		vecN<T, 3> res;
		//if(N != 3) return res;
		for(unsigned int i = 0; i < N; i++){
			res.data[i] = data[(i + 1)%N] * in.data[(i + 2)%N] - data[(i + 2)%N] * in.data[(i + 1)%N];
		}
		return res;
	}

//###############################################################  euler
	inline vecN<T, N> rot_euler(const vecN<T, N> &rot) const{
		vecN<T, N> res;
		res.data[0] = cos(rot.data[2]) * cos(rot.data[1]);
		if(N > 1) res.data[1] = sin(rot.data[2]) * cos(rot.data[1]);
		if(N > 2) res.data[2] = sin(rot.data[2]);
		return res;
	}

//###############################################################  close to zero
	inline void close_to_zero(){
		for(unsigned int i = 0; i < N; i++) if(data[i] < 1.e-6 && data[i] > -1.e-6) data[i] = 0;
	}

//###############################################################  print to std::cerr
	inline void print() const{
		std::cerr << "(" << data[0];
		for(unsigned int i = 1; i < N; i++){
			std::cerr << ", " << data[i];
		}
		std::cerr << ")" << std::endl;
	}

//###############################################################  variables
	T data[N];
protected:
private:
};	//vecN

//###############################################################  Base Matrix Class
template<typename T, const unsigned int X, const unsigned int Y> class matNM{
public:
//###############################################################  constructor
	inline matNM(){
		for(unsigned int i = 0; i < X * Y; i++){
			data[i] = 0;
		}
		/*for(unsigned int i = 0; i < X && i < Y; i++){
			data[i * Y + i] = 1;
		}*/
	}

	inline matNM(vecN<T, Y> in[X]){
		for(unsigned int i = 0; i < X; i++){
			for(unsigned int j = 0; j < Y; j++){
				data[i * X + j] = in[i].data[j];
			}
		}
	}

	inline matNM(T in[X][Y]){
		for(unsigned int i = 0; i < X; i++){
			for(unsigned int j = 0; j < Y; j++){
				data[i * X + j] = in[i][j];
			}
		}
	}

	inline matNM(T in[X * Y]){
		for(unsigned int i = 0; i < X * Y; i++){
			data[i] = in[i];
		}
	}

//###############################################################  assign operator
	inline void operator=(const matNM<T, X, Y> &in){
		for(unsigned int i = 0; i < X * Y; i++){
			data[i] = in.data[i];
		}
		return;
	}

	inline void input(const matNM<T, X, Y> &in){
		for(unsigned int i = 0; i < X * Y; i++){
			data[i] = in.data[i];
		}
		return;
	}

//###############################################################  math operators
	inline matNM<T, X, Y> operator+(const matNM<T, X, Y> &in) const{
		matNM<T, X, Y> res;
		for(unsigned int i = 0; i < X * Y; i++){
			res.data[i] = data[i] + in.data[i];
		}
		return res;
	}

	inline matNM<T, X, Y> operator-(const matNM<T, X, Y> &in) const{
		matNM<T, X, Y> res;
		for(unsigned int i = 0; i < X * Y; i++){
			res.data[i] = data[i] - in.data[i];
		}
		return res;
	}

	template<const unsigned int A>
	inline matNM<T, A, Y> operator*(const matNM<T, A, X> &in){
		matNM<T, A, Y> res;
		for(unsigned int i = 0; i < Y; i++){
			for(unsigned int j = 0; j < A; j++){
				T line[X];
				for(unsigned int k = 0; k < X; k++){
					line[k] = data[k * X + i];
				}
				for(unsigned int k = 0; k < X; k++){
					res.data[j * A + i] += line[k] * in.data[j * A + k];
					//std::cerr << line[k] << " * " << in.data[j * A + k] << std::endl;
				}
				//std::cerr << "result: " << res.data[j * A + i] << std::endl;
			}
		}
		//res.print();
		return res;
	}

	inline vecN<T, X> operator*(const vecN<T, X> &in){
		vecN<T, X> res;
		for(unsigned int i = 0; i < Y; i++){
			T line[X];
			for(unsigned int k = 0; k < X; k++){
				line[k] = data[k * X + i];

			}
			for(unsigned int k = 0; k < X; k++){
				res.data[i] += line[k] * in.data[k];
				//std::cerr << "line " << i << " " << line[k] << " column 0 " << in.data[k] << " result: " << res.data[i] << std::endl;
			}
			//std::cerr << "result: " << res.data[i] << std::endl;
		}
		//res.print();
		return res;
	}

//###############################################################  identity matrix
	inline void identity_mat(){
		for(unsigned int i = 0; i < X && i < Y; i++){
			data[i * Y + i] = 1;
		}
		return;
	}

//###############################################################  translation matrix
	inline void translat_mat(const T in[Y]){
		for(unsigned int i = 0; i < X && i < Y; i++){
			data[i * Y + i] = 1;
		}
		int yc = Y;
		if(X <= Y) yc = X - 1;
		for(unsigned int i = 0; i < yc; i++){
			data[X * Y - Y + i] = in[i];
		}
		return;
	}

	template<const unsigned int A>
	inline void translat_mat(const vecN<T, A> &in){
		for(unsigned int i = 0; i < X && i < Y; i++){
			data[i * Y + i] = 1;
		}
		int yc = A;
		if(X <= A) yc = X - 1;
		for(unsigned int i = 0; i < yc; i++){
			data[X * Y - Y + i] = in.data[i];
		}
		return;
	}

//###############################################################  scaling matrix
	template<const unsigned int A>
	inline void scale_mat(const vecN<T, A> &in){
		for(unsigned int i = 0; i < X && i < Y && i < A; i++){
			data[i * Y + i] = in.data[i];
		}
		if(X == Y) data[X * Y - 1] = 1;
		return;
	}

//###############################################################  rotation matrix
	template<const unsigned int A>
	inline void rot_matrix(const vecN<T, A> &in){
		if(X < 3 && Y < 3 && A < 3) return;
		T crx = cos(in.data[0]), cry = cos(in.data[1]), crz = cos(in.data[2]),
		  srx = sin(in.data[0]), sry = sin(in.data[1]), srz = sin(in.data[2]);
		data[0] = cry * crz;
		data[1] = -cry * srz;
		data[2] = sry;
		data[Y] = crx * srz + srx * sry * crz;
		data[Y + 1] = crx * crz - srx * sry * srz;
		data[Y + 2] = -srx * cry;
		data[2 * Y] = srx * srz - crx * sry * crz;
		data[2 * Y + 1] = srx * crz + crx * sry * srz;
		data[2 * Y + 2] = crx * cry;
		if(X > 3 && Y > 3) data[X * Y -1] = 1;
		return;
	}

	inline void rot_matrix(T in[3]){
		if(X < 3 && Y < 3) return;
		T crx = cos(in[0]), cry = cos(in[1]), crz = cos(in[2]),
		  srx = sin(in[0]), sry = sin(in[1]), srz = sin(in[2]);
		data[0] = cry * crz;
		data[1] = -cry * srz;
		data[2] = sry;
		data[Y] = crx * srz + srx * sry * crz;
		data[Y + 1] = crx * crz - srx * sry * srz;
		data[Y + 2] = -srx * cry;
		data[2 * Y] = srx * srz - crx * sry * crz;
		data[2 * Y + 1] = srx * crz + crx * sry * srz;
		data[2 * Y + 2] = crx * cry;
		if(X > 3 && Y > 3) data[X * Y -1] = 1;
		return;
	}

//###############################################################  close to zero
	inline void close_to_zero(){
		for(unsigned int i = 0; i < X * Y; i++) if(data[i] < 1.e-6 && data[i] > -1.e-6) data[i] = 0;
	}

//###############################################################  print
	inline void print() const{
		//std::cerr << std::endl;
		for(unsigned int j = 0; j < X; j++)std::cerr << "---";
		std::cerr << std::endl;
		for(unsigned int i = 0; i < Y; i++){
			std::cerr << "|" << data[i];
			for(unsigned int j = 1; j < X; j++){
				std::cerr << ", " << data[i + j * Y];
			}
			std::cerr << "|" << std::endl;
		}
		for(unsigned int j = 0; j < X; j++)std::cerr << "---";
		std::cerr << std::endl;
		return;
	}

//###############################################################  variables
	T data[X * Y];
protected:
private:
};	//matNM

//###############################################################  identity matrix
template<typename T, const unsigned int X, const unsigned int Y>
inline matNM<T, X, Y>  identity_mat(matNM<T, X, Y> &res){
	for(unsigned int i = 0; i < X && i < Y; i++){
		res.data[i  * Y + i] = 1;
	}
	return res;
}

//###############################################################  translation matrix
template<typename T, const unsigned int A>
inline matNM<T, 4, 4> translate(const vecN<T, A> &in){
	matNM<T, 4, 4> res;
	for(unsigned int i = 0; i < 4; i++){
		res.data[i * 4 + i] = 1;
	}
	for(unsigned int i = 0; i < 3; i++){
		res.data[12 + i] = in.data[i];
	}
	return res;
}

template<typename T>
inline matNM<T, 4, 4> translate(T in[3]){
	matNM<T, 4, 4> res;
	for(unsigned int i = 0; i < 4; i++){
		res.data[i * 4 + i] = 1;
	}
	for(unsigned int i = 0; i < 3; i++){
		res.data[12 + i] = in[i];
	}
	return res;
}

template<typename T>
inline matNM<T, 4, 4> translate(T x_, T y_, T z_){
	T tmp[] = {x_, y_, z_};
	return translate(tmp);
}

//###############################################################  scaling matrix
template<typename T, const unsigned int A>
inline matNM<T, 4, 4> scale(const vecN<T, A> &in){
	matNM<T, 4, 4> res;
	for(unsigned int i = 0; i < 4 && i < A; i++){
		res.data[i * 4 + i] = in.data[i];
	}
	res.data[15] = 1;
	return res;
}

template<typename T>
inline matNM<T, 4, 4> scale(const T in[3]){
	matNM<T, 4, 4> res;
	for(unsigned int i = 0; i < 3; i++){
		res.data[i * 4 + i] = in[i];
	}
	res.data[15] = 1;
	return res;
}

template<typename T>
inline matNM<T, 4, 4> scale(T sx, T sy, T sz){
	T tmp[] = {sx, sy, sz};
	return scale(tmp);
}

//###############################################################  rotation matrix
template<typename T>
inline matNM<T, 4, 4> rot(T rx, T ry, T rz){
	matNM<T, 4, 4> res;
	T crx = cos(rx), cry = cos(ry), crz = cos(rz),
	  srx = sin(rx), sry = sin(ry), srz = sin(rz);
	res.data[0] = cry * crz;
	res.data[1] = -cry * srz;
	res.data[2] = sry;
	res.data[4] = crx * srz + srx * sry * crz;
	res.data[5] = crx * crz - srx * sry * srz;
	res.data[6] = -srx * cry;
	res.data[8] = srx * srz - crx * sry * crz;
	res.data[9] = srx * crz + crx * sry * srz;
	res.data[10] = crx * cry;
	res.data[15] = 1;
	return res;
}

//###############################################################  perspective matrix
template<typename T>
inline matNM<T, 4, 4> perspective(T fov_v, T aspect, T near, T far){
	matNM<T, 4, 4> res;
	T w = 1 / tan(0.5 * fov_v * 0.01745329251);
	T h = w / aspect;
	T n = (near + far) / (near - far);
	T f = (2 * near * far) / (near - far);
	res.data[0] = h;
	res.data[5] = w;
	res.data[10] = n;
	res.data[11] = f;
	res.data[14] = -1;
	return res;
}

//###############################################################  Base Quaternion Class
template<typename T> class quat{	//Base Quaternion Class
public:
//###############################################################  constructor
	inline quat(){
		data[0] = 1;
		for(unsigned int i = 1; i < 4; i++) data[i] = 0;
	}

	inline quat(T a, T x_, T y_, T z_){
		data[0] = cos(a / 2);
		data[1] = x_ * sin(a / 2);
		data[2] = y_ * sin(a / 2);
		data[3] = z_ * sin(a / 2);
	}

	inline quat(T a, const T in[3]){
		data[0] = cos(a / 2);
		data[1] = in[0] * sin(a / 2);
		data[2] = in[1] * sin(a / 2);
		data[3] = in[2] * sin(a / 2);
	}

	inline quat(const T in[4]){
		data[0] = cos(in[3] / 2);
		data[1] = in[0] * sin(in[3] / 2);
		data[2] = in[1] * sin(in[3] / 2);
		data[3] = in[2] * sin(in[3] / 2);
	}

	inline quat(const vecN<T, 4> &in) : quat(in.data){}

	inline quat(T a, const vecN<T, 4> &in) : quat(a, in.data){}

	inline quat(T a, const vecN<T, 3> &in) : quat(a, in.data){}

	inline quat(const vecN<T, 3> &in){
		data[0] = 0;
		data[1] = in.data[0];
		data[2] = in.data[1];
		data[3] = in.data[2];
	}

//###############################################################  assign operator
	inline void operator=(const quat<T> &in){
		for(unsigned int i = 0; i < 4; i++) data[i] = in.data[i];
		return;
	}

	inline void operator=(const T in[4]){
		data[0] = cos(in[3] / 2);
		data[1] = in[0] * sin(in[3] / 2);
		data[2] = in[1] * sin(in[3] / 2);
		data[3] = in[2] * sin(in[3] / 2);
		return;
	}

	inline void operator=(const vecN<T, 4> &in){
		/*data[0] = cos(in.data[3] / 2);
		data[1] = in.data[0] * sin(in.data[3] / 2);
		data[2] = in.data[1] * sin(in.data[3] / 2);
		data[3] = in.data[2] * sin(in.data[3] / 2);*/
		*this = in.data;
		return;
	}

	inline void operator=(const vecN<T, 3> &in){
		data[0] = 0;
		data[1] = in.data[0];
		data[2] = in.data[1];
		data[3] = in.data[2];
		return;
	}

//###############################################################  normalize
	inline quat<T> norm() const{
		T div = sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3]);
		quat<T> res(data[0] / div, data[1] / div, data[2] / div, data[3] / div);
		return res;
	}

	inline void norm_self(){
		T div = sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3]);
		data[0] /= div;
		data[1] /= div;
		data[2] /= div;
		data[3] /= div;
		return;
	}

//###############################################################  invert
	inline quat<T> invert() const{
		quat<T> res(data[0], -data[1], -data[2], -data[3]);
		return res;
	}

	inline void invert_self(){
		data[1] = -data[1];
		data[2] = -data[2];
		data[3] = -data[3];
		return;
	}

//###############################################################  math operators
	inline quat<T> operator*(const quat<T> &in) const{
		quat<T> res;
		res.data[0] = data[0] * in.data[0] - data[1] * in.data[1] - data[2] * in.data[2] - data[3] * in.data[3];
		res.data[1] = data[0] * in.data[1] + data[1] * in.data[0] + data[2] * in.data[3] - data[3] * in.data[2];
		res.data[2] = data[0] * in.data[2] - data[1] * in.data[3] + data[2] * in.data[0] + data[3] * in.data[1];
		res.data[3] = data[0] * in.data[3] + data[1] * in.data[2] - data[2] * in.data[1] + data[3] * in.data[0];
		return res;
	}

//###############################################################  rotation
	inline void rot(T w_, T x_, T y_, T z_){
		quat<T> tmp;
		tmp.data[0] = cos(w_ / 2);
		tmp.data[1] = x_ * sin(w_ / 2);
		tmp.data[2] = y_ * sin(w_ / 2);
		tmp.data[3] = z_ * sin(w_ / 2);
		*this = tmp * *this;
		return;
	}

	inline void rot(const T in[4]){
		quat<T> tmp;
		tmp.data[0] = cos(in[3] / 2);
		tmp.data[1] = in[0] * sin(in[3] / 2);
		tmp.data[2] = in[1] * sin(in[3] / 2);
		tmp.data[3] = in[2] * sin(in[3] / 2);
		*this = tmp * *this;
		return;
	}

	inline void rot(T a, const T in[3]){
		quat<T> tmp;
		data[0] = cos(a / 2);
		data[1] = in[0] * sin(a / 2);
		data[2] = in[1] * sin(a / 2);
		data[3] = in[2] * sin(a / 2);
		*this = tmp * *this;
		return;
	}

	inline void rot(T a, const vecN<T, 3> &in){
		rot(a, in.data);
		return;
	}

	inline void rot_self(const vecN<T, 4> &in){
		rot(in.data);
		return;
	}

//###############################################################  create matrix
	inline matNM<T, 4, 4> to_mat44() const{
		matNM<T, 4, 4> res;
		res.data[0].data[0] = 1 - 2 * data[2] * data[2] - data[3] * data[3];
		res.data[0].data[1] = 2 * data[1] * data[2] + 2 * data[0] * data[3];
		res.data[0].data[2] = 2 * data[1] * data[3] - 2 * data[0] * data[2];
		//res.data[0].data[3] = 0;
		res.data[1].data[0] = 2 * data[1] * data[2] - 2 * data[0] * data[3];
		res.data[1].data[1] = 1 - 2 * data[1] * data[1] - data[3] * data[3];
		res.data[1].data[2] = 2 * data[2] * data[3] - 2 * data[0] * data[1];
		//res.data[1].data[3] = 0;
		res.data[2].data[0] = 2 * data[1] * data[3] + 2 * data[0] * data[2];
		res.data[2].data[1] = 2 * data[2] * data[3] + 2 * data[0] * data[1];
		res.data[2].data[2] = 1 - 2 * data[2] * data[2] - data[2] * data[2];
		//res.data[2].data[3] = 0;
		//res.data[3].data[0] = 0
		//res.data[3].data[1] = 0
		//res.data[3].data[2] = 0
		res.data[3].data[3] = 1;
		return res;
	}

	inline matNM<T, 4, 4> to_mat44_unnedig() const{
		matNM<T, 4, 4> res;
		res.data[0].data[0] = data[0] * data[0] + data[1] * data[1] - data[2] * data[2] - data[3] * data[3];
		res.data[0].data[1] = 2 * data[1] * data[2] + 2 * data[0] * data[3];
		res.data[0].data[2] = 2 * data[1] * data[3] - 2 * data[0] * data[2];
		//res.data[0].data[3] = 0;
		res.data[1].data[0] = 2 * data[1] * data[2] - 2 * data[0] * data[3];
		res.data[1].data[1] = 2 * data[0] * data[0] - data[1] * data[1] + data[2] * data[2] - data[3] * data[3];
		res.data[1].data[2] = 2 * data[2] * data[3] - 2 * data[0] * data[1];
		//res.data[1].data[3] = 0;
		res.data[2].data[0] = 2 * data[1] * data[3] + 2 * data[0] * data[2];
		res.data[2].data[1] = 2 * data[2] * data[3] + 2 * data[0] * data[1];
		res.data[2].data[2] = data[0] * data[0] - data[1] * data[1] - data[2] * data[2] + data[3] * data[3];
		//res.data[2].data[3] = 0;
		//res.data[3].data[0] = 0
		//res.data[3].data[1] = 0
		//res.data[3].data[2] = 0
		res.data[3].data[3] = 1;
		return res;
	}

//###############################################################  close to zero
	inline void close_to_zero(){
		for(unsigned int i = 0; i < 4; i++) if(data[i] < 1.e-6 && data[i] > -1.e-6) data[i] = 0;
		return;
	}

//###############################################################  print
	inline void print(){
		std::cerr << "q(" << data[0];
		for(unsigned int i = 1; i < 4; i++) std::cerr << ", " << data[i];
		std::cerr << ")" << std::endl;
		return;
	}

	inline void print_len(){
		std::cerr << "q_len(" << sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3]) << ")" << std::endl;
		return;
	}

//###############################################################  variables
	T data[4];
protected:
private:
};

//###############################################################  quat rotation
template<typename T>
inline quat<T> rot(T a, T x_, T y_, T z_){
	quat<T> res;
	res.data[0] = cos(a / 2);
	res.data[1] = x_ * sin(a / 2);
	res.data[2] = y_ * sin(a / 2);
	res.data[3] = z_ * sin(a / 2);
	return res;
}

template<typename T>
inline quat<T> rot(const T in[4]){
	quat<T> res;
	res.data[0] = cos(in[3] / 2);
	res.data[1] = in[0] * sin(in[3] / 2);
	res.data[2] = in[1] * sin(in[3] / 2);
	res.data[3] = in[2] * sin(in[3] / 2);
	return res;
}

template<typename T>
inline quat<T> rot(T a, const T in[3]){
	quat<T> res;
	res.data[0] = cos(in[3] / 2);
	res.data[1] = in[0] * sin(in[3] / 2);
	res.data[2] = in[1] * sin(in[3] / 2);
	res.data[3] = in[2] * sin(in[3] / 2);
	return res;
}

template<typename T>
inline quat<T> rot(T a, const vecN<T, 3> &in){
	return rot_self(a, in.data);
}

template<typename T>
inline quat<T> rot(const vecN<T, 4> &in){
	return rot_self(in.data);
}

}	//namespace mvm

#endif // MVM






