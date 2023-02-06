#pragma once
#include <cstdlib>
#include "Exception.hpp"

namespace custom {

	template <typename T>
	class vector {
	private:
		unsigned _size = 0;
		T* _arr = nullptr;
	public:

		class VecIterator;

		VecIterator begin() noexcept {
			return VecIterator(_arr);
		}
		const VecIterator begin()  const noexcept {
			return VecIterator(_arr);
		}
		VecIterator end() noexcept {
			return VecIterator(_arr+_size);
		}
		const VecIterator end() const noexcept {
			return VecIterator(_arr + _size);
		}

		class VecIterator {
		private:
			T* _cur;
		public:
			explicit VecIterator(T* first) : _cur(first) {}

			VecIterator operator+ (int n) { return VecIterator(_cur + n); }
			VecIterator operator- (int n) { return VecIterator(_cur - n); }

			VecIterator operator++ (int) { return VecIterator(_cur++); }
			VecIterator operator-- (int) { return VecIterator(_cur--); }
			VecIterator operator++ () { return VecIterator(++_cur); }
			VecIterator operator-- () { return VecIterator(--_cur); }

			bool operator!= (const VecIterator& it) { return _cur != it._cur; }
			bool operator== (const VecIterator& it) { return _cur == it._cur; }

			T& operator* () { return *_cur; }

		};
	public:
		vector() {
			_arr = new T[0];
		}
		unsigned size() {
			return _size;
		}
		void resize(unsigned nsize) {
			T* narr = new T[nsize];
			memmove(narr, _arr, (_size > nsize) ? sizeof(T) * nsize : sizeof(T) * _size);
			_arr = narr;
			_size = nsize;
		}
		void push_back(const T& elem) {
			resize(_size+1);
			_arr[_size - 1] = elem;
		}

		void insert( VecIterator it,  const T& _other) {
			if (it == end()) {
				push_back(_other);
				return;
			}
			VecIterator res = end()-1;
			T last = *res;
			for (res; res != it; res--) {
				*res = *(res - 1);
			}
			*res = _other;
			resize(_size+1);
			_arr[_size-1] = last;
		}
		void erase(VecIterator it) {
			(*it).~T();
			for (VecIterator res = it; res != end()-1; res++) {
				*res = *(res + 1);
			}
			//T& obj = *it;
			//obj.~T();
			resize(_size-1);
		}
		T& at(unsigned pos) {
			if (pos >= _size || pos < 0) throw Exception(0, "Vector exception: Out of range!");
			else {
				return _arr[pos];
			}
		}
		void clear() {
			for (VecIterator it = begin(); it != end(); it++) {
				(*it).~T();
			}
			resize(0);
		}
		~vector() {
			clear();
			delete[] _arr;
			_arr = nullptr;
		}
		vector& operator= (const vector& vec) {
			clear();
			resize(vec._size);
			memcpy(_arr, vec._arr, vec._size * sizeof(T));
			return *this;
		}
	};
}
