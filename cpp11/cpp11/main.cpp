//
//  main.cpp
//  cpp11
//
//  Created by Valor Zhong on 15/08/14.
//  Copyright (c) 2014年 Valor Zhong. All rights reserved.
//


#include <iostream>
#include "Vector.h"
#include "Container.h"

typedef int(*Fun)(void);

enum Color {
    red, blue, green
};

Color& operator++(Color& color) {
    return color = Color::blue;
}

double sum(math::Container& v) {
    double result = 0;
    for (int i = 0; i != v.size(); i++) {
        result += v[i];
    }
    
    long* addr = (long*)*(long*)*(long*)(&v);
    
    Fun f = (Fun)addr;
    
    std::cout << "f: " << f() << std::endl;
    
    std::cout << "virtual function " << addr << std::endl;
    
    return result;
}





int main(int argc, const char * argv[]) {
    
//    static_assert(sizeof(int) >= 4, "integers are too small");
//    
//    
//    math::Vector v(10);
//    
//    math::Vector v2 = {1,2};
//    
//    
//    
//    for (int i = 0; i != v.size(); i++) {
//        v[i] += i;
//    }
//    
//    Color c = Color::green;
//    ++c;
//    
//    try {
//        std::cout << "vector[]:" << v[11] << std::endl;
//    } catch (std::out_of_range) {
//        std::cout << "oops, out of range \n";
//    }
//    
//    // insert code here...
//    std::cout << "Hello, World!\n" << sum(v) << std::endl;

    {
        std::unique_ptr<math::Vector> v3 = std::unique_ptr<math::Vector>{new math::Vector{3}};

    }
    
    std::cout << "Hello, World!\n";
    
    return 0;
}




















