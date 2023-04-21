class binaryReader
{
public:
	binaryReader(uint8_t* data) : data_(data)
	{
	}

	binaryReader(uint8_t* data, std::size_t max_position) :
		max_position_(max_position), data_(data)
	{
	}

	template <typename T>
	T read(bool updatePos = true)
	{
		T data;
		if (check_position(sizeof(T)))
		{
			memcpy(&data, data_ + position_, sizeof(T));
			if (updatePos)
				position_ += sizeof(T);
		}
		return data;
	}

	template <typename T>
	void set(T& value)
	{
		if (check_position(sizeof(T)))
		{
			memcpy(&value, data_ + position_, sizeof(T));
			position_ += sizeof(T);
		}
	}

	std::string read_string(std::size_t size)
	{
		if (!check_position(size))
		{
			return "";
		}
		std::string data(reinterpret_cast<const char*>(data_ + position_), size);
		position_ += size;
		return data;
	}

	void skip(std::size_t size)
	{
		if (!check_position(size))
		{
			return;
		}
		position_ += size;
	}

private:
	std::size_t position_ = 0;
	std::size_t max_position_ = -1;
	uint8_t* data_;

	bool check_position(std::size_t size)
	{
		return max_position_ != -1 && position_ + size > max_position_;
	}
};
