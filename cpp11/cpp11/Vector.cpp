//
//  Vector.cpp
//  cpp11
//
//  Created by Valor Zhong on 16/08/14.
//  Copyright (c) 2014年 Valor Zhong. All rights reserved.
//

#include "Vector.h"


math::Vector::Vector(int s): elements{new double[s]}, sz{s} {
    for (int i = 0; i < s; i++) {
        elements[i] = 0;
    }
}

int math::Vector::size() const {
    return sz;
}

double& math::Vector::operator[](int i) {
    if (i < 0 || i >= sz) {
        throw std::out_of_range{"Vector::operator[]"};
    }
    return elements[i];
}

