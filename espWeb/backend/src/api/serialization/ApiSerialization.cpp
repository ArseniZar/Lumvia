#include "ApiSerialization.h"

namespace api
{
    String serializeApiResponse(const ApiErrorResponse &response)
    {

        gson::Str j;
        j('{');
        j[F("ok")] = response.isOk();
        j[F("error_code")] = response.getCode();
        j[F("message")] = response.message;
        j('}');
        return String(su::Text(j));
    }

}
