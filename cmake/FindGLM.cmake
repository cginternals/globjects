
# GLM_FOUND
# GLM_INCLUDE_DIR

FIND_PATH(GLM_INCLUDE_DIR glm/glm.hpp
    $ENV{GLM_HOME}
    $ENV{GLMDIR}
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where glm/glm.hpp resides.")
    
IF(GLM_INCLUDE_DIR)
  SET(GLM_FOUND 1 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
ELSE()
  SET(GLM_FOUND 0 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
  MESSAGE(WARNING "Note: an envvar GLM_HOME assists this script to locate glm.")
ENDIF()

MARK_AS_ADVANCED(GLM_FOUND)
