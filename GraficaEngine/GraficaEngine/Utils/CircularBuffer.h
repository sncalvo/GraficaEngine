#pragma once

#include <memory>
#include <mutex>

template <class T>
class CircularBuffer {
public:
	explicit CircularBuffer(size_t size) :
		_buffer(std::unique_ptr<T[]>(new T[size])),
		_maxSize(size)
	{}

	void put(T item)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		_buffer[_head] = item;

		if (_full)
		{
			_tail = (_tail + 1) % _maxSize;
		}

		_head = (_head + 1) % _maxSize;

		_full = _head == _tail;
	}
	T get()
	{
		std::lock_guard<std::mutex> lock(_mutex);

		if (empty())
		{
			return T();
		}

		// Read data and advance the tail (we now have a free space)
		auto val = _buffer[_tail];
		_full = false;
		_tail = (_tail + 1) % _maxSize;

		return val;
	}
	void reset()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_head = _tail;
		_full = false;
	}
	bool empty() const
	{
		return (!_full && (_head == _tail));
	}
	bool full() const
	{
		return _full;
	}
	size_t capacity() const
	{
		return _maxSize;
	}
	size_t size() const
	{
		size_t size = _maxSize;

		if (!_full)
		{
			if (_head >= _tail)
			{
				size = _head - _tail;
			}
			else
			{
				size = _maxSize + _head - _tail;
			}
		}

		return size;
	}
private:
	std::mutex _mutex;
	std::unique_ptr<T[]> _buffer;
	size_t _head = 0;
	size_t _tail = 0;
	const size_t _maxSize;
	bool _full = 0;
};
