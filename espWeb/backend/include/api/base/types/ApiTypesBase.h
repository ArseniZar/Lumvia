#pragma once
#ifndef API_TYPES_BASE_H
#define API_TYPES_BASE_H

namespace api
{
    /*=========================ApiRequest=============================*/

    struct ApiRequest
    {
        virtual bool isOk() const = 0;
        virtual ~ApiRequest() = default;
    };

    /*=========================ApiResponse=============================*/

    struct ApiResponse
    {
        virtual bool isOk() const = 0;
        virtual int getCode() const = 0;
        virtual ~ApiResponse() = default;
    };
}
#endif // API_TYPES_BASE_H