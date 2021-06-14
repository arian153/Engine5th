#pragma once
#include <string>

namespace Engine5
{
    struct MaterialData
    {
    public:

        bool operator==(const MaterialData& rhs) const
        {
            return (diffuse_type == rhs.diffuse_type) &&
                    (specular_type == rhs.specular_type) &&
                    (normal_type == rhs.normal_type) &&
                    (diffuse0 == rhs.diffuse0) &&
                    (diffuse1 == rhs.diffuse1) &&
                    (diffuse2 == rhs.diffuse2) &&
                    (specular0 == rhs.specular0) &&
                    (normal0 == rhs.normal0);
        }

    public:
        int         diffuse_type  = -1;
        int         specular_type = -1;
        int         normal_type   = -1;
        std::string diffuse0      = "";
        std::string diffuse1      = "";
        std::string diffuse2      = "";
        std::string specular0     = "";
        std::string normal0       = "";
    };
}
