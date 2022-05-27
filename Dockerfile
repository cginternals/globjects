ARG BASE=cginternals/rendering-base:latest
ARG BASE_DEV=cginternals/rendering-base:dev
ARG CPPLOCATE_DEPENDENCY=cginternals/cpplocate:latest
ARG GLBINDING_DEPENDENCY=cginternals/glbinding:latest
ARG PROJECT_NAME=globjects

# DEPENDENCIES

FROM $CPPLOCATE_DEPENDENCY AS cpplocate

FROM $GLBINDING_DEPENDENCY AS glbinding

# BUILD

FROM $BASE_DEV AS build

ARG PROJECT_NAME
ARG COMPILER_FLAGS="-j 4"

COPY --from=cpplocate $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=glbinding $WORKSPACE/glbinding $WORKSPACE/glbinding

ENV cpplocate_DIR="$WORKSPACE/cpplocate"
ENV glbinding_DIR="$WORKSPACE/glbinding"
ENV globjects_DIR="$WORKSPACE/$PROJECT_NAME"

WORKDIR $WORKSPACE/$PROJECT_NAME

ADD cmake cmake
ADD docs docs
ADD deploy deploy
ADD source source
ADD data data
ADD CMakeLists.txt CMakeLists.txt
ADD configure configure
ADD $PROJECT_NAME-config.cmake $PROJECT_NAME-config.cmake
ADD $PROJECT_NAME-logo.png $PROJECT_NAME-logo.png
ADD $PROJECT_NAME-logo.svg $PROJECT_NAME-logo.svg
ADD LICENSE LICENSE
ADD README.md README.md
ADD AUTHORS AUTHORS

RUN ./configure
RUN CMAKE_OPTIONS="-DOPTION_BUILD_TESTS=Off -DOPTION_BUILD_EXAMPLES=On" ./configure
RUN cmake --build build -- $COMPILER_FLAGS

# INSTALL

FROM build as install

ARG PROJECT_NAME

WORKDIR $WORKSPACE/$PROJECT_NAME

RUN CMAKE_OPTIONS="-DCMAKE_INSTALL_PREFIX=$WORKSPACE/$PROJECT_NAME-install" ./configure
RUN cmake --build build --target install

# DEPLOY

FROM $BASE AS deploy

ARG PROJECT_NAME

COPY --from=build $WORKSPACE/cpplocate $WORKSPACE/cpplocate
COPY --from=build $WORKSPACE/glfw $WORKSPACE/glfw
COPY --from=build $WORKSPACE/glbinding $WORKSPACE/glbinding
COPY --from=build $WORKSPACE/glm $WORKSPACE/glm

COPY --from=install $WORKSPACE/$PROJECT_NAME-install $WORKSPACE/$PROJECT_NAME

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glfw/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/cpplocate/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/glbinding/lib
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WORKSPACE/$PROJECT_NAME/lib
