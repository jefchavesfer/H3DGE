#ifndef _FXNUM_HH_
#define _FXNUM_HH_

#include <stdint.h>
#include <cassert>
#include <istream>
#include <ostream>
#include <iostream>

template<unsigned int iwl>
class fxnum {
private:
	int32_t value;
	static const long wl = 32;
	void toFixed(float f) {
		if (f > (1 << iwl)) {
#ifdef DEBUG_OVERFLOW
			std::cerr << "Warning: overflow."
			          << " f = " << f << "; iwl = " << iwl << std::endl;
#endif
			value = ((uint32_t)0x80000000 - 1);
		} else if (f < -(1 << iwl)) {
#ifdef DEBUG_OVERFLOW
			std::cerr << "Warning: overflow."
			          << " f = " << f << "; iwl = " << iwl << std::endl;
#endif
			value = (uint32_t)0x80000000;
		} else {
			value = f * (1 << (wl - iwl));
		}
	}
public:
	fxnum() {value = 0;};
	fxnum(double f) {toFixed(f);}
	template<unsigned int iwl2>
	fxnum(const fxnum<iwl2> &f2) {toFixed((float)f2.toFloat());}

	float toFloat() const {return (float)value / (1 << (wl - iwl));}

	template<unsigned int iwl2>
	fxnum& operator += (const fxnum<iwl2> &f2) {
		toFixed(toFloat() + f2.toFloat());
		return *this;
	}

	fxnum& operator += (const float &fl) {
		toFixed(toFloat() + fl);
		return *this;
	}

	template<unsigned int iwl2>
	fxnum& operator -= (const fxnum<iwl2> &f2) {
		toFixed(toFloat() - f2.toFloat());
		return *this;
	}

	fxnum& operator -= (const float &fl) {
		toFixed(toFloat() - fl);
		return *this;
	}

	template<unsigned int iwl2>
	fxnum& operator *= (const fxnum<iwl2> &f2) {
		toFixed(toFloat() * f2.toFloat());
		return *this;
	}

	fxnum& operator *= (const float &fl) {
		toFixed(toFloat() * fl);
		return *this;
	}

	template<unsigned int iwl2>
	fxnum& operator /= (const fxnum<iwl2> &f2) {
		toFixed(toFloat() / f2.toFloat());
		return *this;
	}

	fxnum& operator /= (const float &fl) {
		toFixed(toFloat() / fl);
		return *this;
	}
};

template<unsigned int iwl1, unsigned int iwl2>
inline float operator + (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() + f2.toFloat();
}

template<unsigned int iwl>
inline float operator + (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() + fl;
}

template<unsigned int iwl>
inline float operator + (const float &fl, const fxnum<iwl> &fx) {
	return fx + fl;
}

template<unsigned int iwl1, unsigned int iwl2>
inline float operator - (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() - f2.toFloat();
}

template<unsigned int iwl>
inline float operator - (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() - fl;
}

template<unsigned int iwl>
inline float operator - (const float &fl, const fxnum<iwl> &fx) {
	return -(fx - fl);
}

template<unsigned int iwl1, unsigned int iwl2>
inline float operator * (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() * f2.toFloat();
}

template<unsigned int iwl>
inline float operator * (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() * fl;
}

template<unsigned int iwl>
inline float operator * (const float &fl, const fxnum<iwl> &fx) {
	return fx * fl;
}

template<unsigned int iwl1, unsigned int iwl2>
float operator / (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() / f2.toFloat();
}

template<unsigned int iwl>
inline float operator / (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() / fl;
}

template<unsigned int iwl>
inline float operator / (const float &fl, const fxnum<iwl> &fx) {
	return 1.0 / (fx / fl);
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator == (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() == f2.toFloat();
}

template<unsigned int iwl>
inline bool operator == (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() == fl;
}

template<unsigned int iwl>
inline bool operator == (const float &fl, const fxnum<iwl> &fx) {
	return fx == fl;
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator != (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return !(f1 == f2);
}

template<unsigned int iwl>
inline bool operator != (const fxnum<iwl> &fx, const float &fl) {
	return !(fx == fl);
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator <= (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() <= f2.toFloat();
}

template<unsigned int iwl>
inline bool operator <= (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() <= fl;
}

template<unsigned int iwl>
inline bool operator <= (const float &fl, const fxnum<iwl> &fx) {
	return fl <= fx.toFloat();
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator < (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() < f2.toFloat();
}

template<unsigned int iwl>
inline bool operator < (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() < fl;
}

template<unsigned int iwl>
inline bool operator < (const float &fl, const fxnum<iwl> &fx) {
	return fl < fx.toFloat();
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator > (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() > f2.toFloat();
}

template<unsigned int iwl>
inline bool operator > (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() > fl;
}

template<unsigned int iwl>
inline bool operator > (const float &fl, const fxnum<iwl> &fx) {
	return fl > fx.toFloat();
}

template<unsigned int iwl1, unsigned int iwl2>
inline bool operator >= (const fxnum<iwl1> &f1, const fxnum<iwl2> &f2) {
	return f1.toFloat() >= f2.toFloat();
}

template<unsigned int iwl>
inline bool operator >= (const fxnum<iwl> &fx, const float &fl) {
	return fx.toFloat() >= fl;
}

template<unsigned int iwl>
inline bool operator >= (const float &fl, const fxnum<iwl> &fx) {
	return fl >= fx.toFloat();
}

template<unsigned int iwl>
inline std::istream& operator >> (std::istream &in, fxnum<iwl> &fx) {
	float tmp;
	in >> tmp;
	fxnum<iwl> fxTmp = tmp;
	fx = fxTmp;
	return in;
}

template<unsigned int iwl>
inline std::ostream& operator << (std::ostream &out, const fxnum<iwl> &fx) {
	out << (float)fx.toFloat();
	return out;
}

#endif
