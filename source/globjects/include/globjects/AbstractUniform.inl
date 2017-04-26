
#pragma once


namespace globjects 
{


enum class UniformType : unsigned char
{
    NONE,

    FLOAT,
    INT,
    UNSIGNED_INT,
    BOOL,
    FLOAT_VEC2,
    FLOAT_VEC3,
    FLOAT_VEC4,
    INT_VEC2,
    INT_VEC3,
    INT_VEC4,
    UNSIGNED_INT_VEC2,
    UNSIGNED_INT_VEC3,
    UNSIGNED_INT_VEC4,
    FLOAT_MAT2,
    FLOAT_MAT3,
    FLOAT_MAT4,
    FLOAT_MAT2x3,
    FLOAT_MAT3x2,
    FLOAT_MAT2x4,
    FLOAT_MAT4x2,
    FLOAT_MAT3x4,
    FLOAT_MAT4x3,
    UNSIGNED_INT64,
    TEXTURE_HANDLE,

    VECTOR_FLOAT,
    VECTOR_INT,
    VECTOR_UNSIGNED_INT,
    VECTOR_BOOL,
    VECTOR_FLOAT_VEC2,
    VECTOR_FLOAT_VEC3,
    VECTOR_FLOAT_VEC4,
    VECTOR_INT_VEC2,
    VECTOR_INT_VEC3,
    VECTOR_INT_VEC4,
    VECTOR_UNSIGNED_INT_VEC2,
    VECTOR_UNSIGNED_INT_VEC3,
    VECTOR_UNSIGNED_INT_VEC4,
    VECTOR_FLOAT_MAT2,
    VECTOR_FLOAT_MAT3,
    VECTOR_FLOAT_MAT4,
    VECTOR_FLOAT_MAT2x3,
    VECTOR_FLOAT_MAT3x2,
    VECTOR_FLOAT_MAT2x4,
    VECTOR_FLOAT_MAT4x2,
    VECTOR_FLOAT_MAT3x4,
    VECTOR_FLOAT_MAT4x3,
    VECTOR_UNSIGNED_INT64,
    VECTOR_TEXTURE_HANDLE,

    ARRAY_FLOAT,
    ARRAY_INT,
    ARRAY_UNSIGNED_INT,
    ARRAY_BOOL,
    ARRAY_FLOAT_VEC2,
    ARRAY_FLOAT_VEC3,
    ARRAY_FLOAT_VEC4,
    ARRAY_INT_VEC2,
    ARRAY_INT_VEC3,
    ARRAY_INT_VEC4,
    ARRAY_UNSIGNED_INT_VEC2,
    ARRAY_UNSIGNED_INT_VEC3,
    ARRAY_UNSIGNED_INT_VEC4,
    ARRAY_FLOAT_MAT2,
    ARRAY_FLOAT_MAT3,
    ARRAY_FLOAT_MAT4,
    ARRAY_FLOAT_MAT2x3,
    ARRAY_FLOAT_MAT3x2,
    ARRAY_FLOAT_MAT2x4,
    ARRAY_FLOAT_MAT4x2,
    ARRAY_FLOAT_MAT3x4,
    ARRAY_FLOAT_MAT4x3,
    ARRAY_UNSIGNED_INT64,
    ARRAY_TEXTURE_HANDLE
};

template <typename ValueType>
struct UniformTypeHelper
{
    static const UniformType value = UniformType::NONE;
};

template <>
struct UniformTypeHelper<float>
{
    static const UniformType value = UniformType::FLOAT;
};

template <>
struct UniformTypeHelper<int>
{
    static const UniformType value = UniformType::INT;
};

template <>
struct UniformTypeHelper<unsigned int>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<bool>
{
    static const UniformType value = UniformType::BOOL;
};

template <>
struct UniformTypeHelper<glm::vec2>
{
    static const UniformType value = UniformType::FLOAT_VEC2;
};

template <>
struct UniformTypeHelper<glm::vec3>
{
    static const UniformType value = UniformType::FLOAT_VEC3;
};

template <>
struct UniformTypeHelper<glm::vec4>
{
    static const UniformType value = UniformType::FLOAT_VEC4;
};

template <>
struct UniformTypeHelper<glm::ivec2>
{
    static const UniformType value = UniformType::INT_VEC2;
};

template <>
struct UniformTypeHelper<glm::ivec3>
{
    static const UniformType value = UniformType::INT_VEC3;
};

template <>
struct UniformTypeHelper<glm::ivec4>
{
    static const UniformType value = UniformType::INT_VEC4;
};

template <>
struct UniformTypeHelper<glm::uvec2>
{
    static const UniformType value = UniformType::UNSIGNED_INT_VEC2;
};

template <>
struct UniformTypeHelper<glm::uvec3>
{
    static const UniformType value = UniformType::UNSIGNED_INT_VEC3;
};

template <>
struct UniformTypeHelper<glm::uvec4>
{
    static const UniformType value = UniformType::UNSIGNED_INT_VEC4;
};

template <>
struct UniformTypeHelper<glm::mat2>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat3>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat4>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat2x3>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat3x2>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat2x4>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat4x2>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat3x4>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<glm::mat4x3>
{
    static const UniformType value = UniformType::UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<gl::GLuint64>
{
    static const UniformType value = UniformType::UNSIGNED_INT64;
};

template <>
struct UniformTypeHelper<TextureHandle>
{
    static const UniformType value = UniformType::TEXTURE_HANDLE;
};

template <>
struct UniformTypeHelper<std::vector<float>>
{
    static const UniformType value = UniformType::VECTOR_FLOAT;
};

template <>
struct UniformTypeHelper<std::vector<int>>
{
    static const UniformType value = UniformType::VECTOR_INT;
};

template <>
struct UniformTypeHelper<std::vector<unsigned int>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<bool>>
{
    static const UniformType value = UniformType::VECTOR_BOOL;
};

template <>
struct UniformTypeHelper<std::vector<glm::vec2>>
{
    static const UniformType value = UniformType::VECTOR_FLOAT_VEC2;
};

template <>
struct UniformTypeHelper<std::vector<glm::vec3>>
{
    static const UniformType value = UniformType::VECTOR_FLOAT_VEC3;
};

template <>
struct UniformTypeHelper<std::vector<glm::vec4>>
{
    static const UniformType value = UniformType::VECTOR_FLOAT_VEC4;
};

template <>
struct UniformTypeHelper<std::vector<glm::ivec2>>
{
    static const UniformType value = UniformType::VECTOR_INT_VEC2;
};

template <>
struct UniformTypeHelper<std::vector<glm::ivec3>>
{
    static const UniformType value = UniformType::VECTOR_INT_VEC3;
};

template <>
struct UniformTypeHelper<std::vector<glm::ivec4>>
{
    static const UniformType value = UniformType::VECTOR_INT_VEC4;
};

template <>
struct UniformTypeHelper<std::vector<glm::uvec2>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT_VEC2;
};

template <>
struct UniformTypeHelper<std::vector<glm::uvec3>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT_VEC3;
};

