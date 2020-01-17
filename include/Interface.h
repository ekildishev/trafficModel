#pragma once

class UpdatableDataInterface
{
public:
    virtual void updateData(int msec) = 0;
};

class UpdatableImageInterface
{
public:
    virtual void updateImage() = 0;
};