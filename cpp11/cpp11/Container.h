//
//  Container.h
//  cpp11
//
//  Created by Valor Zhong on 17/08/14.
//  Copyright (c) 2014年 Valor Zhong. All rights reserved.
//

#pragma once


namespace math {
    
    class Container {
        
    public:
        virtual int size() const = 0;
        virtual double& operator[](int) = 0;
        virtual ~Container(){};
    };
    
}