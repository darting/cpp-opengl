//
//  Vector.h
//  cpp11
//
//  Created by Valor Zhong on 16/08/14.
//  Copyright (c) 2014年 Valor Zhong. All rights reserved.
//

#ifndef __cpp11__Vector__
#define __cpp11__Vector__

#include <iostream>
#include "Container.h"

#endif /* defined(__cpp11__Vector__) */


namespace math {
    class Vector : public Container {
        
    public:
        // constructor
        Vector(int s);
        
        Vector(std::initializer_list<double> lst)
            :elements{new double[lst.size()]}, sz{static_cast<int>(lst.size())} {
                std::copy(lst.begin(), lst.end(), elements);
        }
        
        // copy constructor
        Vector(const Vector& other): Vector(other.size()) {
            std::cout << "vector copy constructor \n";
            for (int i = 0; i < sz; i++) {
                elements[i] = other.elements[i];
            }
        }
        
        // move contructor
        Vector(Vector&& other) {
            std::cout << "vector move constructor \n";
            std::swap(elements, other.elements);
        }
        
        // destructor
        ~Vector() {
            std::cout << "vector destructor \n";
            delete [] elements;
        }
        
        // copy assignment operator
        Vector& operator=(Vector other) {
            std::cout << "vector copy assignment operator \n";
            delete [] elements;
            std::swap(elements, other.elements);
            return *this;
        }
        
        // move assignment operator
        Vector& operator=(Vector&& other) {
            std::cout << "vector move assignment operator \n";
            std::swap(elements, other.elements);
            return *this;
        }
        
        
        
        double& operator[](int i);
        int size() const;
        
        
    private:
        double* elements;
        int sz;
        
    };
}

