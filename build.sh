TOP_DIR=$(dirname $(readlink -f ${BASH_SOURCE[0]:-$0}))

[ ! -d "${TOP_DIR}/build" ] && mkdir ${TOP_DIR}/build

cd ${TOP_DIR}/build

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
         -DCMAKE_BUILD_TYPE=Debug \
         -GNinja
ninja -j32