#pragma once
#ifndef API_SERIALIZATION_H
#define API_SERIALIZATION_H

#include <Arduino.h>
#include <GSON.h>
#include "ApiResponse.h"

namespace api
{
    template <typename T>
    String serializeApiResponse(const ApiSuccessResponse<T> &response);
    String serializeApiResponse(const ApiErrorResponse &response);

}
#include "ApiSerialization.tpp"

#endif // API_SERIALIZATION_H
