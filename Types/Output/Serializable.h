#pragma once

class ISerializer
{
public:
	virtual ~ISerializer() = default;
	virtual void Serialize(const struct SFrameBuffer&) = 0;
};