template <>
struct UniformTypeHelper<std::vector<glm::uvec4>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT_VEC4;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat2>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat3>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat4>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat2x3>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat3x2>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat2x4>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat4x2>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat3x4>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<glm::mat4x3>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT;
};

template <>
struct UniformTypeHelper<std::vector<gl::GLuint64>>
{
    static const UniformType value = UniformType::VECTOR_UNSIGNED_INT64;
};

template <>
struct UniformTypeHelper<std::vector<TextureHandle>>
{
    static const UniformType value = UniformType::VECTOR_TEXTURE_HANDLE;
};

template <size_t Count>
struct UniformTypeHelper<std::array<float, Count>>
{
    static const UniformType value = UniformType::ARRAY_FLOAT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<int, Count>>
{
    static const UniformType value = UniformType::ARRAY_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<unsigned int, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<bool, Count>>
{
    static const UniformType value = UniformType::ARRAY_BOOL;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::vec2, Count>>
{
    static const UniformType value = UniformType::ARRAY_FLOAT_VEC2;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::vec3, Count>>
{
    static const UniformType value = UniformType::ARRAY_FLOAT_VEC3;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::vec4, Count>>
{
    static const UniformType value = UniformType::ARRAY_FLOAT_VEC4;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::ivec2, Count>>
{
    static const UniformType value = UniformType::ARRAY_INT_VEC2;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::ivec3, Count>>
{
    static const UniformType value = UniformType::ARRAY_INT_VEC3;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::ivec4, Count>>
{
    static const UniformType value = UniformType::ARRAY_INT_VEC4;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::uvec2, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT_VEC2;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::uvec3, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT_VEC3;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::uvec4, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT_VEC4;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat2, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat3, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat4, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat2x3, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat3x2, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat2x4, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat4x2, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat3x4, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<glm::mat4x3, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT;
};

template <size_t Count>
struct UniformTypeHelper<std::array<gl::GLuint64, Count>>
{
    static const UniformType value = UniformType::ARRAY_UNSIGNED_INT64;
};

template <size_t Count>
struct UniformTypeHelper<std::array<TextureHandle, Count>>
{
    static const UniformType value = UniformType::ARRAY_TEXTURE_HANDLE;
};


template <typename T, std::size_t Count>
void AbstractUniform::setValue(const gl::GLint location, const std::array<T, Count> & value) const
{
    setValue(location, std::vector<T>(value.data(), value.data()+Count));
}


} // namespace globjects
