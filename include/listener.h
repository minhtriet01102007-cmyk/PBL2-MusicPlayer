#pragma once
#include "user.h"

class Listener : public User{
    protected:

    public:
        void show() const override;
};